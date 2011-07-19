#!/usr/bin/env python

# Converts binary files into C arrays
# usage: bin2c.py fileA varA [fileB varB] ...
import sys
arg = iter(sys.argv[1:])
for f in arg:
    print 'static const char ' + arg.next() + '[] = "' + \
        ''.join([('\\x%02x' % ord(x)) for x in open(f,'rb').read()]) + '";'
