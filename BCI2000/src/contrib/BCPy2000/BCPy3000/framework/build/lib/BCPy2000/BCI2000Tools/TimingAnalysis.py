#!/usr/bin/env python #
# -*- coding: utf-8 -*-
# 
#   $Id: TimingAnalysis.py 5752 2018-05-09 16:38:35Z jhill $
#   
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
# 
#   Copyright (C) 2007-11  Jeremy Hill, Thomas Schreiner,
#                          Christian Puzicha, Jason Farquhar
#   
#   bcpy2000@bci2000.org
#   
#   The BCPy2000 framework is free software: you can redistribute it
#   and/or modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version 3 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
__all__ = ['TimingWindow', 'StimulusTiming']

import os, sys, time, numpy, pylab

from .FileReader import bcistream, ListDatFiles

try: import SigTools
except ImportError: import BCPy2000.SigTools as SigTools


def TimingWindow(filename='.', ind=-1, save=None):
	"""
Recreate BCI2000's timing window offline, from a saved .dat file specified by <filename>.
It is also possible to supply a directory name as <filename>, and an index <ind> (default
value -1 meaning "the last run") to choose a file automatically from that directory.

Based on BCI2000's   src/shared/modules/signalsource/DataIOFilter.cpp where the timing window
content is computed in DataIOFilter::Process(), this is what appears to happen:
    
         Begin SampleBlock #t:
            Enter SignalSource module's first Process() method (DataIOFilter::Process)
            Save previous SampleBlock to file
            Wait to acquire new SampleBlock from hardware
 +--------- Measure SourceTime in SignalSource module
 |   |   |  Make a local copy of all states (NB: all except SourceTime were set during #t-1) ---+
B|  R|  S|  Pipe the signal through the rest of BCI2000                                         |
 |   |   +- Measure StimulusTime in Application module, on leaving last Process() method        |
 |   |                                                                                          |
 |   |                                                                                          |
 |   |   Begin SampleBlock #t+1:                                                                |
 |   +----- Enter SignalSource module's first Process() method (DataIOFilter::Process)          |
 |          Save data from #t, SourceTime state from #t, and other states from #t-1, to file <--+
 |          Wait to acquire new SampleBlock from hardware
 +--------- Measure SourceTime in SignalSource module
            Make a local copy of all states (NB: all except SourceTime were set during #t)
            Leave DataIOFilter::Process() and pipe the signal through the rest of BCI2000
            Measure StimulusTime in Application module, on leaving last Process() method

B stands for Block duration.
R stands for Roundtrip time (visible in VisualizeTiming, not reconstructable from the .dat file)
S is the filter cascade time (marked "Stimulus" in the VisualizeTiming window).

Note that, on any given SampleBlock as saved in the file, SourceTime will be *greater* than
any other timestamp states (including StimulusTime), because it is the only state that is
updated in time to be saved with the data packet it belongs to. All the others lag by one
packet.  This is corrected for at the point commented with ??? in the Python code. 
"""
	
	if hasattr(filename, 'filename'): filename = filename.filename
	
	b = bcistream(filename=filename, ind=ind)
		
	out = SigTools.sstruct()
	out.filename = b.filename
	#print "decoding..."
	sig,states = b.decode('all')
	#print "done"
	b.close()

	dT,T,rT = {},{},{}
	statenames = ['SourceTime', 'StimulusTime'] + ['PythonTiming%02d' % (x+1) for x in range(2)]
	statenames = [s for s in statenames if s in states]
	for key in statenames:
		dT[key],T[key] = SigTools.unwrapdiff(states[key].flatten(), base=65536, dtype=numpy.float64)

	sel, = numpy.where(dT['SourceTime'])
	for key in statenames:
		dT[key] = dT[key][sel[1:]]
		if key == 'SourceTime': tsel = sel[:-1]  # ??? why the shift
		else:                   tsel = sel[1:]   # ??? relative to here?
		T[key] = T[key][tsel+1]

	t0 = T['SourceTime'][0]
	for key in statenames: T[key] -= t0

	t = T['SourceTime'] / 1000

	expected = b.samples2msec(b.params['SampleBlockSize'])
	datestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(b.datestamp))
	paramstr = ', '.join(['%s=%s' % (x,b.params[x]) for x in ['SampleBlockSize', 'SamplingRate', 'VisualizeTiming', 'VisualizeSource']])
	chainstr = '-'.join([x for x,y in b.params['SignalSourceFilterChain']+b.params['SignalProcessingFilterChain']+b.params['ApplicationFilterChain']])
	titlestr = '\n'.join([b.filename, datestamp, paramstr, chainstr])

	SigTools.plot(t[[0,-1]], [expected]*2, drawnow=False)
	SigTools.plot(t, dT['SourceTime'], hold=True, drawnow=False)

	for key in statenames:
		if key == 'SourceTime': continue
		rT[key] = T[key] - T['SourceTime']
		SigTools.plot(t, rT[key], hold=True, drawnow=False)
	
	import pylab
	pylab.title(titlestr)
	pylab.grid(True)
	pylab.xlabel('seconds')
	pylab.ylabel('milliseconds')
	ymin,ymax = pylab.ylim(); pylab.ylim(ymax=max(ymax,expected*2))
	pylab.xlim(xmax=t[-1])
	pylab.draw()
	out.params = SigTools.sstruct(b.params)
	out.summarystr = titlestr
	out.t = t
	out.SourceTime = T['SourceTime']
	out.StimulusTime = T['StimulusTime']
	out.BlockDuration = dT['SourceTime']
	out.BlockDuration2 = dT['StimulusTime']
	out.ProcessingTime = out.StimulusTime - out.SourceTime
	out.ExpectedBlockDuration = expected
	out.rT = rT
	out.dT = dT
	out.T = T
	
	if save:
		pylab.gcf().savefig(save, orientation='landscape')
	
	return out

def StimulusTiming(filename='.', ind=None, channels=0, trigger='StimulusCode > 0', msec=300, offset_msec=0, rectify=False, threshold=0.1, use_eo=False, save=None, **kwargs):
	"""
In <filename> and <ind>, give it
  - a directory and ind=None:  for all .dat files in the directory, in session/run order
  - a directory and ind=an index or list of indices: for selected .dat files in the directory
  - a dat-file name and ind=anything:  for that particular file
  - a list of filenames and ind=anything: for certain explicitly-specified files

<channels>  may be a 0-based index, list of indices, list of channel names, or space- or comma-
			delimited string of channel names
<rectify>   subtracts the median and takes the abs before doing anything else
<threshold> is on the normalized scale of min=0, max=1 within the resulting image
<use_eo>    uses the EventOffset state to correct timings
	"""###
	if hasattr(filename, 'filename'): filename = filename.filename
		
	if ind==None:
		ind = -1
		if os.path.isdir(filename): filename = ListDatFiles(filename)
		
	if not isinstance(filename, (tuple,list)): filename = [filename]
	if not isinstance(ind, (tuple,list)): ind = [ind]
	n = max(len(filename), len(ind))
	if len(filename) == 1: filename = list(filename) * n
	if len(ind) == 1: ind = list(ind) * n
	
	if isinstance(channels, str): channels = channels.replace(',', ' ').split()
	if not isinstance(channels, (tuple,list)): channels = [channels]
	out = [SigTools.sstruct(
			files=[],
			events=[],
			t=None,
			channel=ch,
			img=[],
			edges=[],
			threshold=None,
			EventOffsets=[],
			UseEventOffsets=False,
		) for ch in channels]
	if len(filename) == 0: raise ValueError("no data files specified")
	for f,i in zip(filename, ind):
		b = bcistream(filename=f, ind=i)
		nsamp = b.msec2samples(msec)
		nsamp_offset = b.msec2samples(offset_msec)
		sig,st = b.decode('all')
		statenames = list(zip(*sorted([(-len(x),x) for x in st])))[1]
		criterion = trigger
		for x in statenames: criterion = criterion.replace(x, "st['%s']"%x)
		criterion = numpy.asarray(eval(criterion)).flatten()
		startind = SigTools.edges(criterion)
		print("%d events found in %s" % (len(startind), b.filename))
		
		for s in out:
			s.files.append(b.filename)
			s.events.append(len(startind))
			ch = s.channel
			if isinstance(ch, str): 
				chn = [x.lower() for x in b.params['ChannelNames']]
				if ch.lower() in chn: ch = chn.index(ch.lower())
				else: raise ValueError("could not find channel %s in %s" % (ch,b.filename))
			if len(b.params['ChannelNames']) == len(sig):
				s.channel = b.params['ChannelNames'][ch]
			
			xx = numpy.asarray(sig)[ch]
			if rectify: xx = numpy.abs(xx - numpy.median(xx))
			xx -= xx.min()
			if xx.max(): xx /= xx.max()
			s.threshold = threshold
			for ind in startind:
				if 'EventOffset' in st:
					eo = st['EventOffset'].flat[ind]
					if use_eo:
						ind += eo - 2**(b.statedefs['EventOffset']['length']-1)
						s.UseEventOffsets = True
				else:
					eo = 0
				s.EventOffsets.append(eo)
				x = xx[ind+nsamp_offset:ind+nsamp_offset+nsamp].tolist()
				x += [0.0] * (nsamp - len(x))
				s.img.append(x)
	
	for s in out:
		s.img = numpy.asarray(s.img)
		s.edges = [min(list(x.nonzero()[0])+[numpy.nan]) for x in (s.img > s.threshold)]
		s.edges = b.samples2msec(numpy.asarray(s.edges+nsamp_offset))
		s.t = b.samples2msec(numpy.arange(nsamp_offset, nsamp_offset+nsamp))	
		
	import pylab
	pylab.clf()
	ax = None
	for i,s in enumerate(out):
		ax = pylab.subplot(1, len(out), i+1, sharex=ax, sharey=ax)
		y = y=list(range(1,len(s.img)+1))
		SigTools.imagesc(s.img, x=s.t, y=y, aspect='auto', **kwargs)
		xl,yl = pylab.xlim(),pylab.ylim()
		pylab.plot(s.edges, y, 'w*', markersize=10)
		for j,x in enumerate( s.edges.flat ):
			if not numpy.isnan(x): pylab.text( x, y[j], str(y[j])+'---', ha='right', va='center', color='#00FF00', clip_on=True )
		pylab.xlim(xl); pylab.ylim(yl)
		pylab.grid('on')
		#pylab.ylim([len(s.img)+0.5,0.5]) # this corrupts the image!!
	pylab.draw()
	if save:
		pylab.gcf().savefig(save, orientation='portrait')
	return out
	
if __name__ == '__main__':
	import getopt,sys,os
	execname = os.path.split(getattr(sys, 'argv', [__file__])[0])[1]
	args = getattr(sys, 'argv', [])[1:]
	try: opts,args = getopt.getopt(args, 'shb', ['help', 'stimulus-timing', 'block-timing'])
	except Exception as e: sys.stderr.write(str(e)+'\n'); exit(1)
	
	help = False
	mode = None
	kwargs = {}
	for opt, val in opts:
		if   opt in ('-s', '--stimulus-timing'): mode = 's'
		elif opt in ('-b', '--block-timing'): mode = 'b'
		elif opt in ('-h', '--help'): help = True
		else:
			try: val = int(val)
			except:
				try: val = float(val)
				except: pass
			kwargs[opt.lstrip('-')] = val
			
	for arg in args:
		if arg.startswith('-'): sys.stderr.write('--opts must come before filenames\n'); exit(1)
		if '=' in arg:
			br = arg.index('=')
			key = arg[:br].lstrip('-')
			val = arg[br+1:]
			try: val = int(val)
			except:
				try: val = float(val)
				except: pass
			kwargs[key]=val
		elif 'filename' not in kwargs: kwargs['filename'] = arg
		elif 'save'     not in kwargs: kwargs['save'    ] = arg
		else: sys.stderr.write('too many input arguments\n'); exit(1)
	
	#for k,v in sorted(kwargs.items()): print k,'=',repr(v)

	if mode == 's': func = StimulusTiming
	else: func = TimingWindow
	if help:
		help = '$0: graphical display of either block timing or stimulus timing from a BCI2000 .dat file\n\n' + \
		       '$0 [-b] [opts] filename [outputfilename] : make TimingWindow plot \n' + \
		       '$0  -s  [opts] filename [outputfilename] : make StimulusTiming plot\n\n' + \
		       'Apart from <filename>, supply any other function arguments as opts in --key=val format.\n\n'
		if mode == None: help += 'For more help, use [-h|--help] in conjunction with either -s or -b\n'
		else: help = help.rstrip(' \n.') + (':\n\n$0 -%s ...    calls %s(filename, ...):\n\n' % (mode, func.__name__)) + func.__doc__.lstrip('\n')
		help = help.replace('$0', execname)
		print(help)
		exit(0)
		
	result = func(**kwargs)
	if not kwargs.get('save', None): import pylab; pylab.show()
	