CC= gcc
FLAGS= -lpcap -g 
OPENSSL=/home/ewust/local/
CFLAGS=-g -I$(OPENSSL)/include -Wall #-O2
#CFLAGS=-g -Wall
LD=-L$(OPENSSL)/lib -lssl -lcrypto -ldl
#LD=-lssl -lcrypto -ldl
WATCHDOG=-DWATCHDOG

all: bronnect

bronnect: bronnect.c bronnect.h common.c common.h ssl_api.c ssl_api.h telex_tcp.h telex_block_flow.c telex_block_flow.h ../util/telex_util.h httpd.c httpd.h watchdog.h watchdog.c
	$(CC) $(FLAGS) `pkg-config --cflags --libs glib-2.0` $(WATCHDOG) \
        bronnect.c common.c ssl_api.c telex_block_flow.c \
        ../util/telex_util.c httpd.c watchdog.c -o bronnect $(LD) $(CFLAGS) -levent \
        -I /opt/linux-headers-2.6.37/include -I ../util -I ../ipset-6.0/src/

install: bronnect
	cp ./bronnect /usr/local/bin/bronnect

