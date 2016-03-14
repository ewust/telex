# Create a set with timeout
0 ipset create test nethash family inet6 hashsize 128 timeout 5
# Add zero valued element
1 ipset add test ::/0
# Test zero valued element
1 ipset test test ::/0
# Delete zero valued element
1 ipset del test ::/0
# Try to add /0
1 ipset add test 1:1:1::1/0
# Try to add /128
0 ipset add test 1:1:1::1/128
# Add almost zero valued element
0 ipset add test 0:0:0::0/1
# Test almost zero valued element
0 ipset test test 0:0:0::0/1
# Delete almost zero valued element
0 ipset del test 0:0:0::0/1
# Test deleted element
1 ipset test test 0:0:0::0/1
# Delete element not added to the set
1 ipset del test 0:0:0::0/1
# Add first random network
0 ipset add test 2:0:0::1/24
# Add second random network
0 ipset add test 192:168:68::69/27
# Test first random value
0 ipset test test 2:0:0::255
# Test second random value
0 ipset test test 192:168:68::95
# Test value not added to the set
1 ipset test test 3:0:0::1
# Try to add IP address
0 ipset add test 3:0:0::1
# List set
0 ipset list test | sed 's/timeout ./timeout x/' > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:net6.t.list0 && rm .foo
# Sleep 5s so that element can time out
0 sleep 5
# IP: List set
0 ipset -L test 2>/dev/null > .foo0 && ./sort.sh .foo0
# IP: Check listing
0 diff -I 'Size in memory.*' .foo hash:net6.t.list1 && rm .foo
# Flush test set
0 ipset flush test
# Delete test set
0 ipset destroy test
# eof
