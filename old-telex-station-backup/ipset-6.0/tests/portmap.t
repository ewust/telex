# Range: Create a set from a valid range
0 ipset -N test portmap --from 1 --to 1024
# Range: Add lower boundary
0 ipset -A test 1
# Range: Add upper boundary
0 ipset -A test 1024
# Range: Test lower boundary
0 ipset -T test 1
# Range: Test upper boundary
0 ipset -T test 1024
# Range: Test value not added to the set
1 ipset -T test 1023
# Range: Test value before lower boundary
1 ipset -T test 0
# Range: Test value after upper boundary
1 ipset -T test 1025
# Range: Try to add value before lower boundary
1 ipset -A test 0
# Range: Try to add value after upper boundary
1 ipset -A test 1025
# Range: Delete element not added to the set
1 ipset -D test 567
# Range: Add element in the middle
0 ipset -A test 567
# Range: Delete the same element
0 ipset -D test 567
# Range: List set
0 ipset -L test > .foo
# Range: Check listing
0 diff -I 'Size in memory.*' .foo portmap.t.list0 && rm .foo
# Range: Flush test set
0 ipset -F test
# Range: Delete test set
0 ipset -X test
# Full: Create a full set of ports
0 ipset -N test portmap --from 0 --to 65535
# Full: Add lower boundary
0 ipset -A test 0
# Full: Add upper boundary
0 ipset -A test 65535
# Full: Test lower boundary
0 ipset -T test 0
# Full: Test upper boundary
0 ipset -T test 65535
# Full: Test value not added to the set
1 ipset -T test 1
# Full: List set
0 ipset -L test > .foo
# Full: Check listing
0 diff -I 'Size in memory.*' .foo portmap.t.list1 && rm .foo
# Full: Flush test set
0 ipset -F test
# Full: Delete test set
0 ipset -X test
# Full: Create a full set of ports and timeout
0 ipset -N test portmap --from 0 --to 65535 timeout 5
# Full: Add lower boundary
0 ipset -A test 0 timeout 5
# Full: Add upper boundary
0 ipset -A test 65535 timeout 0
# Full: Test lower boundary
0 ipset -T test 0
# Full: Test upper boundary
0 ipset -T test 65535
# Full: Test value not added to the set
1 ipset -T test 1
# Full: Add element in the middle
0 ipset -A test 567
# Full: Delete the same element
0 ipset -D test 567
# Full: List set
0 ipset -L test | sed 's/timeout ./timeout x/' > .foo
# Full: Check listing
0 diff -I 'Size in memory.*' .foo portmap.t.list3 && rm .foo
# Full: sleep 5s so that elements can timeout
0 sleep 5
# Full: List set
0 ipset -L test > .foo
# Full: Check listing
# 0 diff -I 'Size in memory.*' .foo portmap.t.list2 && rm .foo
# Full: Flush test set
0 ipset -F test
# Full: Delete test set
0 ipset -X test
# eof
