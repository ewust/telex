#!/usr/bin/env python
'''Test the flow blocker by sending it a message.
Currently requires manual verification.
'''

import socket
import struct
import sys

def encode_fixup_ip_addr(addr):
  return struct.unpack('!I', socket.inet_aton(addr))[0]

def make_flow_blocker_packet(sip, sport, dip, dport, block):
  return struct.pack('!BIHIH', 1 if block else 0, encode_fixup_ip_addr(sip),
                     sport, encode_fixup_ip_addr(dip), dport) 

def send_test_packet(ip, port):
  sock = socket.create_connection((ip, port))
  sock.send(make_flow_blocker_packet('1.2.3.4', 0x1234, '5.6.7.8', 0x5678, 1))

def generate_stress_test_packets(block, outfyl):
  dport = 443
  dip = '10.3.0.2'
  sip = '10.2.0.1'
  packets = [make_flow_blocker_packet(sip, sport, dip, dport, block)
             for sport in xrange(10000, 60000)]
  outfyl.write(''.join(packets))

if __name__ == '__main__':
  if len(sys.argv) > 3:
    if sys.argv[3] == 'genstress':
      generate_stress_test_packets(int(sys.argv[1]), open(sys.argv[2], 'wb'))
  else:
    ip, port = sys.argv[1], int(sys.argv[2])
    send_test_packet(ip, port)
