#!/usr/bin/env python

import sys
import os
import argparse
import discode_helper as dh

from ctypes import *

def refreshDCLCache(dclname, dclpath, verbose):
  if verbose:
    print "Refreshing cache for", dclname
  mappings = {}
  p = os.path.join(dclpath, "dist", "lib")
  outfile = os.path.join(dclpath, "dist", "cache.txt")
  f = open(outfile, 'w')
  for libname in os.listdir(p):
    try:
      lib = CDLL(os.path.join(p, libname))
    except:
 #     print "Can't open", libname
      continue
      
    try:
      lib.returnCName.restype = c_char_p
      cname = lib.returnCName()
      mappings[cname] = libname
      f.write(cname + "\t" + libname + "\n")
      if verbose:
        print "\t", cname, "=>", libname
    except:
#      print "Can't read", libname, "name"
      continue
  
#  print mappings
  f.close()

def main(argv):
  parser = argparse.ArgumentParser()
  parser.add_argument("DCL", nargs='?', default='all', help="name of DCL to be cached (cache is refreshed in all DLCs if ommited)")
  parser.add_argument("-v", "--verbose", help="be more verbose and show created cache", action="store_true")
  
  args = parser.parse_args()
  
  dcls = dh.listDCL()
  if args.DCL == 'all':
    for dcl in dcls.keys():
      refreshDCLCache(dcl, dcls[dcl], args.verbose)
  else:
    if args.DCL in dcls.keys():
      refreshDCLCache(args.DCL, dcls[args.DCL], args.verbose)
    else:
      print "DCL [", args.DCL, "] not found!"
      exit(1)

if __name__=="__main__":
  main(sys.argv)
