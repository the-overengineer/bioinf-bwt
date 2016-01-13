#!/usr/bin/env python2.7

import argparse
parser = argparse.ArgumentParser(description = 'Creates a file containg input file with inserted string at given position')
parser.add_argument('inp', metavar = 'input file', type =str, help='name of the file containing a string')
parser.add_argument('pos',metavar='position of deletion', type=int, help='position of deletion')
parser.add_argument('len', metavar = 'number of characters removed', type=int, help='number of characters removed')

args = parser.parse_args()

with open(args.inp, 'r') as inp:
	content = inp.read()


outname=args.inp+'.del'
out = open(outname, 'w')
out.write(content[:args.pos-1]+content[args.pos+args.len-1:])
out.close()
