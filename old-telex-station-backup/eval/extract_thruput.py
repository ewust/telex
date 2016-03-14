#!/usr/bin/env python
import collections
import re
import subprocess
import sys

def extract_data(files, outfilebase, value_re, shortgrep):
  data = collections.defaultdict(list)  # filesize -> [(concurrency, thruput)]
  for filename in files:
    (concurrency, size, _) = filename.split('_', 2)
    thruput = subprocess.Popen(['grep', '-h', shortgrep, filename],
                               stdout=subprocess.PIPE).communicate()[0]
    m = re.match(value_re, thruput)
    try:
      thruput = m.group(1)
    except AttributeError:
      print filename
      print thruput
      raise
    data[size].append((int(concurrency), thruput))

  for filesize, values in data.iteritems():
    outfyl = open('%s.%s.dat' % (outfilebase, filesize), 'w')
    for value in sorted(values):
      outfyl.write('%d\t%s\n' % value)
    outfyl.close()

def main(argv):
  REs = {'thruput':  (r'Requests per second:.*?(\d+\.\d+).*', 'Requests'),
         'failed': (r'Failed requests:.*?(\d+).*', 'Failed'),
         '50pct': (r'  50%.*?(\d+).*', '50%'),
         '90pct': (r'  90%.*?(\d+).*', '90%'),
         '100pct': (r' 100%.*?(\d+).*', '100%')}
  re, shortgrep = REs[argv[1]]
  extract_data(argv[2:], argv[1], re, shortgrep)

if __name__ == '__main__':
  main(sys.argv)
