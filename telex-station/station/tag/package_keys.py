#!/usr/bin/python

import sys

if (len(sys.argv) == 1 or (len(sys.argv)%2) != 1):
    print "Packages multiple private keys into the Telex privkeys file format"
    print "See brohome/README section on privkeys file for more information"
    print "Usage:\n\t%s key1 port1 key2 port2 [..] keyn portn > privkeys" \
             % sys.argv[0]
    exit(1)

# Little endian
def num_to_uint16_t_str(num):
    return chr(num%256) + chr(num/256)


num_keys = len(sys.argv)/2

sys.stdout.write(num_to_uint16_t_str(num_keys))

for i in range(num_keys):
    key_fn = sys.argv[2*i+1]
    port   = int(sys.argv[2*i+2])
    #print "{%s, %d}" % (key_fn, port)
    try:
        f=open(key_fn, 'rb')
        sys.stdout.write(f.readlines()[0])
        sys.stdout.write(num_to_uint16_t_str(port))
        f.close()
    except IOError:
        sys.stderr.write("Error: %s\n" % key_fn)
 
