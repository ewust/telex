# Range: Create a set
0 ipset -N test ipportnethash -6
# Range: Add zero valued element
1 ipset -A test 2:0:0::1,0,0:0:0::0/0
# Range: Test zero valued element
1 ipset -T test 2:0:0::1,0,0:0:0::0/0
# Range: Delete zero valued element
1 ipset -D test 2:0:0::1,0,0:0:0::0/0
# Range: Add almost zero valued element
0 ipset -A test 2:0:0::1,0,0:0:0::0/24
# Range: Test almost zero valued element
0 ipset -T test 2:0:0::1,0,0:0:0::0/24
# Range: Delete almost zero valued element
0 ipset -D test 2:0:0::1,0,0:0:0::0/24
# Range: Add first random value
0 ipset -A test 2:0:0::1,5,1:1:1::1/24
# Range: Add second random value
0 ipset -A test 2:1:0::0,128,2:2:2::2/12
# Range: Test first random value
0 ipset -T test 2:0:0::1,5,1:1:1::2
# Range: Test second random value
0 ipset -T test 2:1:0::0,128,2:2:2::0
# Range: Test value not added to the set
1 ipset -T test 2:0:0::1,5,2:1:1::255
# Range: Test value not added to the set
1 ipset -T test 2:0:0::1,6,1:1:1::1
# Range: Test value not added to the set
1 ipset -T test 2:0:0::2,6,1:1:1::1
# Range: Test value before first random value
1 ipset -T test 2:0:0::0,5,1:1:1::1
# Range: Test value after second random value
1 ipset -T test 2:1:0::1,128,2:2:2::2
# Range: Try to add value before first random value
0 ipset -A test 2:0:0::0,5,1:1:1::1/24
# Range: Try to add value after second random value
0 ipset -A test 2:1:0::1,128,2:2:2::2/12
# Range: List set
0 ipset -L test > .foo0 && ./sort.sh .foo0
# Range: Check listing
0 diff -I 'Size in memory.*' .foo hash:ip6,port,net6.t.list0 && rm .foo
# Range: Flush test set
0 ipset -F test
# Range: Delete test set
0 ipset -X test
# eof
