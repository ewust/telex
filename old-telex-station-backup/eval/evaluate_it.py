#!/usr/bin/env python

import datetime
import os
import subprocess
import sys
import time

PAGE_SIZES = ('64b', '1kb', '4kb')#, '16kb')# '64kb', '256kb', '1mb')
#CONCURRENCIES = [str(x) for x in (1, 2, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100)]

#use this one:
#CONCURRENCIES = [str(x) for x in (1, 2, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100)]#, 125, 150, 175, 200)]

# Just for quick testing:
CONCURRENCIES = [str(x) for x in (1, 2, 5, 10, 20, 35, 50, 75, 100)]#, 125, 150, 175, 200)]

def launch_telex_tunnel():
  dev_null = open('/dev/null', 'rw')
  p = subprocess.Popen(['./telex_tunnel', '8080', 'fafner', '443'],
                       stdout=dev_null, stderr=subprocess.STDOUT)
  return p


def launch_ab(output_name, page_name, nsamples, concurrency, base_url, verbosity):
  output_file = open(output_name, 'w')
  p = subprocess.Popen(['ab', '-r', '-n', str(nsamples), '-c', str(concurrency),
                        '-v', str(verbosity), '-g', "%s.gnuplot" % output_name, base_url % page_name],
                        stdout=output_file,
                        stderr=subprocess.STDOUT)
  return p


TAGGED_URL = 'http://localhost:8080/%s'
def launch_ab_tagged(output_name, page_name, nsamples, concurrency):
  return launch_ab(output_name, page_name, nsamples, concurrency,
                   TAGGED_URL, 1)

UNTAGGED_URL = 'https://10.2.0.1/%s_real'
def launch_ab_untagged(output_name, page_name, nsamples, concurrency):
  return launch_ab(output_name, page_name, nsamples, concurrency,
                   UNTAGGED_URL, 1)

NUM_TESTS = 1000

def main(argv):
  if argv[0] == 'tagged':
    tagged = True
  elif argv[0] == 'untagged':
    tagged = False
  else:
    print 'First argument must be either "tagged" or "untagged"'
    sys.exit(1)

  dual_flip = False  #...unless it is...!
  if argv[1] == 'single':
    dual = False
    brock_tagged = tagged
    tweed_tagged = False
  elif argv[1] == 'dual':
    dual = True
    brock_tagged = tagged
    tweed_tagged = tagged
  elif argv[1] == 'dualflip':
    dual = True
    dual_flip = True
    brock_tagged = tagged
    tweed_tagged = not tagged
  else:
    print 'Second argument must be "single", "dual", or "dualflip"'
    sys.exit(1)


  hostname = os.uname()[1]
  print 'Starting at [%s]' % str(datetime.datetime.today())
  tweed_dir = None
  for concurrency in reversed(CONCURRENCIES):
    for page_size in reversed(PAGE_SIZES):
     #TODO: remind user to do it.

      #local telex tunnel
      os.system("pkill -9 telex_tunnel")
      tunnel = launch_telex_tunnel()
      #second client telex tunnel
      if dual:
        os.system("ssh -n 10.3.0.3 'pkill telex_tunnel; nohup /home/ewust/telex/client/run_telex_tun.sh < /dev/null 2>/dev/null >/dev/null &'")

      outname = '%s_%s_%s_%s.out' % (concurrency, page_size, hostname, argv[0])

      # run bro
      subprocess.call('ssh -t root@tweedledum ipset -F OURSET', shell=True)
      os.system("ssh -n root@bro-earth-orbit 'pkill bro; nohup /home/ewust/telex/net/run_bronnect.sh 10.2.0.1 80 < /dev/null 2>/dev/null >/dev/null &'")
      os.system("ssh -n root@bro-earth-orbit 'cd /home/ewust/telex/brohome; pwd; echo '------%s:' >> ./bro.out; ./run_bro_remote.sh > /dev/null 2>/dev/null < /dev/null'" % outname)
      time.sleep(5)

      print "starting %s" % outname

      if brock_tagged:
        if dual_flip:
            ab = launch_ab_tagged(outname, '1kb', NUM_TESTS, 1)
        else:
            ab = launch_ab_tagged(outname, page_size, NUM_TESTS, concurrency)
      else:
        ab = launch_ab_untagged(outname, page_size, NUM_TESTS, concurrency)
      print "   started local ab"
      if dual:
        today = time.strftime('%b%d.%H:%M')
        ts = 'tagged' if tweed_tagged else 'untagged'
        if tweed_dir is None:
          tweed_dir = "/home/ewust/telex/client/%s_results_%s.tweedledee_%s" % (argv[1], today, ts)
          os.system("ssh -n root@10.3.0.3 'mkdir -p %s'" % tweed_dir)
        tweed_fn = os.path.join(tweed_dir, outname)
        if tweed_tagged:
          url = TAGGED_URL % page_size
        else:
          url = UNTAGGED_URL % page_size
        print "  starting remote ab"
        os.system(("ssh -n root@10.3.0.3 'ab -r -n %d -c %s -v 1 -g %s.gnuplot %s " +
                   " > %s 2>&1'") % (num_tests, concurrency, tweed_fn, url, tweed_fn))
        print "   finished remote ab"

      retval = ab.wait()
      print '[%s] %s returned %d' % (str(datetime.datetime.today()), outname, retval)
      if False and retval:  # Not doing timeout investigation
        #os.system('sudo nc -p 123 notblocked.com 443')
        #raw_input('There was a timeout, investigate')
        pass
      tunnel.kill()
      time.sleep(1)


if __name__ == '__main__':
  main(sys.argv[1:])
