# Create a set with timeout
0 ipset create test hash:ip,port family inet6 timeout 5
# Add partly zero valued element
0 ipset add test 2:0:0::1,0
# Test partly zero valued element
0 ipset test test 2:0:0::1,0
# Delete partly zero valued element
0 ipset del test 2:0:0::1,0
# Add first random value
0 ipset add test 2:0:0::1,5
# Add second random value
0 ipset add test 2:1:0::0,128
# Test first random value
0 ipset test test 2:0:0::1,5
# Test second random value
0 ipset test test 2:1:0::0,128
# Test value not added to the set
1 ipset test test 2:0:0::1,4
# Delete value not added to the set
1 ipset del test 2:0:0::1,6
# Test value before first random value
1 ipset test test 2:0:0::0,5
# Test value after second random value
1 ipset test test 2:1:0::1,128
# Try to add value before first random value
0 ipset add test 2:0:0::0,5
# Try to add value after second random value
0 ipset add test 2:1:0::1,128
# List set
0 ipset list test | sed 's/timeout ./timeout x/' > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:ip6,port.t.list0 && rm .foo
# Sleep 5s so that elements can time out
0 sleep 5
# List set
0 ipset list test > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:ip6,port.t.list1 && rm .foo
# Flush test set
0 ipset flush test
# Add multiple elements in one step
0 ipset add test 1::1,udp:80-85
# Delete multiple elements in one step
0 ipset del test 1::1,udp:81-84
# Check elements after multiple add/del
0 ipset test test 1::1,udp:80
# Delete test set
0 ipset destroy test
# eof
