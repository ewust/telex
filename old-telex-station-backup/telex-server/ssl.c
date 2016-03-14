#include <openssl/ssl.h>
#include <openssl/err.h>
#include "state.h"
#include "proxy.h"
#include "listener.h"


int ssl_log_errors(enum LogLevel level, const char *name)
{	
	int count = 0;
	int err;
	while ((err = ERR_get_error())) {		
		const char *msg = (const char*)ERR_reason_error_string(err);
		const char *lib = (const char*)ERR_lib_error_string(err);
		const char *func = (const char*)ERR_func_error_string(err);
		LogLog(level, name, "%s in %s %s\n", msg, lib, func);
		count++;
	}
	return count;
}

int init_ssl(struct telex_conf *conf)
{

    SSL_library_init();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    conf->ssl_ctx = SSL_CTX_new(TLSv1_server_method());
    if (!conf->ssl_ctx) {
        fprintf(stderr, "Could not initialize SSL context\n");
        return -1;
    }

    if (SSL_CTX_use_certificate_chain_file(
        conf->ssl_ctx, conf->keyfile) == 0) {

        fprintf(stderr, "Can't read certificate file\n");
        return -1; 
    } 

    if (SSL_CTX_use_PrivateKey_file(conf->ssl_ctx, conf->keyfile, 
                                    SSL_FILETYPE_PEM) == 0) {
        fprintf(stderr, "Can't read keyfile\n");
        return -1;
    }

    // Load the Diffie-Hellman prime and generator
    BIO *bio;
    DH *ret;
    bio = BIO_new_file(conf->dh_file, "r");
    if (!bio) {
        fprintf(stderr, "Couldn't open DH file\n");
        return -1;
    }

    ret = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
    BIO_free(bio);

    if (SSL_CTX_set_tmp_dh(conf->ssl_ctx, ret) < 0) {
        fprintf(stderr, "Couldn't set DH parameters\n");
        return -1;
    }

    return 0;
}

