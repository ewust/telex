#!/usr/bin/python

import sys

if (len(sys.argv) != 1):
    print "Usage: %s" % sys.argv[0]
    print "   Prints the cdf of a sorted column of data from stdin (one number per line)."
    print "   Outputs on stdout. "
    print "   Use gnuplot: `plot 'file' with lines` to plot"
    exit()


times = [int(x) for x in sys.stdin.readlines()[1:]]

last = times[0]

for i in range(len(times)):
    if (last != times[i]):
        print "%d    %f" % (last, float(i)/len(times))
    last = times[i] 


print "%d   %f" % (last, 1.0)
        
