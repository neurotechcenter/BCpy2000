# -*- coding: utf-8 -*-
# 
#   $Id: Classification.py 4699 2014-03-15 03:24:39Z jhill $
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
import os
import numpy
import time
import glob
from . import DataFiles
from . import Parameters

try: import SigTools
except ImportError: import BCPy2000.SigTools as SigTools

numpy.seterr(invalid='ignore')

def ClassifyERPs (
		featurefiles,
		C = (10.0, 1.0, 0.1, 0.01),
		gamma = (1.0, 0.8, 0.6, 0.4, 0.2, 0.0),
		keepchan = (),
		rmchan = (),
		rmchan_usualsuspects = ('AUDL','AUDR','LAUD','RAUD','SYNC','VSYNC', 'VMRK', 'OLDREF'),
		rebias = True,
		save = False,
		select = False,
		description='ERPs to attended vs unattended events',
		maxcount=None,
		classes=None,
		folds=None,
		time_window=None,
		keeptrials=None,
	):

	file_inventory = []
	d = DataFiles.load(featurefiles, catdim=0, maxcount=maxcount, return_details=file_inventory)
	if isinstance(folds, str) and folds.lower() in ['lofo', 'loro', 'leave on run out', 'leave one file out']:
		n, folds = 0, []
		for each in file_inventory:
			neach = each[1]['x']
			folds.append(list(range(n, n+neach)))
			n += neach
	
	if 'x' not in d: raise ValueError("found no trial data - no 'x' variable - in the specified files")
	if 'y' not in d: raise ValueError("found no trial labels - no 'y' variable - in the specified files")

	x = d['x']
	y = numpy.array(d['y'].flat)
	if keeptrials != None:
		x = x[numpy.asarray(keeptrials), :, :]
		y = y[numpy.asarray(keeptrials)]
		
	if time_window != None:
		fs = d['fs']
		t = SigTools.samples2msec(numpy.arange(x.shape[2]), fs)
		x[:, :, t<min(time_window)] = 0
		x[:, :, t>max(time_window)] = 0
		
		
	if classes != None:
		for cl in classes:
			if cl not in y: raise ValueError("class %s is not in the dataset" % str(cl))
		mask = numpy.array([yi in classes for yi in y])
		y = y[mask]
		x = x[mask]
		discarded = sum(mask==False)
		if discarded: print("discarding %d trials that are outside the requested classes %s"%(discarded,str(classes)))
		
	n = len(y)
	uy = numpy.unique(y)
	if uy.size != 2: raise ValueError("expected 2 classes in dataset, found %d : %s" % (uy.size, str(uy)))
	for uyi in uy:
		nyi = sum([yi==uyi for yi in y])
		nyi_min = 2
		if nyi < nyi_min: raise ValueError('only %d exemplars of class %s - need at least %d' % (nyi,str(uyi),nyi_min))
			
	y = numpy.sign(y - uy.mean())

	cov,trchvar = SigTools.spcov(x=x, y=y, balance=False, return_trchvar=True) # NB: symwhitenkern would not be able to balance
	
	starttime = time.time()
	
	chlower = [ch.lower() for ch in d['channels']]
	if keepchan in [None,(),'',[]]:
		if isinstance(rmchan, str): rmchan = rmchan.split()
		if isinstance(rmchan_usualsuspects, str): rmchan_usualsuspects = rmchan_usualsuspects.split()
		allrmchan = [ch.lower() for ch in list(rmchan)+list(rmchan_usualsuspects)]
		unwanted = numpy.array([ch in allrmchan for ch in chlower])
		notfound = [ch for ch in rmchan if ch.lower() not in chlower]
	else:
		if isinstance(keepchan, str): keepchan = keepchan.split()
		lowerkeepchan = [ch.lower() for ch in keepchan]
		unwanted = numpy.array([ch not in lowerkeepchan for ch in chlower])
		notfound = [ch for ch in keepchan if ch.lower() not in chlower]
		
	wanted = numpy.logical_not(unwanted)
	print(' ')
	if len(notfound): print("WARNING: could not find channel%s %s\n" % ({1:''}.get(len(notfound),'s'), ', '.join(notfound)))
	removed = [ch for removing,ch in zip(unwanted, d['channels']) if removing]
	if len(removed): print("removed %d channel%s (%s)" % (len(removed), {1:''}.get(len(removed),'s'), ', '.join(removed)))
	print("classification will be based on %d channel%s" % (sum(wanted), {1:''}.get(sum(wanted),'s')))
	print("%d negatives + %d positives = %d exemplars" % (sum(y<0), sum(y>0), n))
	print(' ')
	
	x[:, unwanted, :] = 0
	cov[:, unwanted] = 0
	cov[unwanted, :] = 0
	nu = numpy.asarray(cov).diagonal()[wanted].mean()
	for i in range(len(cov)):
		if cov[i,i] == 0: cov[i,i] = nu
	
	if not isinstance(C, (tuple,list,numpy.ndarray,type(None))): C = [C]
	if not isinstance(gamma, (tuple,list,numpy.ndarray,type(None))): gamma = [gamma]

	c = SigTools.klr2class(lossfunc=SigTools.balanced_loss, relcost='balance')
	c.varyhyper({})
	if c != None: c.hyper.C=list(C)
	if gamma == None: c.hyper.kernel.func = SigTools.linkern
	else: c.varyhyper({'kernel.func':SigTools.symwhitenkern, 'kernel.cov':[cov], 'kernel.gamma':list(gamma)})
	c.cvtrain(x=x, y=y, folds=folds)
	if rebias: c.rebias()
	c.calibrate()

	chosen = c.cv.chosen.hyper
	if gamma == None:
		Ps = None
		Gp = c.featureweight(x=x)
	else:
		Ps = SigTools.svd(SigTools.shrinkcov(cov, copy=True, gamma=chosen.kernel.gamma)).isqrtm
		xp = SigTools.spfilt(x, Ps.H, copy=True)
		Gp = c.featureweight(x=xp)
	
	u = SigTools.stfac(Gp, Ps)
	u.channels = d['channels']		
	u.channels_used = wanted
	u.fs = d['fs']
	u.trchvar = trchvar
	try: u.channels = SigTools.ChannelSet(u.channels)
	except: print('WARNING: failed to convert channels to ChannelSet')

	elapsed = time.time() - starttime
	minutes = int(elapsed/60.0)
	seconds = int(round(elapsed - minutes * 60.0))
	print('%d min %d sec' % (minutes, seconds))
	datestamp = time.strftime('%Y-%m-%d %H:%M:%S')
	csummary = '%s (%s) trained on %d (CV %s = %.3f) at %s' % (
		c.__class__.__name__,
		SigTools.experiment()._shortdesc(chosen),
		sum(c.input.istrain),
		c.loss.func.__name__,
		c.loss.train,
		datestamp,
	)
	description = 'binary classification of %s: %s' % (description, csummary)
	u.description = description
	
	if save or select:
		if not isinstance(save, str):
			save = featurefiles
			if isinstance(save, (tuple,list)): save = save[-1]
			if save.lower().endswith('.gz'): save = save[:-3]
			if save.lower().endswith('.pk'): save = save[:-3]
			save = save + '_weights.prm'
		print("\nsaving %s\n" % save)
		Parameters.Param(u.G.A, Name='ERPClassifierWeights', Section='PythonSig', Subsection='Epoch', Comment=csummary).write_to(save)
		Parameters.Param(c.model.bias, Name='ERPClassifierBias', Section='PythonSig', Subsection='Epoch', Comment=csummary).append_to(save)
		Parameters.Param(description, Name='SignalProcessingDescription', Section='PythonSig').append_to(save)
		if select:
			if not isinstance(select, str): select = 'ChosenWeights.prm'
			if not os.path.isabs(select): select = os.path.join(os.path.split(save)[0], select)
			print("saving %s\n" % select)
			import shutil; shutil.copyfile(save, select)
	
	print(description)
	return u,c
	
	
def Assess( files='*.pk', each=False, return_full=False, C=(1e+4,1e+2,1e-0,1e-2,1e-4,1e-6), gamma=0.0, **kwargs ):
	"""
	Assess(files, each=True, folds='loo')  # each file, leave-one-trial-out
	Assess(files, folds='lofo')            # leave-one-run-out

	# leave-one-run-out example:
	aa = Assess(folds='lofo', return_full=True, C=100)
	a = aa.classifier[0] # or a different one, if there were multiple hyperparameter settings
	[balanced_loss(a.input.y[f], a.output.y[f])[0] for f in a.cv.folding['folds']]
	# gives the balanced loss from testing file during leave-one-file-out
	# (note, it will be slightly different from what was printed during the CV, unless
	# you also specify rebias=False in the call to Assess)
	
	"""
	if isinstance( files, str ):
		if os.path.isdir( files ): files = os.path.join( files, '*.pk' )
		files = glob.glob( files )
	if each: return dict( [ ( file, Assess( files=[ file ], each=False, return_full=return_full, C=C, gamma=gamma, **kwargs ) ) for file in files ] )
	try:
		u, c = ClassifyERPs( files, C=C, gamma=gamma, save=False, **kwargs )
	except Exception as e:
		print('\n***  ERROR: %s ***\n' % str( e ))
		out = ( None, None )
	else:
		if return_full: out = u; out.classifier = [ c ]
		else: out = ( c.loss.train, len( c.input.x ) )
	return out

def PlotSCD( files='*.pk' ):
	import pylab
	if isinstance( files, str ):
		if os.path.isdir( files ): files = os.path.join( files, '*.pk' )
		files = glob.glob( files )
	d = DataFiles.load( files )
	r = SigTools.correlate( d['x'], d['y'], axis=0 )
	SigTools.imagesc( r*numpy.abs(r), y=d['channels'], x=SigTools.samples2msec( list(range( r.shape[1])), d['fs'] ), aspect='auto', balance=0.0, colorbar=True )
	pylab.title( ', '.join( [ '%d: %d' % ( yi, ( d['y'] == yi ).sum() ) for yi in numpy.unique( d['y'] ) ] ) )
	pylab.draw()

def PlotTrials( files='*.pk', channel='Cz' ):
	import pylab
	if isinstance( files, str ):
		if os.path.isdir( files ): files = os.path.join( files, '*.pk' )
		files = glob.glob( files )
	d = DataFiles.load( files )
	chind = d[ 'channels' ].index( channel )
	v = [ ( d[ 'y' ][ i ], i, vi ) for i, vi in enumerate( d[ 'x' ][ :, chind, : ] ) ]
	v = numpy.array( [ vi for yi, i, vi in sorted( v ) ] )
	SigTools.imagesc( v, x=SigTools.samples2msec( list(range( v.shape[1])), d['fs'] ), aspect='auto', balance=0.0, colorbar=True )
	pylab.title( ', '.join( [ '%d: %d' % ( yi, ( d['y'] == yi ).sum() ) for yi in numpy.unique( d['y'] ) ] ) + ( ' (channel %s)' % channel ) )
	pylab.draw()
