#!/bin/bash

ulimit -c unlimited
rm -f /tmp/bro-bronnect
#valgrind -v --tool=memcheck --track-origins=yes --leak-check=full 
/usr/local/bin/bronnect 127.0.0.1 
#> /var/log/telex/bronnect.err
