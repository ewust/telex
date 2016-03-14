@unload weird.bro
@load ssl-bare
@unload weird.bro
redef use_connection_compressor = F;
redef enable_syslog = F;

function do_expire(t: table[conn_id] of telex_info, cid: conn_id) : interval {
    if (cid in t) {
        telex_mod_block(cid, F);
        cleanup_telex_obj(t[cid]);
        delete t[cid];
        return 0secs;
    }
}

global telex_table: table[conn_id] of telex_info
       &read_expire = 23secs
       &expire_func = do_expire;

function cleanup_and_unblock_connection(c: connection) {
    do_expire(telex_table, c$id);
}

function cleanup_connection(c: connection) {
	cleanup_telex_obj(telex_table[c$id]);
	delete telex_table[c$id];
}

event connection_state_remove(c: connection) {
    if (c$id in telex_table) {
        cleanup_and_unblock_connection(c);
    }
}

event ssl_conn_alert(c: connection, version: count, level: count,
		description: count, is_orig: bool) {
	if (is_orig && c$id in telex_table) {
		cleanup_connection(c);
	}
}

function print_telex_table() {
	print "Telex table.........................";
	print fmt("time: %s", current_time());
	for (i in telex_table) {
		local te = telex_table[i];
        print fmt("  %s:%d  <-> %s:%d state: %d, fwd: %s (%d)", 
                i$orig_h, i$orig_p, i$resp_h, i$resp_p, te$state, 
                te$fwd_time, te$do_fwd);
    } 
}


function purge_telex_table() {
    for (i in telex_table) {
        local te = telex_table[i];
         if (te$state < 4 && (current_time() - te$fwd_time) > (5 secs)) {
            
            telex_mod_block(i, F);   # unblock
            cleanup_telex_obj(te);
            delete telex_table[i];
        }
    }
}

# called for each packet, but after all other bro processing has taken place
event new_packet_final(c: connection, p: pkt_hdr) {

    if (c$id in telex_table) {
        local te = telex_table[c$id];

        if (te$do_fwd) {
            if (network_time() > te$fwd_time) {
                if (te$state == 5) {    # sent init data to bronnect
                    #have bronnect forward last packet
                    #we do this by piggybacking off the ol'
                    #bronnect_raw type in the brotocol
                    te$do_fwd = F;
                    
                    telex_send_bronnect_raw_pkt(c$id, te, p$raw); 
                    te$state = 6; # bronnect's responsibility now
                    telex_send_rst(c$id, te); # to the server.
                    cleanup_connection(c);
                } else  {
                    telex_fwd(p$raw);
                }

            } else if (te$state == 5) {
                #missed the boat (maybe?) - client has likely already received this packet
                # (we weren't fast enough on the block)
                te$do_fwd = F;
                te$state = 7;   # missed the boat
                cleanup_and_unblock_connection(c);
            }
        }
    }
}

# called every time we get a client hello 
# we check the client random to see if it is tagged
event ssl_conn_attempt(c: connection, version: count,
                       ciphers: cipher_suites_list, rand: string, client_random: string) {

    # check if we are already tracking this connection
    if (c$id in telex_table) {
        print fmt("already in table");
    } else {
        local tag_context : string = telex_get_context(c$id$resp_h, sub_bytes(client_random, 1, 4), "\x00");
        local shared_key : telex_key_info;

        local is_tagged = telex_tag_get_key(rand, tag_context, shared_key);

        local diff = get_ssl_nonce_timestamp(client_random) - current_time();

        if (is_tagged == F || (diff > 60 sec) || (diff < -60 sec)) {
            # client hello not tagged
            #print fmt("untagged connection (%s - %s = %s)", 
            #            get_ssl_nonce_timestamp(client_random), current_time(), diff);
            skip_further_processing(c$id); 

        } else {
            # Client hello is tagged
            local te: telex_info;

            # initialize the telex_info struct
            te$key_info = shared_key;
            te$dh$client_random = client_random;
            te$tcp$cur_ts$tsval = 0;
            te$tcp$cur_ts$tsecr = 0;
            te$tcp$last_ts$tsval = 0;
            te$tcp$last_ts$tsecr = 0; 
            te$tcp$ts_option = F;
            te$tcp$sack_option = F;
            te$tcp$seq = 0;
            te$tcp$ack = 0;
            te$tcp$rcv_window = 0;
            te$tcp$rcv_window_scale = 0;
            te$tcp$snd_window = 0;
            te$tcp$snd_window_scale = 0;
            te$last_ttl = 0;
            te$last_ip_id = 0;
            te$sock = -1;

            te$state = 1;   # waiting for cipher_spec_change 

            te$do_fwd = T;

            telex_mod_block(c$id, T);
            te$fwd_time = current_time();

            # move to ssl_cipher_change??
            telex_init_bronnect(te, bronnect_socket);  # assigns te$sock for us

            telex_table[c$id] = te;
        }
    }
}

event ssl_conn_server_reply(c: connection, version: count, ciphers: cipher_suites_list, server_random: string, compression_method: count) {
    if (c$id in telex_table) {
        local te = telex_table[c$id];

        te$dh$server_random = server_random;
        #te$dh$comp_method = compression_method;
    }
}

# mostly here for debugging - we should normally cleanup in new_packet_final, but
# in case we missed something, app_data should be our notice to cleanup
function ssl_app_data(c: connection, is_orig: bool, data: string) {

    if (c$id in telex_table) {
        local te = telex_table[c$id];

        if (te$sock == -1) {
            print "telex socket not set!";
        } else {
            print fmt("warn: port %d got app data in state %d", c$id$orig_p, te$state);
            cleanup_and_unblock_connection(c);
        }
    }
}

function ssl_change_cipher(c: connection, is_orig: bool, data: string) {
    if (c$id in telex_table) {
        local te = telex_table[c$id];

        if (te$state == 1) {        # waiting for cipher_spec_change
            if (is_orig == T) {
                te$state = 2;
            } else { 
                # either we missed the client cipher_spec_change or 
                # this is session resumption (which we don't yet support)
                cleanup_and_unblock_connection(c);
                return;
            }
        } else if (te$state == 2 && is_orig == F) { # waiting for server
            te$state = 4; # received both change cipher specs.
        } else {
            print fmt("warning: bad state in ssl_change_cipher: %d", te$state);
            cleanup_and_unblock_connection(c);
            return;
        }
    }
}

function get_iv_from_handshake(data: string) : string {
    return sub_bytes(data, byte_len(data)-15, 16);
}

function ssl_handshake(c: connection, is_orig: bool, data: string) { 
    if (c$id in telex_table) {
        local te = telex_table[c$id];
         
        # We grab the IV from each sides' finished message. AES-CBC's IV is
        # just the last ciphertext sent (in our case, the finished message).
        if (te$state >= 2 && is_orig==T) {
            # client finished   
            te$dh$client_iv = get_iv_from_handshake(data); 

            # we'll set the tcp state here, after receving last packet from client,
            # so that bronnect can call a send on the server's finished message
            # that way, bronnect's tcp stack will retransmit if that forward packet
            # is dropped.
            te$tcp$ack = c$orig$last_seq;   #can these numbers be wrong somehow??
            te$tcp$seq = c$resp$last_seq;
            te$tcp$snd_window = rshift(c$resp$last_window, c$resp$last_window_scale);
            te$tcp$snd_window_scale = c$resp$last_window_scale;
            te$tcp$rcv_window = rshift(c$orig$last_window, c$orig$last_window_scale);
            te$tcp$rcv_window_scale = c$orig$last_window_scale;

            if (te$tcp$cur_ts$tsval == 0) {
                print fmt("Error: port %d has 0 tsval", c$id$orig_p);
            }

            te$last_ip_id = c$resp$last_IP_ID;
            te$last_ttl = c$resp$last_TTL;

        } else if (te$state >= 3 && is_orig==F) {
            # server finished
            te$dh$server_iv = get_iv_from_handshake(data);
        }

        # Once we get both iv's, we can send init data to bronnect
        if (te$state == 4) { 
            if (telex_send_init_data(c$id, te) != 0) {
              cleanup_and_unblock_connection(c);
            } else {
              te$state = 5; # init_data sent to bronnect
            }
        }
    }
}

# store the DH parameters
event ssl_server_key_exchange(c: connection, p: string, g: string, pub_key: string) {
    if (c$id in telex_table) {
        local te = telex_table[c$id];

        te$dh$p = p;
        te$dh$g = g;
        te$dh$pub_key = pub_key;
    }
}

event ssl_record(c: connection, is_orig: bool, typ: int, data: string) {
    if (typ == 23) {
        ssl_app_data(c, is_orig, data);         # app data
    } else if (typ == 20) {
        ssl_change_cipher(c, is_orig, data);    # change cipher spec
    } else if (typ == 22) {
        ssl_handshake(c, is_orig, data);        # handshake
    }
}

event tcp_option(c: connection, is_orig: bool, opt: count, 
                 optlen: count, option: string) {
    if (c$id in telex_table && (opt == 8 || opt == 4)) {
        local te = telex_table[c$id];
    } else {
        return;
    }
    if (opt == 8) {
        #tcp timestamp
        #store in telex_table
        if (!is_orig) {
            local tsval = extract_tcp_timestamp(option, F);
            te$tcp$ts_option = T;
            if (te$tcp$cur_ts$tsval != tsval) {
                te$tcp$last_ts = copy(te$tcp$cur_ts);
                te$tcp$cur_ts$recv_time = network_time();
                te$tcp$cur_ts$tsval = tsval;
                te$tcp$cur_ts$tsecr = extract_tcp_timestamp(option, T);
            }
        }
    } else if (opt == 4) {
      # SACK permitted.
      if (is_orig) {
        te$tcp$sack_option = T;
      }
    }
}

event bro_init() {
    print fmt("%s", bronnect_socket);
    print fmt("%s", privkey_filename);
    telex_init(privkey_filename);
}

