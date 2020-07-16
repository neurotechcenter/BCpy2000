# -*- coding: utf-8 -*-
# 
#   $Id: LearningTools.py 5752 2018-05-09 16:38:35Z jhill $
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
	'binomial', 'seqste',
	'all_pairs', 'one_versus_rest', 'infer_classes',
	'logistic', 'invlogistic', 
	'cg', 'invcg', 
	'dprime', 'running_mean', 'running_cov',
	'svd', 'lda', 'csp', 'csp_itfe',
	'confuse', 'balanced_loss', 'class_loss', 'eeop',
	'linkern', 'sqdist', 'rbfkern', 'guesswidth', 'kview',
	'predictor', 'klr2class', 'lda2class',
	'demodata',
	'plotopt',
	'foldguide', 'experiment',
	'overlapping',
	'spcov', 'shrinkcov', 'spfilt', 'symwhiten', 'symwhitenkern',
	'stfac', 'stfac_filters_and_patterns',
	'correlate', 'correlation_pvalue',
]
import numpy
import copy

from .NumTools import asmatrix, mad, loadmat, savemat, project, isequal
from functools import reduce
__all__ += ['asmatrix', 'mad', 'loadmat', 'savemat', 'project', 'isequal']

from .NumTools import summarize, sdict, reportstruct, sstruct
__all__ += ['summarize', 'sdict', 'reportstruct', 'sstruct', ]

def binomial(x, axis=None):
	"""
	Given a sequence or array of booleans <x>,
	Return a dict containing
	
		'mean':  proportion of True values,
		'ste':   standard error of the mean,
		'n':     number of observations,
	
	These dicts can be combined, to provide
	exact incremental standard error estimates,
	using seqste()
	
	"""###
	x = numpy.asarray(x, dtype=numpy.float64)
	if axis == None: n = x.size
	else: n = x.shape[axis]
	m = x.mean(axis=axis)
	v = (m * (1-m))/(n-1)
	e = v ** 0.5
	return {'mean':m,'ste':e,'n':int(n)}

def test_seqste(p=0.9, n=184):
	x = numpy.random.rand(n) < p
	b = binomial(x)
	n = int(b['n']/3)
	b1 = binomial(x[:n])
	b2 = binomial(x[n:n*2])
	b3 = binomial(x[n*2:])
	
	print(b, "(ground truth)")
	print(seqste(b1,b2,b3))
	print(seqste(b1,b3,b2))
	print(seqste(b2,b1,b3))
	print(seqste(b2,b3,b1))
	print(seqste(b3,b1,b2))
	print(seqste(b3,b2,b1))

def seqste(d1, *more):
	"""
	d1 and d2 both dicts with entries 'mean', 'ste' and 'n'.
	Return a similar dict with a combined estimate of the mean and standard-error-of-mean.
		
	Baker, R.W.R & Nissim, J.A. (1963):
		Expressions for Combining Standard Errors of Two Groups and for Sequential Standard Error
		Nature 198, 1020; doi:10.1038/1981020a0
		http://www.nature.com/nature/journal/v198/n4884/abs/1981020a0.html
	"""###
	if len(more) == 0: return d1
	d2 = more[0]
	keys = ['mean','ste','n']
	if sorted(d1.keys()) != sorted(keys) or sorted(d2.keys()) != sorted(keys):
		raise ValueError('data inputs should be dicts with fields %s' % ','.join(keys))
	def conv(x):
		if isinstance(x, numpy.ndarray): x = numpy.asarray(x, dtype=numpy.float64)
		if isinstance(x, (int,bool)): x = float(x)
		return x
	m1,e1,n1 = [conv(d1[k]) for k in keys]
	m2,e2,n2 = [conv(d2[k]) for k in keys]
	n3 = n1 + n2
	v3 = n1*(n1-1)*e1**2 + n2*(n2-1)*e2**2 + n1*n2*(m1-m2)**2/n3
	v3 /= n3 * (n3-1)
	e3 = v3 ** 0.5
	m3 = (m1*n1 + m2*n2) / n3
	result = {'mean':m3,'ste':e3,'n':int(n3)}
	return seqste(result, *more[1:])

def all_pairs(classes):
	"""
	for neg,pos in all_pairs(5):
		print neg,"versus",pos

	1 versus 2
	1 versus 3
	2 versus 3
	1 versus 4
	2 versus 4
	3 versus 4
	1 versus 5
	2 versus 5
	3 versus 5
	4 versus 5	

	You get the idea. The input may alternatively be a sequence of class
	identifiers.	
	"""###
	if isinstance(classes, (float,int)): classes = list(range(1,int(classes)+1))
	classes = sorted(tuple(set(classes)))
	pairs = []
	for x in range(len(classes)):
		for y in range(x):
			pairs.append((classes[y], classes[x]))
	return tuple(pairs)

def one_versus_rest(classes):
	"""
	for neg,pos in one_versus_rest(5):
		print neg,"versus",pos
		
	(2, 3, 4, 5) versus 1
	(1, 3, 4, 5) versus 2
	(1, 2, 4, 5) versus 3
	(1, 2, 3, 5) versus 4
	(1, 2, 3, 4) versus 5
	
	You get the idea. The convention is for the "rest" to come out first
	(to be labelled as -1 in the binary sub-problem) and the "one" to come
	out second (to be labelled as +1).
	
	The input may alternatively be a sequence of class identifiers.	
	"""###
	if isinstance(classes, (float,int)): classes = list(range(1,int(classes)+1))
	classes = set(classes)
	standardize = lambda x: tuple(sorted(tuple(x))) # because python sorts set((-1,1)) the wrong way round for some inexplicable reason
	if len(classes) == 2: return (standardize(classes),)
	rest = lambda x: standardize(classes.difference((x,)))
	pairs = [(rest(one),one) for one in standardize(classes)]
	return tuple(pairs)
		
def infer_classes(cc, types=None):
	"""
	classes,data = infer_classes(data)
	
	If <data> is a 2-element sequence, assign the classes (-1, +1).
	Otherwise, if <data> is an n-element sequence, return the classes
	1 through n.
	
	If data is a dictionary, the classes will be inferred from the keys,
	and the classes and data delivered in a standardized order.  If all the
	keys are scalar, they will simply be returned sorted in ascending order.
	If any are sequences, then all the keys will be made into sequences,
	sorted within themselves and then delivered in an order that is sorted
	first by decreasing length, then by value.  So, for example, the input
	
		{3:'one',  (2,1): 'rest'}
	
	will yield  classes=(  (1,2) , (3,)  )  and  data=( 'rest' , 'one' ).
	The sorting-by-decreasing-length ensures that, in a one-versus-rest
	problem, the "rest" will always come out first (to be mapped to -1) and
	the "one" will come out second (to be mapped to +1).
	"""###
	
	if len(cc) == 1 and isinstance(cc[0], (list,tuple,set,dict)): cc = cc[0]
	if len(cc) == 2: classes = (-1, +1)
	else: classes = tuple(range(1,len(cc)+1))
	isseq = lambda x: isinstance(x,(tuple,list,set))
	tuplify = lambda x: isseq(x) and tuple(x) or (x,)
	if isinstance(cc, dict):
		k = list(cc.keys())
		v = list(cc.values())
		if True in list(map(isseq, k)):
			k = [tuple(set(tuplify(x))) for x in k]
			keylencmp = lambda x,y: cmp( (-len(x[0]),)+x[0],(-len(y[0]),)+y[0] )
			classes, cc = list(zip(*sorted(zip(k,v),cmp=keylencmp)))
		else:
			classes, cc = list(zip(*sorted(cc.items())))
	if isinstance(types, list): types = tuple(types)
	if types != None and False in [isinstance(c,types) for c in cc]:
		raise TypeError('inputs must be of one of the following types: '+repr(types))
	return classes, cc
	
def logistic(x, deriv=0):
	f = 1.0 / (1.0 + numpy.exp(-x))
	if deriv == 0: return f
	elif deriv == 1: return f * (1.0 - f)
	else: raise ValueError("derivative %s not defined" % str(deriv))

def invlogistic(p):
	return -numpy.log(1.0/p - 1.0)
	
def cg(x, deriv=0):
	import scipy.special
	if deriv == 0: return 0.5 * scipy.special.erfc(-x / 2.0 ** 0.5)
	elif deriv == 1: return numpy.exp(-0.5 * x ** 2.0) * (numpy.pi * 2) ** -0.5
	else: raise ValueError("derivative %s not defined" % str(deriv))
	
def invcg(p):
	import scipy.special
	return 2.0 ** 0.5 * scipy.special.erfinv(2.0 * p - 1.0)


def dprime(*cc, **kwargs):
	"""
	Compute the dprime value (signed square root of the Fisher score)
	between two running_mean or running_cov objects which correspond
	to two different classes.
	"""###
	if len(cc):
		if len(kwargs): raise TypeError("supply either all-unnamed args or all-named")
		classes,cc = infer_classes(cc, [running_mean,running_cov])
		if len(cc) != 2: raise TypeError('expected two inputs')
		return (cc[1].m - cc[0].m) / numpy.sqrt(cc[1].v + cc[0].v)
	if len(kwargs):
		if len(cc): raise TypeError("supply either all-unnamed args or all-named")
		x = kwargs.pop('x')
		y = kwargs.pop('y')
		exemplar_dim = kwargs.pop('exemplar_dim', 0)
		if len(kwargs): raise TypeError("unexpected keyword argument %s" % list(kwargs.keys())[0])
		sub = [slice(None) for d in x.shape]
		sub[exemplar_dim] = [i for i,yi in enumerate(y.flat) if yi > 0.0]
		xsub = x[sub]
		mpos = xsub.mean(axis=exemplar_dim)
		vpos = xsub.var(axis=exemplar_dim)
		sub[exemplar_dim] = [i for i,yi in enumerate(y.flat) if yi < 0.0]
		xsub = x[sub]
		mneg = xsub.mean(axis=exemplar_dim)
		vneg = xsub.var(axis=exemplar_dim)
		return (mpos - mneg) / numpy.sqrt(vpos + vneg)
		
class running_mean(object):
	"""
	An object that keeps track of the mean and variance of a series
	of values x presented online. Each x may be a scalar value or a
	numpy.array.
	
	Exemplars are added to object r by the += operator:
		
		r = running_mean()
		r += x

	The object r has the following properties:
	    	
	    r.n          : number of samples so far
	    r.m          : mean of x so far (same shape as incoming x)
	    r.v          : variance of x so far, normalized by r.n (same
	                   shape as incoming x)		
	    r.v_unbiased : a virtual attribute which returns the variance
	                   normalized by (r.n - 1.0) instead of by r.n
	                   
	If r is created with fullcov=True, then elements x are flattened
	as they are added (so, for one thing, the mean and variance will
	be flat arrays with length equal to the number of elements of x)
	and a full covariance matrix is also computed, and is accessible
	using the properties r.C and r.C_unbiased (analogous to r.v and
	r.v_unbiased).
		                   
	If r.persistence=1.0, then all previous samples are "remembered"
	and each incoming exemplar counts as r.increment number of new
	samples (the increment may be measured in any units you like -
	seconds, for example). If r.persistence < 1.0,  then an
	exponential forgetting factor of 1.0-r.persistence is used, and
	although self.increment is added to s.n,  s.n does not fully
	reflect the number of degrees of freedom in the estimation, which
	is roughly equal to 1/(1.0-r.persistence)
	
	The reset() method zeroes everything.
	"""###
	def __init__(self, persistence=1.0, increment=1.0, fullcov=False):
		"""
		The persistence and increment arguments initialize the
		self.persistence and self.increment attributes.
		"""###
		self.increment = float(increment)
		self.persistence = float(persistence)
		self.fullcov = fullcov
		self.reset()
		
	def reset(self):
		self.sumx2 = 0.0
		self.sumx1 = 0.0
		self.denom = 0.0
		self.n = 0.0
		
	def update(self, x, increment=None):
		if increment == None: increment = self.increment
		persistence = self.persistence
		if self.n == 0.0: persistence = 0.0
		
		if self.fullcov:
			x1 = numpy.asarray(x).flatten()
			xM = numpy.asmatrix(x1)
			x2 = xM.H * xM
		else:
			x1 = x
			x2 = numpy.multiply(x, numpy.conj(x))
			
		self.sumx2 = persistence * self.sumx2 + x2
		self.sumx1 = persistence * self.sumx1 + x1
		self.denom = persistence * self.denom + increment
		self.n += increment
		
	def get_mean(self):
		if self.denom == 0.0: return numpy.nan
		return self.sumx1 / self.denom

	def get_variance_biased(self, return_fullcov=False):
		if self.denom == 0.0: return numpy.nan
		
		if return_fullcov and not self.fullcov:
			raise ValueError("full covariance matrices are not available from this object")
			
		if self.fullcov and not return_fullcov:
			mean_xsquared = self.sumx2.diagonal().A.flatten() / self.denom
		else:
			mean_xsquared = self.sumx2 / self.denom
			
		meanx = self.sumx1 / self.denom
		if self.fullcov and return_fullcov:
			meanx = numpy.asmatrix(meanx)
			squared_meanx = meanx.H * meanx
		else:
			squared_meanx = numpy.multiply(meanx, numpy.conj(meanx))
			
		return mean_xsquared - squared_meanx

	def get_variance_unbiased(self, return_fullcov=False):
		if self.denom <= self.increment: return numpy.nan
		return self.get_variance_biased(return_fullcov=return_fullcov) * (self.denom / (self.denom - self.increment))
		
	@apply
	def m():
		def fget(self): return self.get_mean()
		return property(fget=fget, doc="running mean estimate")
	@apply
	def v():
		def fget(self): return self.get_variance_biased(return_fullcov=False)
		return property(fget=fget, doc="running variance estimate normalized by n (see also v_unbiased)")
	@apply
	def v_unbiased():
		def fget(self): return self.get_variance_unbiased(return_fullcov=False)
		return property(fget=fget, doc="running variance estimate normalized by n-1 (see also v)")		
	@apply
	def C():
		def fget(self): return self.get_variance_biased(return_fullcov=True)
		return property(fget=fget, doc="running covariance estimate normalized by n (see also C_unbiased)")
	@apply
	def C_unbiased():
		def fget(self): return self.get_variance_unbiased(return_fullcov=True)
		return property(fget=fget, doc="running covariance estimate normalized by n-1 (see also C)")
		
	def __iadd__(self, x):
		self.update(x)
		return self
		
	def run(self, x, axis=-1, reset=False):
		"""
		Test the running_mean object by adding <x> one sample at a time,
		where samples are slices concatenated along the specified <axis>.
		
		If <reset> is passed as True, the object is reset first.
		
			running_mean().run(x, axis=0).m
				# should be the same as numpy.mean(x, axis=0)
				
			running_mean().run(x, axis=0).v
				# should be the same as numpy.var(x, axis=0)
				
			running_mean(fullcov=True).run(x, axis=0).C_unbiased
				# should be the same as numpy.cov(x, axis=0)
		"""###
		if reset: self.reset()
		x = numpy.array(x, copy=False)
		x = x.view()
		if axis < 0: axis += len(x.shape)
		x.shape = tuple(list(x.shape) + [1]*(axis+1-len(x.shape)))
		sub = [slice(None)] * len(x.shape)
		for i in range(x.shape[axis]): sub[axis] = i; self += x[sub]
		return self
		
	def plot(self, *pargs, **kwargs):
		"""
		Works only for an object that has accumulated information about
		two-dimensional inputs. Plots an ellipse centred on the computed
		mean, indicating the shape of the covariance of the distribution
		of x. The size of the ellipse is specified by optional keyword
		argument nstd=2.0 (any other arguments are passed through to plot).
		"""###
		if not self.fullcov: raise ValueError("plot method is only available for objects that compute full covariance matrices (construct with fullcov=True)")
		if numpy.asarray(self.m).size != 2: raise ValueError("plot method is only available for objects that have accumulated two-dimensional data")
		nstd = kwargs.pop('nstd', 2.0)
		r = numpy.linspace(0, 2*numpy.pi, 100)
		x = numpy.asmatrix(self.m).A + nstd*(svd(self.C).sqrtm * numpy.matrix([numpy.cos(r), numpy.sin(r)])).A
		from . import Plotting; pylab = Plotting.load_pylab()
		pylab.plot(x[0],x[1],*pargs,**kwargs)
		pylab.draw()

class running_cov(running_mean):
	"""
	A class for computing means, variances and covariances online, optionally with a decay factor.
	It is a subclass of running_mean for which the fullcov attribute is always initialized to True.
	It is included under the separate name running_cov purely for backward compatibility. See
	running_mean for more details.
	"""###
	def __init__(self, persistence=1.0, increment=1.0):
		running_mean.__init__(self, persistence=persistence, increment=increment, fullcov=True)


class ema(running_mean):
	def reset(self):
		self.sumx1 = 0.0
		self.sumx2 = 0.0
		self.n = 0.0
		self.denom = 1.0
	def update(self, x, increment=None):
		if increment == None: increment = self.increment
		persistence = self.persistence
		if persistence == 1.0: persistence = self.n / (self.n + increment)
		if self.n == 0.0: persistence = 0.0
		if self.fullcov:
			x1 = numpy.asarray(x).flatten()
			xM = numpy.asmatrix(x1)
			x2 = xM.H * xM
		else:
			x1 = x
			x2 = numpy.multiply(x, numpy.conj(x))
		self.sumx1 = persistence * self.sumx1 + (1.0 - persistence) * x1
		self.sumx2 = persistence * self.sumx2 + (1.0 - persistence) * x2
		self.n += increment
		
class svd(object):
	"""
	d = svd(A)   where A is m-by-n
	
	The following are computed immediately, using the singular value
	decomposition  A = d.U * numpy.diag(d.s) * d.V.H
	
	             d.s: the singular values of A in decreasing order
	          d.rank: estimated rank r of A, = #{singular values > tol},
	                  where tol defaults to max(m,n)*eps*max(d.s)
	          d.cond: estimated condition number of A, = max(d.s)/min(d.s)
	             d.U: m by r  orthonormal basis for the column space of A
	      d.leftnull: m by min(m,n)-r  the discarded columns of U
	             d.V: n by r  orthonormal basis for the row space of A
	          d.null: n by min(m,n)-r  the discarded columns of V
	      d.original: a copy of A, unless keepcopy is set to False
	      
	The following can then be computed cheaply. They are computed on demand
	the first time they are requested, and then cached:
	
	          d.pinv: pseudo-inverse of A
	         d.sqrtm: X such that  X*X.H = (U*S*U.H) and X.H*X = (V*S*V.H)
	                  (for symmetric A, therefore, X is the matrix-square-root)
	        d.isqrtm: the inverse of d.sqrtm, when A is invertible:  i.e. X
	                  such that X*X.H = (U*S*U.H).I  and  X.H*X = (V*S*V.H).I
	d.reconstruction: reconstruction of A, without using the discarded
	                  columns of d.U and d.V
	"""###
	def __init__(self, A, tol=None, keepcopy=True):
		A = asmatrix(A, copy=keepcopy)
		(U, s, Vh) = numpy.linalg.svd(A, full_matrices=False, compute_uv=True)
		if tol==None: tol = max(A.shape) * numpy.finfo(A.dtype).eps * max(s)
		r = numpy.sum(s > tol)
		if keepcopy: self.original = A
		self.rank = r
		smin,smax = min(s),max(s)
		if smin: self.cond = smax/smin
		else: self.cond = numpy.inf
		self.U = U[:,:r]          # columns of U are an orthonormal basis for the column space of A
		self.s = s[:r]
		self.V = Vh[:r,:].H       # columns of V are an orthonormal basis for the row space of A
		self.null = Vh[r:,:].H
		self.leftnull = U[:,r:]
	def __getattr__(self, key):
		if key == 'pinv':           self.__dict__[key] = self.V * numpy.diag(1.0/self.s)   * self.U.H
		if key == 'sqrtm':          self.__dict__[key] = self.U * numpy.diag(self.s**0.5)  * self.V.H          # X*X.H = (U*S*U.H),   X.H*X = (V*S*V.H)
		if key == 'isqrtm':         self.__dict__[key] = self.U * numpy.diag(self.s**-0.5) * self.V.H          # X*X.H = (U*S*U.H).I, X.H*X = (V*S*V.H).I  for invertible matrices
		if key == 'reconstruction': self.__dict__[key] = self.U * numpy.diag(self.s)       * self.V.H
		if not hasattr(self, key): raise AttributeError(key)
		return self.__dict__.get(key)
	def _getAttributeNames(self):
		return ['pinv', 'sqrtm', 'isqrtm', 'reconstruction']
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		s = '\n'.join([s] + ["    %s: % 3d by % 3d" % (tuple([key]+list(getattr(self,key).shape))) for key in ['U', 'V']])
		return s

class LDAError(Exception): pass
class lda(object):
	"""
	Fisher's linear discriminant analysis.  Example:
	
	    f = lda(ridge=0.1)
	    f.solve(rneg, rpos)  # rneg and rpos are running_cov objects
	                         # corresponding to the negative and positive
	                         # classes: their .m  and .C contain the
	                         # features' means and covariances respectively.
	    rneg += xneg
	    rpos += xpos
	
	    f.rebias(rneg, rpos) # don't re-solve, but re-bias according to the
	                         # updated estimates.
	     
	    f.predict(xtest)     # xtest is a sequence of features for a single
	                         # exemplar, or a features-by-exemplars array.
	
	f.w contains the weights
	f.b contains the bias
	"""###
	def __init__(self, ridge=0.0):
		"""
		Initialize the object's self.ridge attribute, indicating
		the amount of L2 regularization.  The self.ridge parameter is
		scaled by the mean diagonal element of the between-class
		covariance matrix, then added to the diagonal.
		"""###
		self.ridge = ridge
		
	def solve(self, *cc):
		"""
		f.solve(rneg, rpos)  # rneg and rpos are running_cov objects
		f.solve(d)           # the running_cov objects are in a sequence
		                     # or dict d (infer_classes is called).
		"""###
		self.classes,cc = infer_classes(cc, [running_cov])
		if len(cc) < 2: raise LDAError('need at least two classes')
		if len(cc) > 2: raise LDAError('multiclass LDA not supported')
		C = sum([c.C for c in cc]) # say that fast three times
		if self.ridge: C += numpy.eye(C.shape[0]) * self.ridge * C.diagonal().mean()
		self.w = numpy.linalg.solve(C, cc[1].m - cc[0].m)
		self.b = 0.0
		self.rebias(cc)
		return self
		
	def rebias(self, *cc):
		"""
		f.rebias(rneg, rpos)  # rneg and rpos are running_cov objects
		f.rebias(d)           # the running_cov objects are in a sequence
		                      # or dict d (infer_classes is called).
		"""###
		classes,cc = infer_classes(cc, [running_cov])
		m = sum([c.m for c in cc]) / float(len(cc))
		self.b = self.b - self.predict(m)
		
	def predict(self, x):
		"""
		Input is a sequence of features for a single exemplar, or a
		features-by-exemplars array. Output is a real-valued decision
		value per exemplar.
		"""###
		return asmatrix(self.w).H* asmatrix(x) + self.b

	def plot(self, *cc):
		classes,cc = infer_classes(cc, [running_cov])
		m = sum([c.m for c in cc]) / float(len(cc))
		cc[0].plot('b-')
		cc[1].plot('r-')
		db = [-self.w[1], self.w[0]]
		from . import Plotting; pylab = Plotting.load_pylab()
		xl = list(pylab.gca().get_xlim())
		yl = list(pylab.gca().get_ylim())
		pylab.plot([m[0]-2*db[0],m[0]+2*db[0]], [m[1]-2*db[1],m[1]+2*db[1]], 'k-')
		pylab.gca().set_xlim(xl)
		pylab.gca().set_ylim(yl)

class ldatest(object):
	def __init__(self,x1=None,x2=None,ridge=0.0):
		if x1==None: x1 = numpy.random.randn(2,1)+(numpy.matrix(numpy.random.rand(2,2))*numpy.matrix(numpy.random.randn(2,1000))).A
		if x2==None: x2 = numpy.random.randn(2,1)+(numpy.matrix(numpy.random.rand(2,2))*numpy.matrix(numpy.random.randn(2,1000))).A
		self.x1 = x1
		self.x2 = x2
		self.cc = [running_cov().run(x,1) for x in [self.x1,self.x2]]
		self.s = lda(ridge=ridge).solve(self.cc)

	def plot(self):
		from . import Plotting; pylab = Plotting.load_pylab()
		pylab.clf()
		pylab.plot(self.x1[0],self.x1[1],'bx')
		pylab.plot(self.x2[0],self.x2[1],'r+')
		self.s.plot(self.cc)
		ax = pylab.gca();
		xl,yl = ax.get_xlim(),ax.get_ylim()
		x = numpy.linspace(xl[0],xl[1],10)
		y = numpy.linspace(yl[0],yl[1],10)
		xx,yy=numpy.meshgrid(x,y)
		xy = numpy.concatenate((xx.reshape((1,xx.size)),yy.reshape((1,yy.size))),axis=0)
		z = self.s.predict(xy).reshape(xx.shape)
		z = logistic(z)
		z = numpy.array(z, order='C', dtype=numpy.float64, copy=True)
		pylab.contour(x,y,z,numpy.arange(0.1,1.0,0.1))
		pylab.draw()

class CSPError(Exception): pass
class csp(object):
	def __init__(self, classcov, globalcov=None, whichclass=1, normalize=False):
		"""
		Implements the CSP algorithm of Koles (1991).
		
		<classcov> is the class covariance matrix.  Alternatively, it is a
		sequence or dict of class-covariance matrices, in which case the
		class labels are inferred using infer_classes(), and the matrix
		corresponding to <whichclass> is used.
		
		<globalcov> is the global covariance. If omitted, then classcov must
		be a sequence or dict containing information about all relevant
		classes: then <globalcov> is computed as an equal weighting between
		the class of interest <whichclass>, and the mean of other classes.
		
		<normalize> is a boolean option specifying whether to normalize
		covariance matrices by their trace before use.
		
		The output object c has the following attributes:
		
		    c.classes     : the sequence of inferred classes
		    c.whichclass  : the class of interest
		    c.opts        : dict containing options (normalize)
		    c.classcov    : the class covariance matrix of interest (after
		                    normalization, if any)
		    c.globalcov   : the global covariance matrix used (after
		                    normalization, if any)
		    c.whitening   : symmetric whitening or spherizing matrix
		    c.rotation    : rotation directions (one in each column)
		    c.filters     : spatial filters (one in each column)
		    c.patterns    : spatial patterns (one in each column)
		    c.eigenvalues : eigenvalue corresponding to each column
		    
		The best() method is useful for selecting based on eigenvalue.
		"""###
		if not isinstance(classcov, (list,tuple,dict)): classcov = {whichclass:classcov}
		self.classes,classcov = infer_classes(classcov)
		if not whichclass in self.classes: raise CSPError('no information about class'+str(whichclass))

		for i in range(len(classcov)):
			if isinstance(classcov[i],running_mean): classcov[i] = classcov[i].m
			classcov[i] = numpy.matrix(classcov[i]) # copies, and ensures matrixhood
			if normalize: classcov[i] /= classcov[i].trace()			
		d = dict(list(zip(self.classes,classcov)))
		if len(self.classes)==1: self.classes = (-1,+1)
		self.whichclass = whichclass
		self.opts = {'normalize':normalize}
		self.classcov = d.pop(whichclass)
		
		if globalcov==None:
			if len(d) == 0: raise CSPError('too few matrices')
			globalcov = 0.5 * self.classcov + 0.5 * sum(d.values())/len(d)
			# (equal weighting of whichclass and the rest)
		if isinstance(globalcov,running_mean): globalcov = globalcov.m
		globalcov = numpy.matrix(globalcov) # copies, and ensures matrixhood
		if normalize: globalcov /= globalcov.trace()
		self.globalcov = globalcov

		s1 = svd(self.globalcov)
		self.whitening = s1.isqrtm
		m = self.whitening.H * self.classcov/2.0 * self.whitening
		s2 = svd(m)
		self.rotation = s2.U
		self.filters = self.whitening * self.rotation
		self.patterns = self.globalcov * self.filters
		for j in range(self.patterns.shape[1]):
			pattern = self.patterns[:,j]
			if numpy.max(numpy.abs(pattern)) == -numpy.min(pattern):
				pattern *= -1.0
				self.filters[:,j] *= -1.0
				self.rotation[:,j] *= -1.0
		self.eigenvalues = s2.s

	def best(self, n, m=None):
		"""
		c.best(n)  or  c.best([n]) returns a list of indices to the best
		n eigenvalues in the sense of absolute deviation from 0.5.
		
		c.best(n, m) or c.best([n,m])  returns a list of indices to the
		best n eigenvalues from the high end of the spectrum and the
		best m eigenvalues from the low end.
		
		Example:
		    ind = c.best(3, 3)
		    filt = c.filters[:, ind]
		    pat = c.patterns[:, ind]
		    eig = c.eigenvalues[:, ind]
		"""###
		n = numpy.array(n,copy=False).ravel().tolist()
		if len(n) == 1 and m != None:
			n += numpy.array(m,copy=False).ravel().tolist()
		if len(n) == 1:
			n += [0]
			d = numpy.abs(self.eigenvalues - 0.5)
		elif len(n) == 2:
			d = self.eigenvalues
		else: raise CSPError('expected either 1 or 2 numbers')
		pp = [(d[i], i) for i in range(len(d))]
		pp = [p[1] for p in sorted(pp, reverse=True)]
		out = pp[:n[0]]
		if n[1]: out += pp[-n[1]:]
		return out

def csp_itfe(filters, classcov=None, globalcov=None, classprob=None):
	"""
	For a set of spatial <filters> expressed one-per-column, a dict or
	sequence of <classcov> objects ( as supplied to csp() ) return the
	Information-Theoretic Feature Extraction criterion for CSP defined in:
	    Grosse-Wentrup and Buss (2008)
	      IEEE Transactions on Biomedical Engineering 55(8), 1991-2000
	
	Optionally, a list/numpy-array/dict of class probabilities may be
	supplied in <classprob>.
	"""###
	if isinstance(filters, csp):
		c = filters
		filters = c.filters
		if classcov == None: classcov = (c.classcov,)
		if globalcov == None: globalcov = c.globalcov
	if classcov==None: raise CSPError('class covariance matrices not supplied')
	
	if isinstance(globalcov, running_cov): globalcov = globalcov.C
	classes,classcov = infer_classes(classcov, [numpy.ndarray, running_cov])
	M = len(classes)
	if M == 1:
		if globalcov==None: raise CSPError('not enough covariance matrices')
		classcov = classcov[0]
		if isinstance(classcov, running_cov): classcov = classcov.C
		classcov = (2.0*globalcov-classcov, classcov)
		classes = (-1,1)
		M = 2
	#import IPython;IPython.Debugger.Tracer()()
	if classprob == None:
		classprob = numpy.asmatrix(numpy.ones((M,1)))/M
	else:
		if isinstance(classprob, numpy.ndarray):
			classprob = classprob.flatten().tolist()
		cl,clp = infer_classes(classprob)
		if len(cl) != len(classes) or (isinstance(classprob,dict) and cl != classes):
			raise CSPError('class labels are inconsistent between classcov and classprob inputs')
		classprob = numpy.matrix(clp).T
	W = numpy.asmatrix(filters)
	nfilt = W.shape[1]
	proj = numpy.asmatrix(numpy.zeros((nfilt,M)))
	default_globalcov = 0.0
	classcov = list(classcov)
	for j in range(M):
		if isinstance(classcov[j], running_cov): classcov[j] = classcov[j].C
		classcov[j] = numpy.matrix(classcov[j])
		C = classcov[j] / float(M)
		default_globalcov = default_globalcov + C
		proj[:, j].flat = numpy.diag(W.H * C * W)
	if globalcov == None: globalcov = default_globalcov
	nrm = numpy.diag(W.H * globalcov * W)
	nrm.shape += (1,)
	proj = proj / nrm
	v = -numpy.log(numpy.power(proj, 0.5))*classprob - numpy.power((numpy.power(proj,2.0)-1.0)*classprob, 2.0) * 3.0/16.0
	return v

def linkern(x, x2=None, exemplar_dim=0):
	"""
	Computes a linear kernel between data points x
	(or if x2 supplied, between x on the rows and x2
	on columns).  x (and x2) are packed data arrays,
	such that one step along dimension <exemplar_dim>
	is a step from one datapoint or exemplar to the
	next.
	"""###
	if x2 == None: x2 = x
	withself = (id(x2) == id(x))
	if not isinstance(x, numpy.ndarray): x = numpy.asarray(x)
	if withself: x2 = x
	if not isinstance(x2, numpy.ndarray): x2 = numpy.asarray(x2)
	nd = max(exemplar_dim+1, len(x.shape), len(x2.shape))
	if exemplar_dim < 0: exemplar_dim += nd
	ax = list(range(nd))
	ax.remove(exemplar_dim)
	x = project(x, nd-1)
	x2 = project(x2, nd-1)
	return numpy.tensordot(x, x2, axes=(ax,ax))

def rbfkern(x, x2=None, exemplar_dim=0, invcov=None, sigma=1.0):
	"""
	Computes a Gaussian RBF kernel between data points x (or if
	x2 is supplied, between x on the rows and x2 on the columns).
	x (and x2) are packed data arrays, such that one step along
	dimension <exemplar_dim> is a step from one datapoint or
	exemplar to the next. The argument <invcov>, if any, is
	passed through to sqdist() for computing distances, and
	<sigma> is the length scale by which the result is divided.
	"""###
	d2 = sqdist(x=x, x2=x2, exemplar_dim=exemplar_dim, invcov=invcov)
	d2 /= -2.0 * sigma ** 2.0
	d2 = numpy.exp(d2)
	return d2

def guesswidth(x=None, y=None, dsq=None, exemplar_dim=0, invcov=None, norm=2, return_rbfkern=False):
	"""
	Based on data <x>,  and optionally on labels <y>, guess the optimal length scale for
	an RBF kernel (and for return_rbfkern=True, return said kernel in addition to the
	estimated length scale).
	
	Options <exemplar_dim> and <invcov> are passed through to sqdist() in order to
	compute squared distances if you supply <x>. However, for efficiency reasons you
	may wish to precompute the squared distances, passing them in as <dsq> instead of
	<x>, in which case these options do nothing.
	
	More generally, you can base your width estimate on other norms besides the 2-norm
	distance, by setting <norm> to something other than 2.   Then, instead of squared
	distances, you may pass in <dsq> your pre-computed matrix of	
	
		dsq[i,j]   =  sum_over_k (     abs(x[i][k]  -   x[j][k])  **  norm    )
		
	where k indexes the features in each exemplar.  In other words, the elements of dsq
	should be p-norms-raised-to-the-power-p.
	
	"""###
	if dsq == None and x == None: raise ValueError("must supply either x or dsq")
	if dsq != None and x != None: raise ValueError("supply either x or dsq, but not both")
	if dsq == None:
		dsq = sqdist(x, exemplar_dim=exemplar_dim, invcov=invcov)
		if norm != 2: raise RuntimeError("currently, if you want to use norm values other than 2, you must precompute the matrix and pass it in as dsq")
	
	keep = numpy.logical_not(numpy.eye(dsq.shape[0], dtype=numpy.bool))
	if y != None:
		for i,yi in enumerate(y):
			for j,yj in enumerate(y):
				keep[i,j] = isequal(yi,yj)
	
	a = dsq.flat[keep.flatten()]
	sigma = numpy.median(a) ** (1.0/norm)
	if return_rbfkern:
		if x == None: dsq = dsq.copy() # don't modify original if that was passed in
		dsq /= -norm * sigma ** norm
		rbfK = dsq = numpy.exp(dsq)
		return sigma, rbfK
	else:
		return sigma

def sqdist(x, x2=None, exemplar_dim=0, invcov=None):
	"""
	Computes a matrix of squared Euclidean distances
	between data points x (or if x2 supplied, between 
	x on the rows and x2 on columns).  x (and x2) are
	packed data arrays, such that a step along dimension
	<exemplar_dim> is a step from one datapoint or
	exemplar to the next.
	
	If supplied, <invcov> should be a symmetric matrix.
	The distance is then computed in the space
	transformed by this matrix.  If p and q are m-by-1
	numpy.matrix representations of two data points,
	then the distance between them is computed as:
	
		(p-q).T  *  invcov  * (p-q)
	
	If <invcov> is truly the inverse of the covariance
	matrix between the features of <x>, then the result
	of the function is the Mahalanobis distance.
	
	"""###
	if x2 == None: x2 = x
	withself = (id(x2) == id(x))
	if not isinstance(x, numpy.ndarray): x = numpy.asarray(x)
	if withself: x2 = x
	if not isinstance(x2, numpy.ndarray): x2 = numpy.asarray(x2)
	nd = max(exemplar_dim+1, len(x.shape), len(x2.shape))
	if exemplar_dim < 0: exemplar_dim += nd
	ax = list(range(nd))
	ax.remove(exemplar_dim)
	x = project(x, nd-1)
	x2 = project(x2, nd-1)
	if invcov == None:
		xmag = numpy.multiply(x, x)
		for dim in ax: xmag = numpy.expand_dims(xmag.sum(axis=dim), dim)
		if withself:
			x2mag = xmag.view()
		else:
			x2mag = numpy.multiply(x2, x2)
			for dim in ax: x2mag = numpy.expand_dims(x2mag.sum(axis=dim), dim)
		xmag.shape = (xmag.size,1)
		x2mag.shape = (1,x2mag.size)
		return xmag + x2mag - 2 * numpy.tensordot(x, x2, axes=(ax,ax))
	else:
		A = numpy.asmatrix(invcov)
		if exemplar_dim == 0: p = x # project() has already made a view of x
		else: p = x.swapaxes(0, exemplar_dim)
		p.shape = (p.shape[0], p.size/p.shape[0])
		p = numpy.asmatrix(p)
		pA = p * A
		pmag = numpy.multiply(p, pA).sum(axis=1)
		if withself:
			q, qA, qmag = p, pA, pmag.view()
		else:
			if exemplar_dim == 0: q = x2 # project() has already made a view of x2
			else: q = x2.swapaxes(0, exemplar_dim)
			q.shape = (q.shape[0], q.size/q.shape[0])
			q = numpy.asmatrix(q)
			qA = q * A
			qmag = numpy.multiply(q, qA).sum(axis=1)
		pmag.shape = (pmag.size, 1)
		qmag.shape = (1, qmag.size)
		return pmag + qmag - 2 * pA * q.T
		# NB: this assumes A is symmetric --- really it's -p * (A+A.T) * q.T		

def kview(K, y=None):
	from . import Plotting; pylab = Plotting.load_pylab()
	import matplotlib
	pylab.clf()
	K = numpy.asarray(K)
	if y == None:
		fdiff = []
	else:
		if K.shape[1] != K.shape[0]: raise ValueError("K must be square to reorder by label")
		order = list(zip(*sorted([(yi,i) for i,yi in enumerate(y)])))[1]
		y = numpy.asarray(y)[order,:]
		K = K[order,:][:,order]
		fdiff = numpy.where(numpy.diff(y, axis=0))[0] + 0.5
	Plotting.imagesc(K)
	for d in fdiff:
		pylab.plot(pylab.gca().get_xlim(), [d,d], linewidth=2, linestyle='--', color=(0.0,1.0,0.0), scalex=False, scaley=False)
		pylab.plot([d,d], pylab.gca().get_ylim(), linewidth=2, linestyle='--', color=(0.0,1.0,0.0), scalex=False, scaley=False)
	pylab.colorbar()
	pylab.draw()

def confuse(true, predicted, labels=None, exemplar_dim=0):
	"""
	Returns a confusion matrix and a list of unique labels, based on paired lists of
	true and predicted labels.
	
	Output rows correspond to the possible true labels and columns correspond to the
	possible predicted labels. This is the ordering assumed in, for example,
	balanced_loss().
	"""###
	true = numpy.asarray(true)
	predicted = numpy.asarray(predicted)
	nd = max(exemplar_dim+1, len(true.shape), len(predicted.shape))
	if exemplar_dim < 0: exemplar_dim += nd
	true = true.swapaxes(exemplar_dim, 0)
	predicted = predicted.swapaxes(exemplar_dim, 0)
	if len(true) != len(predicted): raise ValueError('mismatched numbers of true and predicted labels')

	def isequal(a,b):
		if isinstance(a, str) and isinstance(b, str): return a == b
		a = numpy.asarray(a)
		b = numpy.asarray(b)
		ndd = len(b.shape) - len(a.shape)
		if ndd > 0: a.shape += (1,) * ndd
		if ndd < 0: b.shape += (1,) * -ndd
		if a.shape != b.shape: return False
		return numpy.logical_or(a == b, numpy.logical_and(numpy.isnan(a), numpy.isnan(b))).all()
	def find(a, b, append=False):
		for i in range(len(b)):
			if isequal(a, b[i]): return i
		if append: b.append(a); return len(b)-1
		else: return None

	n = len(true)
	c = {}
	found_labels = []
	for i in range(n):
		tv,pv = true[i],predicted[i]
		ti = find(tv, found_labels, append=True)
		pi = find(pv, found_labels, append=True)
		key = (ti,pi)
		c[key] = c.get(key,0) + 1

	if labels == None:
		labels = list(found_labels)
		try: labels.sort()
		except: pass
	else:
		labels = list(labels)
		for fi in found_labels:
			if find(fi, labels) == None: raise ValueError('inputs contain labels not listed in the <labels> argument')
	nclasses = len(labels)
	C = numpy.zeros((nclasses, nclasses), dtype=numpy.float64)
	for i in range(nclasses):
		ti = find(labels[i], found_labels, append=False)
		if ti == None: continue
		for j in range(nclasses):
			tj = find(labels[j], found_labels, append=False)
			if tj == None: continue
			C[i,j] = c.get((ti,tj), 0)
	return C,labels
	
def balanced_loss(true=None, predicted=None, confusion_matrix=None):
	"""
	err, se = balanced_loss(true, predicted)
	err, se = balanced_loss(confusion_matrix=C)
		where C = confuse(true, predicted)
	
	A classification loss function. As in confuse(), each row of <true> or
	<predicted> is a label for one instance or data point.
	
	balanced_loss() is asymmetric with regard to its inputs: it is the mean
	of the misclassification rates on each of the classes (as per <true>).
	"""###	
	if confusion_matrix == None:
		predicted = numpy.asarray(predicted).flatten()
		if (predicted > numpy.floor(predicted)).any(): predicted = numpy.sign(predicted)
		confusion_matrix,labels = confuse(true=true, predicted=predicted)

	confusion_matrix = numpy.asarray(confusion_matrix, dtype=numpy.float64)
	hits = confusion_matrix.diagonal()
	totals = confusion_matrix.sum(axis=1)
	hits = hits[totals != 0]
	totals = totals[totals != 0]
	err = 1 - (hits /totals)
	ste = (err * (1 - err) / (totals-1)) ** 0.5
	
	n = totals.min() # combine means and standard errors as if all classes had the same number of members as the smallest class
	                 # (for means, that's just a flat average of error rates across classes; for standard errors it's the most conservative way to do it)
	d = [{'mean':err[i], 'ste':ste[i], 'n':n} for i in range(len(totals))]
	d = seqste(*d)
	return d['mean'],d['ste']


def class_loss(true=None, predicted=None, confusion_matrix=None):
	"""
	err, se = class_loss(true, predicted)
	err, se = class_loss(confusion_matrix=C)
		where C = confuse(true, predicted)
	
	Actually class_loss() is symmetrical in its input arguments, but the
	order (true, predicted) is the convention established elsewhere,
	e.g. in balanced_loss()
	
	A classification loss function. As in confuse(), each row of <true>
	or <predicted> is a label for one instance or data point.
	"""###
	
	if confusion_matrix == None:
		predicted = numpy.asarray(predicted).flatten()
		if (predicted > numpy.floor(predicted)).any(): predicted = numpy.sign(predicted)
		confusion_matrix,labels = confuse(true=true, predicted=predicted)

	confusion_matrix = numpy.asarray(confusion_matrix, dtype=numpy.float64)
	n = confusion_matrix.sum()
	err = 1 - confusion_matrix.trace() / n
	se = (err * (1 - err) / (n-1)) ** 0.5
	return err,se
	
def eeop(f,y):
	"""
	A one-dimensional classifier: given values f and binary labels y (+/- 1),
	return the equal-error operating point. 
	"""###
	e = {
		-1:numpy.zeros((len(f),), dtype=numpy.float64),
		+1:numpy.zeros((len(f),), dtype=numpy.float64),
	}
	y = numpy.sign(y)
	z = sorted(zip(f,y))
	for i,(fi,yi) in enumerate(z):
		if yi == 0: continue
		if i:
			e[yi][i] = e[yi][i-1] + 1
			e[-yi][i] = e[-yi][i-1]
		else:
			e[yi][i] = 1
	for k in e: e[k] /= e[k][-1]
	e[+1] = 1 - e[+1]
	de = abs(e[+1] - e[-1])
	de = de[:-1]
	mde = min(de)
	ind = numpy.where(de==mde)[0]
	#import Plotting; pylab = Plotting.load_pylab(); pylab.subplot(1,2,1); Plotting.plot(z); Plotting.plot(numpy.c_[e[-1],e[+1]], hold=1); pylab.subplot(1,2,2)
	f,y = list(zip(*z))
	f = numpy.asarray(f)
	return numpy.mean([f[ind].mean(), f[ind+1].mean()])

def kern(x, x2=None, spec=None, **kwargs):
	spec = sstruct(spec)
	spec._update(kwargs)
	func = spec.func
	del spec.func
	if spec._fields == ['params']: spec = spec.params 
	spec = dict(spec._allitems())
	return func(x, x2, **spec)	

def demodata(n=(80,40), randseed=None):
	if isinstance(n, int): n = (int(n/2.0), int(n/2.0)+int(int(n/2.0) < n/2.0))
	n1,n2 = n

	m1 = [0.75, 0]
	S1 = [[1, -0.3], [-0.3, 1]]

	m2 = [-0.75, 0]
	S2 = [[1, 0.95], [0.95, 1]]

	oldrandstate = None
	if randseed != None:
		oldrandstate = numpy.random.get_state();
		if isinstance(randseed, int): numpy.random.seed(randseed)
		else: numpy.random.set_state(randseed)
	
	x1 = numpy.random.randn(n1,2) * svd(S1).sqrtm + m1
	x2 = numpy.random.randn(n2,2) * svd(S2).sqrtm + m2

	if oldrandstate != None:
		numpy.random.set_state(oldrandstate)
	
	x = numpy.r_[x1, x2]
	y = [-1] * n1 + [+1] * n2
	return numpy.asarray(x),numpy.asarray(y)

class predictor(sstruct):
	"""
	Virtual superclass for classification and regression problems.
	klr2class is an example of a working subclass, for classification.
	# example 1: single training, kernel computed by hand
	
		from SigTools.LearningTools import *
	
		xtrain,ytrain = demodata()

		c = klr2class(C=1.0, relcost='balance', lossfunc=balanced_loss)
		s = guesswidth(xtrain,ytrain)
		K = rbfkern(xtrain, sigma=s)
		c.train(K=K, y=ytrain)
		print c
	
		xtest,ytest = demodata()
	
		K_testtrain = rbfkern(xtest, xtrain, sigma=s)
		c.test(K=K_testtrain, true=ytest)
		print c
	

	# example 2: using a kernel function inside the cross-validation loop
	
		from SigTools.LearningTools import *
		
		xtrain,ytrain = demodata()

		c = klr2class(relcost='balance', lossfunc=balanced_loss)
		c.varyhyper({'C':[10.0,1.0,0.1], 'kernel.func':rbfkern, 'kernel.sigma':[0.5,1.0,1.5,2.0]})
		c.train(x=xtrain, y=ytrain)
		
		xtest,ytest = demodata()

		c.test(x=xtest, true=ytest)
		c.plotd()
		
	"""###
	def __init__(self, hyper=None, model=None, output=None, verbosity=2, lossfunc=class_loss, lossfield='y'):
		sstruct.__init__(self)
		self.input = sstruct()
		self.input.x=None
		self.input.K=None
		self.input.y=None
		self.input.istrain=None
		self.input.istest=None
		self.hyper = sstruct(hyper)
		self.model = sstruct(model)
		self.output = sstruct(output)
		self.loss = sstruct()
		self.update_loss(func=lossfunc, field=lossfield, training=False, testing=False)
		self.verbosity = verbosity
		self._allowedfields = list(self._fields) + ['cv']
	
	def write(self, txt):
		import sys
		sys.stdout.write(txt)
		sys.stdout.flush()
		
	def update_loss(self, func=None, field=None, training=True, testing=True):
		self.loss.func = getattr(self.loss, 'func', None)
		self.loss.field = getattr(self.loss, 'field', None)		
		newfunc = (func != None and func != self.loss.func)
		newfield = (field != None and field != self.loss.field)
		if newfunc: self.loss.func = func
		if newfield: self.loss.field = field

		if training or newfunc or newfield:
			self.loss.train = None
			self.loss.train_se = None
		if testing or newfunc or newfield:
			self.loss.test = None
			self.loss.test_se = None
		
		if self.loss.func != None and (training or testing):
			if not hasattr(self.output, self.loss.field): raise RuntimeError("could not find subfield 'output.%s'" % self.loss.field)
			out = getattr(self.output, self.loss.field)
			istrain = self.input.istrain
			istest = self.input.istest
			if training and istrain != None and istrain.any(): self.loss.train, self.loss.train_se = self.loss.func(true=self.input.y[istrain], predicted=out[istrain])
			if testing and istest != None and istest.any(): self.loss.test, self.loss.test_se = self.loss.func(true=self.input.y[istest], predicted=out[istest])
		return self
		
	def pretrain(self): pass
	def training(self): raise RuntimeError("no training() method implemented")
	def testing(self): raise RuntimeError("no testing() method implemented")
					
	def doublecv(self, x=None, K=None, y=None, istrain=None, istest=None, outerfg=None, **innerkwargs):
		"""
		All-singing all-dancing double-nested cross-validated training/evaluation procedure.
		
		To train/test once, as per cvtrain(), supply <istrain> and/or <istest>.
		To train/test on multiple folds, supply a foldguide object as <outerfg>.
		
		To perform a hyperparameter grid-search during cross-validation within each
		training set, make sure that self.hyper is an experiment object (for example,
		use the varyhyper() method to do this). Extra **kwargs are passed on to
		cvtrain() which uses them in the construction of a foldguide object, to dictate
		how the inner folding is performed. This includes <randomseed>, which is
		handled in a special way:  a different inner <randomseed> is used for each
		outer fold (the seed is incremented by 1 from one outer fold to the next),
		but the same <randomseed> is used across all grid-search conditions within
		a given outer fold: this ensures that each hyperparameter setting is tested
		on exactly the same train/test splits as each other.
		
		The return value is a list of trained copies of <self>, one per outer-fold.
		If grid-search was performed, each object has .model, .output and .loss fields
		corresponding to the best-in-grid hyperparameter settings for that fold. An
		additional substruct .cv gives some insight into this decision.
		
		Note that it is often convenient to conceptualize inner-cv-with-hyperparameter-
		grid-search as an integral part of an algorithm's "training" procedure.
		Therefore, if an object has an experiment object in self.hyper, this function
		will also be called automatically (in one-outer-fold-only mode) if you call
		self.train() or self.cvtrain(), these two being synonymous for such objects.
		The difference to calling doublecv() directly is merely that the attributes
		of self are then actually updated to match the chosen setting, instead of a
		copy being returned.
		"""###

		folded = (outerfg != None)
		varied = isinstance(self.hyper, experiment)
			
		if folded:
			n_outerfolds = len(outerfg)
			if istest != None or istrain != None: raise ValueError("either supply outerfg, or (istrain and/or istest), but not a mixture")
		else:
			n_outerfolds = 1

		if varied:
			expt = self.hyper
		else:
			expt = [self.hyper]
			if 'cv' in self._fields: del self.cv

		n_conditions = len(expt)
				
		x,K,y,istrain,istest = self.resolve_training_inputs(x=x,K=K,y=y,istrain=istrain,istest=istest)
		if folded and not isequal(outerfg.ids, list(range(len(y)))): raise ValueError("outerfg has the wrong ids for this dataset")
	
		z = numpy.zeros((n_conditions,n_outerfolds), dtype=numpy.float64)
		inner_loss = z + numpy.nan
		inner_se   = z + numpy.nan
		outer_loss = z + numpy.nan
		outer_se   = z + numpy.nan
		chosen     = [None for i in range(n_outerfolds)]
		chosenind  = [None for i in range(n_outerfolds)]

		if 'ntrain' not in innerkwargs and 'ntest' not in innerkwargs and 'folds' not in innerkwargs: innerkwargs['folds'] = 10
		innerseed = innerkwargs.pop('randomseed', None)
		if innerseed == None:
			innerseed = [foldguide.next_randomseed() for ifold in range(n_outerfolds)]
		if isinstance(innerseed, (int,float)):
			innerseed = [innerseed + ifold for ifold in range(n_outerfolds)]

		import inspect
		innernames,xx,yy,innerdefaults = inspect.getargspec(foldguide.__init__)
		folding = dict([(k,v) for k,v in zip(innernames[1:], innerdefaults) if v != None])
		folding.update(innerkwargs)

		prevkernel = None
		makekernel = 'kernel' in expt[0]._fields
		input_for_kernel = None
		if makekernel:
			if K != None: self.input.K = K = None
			#if K != None: raise ValueError("since hyper.kernel field exists, input argument K is unexpected")
			if x == None: raise ValueError("since hyper.kernel field exists, input argument x is expected")
			if expt._order == 'C': expt._reorder_fields('kernel', 0)
			elif expt._order == 'F': expt._reorder_fields('kernel', -1)
			input_for_kernel = x
			x = None
			
		for icond,condition in enumerate(expt):
			if self.verbosity and varied:
				self.write('hyper[%d], condition %d of %d (%s):\n' % (icond, icond+1, len(expt), expt._shortdesc(condition)))
			self_c = self.copy(deep=False)
			self_c.hyper = condition

			if makekernel:
				if not isequal(condition.kernel, prevkernel): 
					K = kern(x=input_for_kernel, spec=condition.kernel)
				del condition.kernel

			for ifold in range(n_outerfolds):
				if folded: istrain,istest = outerfg[ifold]
				self_cf = self_c.copy(deep=False)
				self_cf.cvtrain(x=x,K=K, y=y, istrain=istrain, istest=istest, randomseed=innerseed[ifold], **innerkwargs)
				isbest = (chosen[ifold] == None)
				isbest = isbest or self_cf.loss.train < chosen[ifold].loss.train
				isbest = isbest or (self_cf.loss.train == chosen[ifold].loss.train and self_cf.loss.train_se < chosen[ifold].loss.train_se)
				if isbest:
					chosen[ifold] = self_cf
					chosenind[ifold] = icond
									
				inner_loss[icond,ifold] = self_cf.loss.train
				inner_se[icond,ifold]   = self_cf.loss.train_se
				outer_loss[icond,ifold] = self_cf.loss.test
				outer_se[icond,ifold]   = self_cf.loss.test_se
				
		for ifold in range(n_outerfolds):
			self_f = chosen[ifold]
			if self_f == None: continue
			if 'cv' in self_f._fields: del self_f.cv
			if self_f.input.x == None: self_f.input.x = input_for_kernel
			self_f._setfield('cv.folding', dict(folding, randomseed=innerseed[ifold]))
			self_f._setfield('cv.chosen.index', chosenind[ifold])
			self_f._setfield('cv.chosen.hyper', expt[chosenind[ifold]])
			self_f._setfield('cv.inner.mean', list(inner_loss[:,ifold]))
			self_f._setfield('cv.inner.se',   list(inner_se[:,ifold]))
			self_f._setfield('cv.outer.mean', list(outer_loss[:,ifold]))
			self_f._setfield('cv.outer.se',   list(outer_se[:,ifold]))
			self_f.hyper = self.hyper.copy(deep=True)
			self_f._reorder_fields('verbosity', -1)

		if self.verbosity and varied:
			for ifold,self_f in enumerate(chosen):
				if folded: self.write('outerfg[%d] - ' % (ifold))
				fname, outerstr = self_f.loss_str(field='test')
				fname, innerstr = self_f.loss_str(field='train')
				self.write("picked hyper[%d] (%s): %s = outer %s; inner %s\n" % (
					self_f.cv.chosen.index,
					expt._shortdesc(self_f.cv.chosen.hyper),
					fname,
					outerstr,
					innerstr,
				))
		return chosen

	def loss_str(self, field=None, mean=None, se=None):
		fname = self.loss.func.__name__.replace('_', ' ')
		if field != None:
			if mean == None: mean = self.loss._getfield(field)
			if se == None: se = self.loss._getfield(field + '_se')
		if fname in ['balanced loss', 'class loss']:
			fmt = '%4.1f%%'
			if mean != None: mean *= 100.0
			if se != None: se *= 100.0
		else:
			fmt = '%.3f'
		if mean == None:
			s = 'N/A'
		else:
			s = fmt % mean
			if se != None: s += (' +/- ' + fmt) % se
		return fname, s
	
	def defaults(self): return {}
	
	def varyhyper(self, *pargs, **kwargs):
		if not isinstance(self.hyper, experiment):
			self.hyper = experiment([(k,[v]) for k,v in self.hyper._allitems()])
		if len(kwargs) == 0 and len(pargs) == 0: kwargs = self.defaults()
		for arg in pargs:
			if hasattr(arg, '_allitems'): arg = arg._allitems()
			elif hasattr(arg, 'items'): arg = list(arg.items())
			for k,v in arg: self.hyper._setfield(k,v)
		for k,v in list(kwargs.items()): self.hyper._setfield(k, v)
		return self

	def fixhyper(self, *pargs, **kwargs):
		if 'cv' in self._fields:
			self.hyper = self.cv.chosen.hyper
			del self.cv
		elif isinstance(self.hyper, experiment):
			if len(self.hyper) == 1: self.hyper = self.hyper[0]
			elif len(pargs) == 0 and len(kwargs) == 0: raise ValueError("cannot fix hyperparameters---no CV has been performed")
		for arg in pargs:
			if hasattr(arg, '_allitems'): arg = arg._allitems()
			elif hasattr(arg, 'items'): arg = list(arg.items())
			for k,v in arg: self.hyper._setfield(k,v)
		for k,v in list(kwargs.items()): self.hyper._setfield(k, v)
		return self
			
	def train(self, x=None, K=None, y=None, istrain=None, istest=None, **kwargs):
		if isinstance(self.hyper, experiment): return self.cvtrain(x=x, K=K, y=y, istrain=istrain, istest=istest, **kwargs)
		printtime = kwargs.pop('printtime', False)
		if printtime: import time; tstart = time.time()
		if len(kwargs): raise TypeError("unexpected keyword arguments---use these only when self.hyper is an experiment object, so train() and cvtrain() are synonymous")
			
		if 'kernel' in self.hyper._fields: self.input.K = None
			
		x,K,y,istrain,istest = self.resolve_training_inputs(x=x, K=K,y=y,istrain=istrain,istest=istest)
		input_for_kernel = x
		
		if 'kernel' in self.hyper._fields:
			if x == None: raise ValueError("since hyper.kernel field exists, input x is expected")
			if K != None: raise ValueError("since hyper.kernel field exists, input K is unexpected")
			K = kern(x=x, spec=self.hyper.kernel)
			x = None
			
		for f in set(self.input._fields): setattr(self.input, f, None)

		# subclass method training() will *only* see the training set: precludes label leakage
		if istrain.all():
			self.input.x = x
			self.input.K = K
			self.input.y = y
			self.input.istrain = istrain
			self.input.istest = istest
		else:
			if x != None: self.input.x = numpy.asarray(x)[istrain]
			if K != None: self.input.K = K[istrain,:][:, istrain]
			self.input.y = y[istrain]
			self.input.istrain = istrain[istrain]
			self.input.istest = istest[istrain]

		for f in set(self.model._fields): setattr(self.model, f, None)
		for f in set(self.output._fields): setattr(self.output, f, None)
		if self.input.K == None: self.training(x=self.input.x, y=self.input.y)
		else: self.training(K=self.input.K, y=self.input.y)  # should set model fields

		if K == None: ntrain = len(x)
		else: ntrain = K.shape[1]

		self.input.x = x
		if K != None:
			if istrain[:ntrain].all(): self.input.K = K
			else: self.input.K = K[:, istrain]
		self.input.y = None
		self.input.istrain = istrain
		self.input.istest = istest

		for f in set(self.output._fields): setattr(self.output, f, None)
		if self.input.K == None: self.testing(x=self.input.x)
		else: self.testing(K=self.input.K) # should set output fields
		
		if x == None: x = input_for_kernel
		self.input.x = x
		self.input.K = K
		self.input.y = y
		self.input.istrain = istrain
		self.input.istest = istest
	
		self.update_loss(training=True, testing=True)
		if self.verbosity >= 2: self.write(self.summarystr()[0] + '\n')
		elif self.verbosity >= 1: self.write(self.summarystr()[1])
		if printtime: print("%.2f seconds" % (time.time() - tstart))
		return self
			
	def test(self, x=None, K=None, true=None):
		if x == None and K == None: raise ValueError("either K or x must be supplied")
		
		self.fixhyper()
		if 'kernel' in self.hyper._fields and K==None:
			K = kern(x=x, x2=self.input.x, spec=self.hyper.kernel)
			
		if K == None:
			nnew = len(x)
			full_K = None
		else:
			K = numpy.asmatrix(K, dtype=self.input.K.dtype)
			if K.shape[1] != self.input.K.shape[1]: raise ValueError("test/train kernel must have the same number of columns as the kernel used for training (=%d)" % self.input.K.shape[1])
			nnew = K.shape[0]
			full_K = numpy.r_[self.input.K, K]

		if x == None:
			full_x = None
			self.input.x = None
		else:
			if isinstance(self.input.x, list): full_x = self.input.x + list(x)
			else: full_x = numpy.concatenate((self.input.x,x), axis=0)

		self.loss.test = self.loss.test_se = None
		got_true = (true != None)
		if not got_true:
			true = numpy.zeros((nnew,)+self.input.y.shape[1:], dtype=float)
			true.flat = numpy.nan
			self.loss.test = self.loss.test_se = None
		
		full_y = numpy.r_[self.input.y, project(true, len(self.input.y.shape)-1)]
		full_istrain = numpy.r_[self.input.istrain, numpy.zeros((nnew,), dtype=numpy.bool)]
		full_istest = numpy.r_[self.input.istest * False, numpy.ones((nnew,), dtype=numpy.bool)]
		
		if K != None:
			kcols = full_K.shape[1]
			if full_istrain[:kcols].all(): self.input.K = full_K
			else: self.input.K = full_K[:, full_istrain[:kcols]]
		self.input.x = full_x
		self.input.y = None
		self.input.istrain = full_istrain
		self.input.istest = full_istest
		
		if K == None: self.testing(x=self.input.x)
		else: self.testing(K=self.input.K)
		
		self.input.x = full_x
		self.input.K = full_K
		self.input.y = full_y
		self.input.istrain = full_istrain
		self.input.istest = full_istest
		self.update_loss(training=False, testing=got_true)
		return self
			
	def cvtrain(self, x=None, K=None, y=None, istrain=None, istest=None, **kwargs):
		"""
		train() once on the outer fold (dictated by <istrain> and <istest> as
		usual) reporting the test results in self.loss.test as usual. In the
		self.output fields (for example, self.output.f) the elements
		self.output.f[self.input.istest] are as normal.
		
		Reported performance on the training set (both in self.loss.train
		and in self.output.*[self.input.istrain] ) come from a cross-validation
		within the <istrain> set.
		
		Additional **kwargs are passed through to the construction of a
		foldguide() object.
		
		"""###

		if isinstance(self.hyper, experiment):
			if kwargs.get('outerfg', None) != None: raise ValueError("outerfg is only a valid argument when doublecv() is called directly, not via train() or cvtrain()")
			cc = self.doublecv(x=x,K=K,y=y,istrain=istrain,istest=istest, **kwargs)
			if len(cc) != 1: raise RuntimeError("multiple outputs from doublecv--this shouldn't happen")
			chosen = cc[0]
			self.input = chosen.input
			self.model = chosen.model
			self.output = chosen.output
			self.loss = chosen.loss
			self.cv = chosen.cv
			self._reorder_fields('verbosity', -1)
			return self

		outer = self
		outer.verbosity -= 1
		verb = outer.verbosity
		indent = '  '
		if verb == 1: self.write(indent)
		if verb == 1: numstr = indent
		outer.train(x=x, K=K, y=y, istrain=istrain, istest=istest)
		del istrain, istest	

		sep = '  // '
		if verb == 1: self.write(sep)
		if verb == 1: numstr += outer.summarystr()[2] + sep
		outer.verbosity += 1

		x, K, y = outer.input.x, outer.input.K, outer.input.y
		visible = numpy.where(outer.input.istrain)[0]
		for f,v in self.output._allitems():
			v[self.input.istrain] = numpy.nan		
		#inner = []
		fg = foldguide(ids=visible, labels=y[visible], **kwargs)
		#print ' '.join(['%d'%iii for iii in fg[0][1]])
		result = {'mean':0.0, 'ste':0.0, 'n':0}
		for foldnumber,(tr,val) in enumerate(fg):
			each = self.copy()
			#inner.append(each)
			if 'kernel' in each.hyper._fields and K != None: del each.hyper.kernel
			each.verbosity -= 1
			each.train(x=x, K=K, y=y, istrain=tr, istest=val)
			if verb == 1: numstr += each.summarystr()[2]
			each.verbosity += 1
			result = seqste(result, {'mean':each.loss.test, 'ste':each.loss.test_se, 'n':len(val)})
			for f,v in self.output._allitems():
				v[each.input.istest] = each.output._getfield(f)[each.input.istest]
		
		self.loss.train = result['mean']
		self.loss.train_se = result['ste']
		
		if verb == 1: self.write(' (avg=%.3f)\n' % self.loss.train)
		if verb == 1: self.write(numstr + '\n')

		return self

	def featureweight(self, x=None, primalname='weights', dualname='alpha'):
		if x == None:
			return self.model._getfield(primalname)
		trainind = numpy.where(self.input.istrain)[0]
		if len(x) == len(trainind):
			trainind = list(range(len(x)))
		elif len(x) != self.input.K.shape[0]:
			raise ValueError("wrong number of input data points x")
		w = None
		alpha = self.model._getfield(dualname)
		for i,a in zip(trainind, alpha.flat):
			wi = x[i] * a
			if w == None: w = wi
			else: w += wi
		return w
		
	def summarystr(self):		
		ntrain = sum(self.input.istrain)
		ntest = sum(self.input.istest)
		longstr = self.__class__.__name__
		shortstr = ' (?????) '
		if ntrain == 0:
			longstr = '%s (untrained)' % longstr
		else:
			longstr = '%s trained on %d' % (longstr, ntrain)
			if self.loss.train != None:
				longstr = '%s (%s = %.3f)' % (longstr, self.loss.func.__name__, self.loss.train)
				shortstr = '(%.3f)' % self.loss.train
			if ntest:
				longstr = '%s, tested on %d' % (longstr, ntest)
				if self.loss.test != None:
					longstr = '%s (%s = %.3f)' % (longstr, self.loss.func.__name__, self.loss.test)
					shortstr = ' %.3f ' % self.loss.test
		numstr = '%d:%d ' % (ntrain,ntest)
		shortstr = shortstr.rjust(max(len(shortstr), len(numstr)))
		numstr   =   numstr.rjust(max(len(shortstr), len(numstr)))
		return longstr,shortstr,numstr
	
	def resolve_training_inputs(self, x=None, K=None, y=None, istrain=None, istest=None):

		if x == None: x = self.input.x
		if K == None: K = self.input.K
		if y == None: y = self.input.y
		if istrain == None and istest == None:
			istrain = self.input.istrain
			istest = self.input.istest

		if x == None and K == None: raise ValueError("no data and no kernel supplied")		
		#if x != None and K != None: raise ValueError("supply data or kernel, but not both")
		if y == None: raise ValueError("no labels supplied")
		
		
		if K == None:
			ntest = ntrain = len(x)
			ksiz = (ntest,ntrain)
		else:
			K = numpy.asmatrix(K)
			ntest,ntrain = ksiz = K.shape
		
		if not isinstance(y, numpy.ndarray): y = numpy.asarray(y)
		y = y.view()
		while len(y.shape) < 2: y.shape = y.shape + (1,)
		nlabels = len(y)
		
		if nlabels != ntest: raise ValueError("the number of labels must match the number of rows in the kernel")
		
		use_explicit_istest = False
		if istrain == None:
			istrain = numpy.arange(max(ksiz)) < min(ksiz)
			use_explicit_istest = True
		istrain = numpy.asarray(istrain)
		if istrain.size != max(istrain.shape): raise ValueError("istrain must be a vector")
		istrain = istrain.flatten()
		if istrain.dtype != numpy.bool:
			ind = istrain
			istrain = numpy.arange(max(ksiz)) < 0
			istrain[ind] = True
		if istrain.size not in ksiz:
			if K == None: raise ValueError("x and istrain have mismatched number of points")
			else: raise ValueError("K and istrain  have mismatched number of points")
		if istrain.size < nlabels: istrain = numpy.r_[istrain, [False] * (nlabels - istrain.size)]
		
		if istest == None: istest = numpy.logical_not(istrain)
		istest = numpy.asarray(istest)
		if istest.size != max(istest.shape): raise ValueError("istest must be a vector")
		istest = istest.flatten()
		if istest.dtype != numpy.bool:
			ind = istest
			istest = numpy.arange(max(ksiz)) < 0
			istest[ind] = True
		if istest.size not in ksiz: raise ValueError("K and istest  have mismatched number of points")
		if istest.size < nlabels: istest = numpy.r_[istest, [False] * (nlabels - istest.size)]

		if use_explicit_istest: istrain = numpy.logical_and(istrain, numpy.logical_not(istest))
		if (numpy.where(istrain)[0] > min(ksiz)).any(): raise ValueError("training points are missing from K")
		if (numpy.where(istest)[0] > ntest).any(): raise ValueError("testing points are missing from K")
		if numpy.logical_and(istrain, istest).any(): raise ValueError("some points are designated both training and testing")
		
		return x,K,y,istrain,istest

	def calibrate(self, link=logistic, balance=True):
		from .Optimization import psifit
		p = psifit(link=link, balance=balance, x=self.output.f[self.input.istrain], y=self.input.y[self.input.istrain])
		p.fix(logyoked=-numpy.inf)
		p.fix(loglower=-numpy.inf, logupper=-numpy.inf)
		p.fix(shift=0)
		p.free(logscale=0.0)
		p.optimize()
		fac = numpy.exp(p.logscale)
		if 'alpha' in self.model._fields: self.model.alpha *= fac
		if 'weights' in self.model._fields: self.model.weights *= fac
		self.model.bias *= fac
		self.output.f *= fac
		# f' = k f
		# f  = a x + b  = a (x - s)
		# f' = a'x + b' = a'(x - s')
		# where f' = kf, so a' = ka and  b' = kb,  so  s' = s = -b/a,   so the inflection point (x=s,f=0) hasn't moved (x=s, f still=0).
		# you may think this obvious, but somehow it took me a while to wrap my head around it: the "bias" is not the same as the shift.
		if 'p' in self.output._fields: self.output.p = link(self.output.f)
		self.update_loss()
		return self
	
	def rebias(self):
		if self.verbosity:
			c,classes = confuse(self.input.y, self.output.y); err = 1.0 - c.diagonal() / c.sum(axis=1)
			print("before rebias: bias = %g;  train/CV err on %+d = %.3f;   train/CV err on %+d = %.3f,  train/CV %s = %.3f" % (self.model.bias, classes[0], err[0], classes[1], err[1], self.loss.func.__name__, self.loss.train))
		self.output.f -= self.model.bias
		self.model.bias = -eeop(numpy.asarray(self.output.f).flatten(), numpy.asarray(self.input.y).flatten())
		self.output.f += self.model.bias
		self.output.y = numpy.sign(self.output.f)
		if 'p' in self.output._fields: self.output.p = logistic(self.output.f)
		self.update_loss()
		if self.verbosity:
			c,classes = confuse(self.input.y, self.output.y); err = 1.0 - c.diagonal() / c.sum(axis=1)
			print("after rebias:  bias = %g;  train/CV err on %+d = %.3f;   train/CV err on %+d = %.3f,  train/CV %s = %.3f" % (self.model.bias, classes[0], err[0], classes[1], err[1], self.loss.func.__name__, self.loss.train))
		return self
		
		
	def plotf(self, condition=None, field='output.f'):
		"""
		Plot values (by default self.output.f) separately in red for
		cases where self.input.y > 0 and in blue for cases where
		self.input.y < 0.
		
		<condition> may be 'istrain' or 'istest' to further limit which
		data are viewed, or None (for everything)
		"""###
		neg = (self.input.y<0).flat
		pos = (self.input.y>0).flat
		if condition == None:
			neg = numpy.array(neg)
			pos = numpy.array(pos)
		else:
			neg = self.input[condition] * neg
			pos = self.input[condition] * pos
		if field in self.output._fields: f = self.output._getfield(field)
		else: f = self._getfield(field)
		from . import Plotting
		Plotting.plot(f[neg], hold=False, marker='*', color=(0,0,1), drawnow=False)
		Plotting.plot(f[pos], hold=True,  marker='+', color=(1,0,0), grid=True)
	
	def plotd(self, hold=False, drawnow=True):
		"""
		Plot two-dimensional data, along with decision surface of trained classifier.
		"""###
		xtr = self.input.x[self.input.istrain]
		xts = self.input.x[self.input.istest]
		ytr = self.input.y[self.input.istrain]
		yts = self.input.y[self.input.istest]
			
		dmin = numpy.asarray(self.input.x).min(axis=0)
		dmax = numpy.asarray(self.input.x).max(axis=0)
		expand = (dmax - dmin) * 0.05
		dmin -= expand; dmax += expand
		res = 70
		
		c = self.copy()
		tr = c.input.istrain
		c.input.x = c.input.x[tr]
		c.input.y = c.input.y[tr]
		if c.input.K != None: c.input.K = c.input.K[tr,:][:,tr]  # whereas c.input.K[tr,tr] screws up:  a bug in numpy??
		c.input.istest = c.input.istest[tr]
		c.input.istrain = c.input.istrain[tr]
		x = numpy.linspace(dmin[0], dmax[0], res, endpoint=True)
		y = numpy.linspace(dmin[1], dmax[1], res, endpoint=True)
		xx,yy = numpy.meshgrid(x,y)
		c.test(x=numpy.c_[xx.flat,yy.flat])
		z = numpy.asarray(c.output.f[c.input.istest])
		z.shape = xx.shape
		h = {}
		from . import Plotting
		pylab = Plotting.load_pylab()
		ax = pylab.gca()
		if not hold: ax.cla()
		#h['surf'] = pylab.pcolor(x,y,z)
		h['surf'] = Plotting.imagesc(x=x,y=y,img=z, interpolation='bilinear', cmap=pylab.cm.gray)
		h['contours'] = pylab.contour(x,y,z, linestyles=['--'], colors=[(0,1.0,0)], hold='on')
		h['boundary'] = pylab.contour(x,y,z, (0,), colors=[(0,1.0,0)], linewidths=3, hold='on')
		for hh in list(h.values()): hh.set_clim(numpy.array([-1,+1]) * max(abs(numpy.array([z.min(), z.max()]))))
		if len(ytr): h['tr+'] = pylab.plot(xtr[ytr.flat>0][:,0], xtr[ytr.flat>0][:,1], mec=(0.2,0.0,0.0), mfc=(1.0,0.0,0.0), marker='s', linestyle='none', markersize=7)
		if len(ytr): h['tr-'] = pylab.plot(xtr[ytr.flat<0][:,0], xtr[ytr.flat<0][:,1], mec=(0.0,0.0,0.2), mfc=(0.0,0.0,1.0), marker='s', linestyle='none', markersize=7)
		if len(yts): h['ts+'] = pylab.plot(xts[yts.flat>0][:,0], xts[yts.flat>0][:,1], mec=(0.2,0.0,0.0), mfc=(1.0,0.5,0.5), marker='o', linestyle='none', markersize=7)
		if len(yts): h['ts-'] = pylab.plot(xts[yts.flat<0][:,0], xts[yts.flat<0][:,1], mec=(0.0,0.0,0.2), mfc=(0.5,0.5,1.0), marker='o', linestyle='none', markersize=7)
		ax.set(xlim=[dmin[0],dmax[0]], ylim=[dmin[1],dmax[1]])
		if drawnow: pylab.draw()
		return h
		
	def plotw(self, xtrain=None, normaxis=None, norm=2, x=None, y=None, **kwargs):
		"""
		Plot weights of a linear predictor (supply the training data in <xtrain> if
		this was a kernel implementation of a linear predictor).  Weights may be
		formatted in a 1- or 2-dimensional array (depending on how the data exemplars
		were formatted): a stem plot is used for 1-D and an image for 2-D.
		Dimensionality may be reduced by 1 by taking the L-<norm> norm along axis
		<normaxis>. Optional arguments <x> and <y> supply the x- and (for image plots)
		y-axis data for plotting.
		"""###
		w = self.featureweight(x=xtrain)
		if normaxis != None: w = (w ** norm).sum(axis=normaxis) ** 1.0/norm
		kwargs['aspect'] = kwargs.get('aspect', 'auto')
		kwargs['balance'] = kwargs.get('balance', {None:0.0}.get(normaxis, None))
		kwargs['grid'] = kwargs.get('grid', True)
		if len(w.shape) == 2:
			kwargs['colorbartitle'] = kwargs.get('colorbartitle', 'weight')
			kwargs['colorbarformat'] = kwargs.get('colorbarformat', '%+g')
			from . import Plotting; pylab = Plotting.load_pylab()
			out = Plotting.imagesc(w, x=x, y=y, **kwargs)
		elif len(w.shape) == 1:
			if x == None: x = list(range(len(w)))
			from . import Plotting; pylab = Plotting.load_pylab()
			out = Plotting.stem(x, w, **kwargs)
		else:
			raise ValueError('do not know how to plot %d-dimensional weight array' % len(w.shape))			
		return out
		

def plotopt(cc, hold=False, drawnow=True, **kwargs):
	if not isinstance(cc, (list, tuple)): cc = [cc]
	expt = cc[0].hyper
	lab = [expt._shortdesc(cond) for cond in expt]
	
	inner = sum([numpy.asarray(cci.cv.inner.mean) for cci in cc]) / len(cc)
	innerse = sum([numpy.asarray(cci.cv.inner.se) for cci in cc]) / len(cc)	 # plot the average error-bar size across outer folds, rather than just making the bars unrepresentatively small (we don't actually have that many data points!)
	outer = [{
		'mean': numpy.asarray(cci.cv.outer.mean),
		'ste': numpy.asarray(cci.cv.outer.se),
		'n': sum(cci.input.istest),
	} for cci in cc]
	outer = seqste(*outer)
	outer,outerse = outer['mean'],outer['ste']
	
	n = inner.size
	from . import Plotting
	pylab = Plotting.load_pylab()
	if not hold: pylab.cla()
	kwargs['mec'] = kwargs.get('mec', 'auto')
	kwargs['mfc'] = kwargs.get('mfc', 'auto')
	kwargs['markersize'] = kwargs.get('markersize', 10)
	explicitmarker = 'marker' in kwargs
	if not explicitmarker: kwargs['marker'] = 'o'
	hinner = pylab.errorbar(x=1.0+numpy.arange(n), y=inner, yerr=innerse, **kwargs)
	if not explicitmarker: kwargs['marker'] = 's'
	houter = pylab.errorbar(x=1.0+numpy.arange(n), y=outer, yerr=outerse, **kwargs)
	ax = pylab.gca()
	ax.set_xlim((0,n+1))
	ax.set_xticks(1.0 + numpy.arange(n))
	ax.set_xticklabels(lab, rotation=20, horizontalalignment='right')
	ax.set_yticks(numpy.arange(0.0,1.1,0.1))
	ax.set_ylim([0.0, 1.0])
	ax.grid(True)
	if drawnow: pylab.draw()
	
class klr2class(predictor):
	def __init__(self, C=0.1, relcost=(1.0,1.0), lossfunc=class_loss, lossfield='y'):
		predictor.__init__(self, lossfunc=lossfunc, lossfield=lossfield)
		self.hyper.C = C
		self.hyper.relcost = relcost
		self.model.alpha = None
		self.model.bias = None
		self.output.y = None
		self.output.f = None
		self.output.p = None
	
	def defaults(self): return {'C':[10.0, 3.0, 1.0, 0.3, 0.1, 0.03, 0.01, 0.003, 0.001]}
	
	def training(self, K, y):
		if len(K.shape) != 2 or K.shape[0] != K.shape[1]: raise ValueError('K must be a square matrix')
		if y.size != K.shape[0]: raise ValueError('y must contain one label per row of K')
		
		relcost = self.hyper.relcost
		if relcost == None: relcost = 1.0
		if relcost == 'balance': relcost = float(sum(y<0.0)) / float(sum(y>0.0))
		err = 'hyper.relcost must be a scalar, a two-element sequence, or the string "balance"'
		if isinstance(relcost, str): raise ValueError(err)
		relcost = numpy.asarray(relcost).flatten()
		if relcost.size == 1: relcost = relcost ** [-0.5, 0.5]
		if relcost.size != 2: raise ValueError(err)
		if relcost[0]==relcost[1]: relcost = None
		
		varK = K.diagonal().mean() - K.mean()

		C = self.hyper.C
		C = numpy.asarray(C).flatten()
		if C.size != 1: raise ValueError('hyper.C must be a scalar')
		C = C[0] * varK
		
		from . import klr
		ab,f,J,obj = klr.klr_cg(K=K, Y=y, C=C, wght=relcost, verb=-1)
		self.model.alpha = ab[:-1]
		self.model.bias = ab[-1].flat[0]

	def testing(self, K):
		if K.shape[1] != len(self.model.alpha): raise ValueError('wrong number of training points (columns) in test/train kernel')
		self.output.f = numpy.asmatrix(K) * self.model.alpha + self.model.bias
		self.output.y = numpy.sign(self.output.f)
		self.output.y[self.output.y==0] = 1
		self.output.p = 1.0 / (1.0 + numpy.exp(-self.output.f))
		
class lda2class(predictor):
	
	def __init__(self, shrinkage='optimal', lossfunc=class_loss, lossfield='y'):
		predictor.__init__(self, lossfunc=lossfunc, lossfield=lossfield)
		self.hyper.shrinkage = shrinkage
		self.model.weights = None
		self.model.bias = None
		self.output.y = None
		self.output.f = None
		self.output.p = None
		
	def defaults(self): return {'shrinkage':['optimal', 0.0, 0.001, 0.005, 0.05, 0.5, 0.8, 1.0]}

	def training(self, x, y):
		n = {}; m = {}; c = {}; z = {}
		y = numpy.sign(y.flat)
		for i,yi in enumerate(y):
			n[yi] = n.get(yi, 0) + 1
			m[yi] = m.get(yi, 0.0) + x[i]
		for k,v in list(m.items()): v /= n[k]
		for i,yi in enumerate(y):
			xi = x[i] - m[yi]
			zi = numpy.outer(xi.flat, xi.flat)
			c[yi] = c.get(yi, 0.0) + zi

		if self.hyper.shrinkage == 'optimal':
			for k,v in list(c.items()): v /= n[k] # mean of centered per-exemplar outer products (biased ML estimator of cov)
			for i,yi in enumerate(y):
				xi = x[i] - m[yi]
				zi = numpy.outer(xi.flat, xi.flat)
				z[yi] = z.get(yi, 0.0) + ((zi - c[yi]).flatten() ** 2).sum()
			for k,v in list(c.items()): v *= float(n[k]) / float(n[k]-1) # correct to unbiased estimate 
			for k in z: z[k] = z[k] * float(n[k]) / float(n[k] - 1) ** 3.0    # divide by (n-1) for unbiased estimate of z variances, then multiply by n/(n-1)^2
		else:
			for k,v in list(c.items()): v /= n[k]-1 # straight to unbiased estimate 

		dm = (m[+1] - m[-1])
		
		for k,cov in list(c.items()):
			gamma = self.hyper.shrinkage
			nu = cov.diagonal().mean()
			if gamma == 'optimal':
				denom = (cov.flatten()**2).sum() - 2.0 * nu * cov.trace() - cov.shape[0] * nu ** 2.0
				gamma = z[k] / denom
			shrinkcov(cov, gamma=gamma, nu=nu)
		
		cov = (c[+1] + c[-1]) / 2.0
		w = numpy.linalg.solve(cov, dm.flatten())
		w.shape = dm.shape
		f = numpy.zeros((len(x),),dtype=numpy.float64)
		for i,xi in enumerate(x): f[i] = numpy.inner(w.flat,xi.flat)
		self.model.weights = w
		self.model.bias = -eeop(f,y)
		
	def testing(self, x):
		w = self.model.weights.flat
		self.output.f = numpy.asarray([numpy.inner(w,xi.flat) for xi in x]) + self.model.bias
		self.output.y = numpy.sign(self.output.f)
		self.output.p = logistic(self.output.f)
		
class FoldingError(Exception): pass

class foldguide(object):
	
	randomseed = None # global for all foldguide objects

	@classmethod
	def next_randomseed(cls):
		limit = int(2**31-1)
		if foldguide.randomseed == None:
			saved_state = numpy.random.get_state()
			numpy.random.seed() # randomizes from random-number generator and/or clock...
			foldguide.randomseed = numpy.random.randint(limit)  # ...but doesn't return the seed it found: so let's use the first number it gets that way
			numpy.random.set_state(saved_state)
		r = foldguide.randomseed
		foldguide.randomseed = (foldguide.randomseed + 1) % limit
		return r

	def __init__(self, ids=None, labels=None, folds=None, ntrain=None, ntest=None, balance=True, randomseed='auto'):
		"""
		foldguide constructor parameters:
		   ids:        a list of exemplar ids; shortcut: pass integer n if the ids should be just range(n)
		   labels:     a list of labels, or None if balancing folds doesn't matter (or label information is unavailable)
		   folds:      integer number of folds, or 'LOO' for leave-one-out; could also be a list of lists of test-fold ids for explicit() 
		   ntrain:     the size of each training fold, or None for auto
		   ntest:      the size of each test fold, or None for auto
		   balance:    whether to ensure that each fold contains (approximately) the same relative proportions of classes
		   randomseed: an integer (32-bit), or 'auto' 
		"""###
		#sstruct.__init__(self)
		if ids == None: 
			if labels == None: raise ValueError("either ids or labels must be supplied")
			ids = len(labels)

		if randomseed == 'auto': randomseed = foldguide.next_randomseed()

		if isinstance(ids, foldguide): self.__dict__.update(ids.__dict__); return
		if isinstance(ids, int): ids = list(range(ids))
		self.n = len(ids)
		self.ids = list(ids)
		if labels == None: self.labels = None
		elif len(labels) != self.n: raise ValueError("mismatched number of ids and labels")
		else: self.labels = list(labels)

		explicit_testfolds = None
		if isinstance(folds, (tuple, list)):
			explicit_testfolds = folds
			folds = len(folds)
			if ntrain != None or ntest != None: raise ValueError("cannot specify ntrain or ntest when folds are supplied as an explicit list")
			
		if ntrain != None and ntest != None: raise ValueError("specify either ntrain or ntest, or neither, but not both")
		if ntrain != None and ntrain > self.n / 2.0: ntrain,ntest = None, self.n - ntrain
		if ntest  != None and ntest  > self.n / 2.0: ntrain,ntest = self.n - ntest, None
		swap = False
		if ntrain != None:  foldsize = float(ntrain); swap = True
		elif ntest != None: foldsize = float(ntest)
		else:
			if folds == None: folds = min(len(ids),10)
			if isinstance(folds, str) and folds.lower() in ['loo', 'leave one out']: folds = len(ids)
			foldsize = float(self.n) / folds
		if folds == None: folds = int(round( numpy.ceil(self.n / float(foldsize)) ))

		self.balanced = balance
		
		order = list(range(self.n))
		self.randomseed = randomseed
		if self.randomseed != None:
			saved_state = numpy.random.get_state()
			numpy.random.seed(self.randomseed)
			#print "shuffling with", self.randomseed
			numpy.random.shuffle(order)
			numpy.random.set_state(saved_state)
		
		self.classes = []
		categorized = []
		saved_order = list(order)
		i = len(order)
		while len(order):
			wrap = i >= len(order)
			if wrap: i = 0
			if labels == None: thislabel = None
			else: thislabel = labels[order[i]]
			if wrap:
				matchlabel = thislabel
				self.classes.append(matchlabel)
				categorized.append([])
			if isequal(matchlabel, thislabel):
				categorized[-1].append(order.pop(i))
			else:
				i += 1
		if not balance: categorized = [saved_order]
		self.indices = []
		distributed = [None for i in range(folds)]
		
		riffled = [];
		for cl in range(len(categorized)):
			c = categorized[cl]
			n = len(c)
			k = numpy.linspace(0.0, 1.0, n, endpoint=True)
			riffled += list(zip(k, c))
		riffled = numpy.array([ind for x,ind in sorted(riffled)])
		
		foldstart = numpy.linspace(0.0, self.n-foldsize, folds, endpoint=True)
		for ifold in range(folds):
			start,stop = int(round(foldstart[ifold])),  int(round(foldstart[ifold] + foldsize))
			distributed[ifold] = riffled[start:stop]
						
		a = set(range(self.n))	
		self.indices = tuple([(
			tuple(sorted(a - set(d))),
			tuple(sorted(d)),
		) for d in distributed])
		if swap: self.indices = tuple([(b,a) for a,b in self.indices])
		try: self.classes.sort()
		except: pass
		if explicit_testfolds != None:
			self.explicit(test = explicit_testfolds)
		
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		trmean,tsmean = numpy.mean([ (len(tr),len(ts)) for tr,ts in self.indices], axis=0)
		nfolds = len(self.indices)
		describe_as_balanced = self.balanced and len(self.classes) > 1
		describe_as_balanced = {True:' balanced ', False:' '}[describe_as_balanced]
		s += "\n    %d%sfolds each ~ %d:%d" % (nfolds, describe_as_balanced, round(trmean), round(tsmean))
		s += ", randomseed = %s" % str(self.randomseed)
		over = self.check()
		s += "\n    avg overlap between folds = %.3g%% : %.3g%%" % (100.0 * over['tr']['average'], 100.0 * over['ts']['average'])
		
		ijust = len('%d' % (nfolds - 1))
		trlabels,tslabels = [],[]
		for i in range(nfolds):
			tr,ts = self.indices[i]
			trlabels.append('(' + '/'.join(['%d' % len([ind for ind in tr if self.labels == None or isequal(self.labels[ind], c)]) for c in self.classes]) + ')')
			tslabels.append('(' + '/'.join(['%d' % len([ind for ind in ts if self.labels == None or isequal(self.labels[ind], c)]) for c in self.classes]) + ')')
		trljust = max([len(x) for x in trlabels])
		for i in range(nfolds):
			s += "\n    fold %s --- %s:%s" % (  ('%d'%i).rjust(ijust),  trlabels[i].rjust(trljust),  tslabels[i]  )
		return s

	def __getitem__(self, i):
		return self.get(i, 'ids')

	def __len__(self):
		return len(self.indices)

	def get(self, fold, attr='ids'):
		lookup = getattr(self, attr)
		return (
			tuple([lookup[x] for x in self.indices[fold][0]]),
			tuple([lookup[x] for x in self.indices[fold][1]]),
		)
				
	def check(self):
		nfolds = len(self.indices)
		for i in range(nfolds):
			tr,ts = self.indices[i]
			overlap = sorted(set(tr).intersection(ts))
			union = sorted(set(tr).union(ts))
			if len(tr) == 0:        raise FoldingError("fold %d of foldguide 0x%08x is corrupt: training fold is empty" % (i, id(self),))
			if len(ts) == 0:        raise FoldingError("fold %d of foldguide 0x%08x is corrupt: test fold is empty" % (i, id(self),))
			if len(overlap):        raise FoldingError("fold %d of foldguide 0x%08x is corrupt: overlap of %d items between training and test fold" % (i, id(self), len(overlap),))
			if len(union) < self.n: raise FoldingError("fold %d of foldguide 0x%08x is corrupt: %d items are missing from both training and test fold" % (i, id(self), self.n-len(union)))
			if len(union) > self.n: raise FoldingError("fold %d of foldguide 0x%08x is corrupt: %d extra unexpected items" % (i, id(self), len(union)-self.n))		
		tr,ts = list(zip(*self.indices))
		tr = sorted(set(reduce(tuple.__add__, tr)))
		if len(tr) < self.n: raise FoldingError("foldguide 0x%08x is corrupt: %d items never appear in the training folds" % (id(self), self.n - len(tr)))
		if len(tr) > self.n: raise FoldingError("foldguide 0x%08x is corrupt: %d extra unexpected items appear in the training folds" % (id(self), len(tr) - self.n))
		ts = sorted(set(reduce(tuple.__add__, ts)))
		if len(ts) < self.n: raise FoldingError("foldguide 0x%08x is corrupt: %d items never appear in the test folds" % (id(self), self.n - len(ts)))
		if len(ts) > self.n: raise FoldingError("foldguide 0x%08x is corrupt: %d extra unexpected items appear in the test folds" % (id(self), len(ts) - self.n))
		
		overlap_tr = {}
		overlap_ts = {}
		for j in range(nfolds):
			for i in range(j):
				indi = set(self.indices[i][0])
				indj = set(self.indices[j][0])
				overlap_tr[(i,j)] = 2.0 * float(len(indi.intersection(indj))) / (len(indi) + len(indj))
				indi = set(self.indices[i][1])
				indj = set(self.indices[j][1])
				overlap_ts[(i,j)] = 2.0 * float(len(indi.intersection(indj))) / (len(indi) + len(indj))

		overlap_tr['average'] = numpy.mean(list(overlap_tr.values()))
		overlap_ts['average'] = numpy.mean(list(overlap_ts.values()))
		return {'tr':overlap_tr, 'ts':overlap_ts}
	
	def explicit(self, training=None, test=None):
		"""
		Supply, as either <training> or <test> but not both,  a list of lists of ids.
		The ids will be re-folded explicitly in the specified way.
		"""###
		if training == None and test == None: raise ValueError("must supply either training or test")
		if training != None and test != None: raise ValueError("must supply either training or test, but not both")
		if training != None: folded = training
		else: folded = test
		nfolds = len(folded)
		unmatched = reduce(list.__add__, [[str(x) for x in foldids if x not in self.ids] for foldids in folded])
		if len(unmatched): raise ValueError("ids not found: %s" % ','.join(unmatched))
		result = []
		allind = set(range(len(self.ids)))
		for foldids in folded:
			specified = [self.ids.index(x) for x in foldids]
			rest = allind - set(specified)
			if training != None: result.append((tuple(specified), tuple(rest)))
			else: result.append((tuple(rest), tuple(specified)))
				
		oldindices = self.indices
		self.indices = tuple(result)
		try:
			self.check()
		except FoldingError as e:
			self.indices = oldindices
			raise FoldingError(str(e).split(':')[-1])
		self.randomseed = 'explicit'
		self.balanced = False # TODO: maybe could do better at inferring this
		
class experiment(sstruct):
	"""
	An sstruct subclass, hence inheriting (versions of) the
	_setitem, _getitem, _allfields and _allitems methods.
	
	May have fields and subfields. Values at the leaves of the
	tree are forced, when assigned, to be lists (if they are
	not already lists, they become one-item lists). 

	Iteration over the object iterates over combinations of
	the leaf elements (conditions of the experiment), returning
	sstruct objects.
	
	Example:
	
	expt = {'a.x': 'hello', 'a.y':[1,2,3], 'b':['foo', 'bar']}
	expt = experiment(expt)
	for i,condition in enumerate(expt): print 'condition', i; print condition
	
	Additional methods (over and above inherited sstruct methods)
	include _shape(), _reshape() and _shortdesc()
	
	Conditions can be dereferenced with a single serial index expt[i]
	or with multi-dimensional subscripts expt[p,q,r,...]
	expt._order is a special property which dictates the order in
	which conditions come out when dereferenced with the serial index.
	It is the same as numpy's array order: the default, _order='C',
	means that the last-listed subfield varies fastest, whereas
	the alternative _order='F' would mean that the first-listed
	subfield varies fastest.
	
	"""###
	def __init__(self, _baseobj=None, _order='C', **kwargs):
		self._inherit(_baseobj, _recursive=True, **kwargs)
		self._order = _order
		
	def __setattr__(self, f, v):
		if f == '_order' and v not in ('C', 'F'): raise ValueError("_order must be 'C' or 'F'")
		sstruct.__setattr__(self, f, v)
		if f not in self._fields: return		
		if isinstance(v, list): v = list(v)
		elif isinstance(v, sstruct): v = self.__class__(v)
		else: v = [v]
		self.__dict__[f] = v		
	def __len__(self):
		n = 1
		for f,v in self._allitems(): n *= len(v)
		return n
	def _shape(self):
		"""
		Return a tuple containing the number of levels in each subfield.
		"""###
		return [len(v) for f,v in self._allitems()]
	def _reshape(self, x):
		"""
		Use numpy.reshape() to reshape a sequence x (perhaps a list of results
		from each experimental condition?) into an array the same "shape" as the
		experiment as given by _shape(). 
		"""###
		return numpy.reshape(x, self._shape(), order=self._order)
	def _shortdesc(self, x, delim=', '):
		"""
		Describe condition (or other sstruct) <x> in one line, in terms of how
		it differs from the experiment <self>. Constants (i.e. fields of the
		experiment that have exactly one level) are not mentioned.
		"""###
		if not isinstance(x, sstruct): x = sstruct(x)
		def shortstr(v):
			cand1 = str(v)
			if len(cand1) > 10 and hasattr(v, '__name__') and type(v).__name__.endswith(('function', 'method')): return v.__name__
			if isinstance(v, numpy.ndarray) and len(v.shape)>1: cand2 = '[%s %s]' % (v.__class__.__name__, 'x'.join([str(d) for d in v.shape]))
			elif isinstance(v, (tuple,list,numpy.ndarray)): cand2 = '[%s of %s item%s]' % (v.__class__.__name__, len(v), {1:''}.get(len(v),'s'))
			elif isinstance(v, str): cand2 = '[string length %d]' % len(v)
			else: cand2 = '[...]'
			if len(cand1) > 10 and len(cand2) < len(cand1): return cand2
			return cand1
		terms = [('%s=%s' % (k,shortstr(v))) for k,v in x._allitems() if len(self._getfield(k,[0,1]))>1]
		return delim.join(terms)
	def _ind2sub(self, ind): # TODO:  optionally make this C-order instead of F-order
		shape = self._shape()
		if self._order == 'C': shape = shape[::-1]
		s,n = [],1
		for sh in shape: s.append(n); n *= sh
		if not -n <= ind < n: raise IndexError("index out of range")
		for i in range(len(s)-1, -1, -1):
			tmp = ind % s[i]
			s[i] = (ind - tmp) / s[i]
			ind = tmp
		if self._order == 'C': s = s[::-1]
		return s
	def _sub2ind(self, sub):
		shape = self._shape()
		if self._order == 'C':
			sub = list(sub)[::-1]
			shape = shape[::-1]
		if len(sub) != len(shape): raise IndexError("expected %d subscripts, got %d" % (len(shape),len(s)))
		s,n = [],1
		for sh in shape: s.append(n); n *= sh
		ind,sub = 0,list(sub)
		while len(sub): ind += s.pop(0) * sub.pop(0)
		return ind 
	def __getitem__(self, ind):
		if isinstance(ind, tuple):
			e = sstruct()
			s = list(ind)
			slicing = True in [isinstance(x, slice) for x in s]
			if False in [isinstance(x, (int,slice)) for x in s]: raise IndexError("invalid index type")
			if slicing: e = self.__class__(e)
		elif isinstance(ind, int):
			e = sstruct()
			s = self._ind2sub(ind)
		elif isinstance(ind, str):
			return self._getfield(ind)
		else:
			raise IndexError("invalid index type")
		items = self._allitems()
		if len(s) != len(items): raise IndexError("expected %d subscripts, got %d" % (len(items),len(s)))
		for f,v in items: e._setfield(f, v[s.pop(0)])
		return e	
	def __iadd__(self, other):
		cl = self.__class__
		if not isinstance(other, cl): other = cl(other)
		absent = []
		for f,ov in other._allitems():
			sv = self._getfield(f, absent)
			if id(sv) == id(absent):
				sv = list(ov)
				self._setfield(f, sv)
			elif f not in self._allfields():
				raise ValueError("cannot extend subfield '%s'" % f)
			for ovi in ov:
				for svi in sv:
					if isequal(ovi, svi): break
				else:
					sv.append(ovi)
		return self	
	def __add__(self, other):
		a = copy.deepcopy(self)
		a += other
		return a
	
def overlapping(nsamples=None, windowlength=None, nwindows=None, overlap=None):
		
	if nsamples != None:
		nsamples = int(nsamples)
		if nsamples < 1: raise ValueError("illegal number of samples %d" % nsamples)
	if windowlength != None:
		windowlength = int(windowlength)
		if windowlength < 1: raise ValueError("illegal window length %d" % windowlength)
	if nwindows != None:
		nwindows = int(nwindows)
		if nwindows < 1: raise ValueError("illegal number of windows %d" % nwindows)
	if overlap != None:
		overlap = float(overlap)
		if not 0.0 <= overlap < 1.0: raise ValueError("illegal overlap value %g" % overlap)

	original = {'nsamples':nsamples, 'windowlength':windowlength, 'nwindows':nwindows, 'overlap':overlap}
	
	nnones = sum([v==None for k,v in list(original.items())])
	if nnones > 1: raise ValueError("insufficient information")
	
	def ceil(x): return int(x) + int(x > int(x))
	
	if nsamples == None:
		nsamples = int(round(  windowlength + (nwindows - 1.0) * (1.0 - overlap) * windowlength   ))
		
	if windowlength == None:
		windowlength = int(round(  nsamples / (nwindows + overlap - nwindows * overlap)  ))

	if nwindows == None:
		nwindows = int(round(  float(nsamples - windowlength * overlap) / (windowlength * (1.0 - overlap))  ))

	if nwindows == 0: nwindows = 1
	if nwindows == 1: overlap_samples, windowlength = 0,nsamples
	else: overlap_samples = ceil(float(windowlength * nwindows - nsamples) / (nwindows - 1.0))
	
	overlap = overlap_samples / float(windowlength)
	new_nsamples = windowlength * (nwindows + overlap - nwindows * overlap)
	new_nsamples = int(round(new_nsamples)) # rounding should only be necessary due to numerical precision here
	skipfront = ceil((nsamples-new_nsamples)/2.0)
	step = windowlength - overlap_samples
	if step < 1: raise ValueError("nsamples=%d is too small for %d windows of length %d" % (nsamples,nwindows,windowlength))
	maxstart = nsamples - windowlength
	t0 = list(range(skipfront, maxstart+1, step))
	while len(t0) > nwindows: t0.pop()

	final = {'nsamples':nsamples, 'windowlength':windowlength, 'nwindows':nwindows, 'overlap':overlap}
	for k,v in list(original.items()):
		if v == None: continue
		fv = final[k]
		if k == 'overlap':
			v = round(v * windowlength)
			fv = round(fv * windowlength)
			if abs(v-fv) > 1: raise ValueError('%s=%g is not consistent with other inputs (should be %d/%d = %g)' % (k,original[k],fv,windowlength,fv/windowlength))
		elif v != fv:
			raise ValueError("%s=%g is not consistent with other inputs (should be %d)" % (k,v,fv))
	
	return t0,final
	
def spcov(x, y=None, balance=True, spdim=1, return_trchvar=False):
	"""
	From data <x>, compute a spatial covariance matrix, where
	"space" is the dimension of <x> denoted by <spdim> (cannot
	be 0).
	
	If labels <y> are supplied and <balance> is set to True,
	there is the opportunity to balance the computation: then,
	covariance matrices are computed separately on each class,
	and averaged at the end.
	"""###
	n = {}; c = {}
	axes = None
	if spdim < 1: raise ValueError("spdim cannot be <1 (exemplar dim is assumed to be 0, so that x[i] is exemplar i)")
	ntr, nch = x.shape[0],x.shape[spdim]
	if return_trchvar: trchvar = numpy.zeros((ntr,nch), x.dtype)
	spdim -= 1 # since we will be operating on each x[i]
	for i,xi in enumerate(x):
		denom = 1.0
		xim = xi = numpy.asarray(xi)
		if axes == None: axes = [axis for axis in range(len(xi.shape)) if axis != spdim]
		for axis in axes:
			xim = numpy.expand_dims(xim.mean(axis=axis), axis)
			denom *= xi.shape[axis]
		xi = xi - xim
		ci = numpy.tensordot(xi, xi, axes=(axes,axes))
		if y == None or not balance: yi = 0
		else: yi = y[i]
		c[yi] = c.get(yi, 0.0) + ci
		n[yi] = n.get(yi, 0) + denom
		if return_trchvar: trchvar[i,:].flat = ci.diagonal().flat
	if balance:
		for k,v in list(c.items()): v /= float(n[k])
		c = sum(c.values()) / float(len(n))
	else:
		c = sum(c.values()) / float(sum(n.values()))
	c = numpy.asmatrix(c)
	if return_trchvar: return c, trchvar
	else: return c
		
def shrinkcov(cov, gamma, nu='mean', copy=False):
	"""
	Shrink a covariance matrix <cov> towards a sphere.
	<gamma> is the degree of shrinkage (0.0 for no change,
	1.0 for complete shrinkage to a sphere). <nu> is the
	variance of the sphere: if nu='mean', then use the
	mean of the diagonal elements of <cov> on input.
	
	For copy=False, modify <cov> in place and return it.
	For copy=True, operate on and return a copy of <cov>.
	"""###
	if not 0.0 <= gamma <= 1.0: raise ValueError("illegal shrinkage value")
	if not isinstance(cov, numpy.ndarray): cov = numpy.array(cov, dtype=numpy.float64)
	if nu == 'mean': nu = cov.diagonal().mean()
	elif nu == 'diag': nu = cov.diagonal() 
	if isinstance(nu, (list,tuple,numpy.ndarray)): nu = numpy.asarray(nu, dtype=cov.dtype).flatten()
	else: nu = float(nu)
	if copy: cov = cov * (1.0 - gamma)
	elif gamma: cov *= 1.0 - gamma
	if gamma: cov.flat[::cov.shape[1]+1] += gamma * nu
	return cov

def spfilt(x, W, copy=False):
	"""
	Each x[i] is a space-by-time signal array.  W is a spatial filtering
	matrix with one filter per *row* (so each W[i] is a filter).

	For copy=False, modify x in place and return it (only possible if
	W is square). For copy=True, operate on and return a new array.
	"""###
	for i,xi in enumerate(x):
		Wxi = numpy.dot(W, xi)
		if i == 0:
			xout = x
			if copy: xout = numpy.zeros((len(x),)+Wxi.shape, dtype=x.dtype)
			elif xout[0].shape != Wxi.shape: raise ValueError("in-place spatial-filtering only works with square W: otherwise must call with copy=True")
		xout[i].flat = Wxi.flat
	return xout
	
def symwhiten(x, cov=None, gamma=0.0, copy=False, **kwargs):
	"""
	If <cov> is supplied, use that, otherwise estimate a spatial
	covariance matrix by calling spcov() on the data <x>, passing
	through any other keyword args supplied to that.
	
	First, if gamma > 0.0, obtain a shrunken copy of <cov> by
	calling shrinkcov() with that <gamma> setting.
	
	Take the (symmetrical) matrix-square-root of the inverse of
	the resulting (shrunken, or not) covariance matrix, and use
	that to spatially filter the data with spfilt().
	
	Return (x,W) where x is the spatially filtered data and W is
	the matrix of spatial filters (one per row).
	"""###
	if cov == None:
		cov = spcov(x=x, **kwargs)
		cov = shrinkcov(cov, gamma=gamma, copy=False)
	else:
		cov = shrinkcov(cov, gamma=gamma, copy=True)
	W = svd(cov).isqrtm
	x = spfilt(x, W, copy=copy)
	return x,W

def symwhitenkern(x, x2=None, gamma=0.0, cov=None):
	if x2 == None:
		x,W = symwhiten(x, copy=True, cov=cov, gamma=gamma)
	else:
		x2,W = symwhiten(x2, copy=True, cov=cov, gamma=gamma)
		x = spfilt(x, W, copy=True)
	return linkern(x=x, x2=x2)

def stfac(Gp, Ps=None, Pt=None, maxrank=numpy.inf):
	"""
	Gp:  S x T  weights in preconditioned space
	Ps:  S x S  spatial preconditioner (e.g. whitener), default = eye(S)
	Pt:  T x T  temporal preconditioner, default = eye(T)
	"""###

	Gp = numpy.asmatrix(Gp)
	S,T = Gp.shape
	if Ps == None: Ps = numpy.eye(S, dtype=numpy.float64)
	if Pt == None: Pt = numpy.eye(T, dtype=numpy.float64)

	u = sstruct()
	u.Ps = numpy.asmatrix(Ps)
	u.Pt = numpy.asmatrix(Pt)
	u.G = u.Ps * Gp * u.Pt.H
	u.Gp = Gp
	decomp = svd(u.Gp)
	u.Rs, u.Rt = decomp.U,decomp.V
	u.sv = decomp.s
	
	nfac = sum(u.sv/(max(S,T)*max(u.sv)) > 1e-8)
	nfac = min(nfac, maxrank)

	u.Rs = u.Rs[:, :nfac]
	u.Rt = u.Rt[:, :nfac]

	u = stfac_filters_and_patterns(u, D=numpy.diag(u.sv[:nfac]), S=numpy.eye(nfac, dtype=numpy.float64))
	return u

def stfac_filters_and_patterns(u, D=None, S=None, B=None, Ss=None, St=None):

	dt = numpy.float64
	nfac = u.Rs.shape[1]
	I = numpy.eye(nfac, dtype=dt)
	if D  == None: D  = numpy.asmatrix(u._getfield('D',  I.copy()), dtype=dt)
	if S  == None: S  = numpy.asmatrix(u._getfield('S',  I.copy()), dtype=dt)
	if B  == None: B  = numpy.asmatrix(u._getfield('B',  I.copy()), dtype=dt)
	if Ss == None: Ss = numpy.asmatrix(u._getfield('St', I.copy()), dtype=dt)
	if St == None: St = numpy.asmatrix(u._getfield('St', I.copy()), dtype=dt)
	
	if not isequal(D,  numpy.diag(numpy.diag(D ))): raise ValueError('D must be a diagonal matrix')
	if not isequal(S,  numpy.diag(numpy.diag(S ))): raise ValueError('S must be a diagonal matrix')
	if not isequal(Ss, numpy.diag(numpy.diag(Ss))): raise ValueError('Ss must be a diagonal matrix')
	if not isequal(St, numpy.diag(numpy.diag(St))): raise ValueError('St must be a diagonal matrix')

	u.S  = numpy.asmatrix(S,  dtype=dt)
	u.B  = numpy.asmatrix(B,  dtype=dt)
	u.Ss = numpy.asmatrix(Ss, dtype=dt)
	u.St = numpy.asmatrix(St, dtype=dt)

	u_B_I = u.B.I
	u_S_I = u.S.I
	u_Ps_I = u.Ps.I
	u_Pt_I = u.Pt.I

	u.Ws = u.Ps  * u.Rs * u.S * u.B.H * u.Ss
	u.Wt = u.Pt  * u.Rt * u.S * u_B_I * u.St
	
	u.As = u_Ps_I.H * u.Rs * u_S_I * u_B_I * u.Ss.I
	u.At = u_Pt_I.H * u.Rt * u_S_I * u.B.H * u.St.I
	u.D = numpy.asmatrix(D)
	u.d = numpy.diag(u.D.A)
	u.H  = u.As *   u.D    * u.At.H
	u.Q  = u.As *   u.D.I  * u.At.H

	u.G  = u.Ws   * u.D * u.Wt.H    # should already be the case, unless (for example) rank was explicitly reduced since Gp and G were computed
	u.Gp = u_Ps_I * u.G * u_Pt_I.H
	
	return u


def correlate(x, y, axis=0):
	x = numpy.asarray(x, dtype=float).view()
	y = numpy.asarray(y, dtype=float).view()
	x.shape = list(x.shape) + [1] * (len(y.shape) - len(x.shape))
	y.shape = list(y.shape) + [1] * (len(x.shape) - len(y.shape))
	xm = numpy.expand_dims( x.mean(axis=axis), axis )
	ym = numpy.expand_dims( y.mean(axis=axis), axis )
	xs = numpy.expand_dims( x.std(axis=axis), axis )
	ys = numpy.expand_dims( y.std(axis=axis), axis )
	x = x - xm
	y = y - ym
	x = x / xs
	y = y / ys
	return (x * y).mean(axis=axis)
	
def correlation_pvalue(r, n, two_tailed=True ):
	r = numpy.asarray( r, float )
	n = numpy.asarray( n, float )
	dof = n - 2.0
	t = r / ( ( 1.0 - r ** 2.0 ) / dof ) ** 0.5
	import scipy.stats
	p =	scipy.stats.t( dof ).cdf( t )
	positive = ( r >= 0.0 ).astype( float )
	p = positive + ( 1.0 - 2.0 * positive ) * p
	if two_tailed: p *= 2.0
	return p
	
	