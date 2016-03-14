# Create a set with timeout
0 ipset create test hash:ip,port,net timeout 5
# Add partly zero valued element
0 ipset add test 2.0.0.1,0,192.168.0.0/24
# Test partly zero valued element
0 ipset test test 2.0.0.1,0,192.168.0.0/24
# Delete partly zero valued element
0 ipset del test 2.0.0.1,0,192.168.0.0/24
# Add first random value
0 ipset add test 2.0.0.1,5,192.168.0.0/24
# Add second random value
0 ipset add test 2.1.0.0,128,10.0.0.0/16
# Test first random value
0 ipset test test 2.0.0.1,5,192.168.0.0/24
# Test second random value
0 ipset test test 2.1.0.0,128,10.0.0.0/16
# Test value not added to the set
1 ipset test test 2.0.0.1,4,10.0.0.0/16
# Delete value not added to the set
1 ipset del test 2.0.0.1,6,10.0.0.0/16
# Test value before first random value
1 ipset test test 2.0.0.0,5,192.168.0.0/24
# Test value after second random value
1 ipset test test 2.1.0.1,128,10.0.0.0/16
# Try to add value before first random value
0 ipset add test 2.0.0.0,5,192.168.0.0/25
# Try to add value after second random value
0 ipset add test 2.1.0.1,128,10.0.0.0/17
# List set
0 ipset list test | sed 's/timeout ./timeout x/' > .foo0 && ./sort.sh .foo0
# Check listing
0 diff -I 'Size in memory.*' .foo hash:ip,port,net.t.list0 && rm .foo
# Sleep 5s so that elements can time out
0 sleep 5
# List set
0 n=`ipset save test|wc -l` && test $n -eq 1
# Flush test set
0 ipset flush test
# Delete set
0 ipset destroy test
# eof
