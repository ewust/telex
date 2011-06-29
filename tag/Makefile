CFLAGS=-g -O3 -Wall -I$(INCLUDE) -L$(LIB)

TARGETS=tag.o ptwist168.o genkeys

all: $(TARGETS)

ptwist168: ptwist168.cpp
	g++ $(CFLAGS) -DTESTING -o $@ $^

rserv.o rclient.o genkeys.o ptwist168.o tag.o:: ptwist.h

rserv: rserv.o ptwist168.o
	g++ -g -o $@ $^

rclient: rclient.o ptwist168.o
	gcc -g -o $@ $^ -lssl

genkeys: genkeys.o ptwist168.o
	gcc -g $(CFLAGS) -o $@ $^ -lssl -lcrypto

.PHONY: clean
clean:
	-rm -f *.o

.PHONY: veryclean
veryclean: clean
	-rm -f $(TARGETS)
	-rm -f privkey pubkey tags
