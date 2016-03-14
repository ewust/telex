CC= gcc
FLAGS= -lpcap -g 
OPENSSL=/home/ewust/local/
CFLAGS=-g -I$(OPENSSL)/include -Wall #-O2
#CFLAGS=-g -Wall
LD=-L$(OPENSSL)/lib -lssl -lcrypto -ldl
#LD=-lssl -lcrypto -ldl
WATCHDOG=-DWATCHDOG

all: flow_blocker


flow_blocker: telex_block_flow.* flow_blocker.*
	$(CC) $(FLAGS) -I ../util ../util/telex_util.c flow_blocker.c telex_block_flow.c telex_ipset.c -o flow_blocker `pkg-config --cflags --libs glib-2.0` -levent -I ../ipset-6.0/src/ -laddipportip

