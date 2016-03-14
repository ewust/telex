CC= gcc
FLAGS= -lpcap -g 
OPENSSL=/home/ewust/local/
CFLAGS=-g -I$(OPENSSL)/include -Wall #-O2
#CFLAGS=-g -Wall
LD=-L$(OPENSSL)/lib -lssl -lcrypto -ldl
#LD=-lssl -lcrypto -ldl
WATCHDOG=-DWATCHDOG

all: bronnect 
#flow_blocker bronnect

#pretty sure this isn't how you're are supposed to do things...
TAG_FLAGS=-I../tag/ -I../tag/relic-0.2.3/include -I../tag/relic-0.2.3/build/include \
	../tag/relic-0.2.3/build/lib/librelic_s.a  \
	-L../tag/relic-0.2.3/build/lib -lrelic_s -lgmp

TAG_FILES=../tag/tag.o ../tag/ptwist.o

../tag/tag.o: ../tag/tag.c
	cd ../tag/ && make tag && cd ../net/

bron: bron_job.c
	$(CC) $(FLAGS) bron_job.c -o bron

flow_blocker: telex_block_flow.* flow_blocker.*
	$(CC) $(FLAGS) -I ../util ../util/telex_util.c flow_blocker.c telex_block_flow.c telex_ipset.c -o flow_blocker `pkg-config --cflags --libs glib-2.0` -levent -I ../ipset-6.0/src/ -laddipportip


bronnect: bronnect.c bronnect.h client.c common.c common.h ssl_api.c ssl_api.h client.h telex_tcp.h telex_tcp.c telex_block_flow.c telex_block_flow.h ../util/telex_util.h httpd.c httpd.h watchdog.h watchdog.c
	$(CC) $(FLAGS) `pkg-config --cflags --libs glib-2.0` $(WATCHDOG) \
        bronnect.c client.c common.c ssl_api.c telex_tcp.c telex_block_flow.c \
        ../util/telex_util.c httpd.c watchdog.c -o bronnect $(LD) $(CFLAGS) -levent \
        -I /opt/linux-headers-2.6.37/include -I ../util -I ../ipset-6.0/src/

install: bronnect
	cp ./bronnect /usr/local/bin/bronnect

ifcp: ifcp.c
	$(CC) $(FLAGS) ifcp.c -o ifcp

test: test.c 
	$(CC) $(FLAGS) `pkg-config --cflags --libs glib-2.0` test.c -o test $(LD) $(CFLAGS) -levent
