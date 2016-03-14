#include "common.h"
#include "client.h"

static char *host=HOST;
static int port=PORT;
static int require_server_auth=0;

int main(int argc, char *argv[])
{
	SSL_CTX *ctx;
	SSL *ssl;
	BIO *sbio;
	int sock;
	extern char *optarg;
	int c;
	char *msg = "HELLO, WORLD!!!\n";

	while((c=getopt(argc,argv,"h:p:i"))!=-1){
		switch(c){
		case 'h':
			if(!(host=strdup(optarg)))
				err_exit("Out of memory");
			break;
		case 'p':
			if(!(port=atoi(optarg)))
				err_exit("Bogus port specified");
			break;
		case 'i':
			require_server_auth=0;
			break;
		}
	}

	/* Build our SSL context*/
	ctx=initialize_ctx(KEYFILE,PASSWORD);

	/* Connect the TCP socket*/
	sock=tcp_connect(host,port);

	/* Connect the SSL socket */
	ssl=SSL_new(ctx);
	sbio=BIO_new_socket(sock,BIO_NOCLOSE);
	SSL_set_bio(ssl,sbio,sbio);

	//set the client random
	ssl->telex_client_random = "hello, world. love, telex...";

	//ssl_connect will call s23_client_hello and send our random
	//as well as perform everything including change cipher spec
	//and encrypted handshakes
	if(SSL_connect(ssl)<=0)
		berr_exit("SSL connect error");
	//if(require_server_auth)
	//	check_cert(ssl,host);
 

	//yay tcp!!    
	send(sock, msg, strlen(msg), 0);

	/* Shutdown the socket */
	destroy_ctx(ctx);
	close(sock);

	return 0;
}
