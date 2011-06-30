INCLUDE=-I../local/include
LIB=-L../local/lib

# Uncomment these 2 lines if you installed openssl/libevent 
# to the default location /usr/local (e.g. no ./configure --prefix=)
#INCLUDE=-I/usr/local/ssl/include -I/usr/local/include
#LIB=-L/usr/local/ssl/lib -L/usr/local/lib

CFLAGS=-O3 -Wall $(INCLUDE)
LDFLAGS=$(LIB) -lssl -lcrypto -levent_core -levent_extra -levent_openssl -largtable2

OBJS=listener.o logger.o proxy.o ssl.o util.o client.o tag/tag.o tag/ptwist168.o
TARGETS=telex-client

all: $(TARGETS)

tag/*.o:
	$(MAKE) -C tag $@

telex-client: $(OBJS)
	g++ -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: clean
clean:	
	-rm -f $(TARGETS)
	-rm -f $(OBJS)
	$(MAKE) -C tag clean
