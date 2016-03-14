#!/bin/bash

ulimit -c unlimited
rm -f /tmp/bro-bronnect
#valgrind -v --tool=memcheck --track-origins=yes --leak-check=full 
LD_LIBRARY_PATH=/home/ewust/local/lib /usr/local/bin/bronnect 127.0.0.1 
#> /var/log/telex/bronnect.err
                             # 141.212.109.224 80
