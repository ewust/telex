#!/bin/bash

#taskset -p 0x01 
nohup /usr/local/bro/bin/bro -i eth0 -f 'ether[11]!=7 and tcp port 443' \
    -e 'bronnect_socket = "/tmp/bro-bronnect"; privkey_filename = "privkeys";' \
    --use-binpac ./site/telex-ssl.bro 2>> bro.out >> bro.stdout& 

# Apologies for the ugly hack (if you find a better way to do this, let me know):
# change ether[11]!=7 to be whatever the last octet in your mac address is for Telex Station
# for example, if your mac address is 00:23:10:70:78:7c
# you would change it to ether[11]!=124  (124 = 0x7c)
# This is to prevent bro from capturing data that it sends out, and only captures
# data sent to it by the router's tap. (Make sure the router's MAC doesn't end in
# the same octet, though. Otherwise, specify more ether[10], ether[9], etc)
