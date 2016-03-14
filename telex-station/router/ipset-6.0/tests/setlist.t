# Create dummy set
0 ipset -N dummy list:set
# Create base set foo
0 ipset -N foo ipmap --from 2.0.0.1 --to 2.1.0.0
# Create base set bar
0 ipset -N bar iphash
# Create setlist kind of set
0 ipset -N test setlist
# Swap test and dumy sets
0 ipset -W test dummy
# Destroy dummy set
0 ipset -X dummy
# Add foo set to setlist
0 ipset -A test foo
# Test foo set in setlist
0 ipset -T test foo
# Test nonexistent set in setlist
1 ipset -T test nonexistent
# Try to delete foo set
1 ipset -X foo
# Add bar set to setlist, after foo
0 ipset -A test bar
# Test bar,after,foo
0 ipset -T test bar,after,foo
# Test foo,before,bar
0 ipset -T test foo,before,bar
# Test bar,before,foo
1 ipset -T test bar,before,foo
# Test foo,after,bar
1 ipset -T test foo,after,bar
# Save sets
0 ipset -S > setlist.t.restore
# Delete bar,before,foo
1 ipset -D test bar,before,foo
# Delete foo,after,bar
1 ipset -D test foo,after,bar
# Delete bar,after,foo
0 ipset -D test bar,after,foo
# Flush test set
0 ipset -F test
# Delete test set
0 ipset -X test
# Delete all sets
0 ipset -X
# Restore saved sets
0 ipset -R < setlist.t.restore
# List set
0 ipset -L test > .foo
# Check listing
0 diff -I 'Size in memory.*' .foo setlist.t.list0 && rm .foo
# Flush all sets
0 ipset -F
# Delete all sets
0 ipset -X && rm setlist.t.restore
# eof
