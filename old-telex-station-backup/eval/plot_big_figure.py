#!/usr/bin/env python
import matplotlib
matplotlib.use('cairo.pdf')
from matplotlib import pyplot
matplotlib.rcParams['font.size'] = 24
matplotlib.rcParams['font.family'] = 'sans-serif'
matplotlib.rcParams['ytick.major.pad'] = 8
#matplotlib.rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
#matplotlib.rcParams['font.sans-serif'] = 'Helvetica'
matplotlib.rcParams['legend.fontsize'] = 'small'
import os.path

def joinfyls(fyls):
  data = [dict([line.split() for line in open(fyl, 'r')]) for fyl in fyls]
  print fyls, datab
  data = [(int(k), sum(float(d[k]) for d in data)) for k in data[0] if k in data[1]]
  data.sort()
  return data


COLORS = ('#0085c2', '#ed1c24')

def main(tlsfyl, tls2fyls, telexfyl,
         telexfyls, outname, yaxis, title):
  size = os.path.basename(tlsfyl).split('.')[0]
  data = [line.split() for line in open(tlsfyl, 'r')]
  plots = []

  fig = pyplot.gcf()
  size = fig.get_size_inches()
  fig.set_size_inches(size[0] * 1.1, size[1] * 1.1)

  ax = pyplot.subplot(1, 1, 1)
  # ax.plot([x[0] for x in data], [x[1] for x in data], marker='s',
  #             color='k',
  #             linestyle='-',
  #             label='TLS (fast client)')
  for idx, fyl in enumerate(tls2fyls):
    data = [line.split() for line in open(fyl, 'r')]
    p, = ax.plot([x[0] for x in data], [x[1] for x in data], marker=('s' if not idx else 'D'),
                    color=COLORS[idx],
                    linestyle='-',
                    markeredgewidth=0,
                    label='Client %s TLS' % ('B' if not idx else 'A'))
    p.set_markersize(p.get_markersize() * 1.2)
    plots.append(p)

  # data = [line.split() for line in open(telexfyl, 'r')]
  # ax.plot([x[0] for x in data], [x[1] for x in data], marker='+',
  #             color='k',
  #             linestyle='--',
  #             label='Telex (fast client)')
  #data = [line.split() for line in open(telex2fyl, 'r')]
  # ax.plot([x[0] for x in data], [x[1] for x in data], marker='.',
  #             color='k',
  #             linestyle='--',
  #             label='Telex (slow client)')

  for idx, fyl in enumerate(telexfyls):
    data = [line.split() for line in open(fyl, 'r')]
    p, = ax.plot([x[0] for x in data], [x[1] for x in data], marker=('s' if not idx else 'D'),
                    color=COLORS[idx],
                    linestyle='--',
                    markeredgewidth=0,
                    label='Client %s Telex' % ('B' if not idx else 'A'))
    p.set_markersize(p.get_markersize() * 1.2)
    plots.append(p)
  import operator

  handles, labels = ax.get_legend_handles_labels()
  hl = sorted(zip(handles, labels), key=operator.itemgetter(1))
  print zip(*hl)
  handles2, labels2 = zip(*hl)
  ax.legend(handles2, labels2, loc=0)
#  pyplot.legend(plots, ('TLS (Client A)', 'TLS (Client B)', 'Telex (Client A)',
                   #                 'Telex (slow client)',
#                 'Telex (Client B)'), loc=0)
#  pyplot.title(title)
  pyplot.xlim(xmin=0, xmax=50)
  pyplot.ylim(ymin=0)#, ymax=100)
  pyplot.xlabel('Concurrent connections per client')
  pyplot.ylabel(yaxis)
  fig.subplots_adjust(left=0.2)
  ax.yaxis.set_label_coords(-0.125, 0.5)
  # for tick in ax.get_yaxis().get_major_ticks():
  #   tick.set_pad(5)
  #   tick.label1 = tick._get_text1()
  pyplot.savefig(outname, bbox_inches='tight')
if __name__ == '__main__':
  import sys
  main(sys.argv[1], sys.argv[2:4], sys.argv[4], sys.argv[5:7], sys.argv[9], sys.argv[7], sys.argv[8])
