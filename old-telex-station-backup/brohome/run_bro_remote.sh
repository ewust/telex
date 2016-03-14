#!/bin/bash

#taskset -p 0x01 
nohup /usr/local/bro/bin/bro -i eth1 -f 'ether[11]!=7 and tcp port 443' \
    -e 'bronnect_socket = "/tmp/bro-bronnect"; privkey_filename = "privkeys.0.0.2";' \
    --use-binpac ./site/telex-ssl.bro 2>> bro.out >> bro.stdout& 

#</dev/null 2>&1 > /dev/null &
