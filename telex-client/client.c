#include <event2/dns.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/event.h>

#include <argtable2.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "listener.h"
#include "state.h"
#include "proxy.h"
#include "logger.h"
#include "ssl.h"

static int telex_client(int listen_port, int remote_port, int debug_level, const char *remote_host, 
		 const char *keyfile, const char *cafile)
{
	if (debug_level >= 0) {
		LogOutputStream( stdout );
		LogOutputStream( stdout );
		LogOutputLevel( debug_level );  
	}

	struct telex_conf conf;
	memset(&conf, 0, sizeof(struct telex_conf));
	conf.notblocked_port = remote_port;
	conf.notblocked_host = remote_host;	
	conf.ca_list = (char *)cafile;
	conf.keyfile = (char *)keyfile;

	// counters
	conf.count_tunnels = 0;
	conf.count_open_tunnels = 0;

	if (ssl_init(&conf) < 0) {
		LogFatal("main", "Could not initialize OpenSSL");				
		return 1;
	}

	int ret = InitAndListenLoop(listen_port, (evconnlistener_cb)proxy_accept_cb, &conf);

	if (conf.ssl_ctx) {
		ssl_done(&conf);
	}
	if (conf.dns_base) {
		evdns_base_free(conf.dns_base, 1);
	}
	return ret;
}

int main(int argc, char *argv[])
{	
	struct arg_int *lport = arg_int0("p", "port", "<localport>",  "listening port (default is 8888)");
	struct arg_int *debug = arg_int0("d", "debug",  "<level>", "debug output level (default is 4)");	
#ifdef PUBKEY_DATA_
	struct arg_str *key   = arg_str0("k", "key",    "<keyfile>", "public key file (default is built-in)");
#else
	struct arg_str *key   = arg_str0("k", "key",    "<keyfile>", "public key file (default is pubkey)");
#endif
#ifdef ROOTPEM_DATA_
	struct arg_str *certdb= arg_str0(NULL, "certdb",    "<certfile>", "trusted CA database (default is built-in)");
#else
	struct arg_str *certdb= arg_str0(NULL, "certdb",    "<certfile>", "trusted CA database (default is root.pem)");
#endif
	struct arg_lit *help  = arg_lit0(NULL,"help",              "print this help and exit");
	struct arg_str *host  = arg_str1(NULL, NULL,    "<host>[:port]",  "non-blocked TLS server");
	struct arg_end *end   = arg_end(20);
	void *argtable[] = {lport, debug, key, certdb, help, host, end};
	const char* progname = "telex-client";
    int nerrors;
    int ret=0;
    assert(!arg_nullcheck(argtable));

	// defaults:
	lport->ival[0] = 8888;
	debug->ival[0] = 3;
#ifdef PUBKEY_DATA_
	key->sval[0] = NULL;
#else
	key->sval[0] = "pubkey";
#endif
#ifdef ROOTPEM_DATA_
	certdb->sval[0] = NULL;
#else
	certdb->sval[0] = "root.pem";
#endif

	nerrors = arg_parse(argc,argv,argtable);
    if (help->count > 0) {
		printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        printf("\nEstablishes covert, encrypted tunnels, disguised as connections to <host>.\n\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        printf("\n");		
		ret = 0;
	} else if (nerrors > 0) {
		arg_print_errors(stdout,end,progname);
		printf("Try '%s --help' for more information.\n", progname);
		ret = 1;
	} else if (argc == 1) {
        printf("Try '%s --help' for more information.\n", progname);
        ret = 0;
	} else {		

      int port = 443;
      char hstr[255];
      assert(host->sval[0]);
      strncpy(hstr, host->sval[0], sizeof(hstr)-1);

      char *pstr=0;
      strtok(hstr, ":");
      pstr = strtok(NULL, ":");
      if (pstr) {
	port = strtol(pstr, NULL, 10);
	if (port < 1 || port > 65535) {
	  fprintf(stderr, "Invalid remote port: %d", port);
	  return 1;
	}
      }

		printf("WARNING: This software is an experimental prototype intended for\n");
        printf("         researchers. It does not provide strong security and is\n");
        printf("         UNSAFE FOR REAL-WORLD USE. For details of current limitations\n");
        printf("         of our proof-of-concept, please see telex-client/ISSUES.\n");

      ret = telex_client(lport->ival[0], port, debug->ival[0], hstr, key->sval[0], certdb->sval[0]);
    }
    arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));
    return ret;
}
