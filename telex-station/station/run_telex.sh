#!/bin/bash

pkill bro
nohup ~/telex-station/station/net/run_bronnect.sh < /dev/null 2>&1 >> bronnect.err &
cd ~/telex-station/station/brohome
./run_bro_remote.sh
