#!/bin/bash

#taskset -p 0x01 
ulimit -c unlimited
/usr/local/bro/bin/bro -i eth1 -f "ether[11]!=7 and tcp port 443" \
    -e 'bronnect_socket = "/tmp/bro-bronnect"' --use-binpac ./site/telex-ssl.bro
