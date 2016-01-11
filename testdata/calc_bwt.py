#!/usr/bin/env python2.7
#Creates a file containg a bwt of a text from file given to input
import argparse
parser = argparse.ArgumentParser(description = 'Creates a file containg a bwt of a text from file given to input')
parser.add_argument('inp', metavar = 'input file', type =str, help='name of the file containing a string')
args = parser.parse_args()
with open(args.inp, 'r') as inp:
	content = inp.read()
content += '$'
table = sorted([content[i:] + content[:i] for i in range(len(content))])
outname = '.'.join(args.inp.split('.')[:-1]) + '.out'
out = open(outname, 'w')
out.write(''.join([a[-1] for a in table]))
out.close()
for i in content.split('>')[1:]:
	i = ''.join(i.split('\n')[1:])
	outname = args.inp.split('.')[1] + '_' + str(len(i)) + '.in'
	out = open(outname, 'w')
	out.write(i)
	out.close()