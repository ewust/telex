# Create a set with timeout
0 ipset create test hash:net,port family inet6 hashsize 128 timeout 5
# Add zero valued element
1 ipset add test ::/0,tcp:8
# Test zero valued element
1 ipset test test ::/0,tcp:8
# Delete zero valued element
1 ipset del test ::/0,tcp:8
# Try to add /0
1 ipset add test 1:1:1::1/0,tcp:8
# Try to add /128
0 ipset add test 1:1:1::1/128,tcp:8 timeout 0
# Add almost zero valued element
0 ipset add test 0:0:0::0/1,tcp:8
# Test almost zero valued element
0 ipset test test 0:0:0::0/1,tcp:8
# Test almost zero valued element with UDP
1 ipset test test 0:0:0::0/1,udp:8
# Delete almost zero valued element
0 ipset del test 0:0:0::0/1,tcp:8
# Test deleted element
1 ipset test test 0:0:0::0/1,tcp:8
# Delete element not added to the set
1 ipset del test 0:0:0::0/1,tcp:8
# Add first random network
0 ipset add test 2:0:0::1/24,tcp:8
# Add second random network
0 ipset add test 192:168:68::69/27,icmpv6:ping
# Test first random value
0 ipset test test 2:0:0::255,tcp:8
# Test second random value
0 ipset test test 192:168:68::95,icmpv6:ping
# Test value not added to the set
1 ipset test test 3:0:0::1,tcp:8
# Try to add IP address
0 ipset add test 3:0:0::1,tcp:8
# List set
0 ipset list test | sed 's/timeout ./timeout x/' > .foo0 && ./sort.sh .foo0
# Save set
0 ipset save test > hash:net6,port.t.restore
# Check listing
0 diff -I 'Size in memory.*' .foo hash:net6,port.t.list0 && rm .foo
# Sleep 5s so that element can time out
0 sleep 5
# IP: List set
0 ipset -L test 2>/dev/null > .foo0 && ./sort.sh .foo0
# IP: Check listing
0 diff -I 'Size in memory.*' .foo hash:net6,port.t.list1 && rm .foo
# Destroy set
0 ipset x test
# Restore set
0 ipset restore < hash:net6,port.t.restore && rm hash:net6,port.t.restore
# List set
0 ipset list test | sed 's/timeout ./timeout x/' > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:net6,port.t.list0 && rm .foo
# Flush test set
0 ipset flush test
# Add multiple elements in one step
0 ipset add test 1::1/64,80-84 timeout 0
# Delete multiple elements in one step
0 ipset del test 1::1/64,tcp:81-82
# Check number of elements after multi-add/multi-del
0 n=`ipset save test|wc -l` && test $n -eq 4
# Delete test set
0 ipset destroy test
# eof
