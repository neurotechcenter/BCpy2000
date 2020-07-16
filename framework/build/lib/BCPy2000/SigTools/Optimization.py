# -*- coding: utf-8 -*-
# 
#   $Id: Optimization.py 3445 2011-08-08 15:46:38Z jhill $
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
	'sstruct', 'objective', 'chgradtest', 'psifit',
	'logistic', 'invlogistic', 'cg', 'invcg',
]
import numpy

from .NumTools import sstruct
from .LearningTools import logistic, invlogistic, cg, invcg

def flatten(s, include=None, exclude=None, labels=False):
	if isinstance(s, sstruct): kvtuples = s._allitems()
	elif isinstance(s, dict): kvtuples = sorted(s.items())
	else:
		if labels:
			if isinstance(s, (tuple,list,numpy.ndarray)):
				s = numpy.asarray(s)
				return [str(list(numpy.unravel_index(i, s.shape))).replace(' ','') for i in range(s.size)]
			else:
				return ['']
		else:
			return numpy.asarray(s).flatten()
	vals = []
	for k,v in kvtuples:
		if k != None and include != None and k not in include: continue
		if k != None and exclude != None and k in exclude: continue
		v = flatten(v, labels=labels)
		if labels: vals += ['.'+k+vi for vi in v]
		else: vals.append(v)
	if labels: return vals
	elif len(vals) == 1: return vals[0]
	else: return numpy.concatenate(vals, axis=0)

def unflatten(s, vals, include=None, exclude=None):
	if isinstance(s, sstruct): kvtuples = s._allitems()
	elif isinstance(s, dict): kvtuples = sorted(s.items())
	elif isinstance(s, numpy.ndarray): kvtuples = [(None,s)]
	else: kvtuples = list(zip([None for si in s],s))
	pos = 0
	for i,(k,v) in enumerate(kvtuples):
		if k != None and include != None and k not in include: continue
		if k != None and exclude != None and k in exclude: continue
		if isinstance(v, numpy.ndarray): n = v.size; v.flat = vals[pos:pos+n]
		elif isinstance(v, list):  n = len(v); v[:] = vals[pos:pos+n]
		elif isinstance(v, tuple): n = len(v); v = v.__class__(vals[pos:pos+n])
		else: n = 1; v = v.__class__(vals[pos])
		pos += n
		if isinstance(s, sstruct): s._setfield(k,v)
		elif isinstance(s, dict): s[k] = v
		elif not isinstance(s, numpy.ndarray): s[i] = v
	return s
	
class NoSubclassMethod(Exception): pass

class objective(sstruct):
	
	def __init__(self, _baseobj=None, **kwargs):
		self._inherit(_baseobj, _recursive=True, **kwargs)
		self._free = set()
		
	def free(self, *pargs, **kwargs):
		if len(pargs) + len(kwargs) == 0: pargs = self._allfields()
		for args in list(pargs) + list(kwargs.keys()):
			if not isinstance(args, (list,tuple)): args = (args,)
			for arg in args: self._free.add(arg)
		for k,v in list(kwargs.items()): self._setfield(k,v)
		
	def fix(self, *pargs, **kwargs):
		if len(pargs) + len(kwargs) == 0: pargs = self._allfields()
		for args in list(pargs) + list(kwargs.keys()):
			if not isinstance(args, (list,tuple)): args = (args,)
			for arg in args:
				if arg in self._free: self._free.remove(arg)
		for k,v in list(kwargs.items()): self._setfield(k,v)
		
	def _f(self, x, force1D=False, *unused):
		unflatten(self, x, include=self._free)
		f = self.eval()
		if force1D or not isinstance(f, float): f = flatten(f)
		return f
		
	def _df(self, x, force2D=False, *unused):
		unflatten(self, x, include=self._free)
		unflatten(self._deriv, x*0.0, include=self._free)
		df = self.deriv()
		if df == None:
			if not self._scalar: raise RuntimeError("if using the _deriv structure, zero-order function output should be scalar")
			df = flatten(self._deriv, include=self._free)
		else:
			if self._scalar:
				if numpy.asarray(df).shape != (self._np,):
					raise RuntimeError("if deriv() returns an output and zero-order output is scalar, deriv() output shape should (nparams,)")
			else:
				if df.shape != (self._nf,self._np):
					raise RuntimeError("if deriv() returns an output and zero-order output is non-scalar, deriv() output shape should (noutputs, nparams)")
		if force2D and len(numpy.asarray(df).shape) == 1: df = numpy.expand_dims(df, 0)
		return df

	def _ddf(self, x, force2D=False, *unused):
		unflatten(self, x, include=self._free)
		if self._dderiv != None:  unflatten(self._dderiv, x * 0.0, include=self._free)
		ddf = self.dderiv()
		if ddf == None:
			if not self._scalar: raise RuntimeError("if using the _dderiv structure, zero-order function output should be scalar")
			ddf = flatten(self._dderiv, include=self._free)
		else:
			if self._scalar:
				if numpy.asarray(ddf).shape != (self._np,):
					raise RuntimeError("if dderiv() returns an output and zero-order output is scalar, dderiv() output shape should (nparams,)")
			else:
				if ddf.shape != (self._nf,self._np):
					raise RuntimeError("if dderiv() returns an output and zero-order output is non-scalar, dderiv() output shape should (noutputs, nparams)")
		if force2D and len(numpy.asarray(ddf).shape) == 1: ddf = numpy.expand_dims(ddf, 0)
		return ddf

	def _H(self, x, *unused):
		unflatten(self, x, include=self._free)
		H = self.hessian()
		if H == None: H = self._hessian
		if H.shape != (self._np, self._np): # TODO(HHH)
			raise RuntimeError("hessian() output shape should (nparams, nparams)")
		return H
	
	def reset(self):
		pass
		
	def eval(self):
		"""
		Return function value f
		"""###
		raise NoSubclassMethod("this %s subclass should at least implement an eval() method" % self.__class__.__name__)
		
	def deriv(self):
		"""
		Compute first-derivatives (optional).
		
		This should either:
			fill in the fields of self._deriv and return None (possible only
			if the zero-order function output is a scalar) 
		or:
			If the zero-order function output is scalar, return a sequence or
			one-dimensional array of derivatives of the output with respect to
			each parameter.
			If the zero-order function output is non-scalar, return a
			two-dimensional (number-of-outputs by number-of-parameters) array.
		"""###
		raise NoSubclassMethod("this %s subclass has not implemented a first-order deriv() method" % self.__class__.__name__)
	def dderiv(self):
		"""
		Compute second-order derivatives (optional).
		
		This should either:
			fill in the fields of self._dderiv and return None (possible only
			if the zero-order function output is a scalar) 
		or:
			If the zero-order function output is scalar, return a sequence or
			one-dimensional array of derivatives of the output with respect to
			each parameter.
			If the zero-order function output is non-scalar, return a
			two-dimensional (number-of-outputs by number-of-parameters) array.
		"""###
		raise NoSubclassMethod("this %s subclass has not implemented a second-order dderiv() method" % self.__class__.__name__)
	def hessian(self):
		"""
		Compute a Hessian (optional).
		
		This should be a matrix, number-of-parameters by number-of-parameters.
		"""###   # TODO(HHH)
		raise NoSubclassMethod("this %s subclass has not implemented a hessian() method" % self.__class__.__name__)
	
	def _reset(self):
		self._p0 = sstruct([(k,v) for k,v in self._allitems() if k in self._free])
		flat_p0_free = flatten(self._p0, include=self._free)
		self._np = flat_p0_free.size
		self._f0 = self.eval()
		self._scalar = not hasattr(self._f0, '__len__')
		flat_f0_free = flatten(self._f0)
		self._nf = flat_f0_free.size

		try: self.deriv(); raise Exception()
		except NoSubclassMethod: self._deriv = None
		except:
			self._deriv = sstruct()
			for k,v in self._allitems():
				if k in self._free: self._deriv._setfield(k, v * 0.0)

		try: self.dderiv(); raise Exception()
		except NoSubclassMethod: self._dderiv = None
		except:
			self._dderiv = sstruct()
			for k,v in self._allitems():
				if k in self._free: self._dderiv._setfield(k, v * 0.0)

		try: self.hessian(); raise Exception()
		except NoSubclassMethod: self._hessian = None
		except:
			siz = [self._np, self._np]
			self._hessian = numpy.matrix(siz, dtype=numpy.float64)  # TODO(HHH): use of Hessians currently assumes scalar zero-order function output

		self.reset() # call developer's subclass method
		return flat_p0_free
		
	def optimize(self, *pargs):
		if len(pargs): self.fix(); self.free(*pargs)
		flat_p0_free = self._reset()
		import scipy.optimize # TODO: right now only bfgs is used. could make this an option, and/or intelligently detect whether deriv, dderv and hessian have been subclassed...
		p = scipy.optimize.fmin_bfgs(f=self._f, x0=flat_p0_free, fprime=self._df, disp=False)
		unflatten(self, p, include=self._free)
		return self

	def chgrad(self, plot=False, **kwargs):
		
		for k,v in list(kwargs.items()):
			self._setfield(k,v)
	
		pflat = self._reset()	
		out = sstruct()
		out.params = self._p0.copy()
		out._setfield('f.exact', self._f(pflat, force1D=True))
		out._setfield('df.exact', self._df(pflat, force2D=True))
		out._setfield('df.approx', out.df.exact * numpy.nan)

		try: out._setfield('ddf.exact', self._ddf(pflat, force2D=True))
		except NoSubclassMethod: do_ddf = False
		else: do_ddf = True; out._setfield('ddf.approx', out.ddf.exact * numpy.nan)

		try: out._setfield('H.exact', self._H(pflat))
		except NoSubclassMethod: do_hessian = False
		else: do_hessian = True; out._setfield('H.approx', out.H.exact * numpy.nan)
				
		fdelta = 1e-4
		e = fdelta * abs(pflat)
		e = numpy.fmax(1e-12, e)		
		keepgoing = (e > 0) # all True to start

		rpt = 0
		while keepgoing.any():
			rpt += 1	
			for iparam in range(self._np):	
				mask = pflat * 0.0
				mask.flat[iparam] = 0.5	
				p_below = pflat - e * mask
				p_above = pflat + e * mask				
				f_below = self._f(p_below)
				f_above = self._f(p_above)							
				adfj = (f_above - f_below) / e[iparam]
				out.df.approx[:, iparam] = adfj  # TODO(III): store data from multiple rpts?
				if do_hessian or do_ddf:
					df_below = self._df(p_below)
					df_above = self._df(p_above)
					addfj = (df_above - df_below) / e[iparam]			
				if do_ddf:
					out.ddf.approx[:, iparam] = addfj[:, iparam] # TODO(III): store data from multiple rpts?			
				if do_hessian:
					out.H.approx[:, iparam].flat = addfj.flat # TODO(III): store data from multiple rpts?			
				converged = True      # TODO(III): could check for convergence here. currently, only one iteration is performed
				if converged: keepgoing[iparam] = False
			e[keepgoing] /= 2
		
		unflatten(self, pflat, include=self._free)
		self._reset()
		out.plabels = ['p'+x for x in flatten(self, include=self._free, labels=True)]
		out.flabels = ['f'+x for x in flatten(self.eval(), include=self._free, labels=True)]
		
		fields = ['df', 'ddf', 'H']
		for k in fields:
			v = out._getfield(k, None)
			if v == None: continue
			v.abserr = abs(v.exact-v.approx)
			v.maxabserr = v.abserr.max()
			v.propabserr = v.abserr / numpy.fmax(abs(v.approx), 1e-13)
			v.maxpropabserr = v.propabserr.max()
		
		if plot:
			from . import Plotting
			pylab = Plotting.load_pylab()
		
			fields = ['df', 'ddf', 'H']
			labels = {'df':(out.flabels,out.plabels, 'd%s / d%s'),
			          'ddf':(out.flabels,out.plabels, 'd^2%s / d%s^2'),
			          'H':(out.plabels,out.plabels, 'df/(d%s)(d%s)'),
			}
			got = dict([(k,k in out._fields) for k in fields])			
			nplots = sum(got.values())
			for i,k in enumerate(fields):
				v = out._getfield(k, None)
				if v == None: continue
				if nplots > 1: pylab.subplot(1, nplots, i+1)
				x = abs(v.approx.flatten())
				y = v.abserr.flatten()
				both0 = numpy.logical_and(x==0.0, y==0.0)
				x[both0] = numpy.nan
				y[both0] = numpy.nan
				x[x==0.0] = 1e-13
				y[y==0.0] = 1e-13
				x = numpy.log10(x)
				y = numpy.log10(y)
				pylab.cla()
				ax = pylab.gca()
				ax.indicator, = pylab.plot([numpy.nan,numpy.nan],[numpy.nan,numpy.nan], color=(1,0,1), linewidth=2)
				v.handles = pylab.plot(numpy.expand_dims(x,0), numpy.expand_dims(y,0), linestyle='None', marker='o', markersize=10, picker=5)
				lab = labels[k]
				for j,h in enumerate(v.handles):
					subs = numpy.unravel_index(j, v.approx.shape)
					h.txt = (lab[2]+'\nprop. abs. err. = %g') % (lab[0][subs[0]], lab[1][subs[1]], v.propabserr.flat[j])
				ax.set_xlabel('log10 of abs approx value')
				ax.set_ylabel('log10 of abs error')
				ax.grid(True)
			
			def onpick(evt):
				h = evt.artist
				x,y = h.get_xdata(), h.get_ydata()
				ax = h.axes
				ax.set_title(h.txt)
				ind = ax.indicator
				xl,yl = numpy.asarray(ax.get_xlim()),numpy.asarray(ax.get_ylim())
				ind.set_xdata([x.mean(), xl.mean()])
				ind.set_ydata([y.mean(), yl.max() + numpy.diff(yl) * 0.01])
				ind.set_clip_on(False)
				ind.set_color(h.get_color())
				pylab.draw()
				
			pylab.gcf().canvas.mpl_connect('pick_event', onpick)
			pylab.draw()
			
		return out

class chgradtest(objective):
	# test function for chgrad: p is a two-element vector of parameters, x may be any size
	# full hessian may be returned, or just the diagonal (the latter being the default).
	
	def __init__(self, p, x):
		objective.__init__(self)
		self.x = numpy.asarray(x,dtype=numpy.float64)
		self.p = numpy.asarray(p,dtype=numpy.float64)
		if self.p.size != 2: raise ValueError("p should be a 2-element array")
		self._u = numpy.ones(self.x.shape);
		self._z = numpy.zeros(self.x.shape);
		self.fix('x')
		self.free('p')
	
	def eval(self):	
		x,u,z = self.x,self._u,self._z
		pdim = ([0]+[i for i,d in enumerate(x.shape) if d != 1])[-1] # last non-singular dim, or 0 if none
		p,q = numpy.asarray(self.p,dtype=numpy.float64).flat
		f = (2*x - 5*p) * (3*x + q**2)
		return f
	
	def deriv(self):
		x,u,z = self.x,self._u,self._z
		p,q = numpy.asarray(self.p,dtype=numpy.float64).flat
		df = numpy.c_[-15*x-5*q**2,  -10*p*q+4*x*q]
		return df  # -15xp -5pq^2 +2xq^2	

	def dderiv(self):
		x,u,z = self.x,self._u,self._z
		p,q = numpy.asarray(self.p,dtype=numpy.float64).flat
		ddf = numpy.c_[z, -10*p+4*x]
		return ddf

	def hessian(self):
		if self.x.size != 1: raise NoSubclassMethod() # pretend we're not here
		x,u,z = self.x,self._u,self._z
		p,q = numpy.asarray(self.p,dtype=numpy.float64).flat
		H = numpy.r_[numpy.c_[z, -10*q*u], numpy.c_[-10*q*u, -10*p+4*x]]
		return H
	


class psifit(objective):
	def __init__(self, x, y, **kwargs):
		objective.__init__(self)
		self.x = numpy.asarray(x).flatten()
		self.y = numpy.asarray(y).flatten()
		self.balance = False
		self.link = logistic
		self.fix(); self.free(shift=0.0, logscale=0.0)
		#self.shift = 0.0
		#self.logscale = 0.0
		#self._free = set(['shift', 'logscale'])
		self._update(kwargs)
		af = self._allfields()
		if 'loglower' in af and self.loglower > -numpy.inf: self._free.add('loglower')
		else: self.loglower = -numpy.inf
		if 'logupper' in af and self.logupper > -numpy.inf: self._free.add('logupper')
		else: self.logupper = -numpy.inf
		if 'logyoked' in af and self.logyoked > -numpy.inf: self._free.add('logyoked')
		else: self.logyoked = -numpy.inf
		if 'logyoked' in self._free and 'loglower' in self._free: self._free.remove('loglower')
		if 'logyoked' in self._free and 'logupper' in self._free: self._free.remove('logupper')
	
	def intermediates(self, x=None, y=None):
		if x == None: x = self.x
		if y == None: y = self.y
		el = numpy.exp(self.loglower)
		eu = numpy.exp(self.logupper)
		eq = numpy.exp(self.logyoked)
		eb = numpy.exp(self.logscale)
		a = self.shift
		h = 1.0 - el - eu - 2.0 * eq
		if h < 0.0: raise RuntimeError("impossible parameter values")
		u = eb * (x - a)
		f = self.link(u)
		p = el + eq + h * f # estimated probability(y>0)
		r = y * p + (1.0 - y) / 2.0   # rectified:  r = p where y=+1,  1-p where y=-1
		nlp = -numpy.log(r)
		return el,eu,eq,eb,h,u,f,p,r,nlp
	
	def psi(self, x):
		el,eu,eq,eb,h,u,f,p,r,nlp = self.intermediates(x=x, y=x*0.0)
		return p
		
	def eval(self):
		el,eu,eq,eb,h,u,f,p,r,nlp = self.intermediates()
		if self.balance:
			neg,pos = self.y<0,self.y>0
			return (nlp[neg].mean() + nlp[pos].mean()) / 2.0
		else:
			return nlp.mean()
			
	def deriv(self):
		if self.balance: neg,pos = self.y<0,self.y>0
		el,eu,eq,eb,h,u,f,p,r,nlp = self.intermediates()
		hfprime = h * self.link(u, deriv=1)
		d = self._deriv
		for k in d._allfields():
			if   k == 'loglower': v = (-el)     * f + el
			elif k == 'logupper': v = (-eu)     * f
			elif k == 'logyoked': v = (-eq*2.0) * f + eq
			elif k == 'logscale': v = hfprime * u
			elif k == 'shift':    v = hfprime * -eb
			v = v * (-self.y/r)
			if self.balance:  v = (v[neg].mean() + v[pos].mean()) / 2.0
			else: v = v.mean()
			d._setfield(k, v)
	
	def plot(self, nbins=None, nperbin=5, hold=False, drawnow=True, **kwargs):
		from . import Plotting
		pylab = Plotting.load_pylab()
		import matplotlib
		r = numpy.array([min(self.x), max(self.x)])
		r = r.mean() + 1.1 * (r - r.mean())
		b = numpy.linspace(r[0], r[1], num=200, endpoint=True)
		p = self.psi(b)

		binned = []
		if nbins == None:
			xx,yy = list(zip(*sorted(zip(self.x, self.y))))
			xx,yy = numpy.asarray(xx), numpy.asarray(yy)
			for i in range(0,len(xx),nperbin):
				x = (xx[i:i+nperbin]).mean()
				y = (yy[i:i+nperbin]>0).mean()
				binned.append((x,y))		
		else:
			bins = numpy.linspace(r[0], r[1], num=nbins, endpoint=True)
			bins = list(zip(bins[:-1], bins[1:]))
			for lo,up in bins:
				sel = numpy.logical_and(self.x < up, self.x >= lo)
				x = (self.x[sel]).mean()
				y = (self.y[sel]>0).mean()
				binned.append((x,y))

		x,y = list(zip(*binned))

		if not hold: pylab.cla()
		lineprops = dict(kwargs, marker='None')
		line = pylab.plot(b, p, **lineprops)
		dotprops = dict({'marker':'o', 'markersize':10}, **kwargs);
		dotprops.update({'linestyle':'None', 'color':line[0].get_color()})
		binned = pylab.plot(x, y, **dotprops)
		pylab.gca().grid(True)
		if drawnow: pylab.draw()
		return line, binned



