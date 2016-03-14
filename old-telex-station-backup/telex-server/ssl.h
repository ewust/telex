#ifndef _SSL_H
#define _SSL_H


int ssl_log_errors(enum LogLevel level, const char *name);
int init_ssl(struct telex_conf *conf);

#endif
