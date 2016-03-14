import couleur
import dpkt
import dpkt.ethernet

import itertools
import struct
import sys

#START_MARKERS = ('#{red}', '#{green}')
#END_MARKERS = ('#{reset}', '#{reset}')
START_MARKERS = ('<span class="mismatch">', '<span class="match">', '<span class="special">')
END_MARKERS = ('</span>', '</span>', '</span>')

ITS_OK_TO_BE_DIFFERENT = ('id', 'sum', 'seq', 'ack', 'sport', 'dport', 'win')


def hexdump_diff_helper(p1, p2):
    """Return a hexdump output string of the given packets."""
    global ITS_OK_TO_BE_DIFFERENT
    n = 0
    res = [[], []]
    if type(p1) != type(p2):
        raise Exception('Cannot diff packets of different types!')
    if isinstance(p1, str): # All done.
      return res

    p1_hdr = p1.pack_hdr()
    p2_hdr = p2.pack_hdr()
    offset = 0
    for field, typecode, default in p1.__hdr__:
        equal = getattr(p1, field) == getattr(p2, field)
        if (field in ITS_OK_TO_BE_DIFFERENT):
            equal = 2
        size = struct.calcsize(typecode)
        for i, p in enumerate((p1_hdr, p2_hdr)):
          new_item =  ''.join('%02x' % ord(x) for x in p[offset:offset + size])
          res[i].append('<span class="field"><span class="label">'+field+': </span>' + START_MARKERS[equal] + new_item + END_MARKERS[equal] + '</span>')
        offset += size
    rest = hexdump_diff_helper(p1.data, p2.data)
    res[0].extend(rest[0])
    res[1].extend(rest[1])
    return res

def hexdump_diff(p1, p2):
    res = hexdump_diff_helper(p1, p2)
    #for x in range(0, len(res[0]), 16):
    #    out += res0[x:x+16] + "   |   " + res1[x:x+16] + "\n"
    #return out
    return '<div class="packet left">' + ' '.join(res[0]) + '</div>' + \
           '<div class="packet right">' + ' '.join(res[1]) + '</div><div class="clearfix"></div><br/>'

def pcap_packets(fylname):
    f = open(fylname)
    pcap = dpkt.pcap.Reader(f)
    for ts,buf in pcap:
        eth = dpkt.ethernet.Ethernet(buf)
        ip = eth.data
        yield ip

if __name__ == '__main__':
  print '<html><head><link rel="stylesheet" href="style.css" type="text/css" /></head><body>'
  packets1 = pcap_packets(sys.argv[1])
  packets2 = pcap_packets(sys.argv[2])
  couleur.proxy(sys.stdout).enable()
  for packet1, packet2 in itertools.izip(packets1, packets2):
    print hexdump_diff(packet1, packet2)
  print '</body></html>'
