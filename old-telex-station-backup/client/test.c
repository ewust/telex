#include "ssl_api.h"
#include "../util/telex_util.h"

int main()
{
    SSL *ssl_client, *ssl_server;
    char *msg = "hello, world!\n";
    struct secrets rsecret, wsecret;

    char *out;
    char *decrypt;
    int out_len;
    char *key = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRS";
    char *iv = "0123456789abcdef";
    char *mac_secret = "shhhkeep this_secret!1one";
    

    msg = malloc(1000);
    memset(msg, '$', 1000);
    msg[1000-1] = 0;



    memcpy(rsecret.key, key, 32);
    memcpy(wsecret.key, key, 32);

    memcpy(rsecret.iv, iv, 16);
    memcpy(wsecret.iv, iv, 16);

    memcpy(rsecret.mac_secret, mac_secret, 20);
    memcpy(wsecret.mac_secret, mac_secret, 20);

    char secret[] = "this sure is a secret";
    char secret2[] = "this sure is a secret";
    ssl_client = NULL; //FIXME get_live_ssl_obj(secret, strlen(secret), 0);
    ssl_server = NULL; //FIXME get_live_ssl_obj(secret2, strlen(secret2), 1);
 
    out_len = ssl_encrypt(ssl_client, msg, strlen(msg), &out);

    printf("encryption gives us %d bytes:\n", out_len);
    hexdump(out, out_len);
    printf("\n");


    out_len = ssl_decrypt(ssl_server, out, out_len, &decrypt);

    printf("decryption1: %d bytes: %s\n", out_len, decrypt);    

    free(out);
    free(decrypt);


    out_len = ssl_encrypt(ssl_client, msg, strlen(msg), &out);
    printf("encryption2: gives us %d bytes:\n", out_len);
    hexdump(out, out_len);
    printf("\n");

    
    out_len = ssl_decrypt(ssl_server, out, out_len, &decrypt);

    printf("decryption2: %d bytes: %s\n", out_len, decrypt);

    free(out);
    free(decrypt);

    BIGNUM *bn;
    int i;
    bn = telex_ssl_get_dh_key("this is some telex_secret, 16 bytes", NULL);
    for (i=0; i<bn->top; i++) {
        printf("%016lx", bn->d[i]); 
    }
    printf("\n");
    free(bn);

    bn = telex_ssl_get_dh_key("this is some telex_secret, 16 bytes", NULL);
    for (i=0; i<bn->top; i++) {
        printf("%016lx", bn->d[i]); 
    }
    printf("\n");
    free(bn);

    


    return 0;

}
