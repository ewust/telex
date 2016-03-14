#include <event2/dns.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/event.h>

#include <argtable2.h>

#include "listener.h"
#include "proxy.h"
#include "state.h"
#include "logger.h"
#include "ssl.h"

int telex_server(int listen_port, int local_service_port, int local_telex_port,
                 const char *keyfile, const char *dhfile, const char *privkey_fn)
{
	LogOutputStream( stdout );
	LogOutputStream( stdout );
	LogOutputLevel(9);  

    struct telex_conf conf;
    memset(&conf, 0, sizeof(struct telex_conf));
    conf.keyfile = keyfile;
    conf.dh_file = dhfile;
    tag_init();
    tag_load_privkey(privkey_fn, conf.privkey);
    
    conf.untag_port = local_service_port;
    conf.tag_port = local_telex_port;

    if (init_ssl(&conf) < 0) {
        fprintf(stderr, "Could not init OpenSSL\n");
        return 1;
    }
   
    InitAndListenLoop(listen_port, (evconnlistener_cb)tcp_accept_cb, &conf); 
    return 0;
}

int main(int argc, char *argv[])
{ 
	struct arg_int *lport = arg_int0("p", "port", "<localport>",  "listening port (default is 443)");
	struct arg_int *debug = arg_int0("d", "debug",  "<level>", "debug output level (default is 3)");
	struct arg_str *key   = arg_str0("k", "key",    "<keyfile>", "private key file (default is privkey)");
	struct arg_str *cert= arg_str0(NULL, "cert",    "<certfile>", "Server private key and cert (default is server.pem)");
	struct arg_lit *help  = arg_lit0(NULL,"help",              "print this help and exit");
	struct arg_str *service_p = arg_str1(NULL, NULL,    "[host]:<port>",  "default service ");
	struct arg_str *telex_p  = arg_str1(NULL, NULL,    "[host]:<port>",  "telex service ");
	struct arg_end *end   = arg_end(20);
	void *argtable[] = {lport, debug, key, certdb, help, host, end};
	const char* progname = "telex-client";
    int nerrors;
    int ret=0;
    assert(!arg_nullcheck(argtable));

	// defaults:
	lport->ival[0] = 443;
	debug->ival[0] = 3;
	key->sval[0] = "privkey";
	cert->sval[0] = "root.pem";

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

      int port;
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

    // TODO: read args
    telex_server(lport->ival[0], 8080, 8081, "server.pem", "dh1024.pem", "privkey");
    
    arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));
    return 0;
}


