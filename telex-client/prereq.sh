#!/bin/sh

TELEX_HOME=`pwd`
cd $TELEX_HOME

__exists() {
	which $1 1>/dev/null 2>&1
}

get="fetch";
! __exists fetch && get="curl -OL";

$get http://www.openssl.org/source/openssl-1.0.0d.tar.gz
$get http://monkey.org/~provos/libevent-2.0.12-stable.tar.gz
$get http://prdownloads.sourceforge.net/argtable/argtable2-13.tar.gz
    
# Setting up:
    mkdir -p $TELEX_HOME/req/local
    cd $TELEX_HOME/req

# Building required libraries:

    # patched OpenSSL
    cd $TELEX_HOME/req/
    tar xf ../openssl-1.0.0d.tar.gz
    cd openssl-1.0.0d
    patch -p1 < ../../openssl-1.0.0d-telex.patch
    ./config --shared --prefix=$TELEX_HOME/req/local/ --openssldir=$TELEX_HOME/req/local/
    make
    make install # no root (sudo) needed for local install

    # libevent2, linked against patched OpenSSL
    cd $TELEX_HOME/req/
    tar xf ../libevent-2.0.12-stable.tar.gz 
    cd libevent-2.0.12-stable
    LDFLAGS="-L$TELEX_HOME/req/local/lib" && export LDFLAGS
    CFLAGS="-I$TELEX_HOME/req/local/include" && export CFLAGS
    ./configure --prefix=$TELEX_HOME/req/local
    make
    make install 

    # argtable 2 (libargtable2-dev)
    cd $TELEX_HOME/req/
    tar xf ../argtable2-13.tar.gz
    cd argtable2-13
    ./configure --prefix=$TELEX_HOME/req/local
    make
    make install
    
# Building telex-client:
#cd $TELEX_HOME/
#make
# Running telex-client: 
#LD_LIBRARY_PATH=$TELEX_HOME/req/local/lib ./telex-client notblocked.telex.cc
