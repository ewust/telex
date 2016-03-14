#!/usr/bin/env python
import matplotlib
matplotlib.use('cairo.pdf')
from matplotlib import pyplot

import os.path

def main(tlsfyl, telexfyl, outname):
  size = os.path.basename(tlsfyl).split('.')[0]
  data = [line.split() for line in open(tlsfyl, 'r')]

  pyplot.plot([x[0] for x in data], [x[1] for x in data], marker='s',
              color='k',
              linestyle='-',
              label='TLS')
  data = [line.split() for line in open(telexfyl, 'r')]
  pyplot.plot([x[0] for x in data], [x[1] for x in data], marker='D',
              color='k',
              linestyle='--',
              label='Telex')
  pyplot.legend(('TLS', 'Telex'), loc=4)
  pyplot.title('HTTP Request throughput (%s file)' % size)
  pyplot.xlim(xmax=50)
  pyplot.ylim(ymin=0)
  pyplot.xlabel('Concurrent connections')
  pyplot.ylabel('Mean requests completed per second')
  pyplot.savefig(outname)
if __name__ == '__main__':
  import sys
  main(sys.argv[1], sys.argv[2], sys.argv[3])
