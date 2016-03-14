INCLUDE=-I./req/local/include
LIB=-L./req/local/lib

# Uncomment these 2 lines if you installed openssl/libevent 
# to the default location /usr/local (e.g. no ./configure --prefix=)
#INCLUDE=-I/usr/local/ssl/include -I/usr/local/include
#LIB=-L/usr/local/ssl/lib -L/usr/local/lib

CFLAGS=-g -Wall $(INCLUDE)
LDFLAGS=$(LIB) -lssl -lcrypto -levent_core -levent_extra -levent_openssl -largtable2

OBJS=ssl.o listener.o logger.o util.o proxy.o server.o tag/tag.o tag/ptwist168.o
TARGETS=telex-server

all: $(TARGETS)

tag/*.o:
	$(MAKE) -C tag $@

telex-server: $(OBJS)
	g++ -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: clean
clean:	
	-rm -f $(TARGETS)
	-rm -f $(OBJS)
	$(MAKE) -C tag clean
