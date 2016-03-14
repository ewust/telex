@load ssl

type telex_info: record {
	shared_key: string;
};

global telex_table: table[conn_id] of telex_info; 

redef ssl_compare_cipherspecs = F;

redef ssl_analyze_certificates = F;

redef ssl_verify_certificates = F;

redef ssl_log_ciphers = F;

event ssl_conn_attempt(c: connection, version: count,
                       ciphers: cipher_suites_list, rand: string) {
  print fmt("ClientRandom from %s:%d -> %s:%d:\n\t %s\n", 
		c$id$orig_h, c$id$orig_p, c$id$resp_h, c$id$resp_p, rand);
  
  if (c$id in telex_table) {
    print fmt("already in table\n");
  } else {
    if (telex_tag_get_key(rand) != "") {
      print fmt("it's even tagged!\n");
      local te : telex_info;
      te$shared_key = rand;
      telex_table[c$id] = te;
    }
  }
}

event ssl_application_data(c: connection, data: string) {

}

function ssl_app_data(c: connection, is_orig: bool, data: string) {

  if (c$id in telex_table){
    print fmt("In table! key: %s\n", telex_table[c$id]$shared_key);
    print fmt("Application Data: %s:%d -> %s:%d\n", 
		c$id$orig_h, c$id$orig_p, c$id$resp_h, c$id$resp_p);
  } else {
    print fmt("Not in table %s:%d -> %s:%d\n",
		c$id$orig_h, c$id$orig_p, c$id$resp_h, c$id$resp_p);
  }
}

event protocol_violation(c: connection, atype: count, aid: count, reason: string) {
  print fmt("Protocol violation: %s", reason);
}

event ssl_client_key_exchange(c: connection, data: string) {
  print fmt("Client key exchange: %s", data);
}

event ssl_record(c: connection, is_orig: bool, typ: int, data: string) {
  print fmt("Raw SSL record. %s:%d -> %s:%d\n\tOrig: %d Type: %d",
		c$id$orig_h, c$id$orig_p, c$id$resp_h, c$id$resp_p, is_orig, typ);

#  print fmt("last IP ID: %d", c$last_ip_id); # doesn't work.
  if (typ == 23) {
    #application data
    ssl_app_data(c, is_orig, data);
  }

}

event ssl_change_cipher_spec(c: connection) {
  print "ChangeCipherSpec received.";
}

event ssl_state_change(c: connection, olds: string, news: string) {
  print fmt("SSL state change: %s -> %s", olds, news);
}
#event ssl_conn_established(c: connection, version: count, cipher: string

event bro_init() {
    telex_init();
}
