CC=gcc
OPENSSL=/usr/local/ssl
CFLAGS=-g -I$(OPENSSL)/include -Wall -O2
LD=-L$(OPENSSL)/lib64 -lssl -lcrypto -ldl

OBJS=common.o ssl_api.o ../util/telex_util.c

all: telex_client test telex_tunnel

telex_client: telex_client.o client.o $(OBJS)
	$(CC) telex_client.o client.o $(OBJS) -o telex_client $(LD)

telex_send_hello: telex_send_hello.c client.o libtelextunnel.c $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) -I../tag $(LD) -lgmp \
	../tag/tag.o ../tag/ptwist.o ../tag/relic-0.2.3/build/lib/librelic_s.a

telex_tunnel: telex_tunnel.c client.o server.o libtelextunnel.c $(OBJS)
	$(CC) $(LD) $^ -o $@ $(CFLAGS) -DWATCHDOG -I../tag -lgmp -levent  \
	`pkg-config --cflags --libs glib-2.0` \
	../tag/tag.o ../tag/ptwist168.o

test: ssl_api.h test.o $(OBJS)
	$(CC) test.o $(OBJS) -o test $(LD)

clean:	
	rm *.o telex_client telex_send_hello
