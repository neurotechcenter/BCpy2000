# -*- coding: utf-8 -*-
# 
#   $Id: Plotting.py 5752 2018-05-09 16:38:35Z jhill $
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
__all__ = [
	'plot', 'plotsig',
	'imagesc', 'scatterplot', 'scatcmp', 'stem',
	'colorbar', 'rmcolorbar',
	'make_cmap', 'complement_cmap', 'reverse_cmap', 'show_cmaps',
	'subplots',
	'curve',
	'cax',
	'superimpose', 'look', 'hotfig', 
]

import numpy
from .Basic import getfs,fft2ap,fft
from .NumTools import isnumpyarray, project
from functools import reduce

def filterdict(d, exact=(), startswith=None, endswith=None):
	f = dict([(k,v) for k,v in list(d.items()) if k in exact or (startswith != None and k.startswith(startswith)) or (endswith != None and k.endswith(endswith))])
	for k in f: d.pop(k)
	return f

def plot(*pargs,**kwargs):
	"""
	A wrapper around pylab.plot that reduces dimensionality-related fiddliness.
	
	plot(x, y)
	plot(y)
	
	where either x or y, independent of each other, can specify multiple lines.
	
	Additional options and their defaults:
		axis = 0         Along which dimension can one step while staying on
		                 the same graphical line?
		stem = False     If True, do a stem plot instead of an ordinary line
		grid = True      Whether to turn on the grid.
		balance = None   If None, do nothing. If a numeric value, balance the
		                 y axis limits around this value.
		aspect = None    If 'auto', 'image' or 'equal', apply that aspect mode
		hold = False     If False, clear the axes before plotting.
		drawnow = True   If True, execute pylab.draw() after plotting.
	"""###
	hold = kwargs.pop('hold', False)
	axis = kwargs.pop('axis', 0)
	drawnow = kwargs.pop('drawnow', True)
	grid = kwargs.pop('grid', True)
	balance = kwargs.pop('balance', None)
	stem = kwargs.pop('stem', False)
	aspect = kwargs.pop('aspect', None)
	axkwargs = filterdict(kwargs, ['title', 'xlabel', 'ylabel', 'ylim', 'xlim'])

	pargs = list(pargs) # makes a copy, at least of the list container
	allvec = True
	for i in range(len(pargs)):
		if isinstance(pargs[i], (tuple,list)):
			pargs[i] = numpy.array(pargs[i],dtype=numpy.float64)
		if isinstance(pargs[i], numpy.ndarray):
			allvec &= (max(pargs[i].shape) == numpy.prod(pargs[i].shape))
			if len(pargs[i].shape) > 1:
				pargs[i] = project(pargs[i],axis).swapaxes(0,axis)
			isinf = numpy.isinf(pargs[i])
			if numpy.any(isinf):
				pargs[i] = pargs[i].copy()
				pargs[i][isinf] = numpy.nan # crude workaround---pylab.plot can't cope with infinite values
	if allvec:
		for i in range(len(pargs)):
			if isinstance(pargs[i], numpy.matrix):  pargs[i] = pargs[i].A
			if isinstance(pargs[i], numpy.ndarray): pargs[i] = pargs[i].flatten()	
	pargs = tuple(pargs)

	pylab = load_pylab()
	if not hold: # undo a few known hangovers before clearing
		ax = pylab.gca()
		ax.set_aspect('auto') # image aspect ratio
		ax.set_ylim(sorted(ax.get_ylim())) # ydir reversed
		rmcolorbar(drawnow=False) # colorbar
		pylab.cla()
	
	if stem:
		if len(pargs) == 1: pargs = (list(range(pargs[0].shape[axis])),) + pargs
		p = pylab.stem(*pargs, **kwargs)
		x = pargs[0]
		xl = numpy.r_[float(x[0]), float(x[-1])]
		if len(x) == 1: xl += [-0.5, 0.5]
		else: xl += numpy.r_[float(x[0]-x[1]), float(x[-1]-x[-2])] / 2.0
		pylab.gca().set_xlim(xl)
	else:
		p = pylab.plot(*pargs,**kwargs)
	pylab.grid(grid)
	ax = pylab.gca()
	if balance != None:
		yl = numpy.array(ax.get_ylim())
		yl = balance + numpy.array([-1,+1]) * max(abs(yl - balance))
		ax.set_ylim(yl)
	if aspect != None: ax.set_aspect({'image':'equal'}.get(aspect, aspect))
	ax.set(**axkwargs)
	if drawnow: pylab.draw()
	return p
	
def plotsig(x, samplingfreq_hz=None, hold=False, axis=0, welch=0, **kwargs):
	"""
	Makes two subplots, showing time-series <x> in the upper panel and its
	amplitude spectrum in the lower panel.  Set <hold> in order to re-use
	a previous figure.
	
	Any additional keyword arguments are passed through to pylab.plot for
	both subplots.
	"""###
	fs = getfs(samplingfreq_hz)
	if fs==None: fs = getfs(x,2.0)
	if hasattr(x, 'x'): x = x.x
	elif hasattr(x, 'y'): x = x.y
	
	if not isnumpyarray(x):
		axis = 0
		if isinstance(x[0], list) or isinstance(x[0], tuple): axis = 1
		x = numpy.array(x,dtype='float')

	xwin = x = project(x,axis).swapaxes(0, axis)
	nsamp = x.shape[0]
	
	class Unfinished(Exception): pass
	if welch==1: xwin = x * project(hanning(nsamp),len(x.shape)-1)
	elif welch > 0: raise Unfinished("Welch periodogram not yet implemented")
	
	t = numpy.arange(0, nsamp) / float(fs)
	ap = fft2ap(fft(xwin,axis=0),samplingfreq_hz=fs,axis=0)
	f = ap['freq_hz']
	a = 20.0 * numpy.log10(ap['amplitude'])

	pylab = load_pylab()
	if not hold: pylab.clf()
	
	pylab.subplot(2,1,1)
	h1 = pylab.plot(t,x,**kwargs)
	ax = pylab.gca()
	ax.set_xlim(t[0], t[-1])
	ax.xaxis.grid(True)
	ax.yaxis.grid(True)
	
	pylab.subplot(2,1,2)
	a[numpy.isinf(a)] = numpy.nan # crude workaround---pylab.plot can't cope with infinite values
	h2 = pylab.plot(f,a,**kwargs)
	ax = pylab.gca()
	ax.set_xlim(f[0], f[-1])
	ax.xaxis.grid(True)
	ax.yaxis.grid(True)

	pylab.draw()

def wavplotsig(self, *pargs, **kwargs): plotsig(self, *pargs, **kwargs)
try:
	try: from WavTools.Base import wav
	except ImportError: from BCPy2000.WavTools.Base import wav
	wav.plotsig = wavplotsig
except:
	pass

def imagesc(img, x=None, y=None, hold=False, drawnow=True, aspect='image', balance=None, colorbar=False, grid=False, ydir='reverse', **kwargs):
	kwargs['interpolation'] = kwargs.get('interpolation', 'nearest')
	kwargs['cmap'] = kwargs.get('cmap', 'kelvin_i')
	cbkwargs = filterdict(kwargs, startswith='colorbar')
	cbkwargs = dict([(k[len('colorbar'):],v) for k,v in list(cbkwargs.items())])
	axlabelkwargs = filterdict(kwargs, ['title', 'xlabel', 'ylabel'])
	auto_aspect = {'equal':False, 'image':False, 'auto':True}.get(aspect)
	if auto_aspect is None: raise ValueError('aspect should be "image" or "auto"')
	pylab = load_pylab()
	ax = pylab.gca()
	if not hold: ax.cla()
	
	img = numpy.asarray(img)

	xlab,ylab = None,None
	if isinstance(x, (tuple,list,numpy.ndarray)) and isinstance(x[0], str): xlab,x = x,None
	if isinstance(y, (tuple,list,numpy.ndarray)) and isinstance(y[0], str): ylab,y = y,None
	if x is None: x = numpy.arange(img.shape[1], dtype=numpy.float64)
	if y is None: y = numpy.arange(img.shape[0], dtype=numpy.float64)
	x = numpy.asarray(x).flatten()
	y = numpy.asarray(y).flatten()
	if len(x) == 1: xl = [ x[ 0 ] - 0.5, x[ 0 ] + 0.5 ]	
	else: xl = [x[0] - 0.5 * (x[1]-x[0]),   x[-1] + 0.5 * (x[-1]-x[-2])]
	if len(y) == 1: yl = [ y[ 0 ] - 0.5, y[ 0 ] + 0.5 ]	
	else: yl = [y[0] - 0.5 * (y[1]-y[0]),   y[-1] + 0.5 * (y[-1]-y[-2])]
	tol = 1e-10
	regular = ( len(x) == 1 or (numpy.diff(x).ptp() < tol) ) and ( len(y) == 1 or (numpy.diff(y).ptp() < tol) )
	if ydir == 'reverse': yl = yl[::-1]; origin='upper'
	else: origin='lower'
	if not regular: raise ValueError('x and y data must be regularly spaced') # NonUniformImage just doesn't play nice, with ydir or with clim scaling
	h = pylab.imshow(img, extent=xl+yl, origin=origin, **kwargs)		
	if xlab != None: ax.set_xticks(x); ax.set_xticklabels(list(xlab))
	if ylab != None: ax.set_yticks(y); ax.set_yticklabels(list(ylab))
	if auto_aspect: ax.set_aspect('auto')
	else: ax.set_aspect('equal')
	pylab.grid(grid)
	ax.set_xlim(xl)
	ax.set_ylim(yl)
	ax.set(**axlabelkwargs)
	if balance != None:
		c = numpy.array(h.get_clim())
		c = balance + numpy.array([-1,+1]) * max(abs(c - balance))
		h.set_clim(c)
	if colorbar: _colorbar(parent_axes=ax, **cbkwargs)
	else: rmcolorbar(parent_axes=ax)
	if drawnow: pylab.draw()
	return h

def scatterplot(*pargs, **kwargs):
	"""
	scatterplot(xy)         # scatterplots xy[:,1].flat against xy[:,0].flat
	scatterplot(xy1, 'r*')
	scatterplot(xy1, 'r*', xy2, 'bs')
	scatterplot(xy1, 'r*', xy2, 'bs', markersize=10, hold=False, drawnow=True)
	
	scatterplot(x, y)      # scatterplots y.flat against x.flat
	scatterplot(x1, y1, 'r*')
	scatterplot(x1, y1, 'r*', x2, y2, 'bs')
	scatterplot(x1, y1, 'r*', x2, y2, 'bs', markersize=10, hold=False, drawnow=True)
	"""
	x = None
	y = None
	plotargs = []
	connected = []
	if not isinstance(pargs[-1], str):
		pargs = list(pargs) + ['*']
	for i,arg in enumerate(pargs):
		if isinstance(arg, str):
			if y is None:
				if max( x.shape ) == x.size: x, y = numpy.arange( x.size ), x.flatten()
				else: x,y = x[:, 0], x[:, 1]
			plotargs += [x.flatten(), y.flatten(), arg]			
			connected.append('-' in arg)
			x = y = None
		elif x is None: x = arg
		elif y is None: y = arg
	h = plot(*plotargs, **kwargs)
	for hi in zip(h, connected):
		if not connected: hi.set_linestyle('None')
	return h
	
def scatcmp(a, b, hold=False, drawnow=True, markerorder=None, **kwargs):
	kwargs['linestyle'] = kwargs.get('linestyle', 'None')
	grid = kwargs.pop('grid', True)
	grid = {'on':True, 'off':False}.get(grid,grid)
	pylab = load_pylab()
	if not hold: pylab.cla()
	h = plot(a, b, drawnow=False, hold=hold, **kwargs)
	import matplotlib
	if 'marker' not in kwargs:
		if markerorder is None:
			mm = matplotlib.lines.lineMarkers
			markerorder = sorted([m for m in mm if isinstance(m, str) and len(m.strip())]) + sorted([m for m in mm if not isinstance(m, str)])
		for i,hi in enumerate(h): hi.set_marker(markerorder[i % len(markerorder)])
	ax = pylab.gca()
	lim = ax.get_xlim() + ax.get_ylim()
	lim = (min(lim),max(lim))
	ax.set_xlim(lim)
	ax.set_ylim(lim)
	ax.grid(grid)
	pylab.plot(lim, lim, linestyle='-', color=(0.6,0.6,0.6), scalex=False, scaley=False)
	if drawnow: pylab.draw()
	return h

def stem(*pargs, **kwargs):
	kwargs['stem'] = True
	return plot(*pargs, **kwargs)


def colorbar(mappable=None, parent_axes=None, **kwargs):
	"""
	Create a colorbar in such a way that it can be found again and reused, or removed.
	Or, if one already exists associated with the specified parent_axes, use and
	update the existing one.  Use rmcolorbar() to remove.  pylab, y u no do this?
	"""###
	pylab = load_pylab()
	if mappable is None:
		if parent_axes is None: mappable = pylab.gci()
		else: mappable = parent_axes.get_images()[-1]  # NB: doesn't find patch, surf, etc - only images. So to use this with non-images, supply the handle explicitly instead of relying on gca()
	ax = parent_axes
	if ax is None: ax = mappable.get_axes()
	cb = getattr(ax, 'colorbar', None)
	if cb is None:
		kwargs['ax'] = ax
		ax.oldposition = ax.get_position()
	else:
		kwargs['cax'] = cb.ax
		cb.ax.cla()
	title = kwargs.pop('title', None)
	ax.colorbar = pylab.colorbar(mappable, **kwargs)
	if title != None: ax.colorbar.ax.set_title(title)
	pylab.draw()
	return ax.colorbar
_colorbar = colorbar

def rmcolorbar(parent_axes=None, drawnow=True):
	"""
	Remove a colorbar created with colorbar() from this module
	"""###
	pylab = load_pylab()
	ax = parent_axes
	if ax is None: ax = pylab.gca()
	cb = getattr(ax, 'colorbar', None)
	if cb is None: return
	cb.ax.get_figure().delaxes(cb.ax)
	ax.colorbar = None
	if getattr(ax, 'oldposition', None) != None:
		ax.set_position(ax.oldposition)
	if drawnow: pylab.draw()

def load_pylab():
	try:
		import matplotlib,sys
		set_interactive = ('matplotlib.backends' not in sys.modules)
		if set_interactive: matplotlib.interactive(True)
		import pylab
	except:
		print(__name__, "module failed to import pylab: plotting methods will not work")
	else:
		if set_interactive and hasattr(pylab, 'ion'): pylab.ion()
		return pylab

def make_cmap(cmap, name, n=256):
	pylab = load_pylab(); import matplotlib
	cmap = matplotlib.colors.LinearSegmentedColormap(name, cmap, n)
	try: matplotlib.cm.register_cmap(name=name, cmap=cmap)
	except: print("failed to register colormap '%s'" % name)
	return cmap

def complement_cmap(cmap, name=None, n=256):
	pylab = load_pylab()
	if isinstance(cmap, str): cmap = pylab.cm.get_cmap(cmap)
	cmap = getattr(cmap, '_segmentdata', cmap)
	out = {}
	for k in ('red', 'green', 'blue'):
		v = numpy.asarray(cmap[k])
		v[:, 1:] = 1.0 - v[:, 1:]
		out[k] = v
	if name != None: out = make_cmap(out, name=name, n=n)
	return out

def reverse_cmap(cmap, name=None, n=256):
	pylab = load_pylab()
	if isinstance(cmap, str): cmap = pylab.cm.get_cmap(cmap)
	cmap = getattr(cmap, '_segmentdata', cmap)
	out = pylab.cm.revcmap(cmap)
	if name != None: out = make_cmap(out, name=name, n=n)
	return out
	
def show_cmaps(*pargs):
	pylab = load_pylab()
	cmaps = []
	for arg in pargs:
		if isinstance(arg, str): arg = arg.split()
		if isinstance(arg, (tuple,list)): cmaps += arg
		else: cmaps.append(arg)
	if len(cmaps) == 0: cmaps=sorted([m for m in pylab.cm.cmap_d if not m.endswith("_r")])
	pylab.clf()
	for i,cmap in enumerate(cmaps):
		pylab.subplot(len(cmaps), 1, i+1)
		if isinstance(cmap, str): cmap = pylab.cm.get_cmap(cmap)
		pylab.imshow([numpy.linspace(0.0,1.0,256,endpoint=True)], cmap=cmap)
		pylab.gca().set(aspect='auto', yticks=[0.0], yticklabels=[cmap.name])
	pylab.draw()

def subplots(r, c=None, fig=None, **kwargs):
	pylab = load_pylab()
	if fig is None: fig = pylab.gcf()
	if c is None:
		if isinstance(r, int):
			nPlots = r
			ar = fig.get_size_inches()
			ar = float(ar[0])/float(ar[1])
			layout = numpy.r_[nPlots/ar, nPlots*ar] ** 0.5
			i = numpy.argmin(abs(layout - numpy.round(layout)))
			r = int(round(layout[i]))
			c = int(numpy.ceil(nPlots/float(r)))
			if i == 1: r,c = c,r
			while r * (c-1) >= nPlots: c -= 1
			while (r-1) * c >= nPlots: r -= 1
		else:
			r,c = r
	i = 0
	ax = []
	for ri in range(r):
		row = []; ax.append(row)
		for ci in range(c):
			row.append(pylab.subplot(r, c, i+1, **kwargs))
			i += 1
	return numpy.array(ax)
	
def make_kelvin():
	kelvin_i = {
		'red': (
			(0.000, 0.0, 0.0, ),
			(0.350, 0.0, 0.0, ),
			(0.500, 1.0, 1.0, ),
			(0.890, 1.0, 1.0, ),
			(1.000, 0.5, 0.5, ),
		),
		'green': (
			(0.000, 0.0, 0.0, ),
			(0.125, 0.0, 0.0, ),
			(0.375, 1.0, 1.0, ),
			(0.640, 1.0, 1.0, ),
			(0.910, 0.0, 0.0, ),
			(1.000, 0.0, 0.0, ),
		),
		'blue': (
			(0.000, 0.5, 0.5, ),
			(0.110, 1.0, 1.0, ),
			(0.500, 1.0, 1.0, ),
			(0.650, 0.0, 0.0, ),
			(1.000, 0.0, 0.0, ),
		),
	}
	kelvin_i = make_cmap(kelvin_i, 'kelvin_i', 256)
	kelvin_r = complement_cmap(kelvin_i, 'kelvin_r', 256)
	kelvin   = reverse_cmap(kelvin_r, 'kelvin', 256)

make_kelvin()

def curve(x,y=None, hold=False, drawnow=True, **kwargs):
	pylab = load_pylab(); import matplotlib, matplotlib.path
	if not hold: pylab.cla()
	kwargs['facecolor'] = kwargs.get('facecolor', 'None')
	if y is None: xy = x
	else: xy = numpy.c_[x,y]
	codes = [matplotlib.path.Path.MOVETO] + [matplotlib.path.Path.CURVE4] * (len(xy)-1)
	path = matplotlib.path.Path(xy,codes)
	patch = matplotlib.patches.PathPatch(path, lw=2, **kwargs)
	pylab.gca().add_patch(patch)
	if drawnow: pylab.draw()

def cax( handles=None, whichAx='c', lim=None, min=None, max=None, include=None, common=False, balance=None, midpoint=None, auto=False ):
	import pylab
	
	def search_handles( handles, reclevel=0 ):
		out = []
		if reclevel >= 10: zark # allows `debug` of apparent indefinite recursion
		try: handles = list( handles )
		except: handles = [ handles ]
		for h in handles:
			if isinstance( h, ( int, float ) ):
				if h == 0: h = pylab.get_fignums()
				elif pylab.fignum_exists( h ): h = pylab.figure( h )
			while hasattr( h, 'axes' ) and h.axes is not h: h = h.axes   # this was intended to grab the children of a figure, but it can also grab the parent of an AxesImage....
			try: h = list( h )
			except: out.append( h )
			else: out.extend( search_handles( h, reclevel=reclevel+1 ) )
		return out
	if handles in [ 'gca' ]: handles = pylab.gca()
	elif handles in [ None, 'gcf' ]: handles = pylab.gcf()
	handles = search_handles( handles )
	if whichAx == 'c': handles = reduce( list.__add__, [ ax.images for ax in handles ] )  #... with the unintended side-effect that if you pass an AxesImage handle, the function will affect it and any of its siblings on the same axes
	
	all = []
	
	for handle in handles:
		getter = getattr( handle, 'get_' + whichAx + 'lim' )
		setter = getattr( handle, 'set_' + whichAx + 'lim' )
		if auto:
			if whichAx == 'c': handle.autoscale()
			else: handle.autoscale( enable=True, axis=whichAx, tight=True )
		v = list( getter() )
		if lim != None: v = [ numpy.min( lim ), numpy.max( lim ) ]
		if min != None: v[ 0 ] = min
		if max != None: v[ 1 ] = max
		if include != None: v = v + list( include ); v = [ numpy.min( v ), numpy.max( v ) ]
		if balance and midpoint is None: midpoint = 0.0
		if balance == False: midpoint = None
		if midpoint != None:
			hw = numpy.max( [ abs( x - midpoint ) for x in v ] )
			v = [ midpoint - hw, midpoint + hw ]
		setter( v )
		all.extend( v )
		
	if common:
		v = [ numpy.min( all ), numpy.max( all ) ]
		for handle in handles:
			setter = getattr( handle, 'set_' + whichAx + 'lim' )
			setter( v )
	pylab.draw()

def superimpose( figs='all' ):
	import pylab
	if figs == 'all': figs = pylab.get_fignums()
	if not isinstance( figs, ( tuple, list ) ): figs = [ figs ]
	figs = list( figs )
	for i, f in enumerate( figs ):
		if isinstance( f, int ): figs[ i ] = pylab.figure( f )
	geom = figs[ 0 ].canvas.manager.window.geometry()
	for fig in figs[ 1: ]: fig.canvas.manager.window.geometry( geom )
	pylab.draw()

def look( fig=None ):
	import pylab
	if fig is None and len( pylab.get_fignums() ): fig = pylab.gcf()
	if isinstance( fig, int ): fig = pylab.figure( fig )
	try: fig.canvas._tkcanvas.focus_force()
	except:
		try: fig.canvas.manager.window.focus_force() # works but then removes focus from whichever part of the figure is listening to keystrokes :-(
		except: pass

def hotfig( figs='all', align=False ):
	import pylab
	if figs == 'all': figs = pylab.get_fignums()
	if not isinstance( figs, ( tuple, list ) ): figs = [ figs ]
	figs = list( figs )
	for i, f in enumerate( figs ):
		if isinstance( f, int ): figs[ i ] = pylab.figure( f )
	if align == True: align = figs
	if align: superimpose( align )
	def kp(event):
		if not isinstance( event.key, str ): return
		key = event.key.lower()
		if key.startswith( 'alt+' ): key = key[ 4: ]
		codes = list( '1234567890QWERTYUIOP'.lower() )
		if key in codes: target = int( codes.index( key ) ) + 1
		elif key == '[': target = event.canvas.figure.number - 1
		elif key == ']': target = event.canvas.figure.number + 1
		else: target = None
		if target != None and pylab.fignum_exists( target ): look( target )
		
	for fig in figs: fig.canvas.mpl_connect( 'key_press_event', kp )
	look()
