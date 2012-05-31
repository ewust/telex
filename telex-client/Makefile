INCLUDE=-I./req/local/include
LIB=-L./req/local/lib

# Uncomment these 2 lines if you installed openssl/libevent 
# to the default location /usr/local (e.g. no ./configure --prefix=)
#INCLUDE=-I/usr/local/ssl/include -I/usr/local/include
#LIB=-L/usr/local/ssl/lib -L/usr/local/lib

# Hardening and warnings for building with gcc
GCCWARNINGS = -Wall -fno-strict-aliasing -W -Wfloat-equal -Wundef      \
-Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes              \
-Wwrite-strings -Wredundant-decls -Wchar-subscripts -Wcomment          \
-Wformat=2 -Wwrite-strings -Wmissing-declarations -Wredundant-decls    \
-Wnested-externs -Wbad-function-cast -Wswitch-enum -Winit-self         \
-Wmissing-field-initializers              \
-Wold-style-definition -Waddress -Wmissing-noreturn -Wnormalized=id    \
-Woverride-init -Wstrict-overflow=1 -Wextra -Warray-bounds             \
-Wstack-protector -Wformat -Wformat-security -Wpointer-sign
GCCHARDENING=-D_FORTIFY_SOURCE=2 -fstack-protector-all -fwrapv -fPIC --param ssp-buffer-size=1
LDHARDENING=-z relro -z now

EXTRACFLAGS=-g -O2 $(EXTRA_CFLAGS) $(GCCHARDENING) $(GCCWARNINGS) -Werror
EXTRALDFLAGS= $(LDHARDENING)

CFLAGS+=-Wall $(INCLUDE) $(EXTRACFLAGS)
LDFLAGS+=$(LIB) $(EXTRALDFLAGS) -lssl -lcrypto -levent_core -levent_extra -levent_openssl -largtable2

OBJS=listener.o logger.o proxy.o ssl.o util.o client.o tag/tag.o tag/ptwist168.o
TARGETS=telex-client

all: $(TARGETS)

tag/*.o:
	$(MAKE) -C tag $@

telex-client: $(OBJS)
	$(CXX) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: clean
clean:	
	-rm -f $(TARGETS)
	-rm -f $(OBJS)
	$(MAKE) -C tag clean

veryclean:
	rm -rf ./req/
	rm *.tar.gz
