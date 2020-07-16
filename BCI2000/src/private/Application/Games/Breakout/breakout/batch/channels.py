#!/usr/bin/python

import os
import sys

def ask(prompt='? '):
	sys.stdout.write(prompt + ' ')
	sys.stdout.flush()
	a = sys.stdin.readline().strip()
	for x in ';,': a = a.replace(x, ' ')
	b = []
	for ai in a.split():
		ai = [int(x) for x in ai.split(':')]
		if len(ai) == 1: ai = [ai[0], ai[0]]
		if len(ai) == 2: ai = [ai[0], 1, ai[1]]
		if len(ai) != 3: raise ValueError('huh?')
		b += range(ai[0], ai[2]+1, ai[1])
	return b

def main():
	print '\nexpress ranges in Matlab format\n'
	
	rge = ask('channel range:')
	if len(rge) == 1: raise ValueError('need more than one electrode')

	bad = ask('exclude channels:')
	trans = [x for x in rge if x not in bad]
	if len(trans) < 2: raise ValueError('that leaves fewer than 2 electrodes')

	coi = ask('channels of interest (NB: SECOND channel mentioned will map to right):')
	if len(coi) == 1: coi += coi

	fmt = 'CH%03d'

	spfilt = []
	for i,c in enumerate(sorted(set(coi))):
		if c not in trans:
			raise ValueError('channel %d cannot be a channel of interest since it is not transmitted' % c)
		for other in trans:
			if c == other: wt = len(trans) - 1
			else: wt = -1
			spfilt.append( (fmt + ' ' + fmt + ' %g') % (other, c, wt) )
			
	clas = []
	for i,c in enumerate(coi):
		bin = 1
		wt = 1.0
		clas.append( (fmt + ' %d %d %g') % (c, bin, i+1, wt))
		
	SpatialFilter = '%d { In Out Wt }   %s' % (len(spfilt), '   '.join(spfilt))
	TransmitChList = '%d  %s' % (len(trans), ' '.join([fmt % x for x in trans]))
	Classifier = '%d { input%%20channel input%%20element output%%20channel weight }  %s' % (len(clas), '   '.join(clas))

	params = """
Source:Online%20Processing              list      TransmitChList=           $TRANSMITCHLIST    // list of transmitted channels
Filtering:Hijacking                     int       Hijack=                        0             // Turn on Hijacking? (boolean)
Filtering:SpatialFilter                 int       SpatialFilterType=             2             // spatial filter type 0: none, 1: full matrix, 2: sparse matrix, 3: common average reference (CAR) (enumeration)
Filtering:SpatialFilter                 matrix    SpatialFilter=      $SPATIALFILTER           // spatial filtering matrix
Filtering:LinearClassifier              matrix    Classifier=         $CLASSIFIER              // Linear classification weight matrix in sparse representation
	""".replace("$TRANSMITCHLIST", TransmitChList).replace("$SPATIALFILTER", SpatialFilter).replace("$CLASSIFIER", Classifier)

	filename = '..\\parms\\SelectedChannels.prm'
	f = open(filename, 'w')
	f.write(params)
	f.close()
	os.system('edit ' + filename)

if __name__ == '__main__':
	keepGoing = True
	while keepGoing:
		try: main()
		except ValueError, e: print e; print;
		else: keepGoing = False
		
