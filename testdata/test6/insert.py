#!/usr/bin/env python2.7

import argparse
parser = argparse.ArgumentParser(description = 'Creates a file containg input file with inserted string at given position')
parser.add_argument('inp', metavar = 'input file', type =str, help='name of the file containing a string')
parser.add_argument('str', metavar = 'string to insert', type=str, help='name of the file containing a string to insert')
parser.add_argument('pos',metavar='position of insertion', type=int, help='position of insert')

args = parser.parse_args()

with open(args.inp, 'r') as inp:
	content = inp.read()
with open(args.str, 'r') as inp:
	insert = inp.read()

outname=args.inp+'.ins'
out = open(outname, 'w')
out.write(content[:args.pos-1]+insert+content[args.pos-1:])
out.close()
