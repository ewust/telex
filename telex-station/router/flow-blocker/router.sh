#!/bin/bash

# remove reverse-path filtering (allow IP spoofing from Station)
/bin/echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
/bin/echo 0 > /proc/sys/net/ipv4/conf/eth2/rp_filter

/usr/bin/pkill flow_blocker
/sbin/iptables -t mangle -F
/sbin/iptables -F
/usr/local/sbin/ipset -X OURSET
/usr/local/sbin/ipset -N OURSET ipportiphash

# add both of bro's IPs to an ipset, so we don't send packets to bro twice
/usr/local/sbin/ipset -X BRO
/usr/local/sbin/ipset -N BRO iphash
/usr/local/sbin/ipset -A BRO 10.4.0.2
/usr/local/sbin/ipset -A BRO 141.212.109.159    # replace with public IP of Telex-station

/sbin/iptables-restore < ./router.fw

#iptables -A FORWARD -p tcp -m physdev --physdev-in eth2 -j ACCEPT
#iptables -A FORWARD -p tcp --sport 443 -m set --match-set OURSET src,dst,dst -j DROP
#iptables -A FORWARD -p tcp --dport 443 -m set --match-set OURSET dst,src,src -j DROP
#iptables -t mangle -A PREROUTING -m physdev ! --physdev-in eth2 -m set ! --match-set BRO dst -j TEE --gateway 10.4.0.2
##iptables -t mangle -A PREROUTING -i eth2 -j MARK --set-mark 1

#if pgrep flow_blocker; then
#    pkill flow_blocker;
#    sleep 90 
#fi
#CPUPROFILE=/tmp/fb.prof nohup ./flow_blocker 10.4.0.1 4501 < /dev/null > /dev/null &
nohup ~/telex-station/router/flow-blocker/flow_blocker 10.4.0.1 4501 < /dev/null > /dev/null &
/bin/echo "router.sh exiting"
exit 0
