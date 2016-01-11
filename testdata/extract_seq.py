#!/usr/bin/env python2.7
#Takes a file containing unmasked genomic DNA sequences downloaded
#from bacteria.ensembl.org and splits into strings with string
#length specified in output file name
import argparse
parser = argparse.ArgumentParser(description = 'Takes a file containing unmasked genomic DNA sequences downloaded from bacteria.ensembl.org and splits into strings with string length specified in output file name')
parser.add_argument('inp', metavar = 'input file', type =str, help='name of the file containing DNA sequences')
args = parser.parse_args()
with open(args.inp, 'r') as inp:
	content = inp.read()
for i in content.split('>')[1:]:
	i = ''.join(i.split('\n')[1:])
	outname = args.inp.split('.')[1] + '_' + str(len(i)) + '.in'
	out = open(outname, 'w')
	out.write(i)
	out.close()