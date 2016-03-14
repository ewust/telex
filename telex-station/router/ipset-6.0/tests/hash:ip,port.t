# Create a set with timeout
0 ipset create test hash:ip,port timeout 5
# Add partly zero valued element
0 ipset add test 2.0.0.1,0
# Test partly zero valued element
0 ipset test test 2.0.0.1,0
# Delete partly zero valued element
0 ipset del test 2.0.0.1,0
# Add first random value
0 ipset add test 2.0.0.1,5
# Add second random value
0 ipset add test 2.1.0.0,128
# Test first random value
0 ipset test test 2.0.0.1,5
# Test second random value
0 ipset test test 2.1.0.0,128
# Test value not added to the set
1 ipset test test 2.0.0.1,4
# Delete value not added to the set
1 ipset del test 2.0.0.1,6
# Test value before first random value
1 ipset test test 2.0.0.0,5
# Test value after second random value
1 ipset test test 2.1.0.1,128
# Try to add value before first random value
0 ipset add test 2.0.0.0,5
# Try to add value after second random value
0 ipset add test 2.1.0.1,128
# List set
0 ipset list test | sed 's/timeout ./timeout x/' > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:ip,port.t.list0 && rm .foo
# Sleep 5s so that elements can time out
0 sleep 5
# List set
0 ipset list test > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:ip,port.t.list1 && rm .foo
# Flush test set
0 ipset flush test
# Add multiple elements in one step
0 ipset add test 1.1.1.1-1.1.1.4,80-84
# Delete multiple elements in one step
0 ipset del test 1.1.1.2-1.1.1.3,tcp:81-82
# Check number of elements after multi-add/multi-del
0 n=`ipset save test|wc -l` && test $n -eq 17
# Delete test set
0 ipset destroy test
# Create a set
0 ipset create test hash:ip,port
# Add element without specifying protocol
0 ipset add test 2.0.0.1,80
# Add "same" element but with UDP protocol
0 ipset add test 2.0.0.1,udp:80
# Test element without specifying protocol
0 ipset test test 2.0.0.1,80
# Test element with TCP protocol
0 ipset test test 2.0.0.1,tcp:80
# Test element with UDP protocol
0 ipset test test 2.0.0.1,udp:80
# Add element with vrrp
0 ipset add test 2.0.0.1,vrrp:0
# Test element with vrrp
0 ipset test test 2.0.0.1,vrrp:0
# List set
0 ipset list test > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:ip,port.t.list2 && rm .foo
# Delete set
0 ipset destroy test
# eof
