# -*- coding: utf-8 -*-
# 
#   $Id: NumTools.py 5510 2016-08-22 17:45:38Z jhill $
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
	'whoami', 
	'isnumpyarray', 
	'disp', 'cat', 'mad', 
	'flip', 'project', 'trimtrailingdims', 'isequal', 'unwrapdiff', 'diffx', 'edges', 'refrac', 'epochs',
	'summarize', 'sdict', 'reportstruct', 'sstruct', 'ssdiff', 'sscommon',
	'loadmat', 'savemat', 'pickle', 'unpickle', 
]

import numpy
import types
import copy

def whoami():
	from sys import _getframe as getframe
	return getframe(1).f_code.co_name

def disp(*pargs,**kwargs):
	"""
	An alternative (more Matlab-like) way to display numpy arrays that have 2
	or fewer dimensions. Use the <fmt> keyword argument to supply the printf-
	style format string to be used on each element.
	
	See also summarize()
	"""###
	fmt=kwargs.pop('fmt','% g')
	indent=kwargs.pop('indent',0)
	inputname=kwargs.pop('inputname','') # TODO: is there an equivalent of matlab's inputname call?
	if len(kwargs): raise TypeError("%s() got an unexpected keyword argument '%s'"%(whoami(),list(kwargs.keys())[0]))
	spc = '  ' * indent
	import re as regular_expression
	for a in pargs:
		if isinstance(a,list) or isinstance(a,tuple):
			for i in range(len(a)): disp(a[i],fmt=fmt,indent=indent,inputname=inputname+"[%d]"%i)
			continue
		if isinstance(a,dict):
			for i in list(a.keys()):  disp(a[i],fmt=fmt,indent=indent,inputname=inputname+"['%s']"%i)
			continue
		if not isnumpyarray(a): print(inputname + " = " + a.__repr__() + "\n"); continue
		if len(inputname): print("%s = " % inputname)
		a = project(a,1)
		comp = numpy.iscomplexobj(a)
		(nrows,ncols)=a.shape
		nchars = numpy.zeros(shape=a.shape,dtype='complex')
		arstr = []
		aistr = []
		colw = 0
		stripsign = regular_expression.compile(r'^(\s*)[\+\- ]')
		for i in range(0,nrows):
			arstr.append([])
			aistr.append([])
			for j in range(0,ncols):
				val = a[i,j].real
				realstr = (fmt%val).rstrip();
				arstr[-1].append(realstr)
				imagstr = ''
				if comp:
					val = a[i,j].imag
					opstr = ' + '
					if val < 0.0: opstr = ' - '
					imagstr = (fmt%val).rstrip() + 'j'
					imagstr = opstr + stripsign.sub(r'\1', imagstr);
				aistr[-1].append(imagstr)
				nchars[i,j]=complex(len(realstr),len(imagstr))
				colw=max(colw,len(realstr),len(imagstr))
		for i in range(0,nrows):
			s = spc
			for j in range(0,ncols):
				s += arstr[i][j] + ' ' * int(colw-nchars[i,j].real)
				if comp: s += aistr[i][j] + ' ' * int(colw-nchars[i,j].imag)
				if j < ncols-1: s += ',  '
			print(s)
		print("")

def isnumpyarray(x):
	return isinstance(x, numpy.ndarray)
	
def cat(*pargs,**kwargs):
	"""
	cat(a1,a2,...., axis=1)
	
	Like numpy.concatenate, but makes a decent effort to project the arrays
	up into the number of dimensions required by axis, and by the highest-
	dimensional input.
	"""###
	axis=kwargs.pop('axis',1)
	if len(kwargs): raise TypeError("%s() got an unexpected keyword argument '%s'"%(whoami(),list(kwargs.keys())[0]))
	if len(pargs) == 1 and isinstance(pargs[0], (tuple,list)): pargs = pargs[0]
	pargs = list(pargs) # makes a copy, at least of the list container
	nd = max([axis] + [len(x.shape)-1 for x in pargs if isinstance(x,numpy.ndarray)])
	for i in range(0,len(pargs)):
		if not isinstance(pargs[i], numpy.ndarray) or nd > 1:
			pargs[i] = numpy.asarray(pargs[i],dtype='float')
		pargs[i] = project(pargs[i],nd)
	return numpy.concatenate(tuple(pargs), axis=axis)
	
def mad(a,b=None,axis=None):
	"""
	Maximum absolute difference between two arrays, or maximum absolute
	value of one array, along a certain axis (if given) or among all
	elements (if not).
	"""###
	if b != None:
		a = numpy.asarray(a)
		b = numpy.asarray(b)
		if a.size == 1: a.shape = (1,)
		else: a = project(a, max(axis, len(b.shape)-1))
		if b.size == 1: b.shape = (1,)
		else: b = project(b, max(axis, len(a.shape)-1))
		if a.size != 1 and b.size != 1 and a.shape != b.shape: raise ValueError('unmatched shapes')
		a = a - b
	a = numpy.abs(a)
	if axis!=None: a = project(a, axis)
	return numpy.max(a,axis=axis)

def flip(x, axis=0):
	if not isnumpyarray(x): x = numpy.array(x)
	subs = [slice(None, None) for s in x.shape]
	subs[axis] = slice(None, None, -1)
	return x[subs]

def project(a, maxdim):
	"""
	Return a view of the numpy array <a> that has at least <maxdim>+1
	dimensions.
	
	See also trimtrailingdims()
	"""###
	if isinstance(a, numpy.matrix) and maxdim > 1:
		a = numpy.asarray(a)
	else:
		a = a.view()
	a.shape += (1,) * (maxdim-len(a.shape)+1)
	return a

def trimtrailingdims(a):
	"""
	Similar to numpy.squeeze() but only removes high dimensions, like
	matlab's squeeze. Returns a view of array <a>.
	
	See also project()
	"""###
	a = numpy.asarray(a).view()
	sh = list(a.shape)
	while len(sh) and sh[-1]==1: sh.pop(-1)
	a.shape = tuple(sh)
	return a

def asmatrix(x, dtype=numpy.float64, copy=False):
	"""
	Very similar to numpy.asmatrix, except that (a) dtype is either
	specified explicitly or assumed to be float64 (where numpy would
	guess)  and (b) lists/tuples/1-D arrays become n-by-1 matrices
	instead of numpy's bizarre default of 1-by-n.
	"""###
	if isinstance(x, numpy.matrix):
		if copy: return x.copy()
		else: return x
	if hasattr(x,'shape'):
		shape = x.shape
	else:
		x = numpy.array(x, dtype=dtype)
		shape = x.shape
	x = numpy.matrix(x, dtype=dtype, copy=copy)
	if len(shape)<2: x = x.T
	return x

def isequal(a, b):
	"""
	Compare the content of two items a and b (which may be, or may contain,
	numpy arrays) to check whether they contain the same information. In doing
	so, obey the following common-sense rules:
	
	- lists, tuples and 1-dimensional numpy arrays containing the same
	  information are the same;
	- a 3x2 array (for example) is the same as a 3x2x1x1x1x1 array with the
	  same content;
	- a NaN is a NaN is a NaN. Plain NaN and Peshwari NaN are considered equal.
	
	"""###
	if isinstance(a,numpy.ndarray) or isinstance(b,numpy.ndarray):
		try: a = numpy.asarray(a).view()
		except: return False
		try: b = numpy.asarray(b).view()
		except: return False
		a.shape = a.shape + (1,) * (len(b.shape) - len(a.shape))
		b.shape = b.shape + (1,) * (len(a.shape) - len(b.shape))
		if a.shape != b.shape: return False
		return numpy.logical_or(a == b, numpy.logical_and(numpy.isnan(a), numpy.isnan(b))).all()
	if isinstance(a, dict): a = sorted(a.items())
	if isinstance(b, dict): b = sorted(b.items())
	if isinstance(a, (tuple,list)) and isinstance(b, (tuple,list)):
		if len(a) != len(b): return False
		for ai,bi in zip(a,b):
			if not isequal(ai,bi): return False
		return True
	floats = (float,numpy.float32,numpy.float64,getattr(numpy,'float96',numpy.float),getattr(numpy,'float128',numpy.float))
	if isinstance(a, floats) and isinstance(b, floats) and numpy.isnan(a) and numpy.isnan(b): return True
	return a == b

def unwrapdiff(x, base=2*numpy.pi, axis=None, startval=None, dtype=None):
	"""
	Assume X is a wrapped version of an underlying value Y we're interested
	in. For example, it's a 16-bit value that wraps around at 65536, or it's
	an angle which wraps back to 0 at 2*pi.
	
	<base> is the value (65536 or 2*pi in the above examples) such that
	X = Y % base.  The default value of <base> is 2*pi.
	
	Let dY be the numeric diff of Y in dimension <axis>, computed from X
	by unwrapping in order to avoid jumps larger than base/2.  Thus if
	base=65536, a jump from 65535 to 1 is considered as a step of +2.
	If  base=360, a jump from 10 to 350 is considered as a step of -20.
	
	Y is then reconstructed based on dY and <startval> (which defaults to
	the actual initial value(s) of Y).
	
	Return value is (dY,Y).
	"""###
	if dtype != None: x = numpy.asarray(x, dtype=dtype)
	if axis == None:
		axis, = numpy.where(numpy.array(x.shape) != 1)
		if len(axis): axis = axis[0]
		else: axis = 0
	x = x % base
	d = numpy.diff(x, axis=axis)
	nj = d < -base/2.0
	d[nj] += base
	pj = d > base/2.0
	d[pj] -= base
	d[numpy.isnan(d)] = 0
	sub = [slice(None) for i in x.shape]
	sub[axis] = [0]
	sv = x[sub]
	if startval != None: sv = sv*0 + startval
	x = numpy.cumsum(numpy.concatenate((sv,d), axis=axis), axis=axis)
	return d,x

def diffx(x, order=1, axis=-1, pBefore=0.5, padVal=0):
	"""
	Padded diff.  Take a numerical diff of order <order> along axis <axis>,
	but return a result that is the same size and shape as the input <x>.
	Do this by padding with zeros (or copies of the value <padVal>, if
	non-zero) at the beginning and end.
	
	The proportion of padding at the beginning is given by <pBefore>,
	which defaults to 0.5. For odd values of <order>, the number of values
	to be padded at the beginning is rounded---so by default, with order=1,
	the result will be padded with a single zero at the beginning and
	nothing at the end. 
	
	This results in the following default behaviour, which is quite natural
	for plotting:
	
	>>> diffx([1,2,4])
	array([0, 1, 2])

	>>> diffx([1,2,4], order=2)
	array([0, 1, 0])
	
	>>> diffx([1,2,4], pBefore=0)
	array([1, 2, 0])

	"""###
	nBefore = int(round(pBefore * order))
	nAfter = order - nBefore
	x = numpy.asarray(x)
	if len(x.shape)==0: x.shape = [1]
	sub = [slice(None,None) for i in x.shape]
	sub[axis] = slice(nBefore, x.shape[axis]-nAfter)
	dx = numpy.zeros_like(x)
	if(padVal): dx.fill(padVal)
	dx[sub] = numpy.diff(x, n=order, axis=axis)
	return dx

def edges( x, axis=-1, direction=+1, exceed=0.0, return_logical=False ):
	"""
	Returns a set of indices marking rising edges of <x> (or falling edges,
	if <direction> is -1) working along the dimension <axis>.
	
	Often you will want to threshold your signal *before* passing it
	in, e.g.  edge(signal > threshold), but it is also possible to
	customize the <exceed> parameter, thereby defining an edge as any
	jump in the specified <direction> whose size is greater than <exceed>.
	
	If <return_logical> is True, then instead of an array of indices,
	a logical array the same size as x is returned, wherein the edges are
	marked True.
	
	You might want to pass the indices to refrac() and/or epochs().
	"""###
	x = numpy.asarray( x, dtype=numpy.float64 )  # v. important to recast, in case pre-existing data type is unsigned
	edges = ( direction * diffx( x, axis=axis ) > exceed )
	if not return_logical: edges = sorted( set( edges.nonzero()[ axis ] ) )
	return edges

def refrac( indices, interval, fs=1.0 ):
	"""
	Given an ascending sequence of <indices> (expressed in samples)
	remove those that occur too close together. In other words,
	remove any index that is less than <interval> away from its
	predecessor, where <interval> is expressed in samples by default,
	but may be expressed in other time units if the number of samples
	per time unit is supplied as <fs>.
	
	You may often wish to call this function on the results of edges()
	before proceeding to epochs(),  although you can also tell epochs()
	to call refrac() automatically for you.	
	"""###
	if isnumpyarray( indices ):
		indices = indices.flatten()
		if indices.dtype == numpy.bool:
			if indices.size != max( indices.shape ): raise ValueError( "if a logical array is supplied, it should only have one non-singleton dimension" )
			indices = indices.nonzero()[ 0 ]
	interval_samples = int( round( float( fs ) * interval ) )
	out, previous = [], None
	for ind in indices:
		if previous == None or ind - previous >= interval_samples:
			out.append( int( ind ) )
			previous = ind
	out = indices.__class__( out )
	return out

def epochs( x, start_indices, length, offset=0, refractory=0, fs=1.0, axis=-1, return_partial=False, return_array=False ):
	"""
	Returns three outputs: epochlist, timebase, indices_used
	 
	epochlist is a list of arrays (or a numpy array if
	<return_array> is set to True).  Each array in the list is a
	slice of <x> along the specified <axis>, starting at the
	corresponding index in the sequence of <start_indices>. You
	probably found the start indices using the edges() function.
	
	Slices continue along axis <axis> for a number of samples specified
	by <length>.  By default <length> is expressed in samples, but it
	is also possible to express it in other time units if you pass the
	number of samples per time-unit as <fs>. In the same units, an
	<offset> and a <refractory> period can be specified. The offset
	dictates when each epoch starts relative to the nominal start index.
	If <refractory> is > 0, a slice will not be recorded if its start
	index occurs closer to the previous start index than this
	(the refrac() function is called internally to achieve this).
	
	Slices that would extend beyond the end of <x> are not returned,
	unless <return_partial> is set to True (in which case you cannot
	also set <return_array> to True).
	"""###
	out = []
	if return_array and return_partial: raise ValueError( 'the return_array and return_partial options cannot both be set to True' )
	length_samples = int( round( float( fs ) * length ) )
	offset_samples = int( round( float( fs ) * offset ) )
	if not isnumpyarray( x ): x = numpy.asarray( x )
	sub = [ slice( None, None ) for i in x.shape ]
	start_indices = refrac( start_indices, interval=refractory, fs=fs ) # even if refractory=0, this also wrangles the type/shape of start_indices as necessary
	indices_used = []
	timebase = ( numpy.arange( float( length_samples ) ) + offset_samples ) / float( fs )
	for ind in start_indices:
		start = ind + offset_samples
		stop = start + length_samples
		sub[ axis ] = slice( max( 0, start ), stop )
		epoch = x[ sub ]
		if epoch.shape[ axis ] == length_samples or return_partial:
			out.append( epoch )
			indices_used.append( ind )
	if return_array: out = numpy.array( out )
	return out, timebase, indices_used
	
def summarize(a):
	"""
	Returns a somewhat easier-to-swallow string representation of
	an object, when the full repr() would otherwise be too long.
	In particular, summarize the interesting atrributes of numpy
	arrays and matrices.
	
	The sdict class uses this automatically.
	
	See also reportstruct() and its associated class sstruct, which
	go one step further and allow pretty-printing of hierarchical
	data structures. 
	"""###
	if isinstance(a, type(None)): return "None"

	if isinstance(a, numpy.matrix): atype = 'numpy.matrix'
	elif isinstance(a, numpy.ndarray): atype = 'numpy.array'
	elif isinstance(a, (types.FunctionType, types.BuiltinFunctionType)):
		atype = 'function ' + a.__module__ + '.' + a.__name__
		return atype
	elif isinstance(a, (types.MethodType, types.BuiltinMethodType)):
		atype = 'method ' + '.'.join((a.__self__.__class__.__module__, a.__self__.__class__.__name__, a.__name__))
		obj = getattr(a, 'im_self', None)
		if obj != None: atype += ' of %s instance 0x%08x' % (obj.__class__.__name__, id(obj))
		return atype
	else: atype = a.__class__.__name__
	
	if hasattr(a, 'shape'):
		shape = 'shape='+str(a.shape).replace(' ', '')
		nels = numpy.prod(a.shape)
	elif hasattr(a, '__len__'):
		shape = 'length='+str(len(a))
		nels = len(a)
	else:
		shape = ''
		nels = 1
	
	if isinstance(a, str) and nels < 20:
		atype += '(%s)'%repr(a)
	elif nels < 20:
		s = ','.join(str(a).split()).replace('[,','[').replace(',]',']').replace(',,',',')
		if len(s) < 20: atype += '(%s)'%s

	if isinstance(a, (float,int,str)): addr = ''
	else: addr = 'id=0x%08x' % id(a)

	
	if isinstance(a, numpy.ndarray): base = a.base
	else: base = None
	if base == None: base = ''
	else: base = 'base=0x%08x' % id(base)
	
	dtype = getattr(getattr(a, 'dtype', ''), 'name', '')
	if dtype != '': dtype = 'dtype=numpy.'+dtype
	
	try: order = a.flags.farray and "order='F'" or "order='C'"
	except: order = ''
	
	s = ', '.join(filter(len, (addr, shape, dtype, order, base)))
	s = ': '.join(filter(len, (atype, s)))
	return s	

class sdict(dict):
	"""
	A dict subclass which uses summarize() to represent its items rather than
	the whole string representation of each one.  Items are also accessible
	in "lazy" fashion, like .this  as well as like ['this'], unless a genuine
	attribute of the same name exists.  For yet more sophistication
	(construction and processing of hierarchical data structures) see sstruct.
	"""###
	def __init__(self, *pargs, **kwargs):
		if len(pargs)==1 and len(kwargs)==0 and isinstance(pargs[0], (tuple,list)):
			z = list(zip(list(range(len(pargs[0]))), pargs[0]))
			dict.__init__(self, z)
		else:
			dict.__init__(self, *pargs, **kwargs)
		
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		k = sorted(self.keys())
		ks = ["'%s'"%str(x) for x in sorted(self.keys())]
		fmt = '% '+str(4+max([0]+list(map(len,ks))))+"s: %s"
		return '\n'.join([s] + [fmt%(str(x),summarize(self[x])) for x in k])
	def copy(self):
		return self.__class__(self)
	def __setattr__(self, key, value):
		if key in self.__dict__: self.__dict__[key] = value
		elif key in self: self[key] = value
		else: raise AttributeError("'%s' object has no attribute '%s'"%(self.__class__.__name__, key))
	def __getattr__(self, key):
		if key in self.__dict__: return self.__dict__[key]
		elif key in self: return self[key]
		else: raise AttributeError("'%s' object has no attribute '%s'"%(self.__class__.__name__, key))
	def _getAttributeNames(self):
		return list(self.keys())

def reportstruct(x, indent=None, maxstrlen=24):
	"""
	Recursive pretty-printing for dictionaries and objects
	See also summarize() and the classes sdict and sstruct.
	"""###
	s = ''
	xcl = x.__class__.__name__
	if indent == None:
		indent = 2
		s = object.__repr__(x)
		s = ' ' * indent + s + '\n'	
	if not isinstance(x, dict) and hasattr(x, '__dict__'): x = x.__dict__
	if not isinstance(x, dict) or False in [isinstance(f, str) for f in x]: return summarize(x)
	if len(x) == 0: return s
	ndashes = 1 + max([len(str(f)) for f in x])
	fields = x.get('_fields', sorted(x.keys()))
	for f in fields:
		v = x[f]
		f = str(f)
		s += ' ' * indent + '+' + '-' * (ndashes - len(f)) + f + ': '
		if isinstance(v, float): s += '%.12g\n' % v
		elif isinstance(v, (list,tuple,float,int,bool,str,type(None))) and len(repr(v)) < maxstrlen and '\n' not in repr(v): s += repr(v) + '\n'
		elif isinstance(v, (types.FunctionType,types.BuiltinFunctionType,types.MethodType,types.BuiltinMethodType)):
			s += summarize(v) + '\n'
		else:
			if hasattr(v, '_fields') or (isinstance(v, dict) and False not in [isinstance(ff,str) for ff in v]):
				vcl = v.__class__.__name__
				if vcl != xcl and vcl != 'sstruct': s+= '(%s)'%(vcl)
				s += '\n' + reportstruct(v, indent=indent+ndashes+1, maxstrlen=maxstrlen)
			else: s += summarize(v) + '\n'
	#s = s.encode('iso-8859-1', 'replace')
	return s

class ghostsstruct(object):
	def __init__(self, parent, fieldname):
		self.__dict__['_parent'] = parent
		self.__dict__['_fieldname'] = fieldname
	def __getattr__(self, f):
		return ghostsstruct(self._parent, self._fieldname + '.' + f)
	def __setattr__(self, f, v):
		self._parent._setfield(self._fieldname + '.' + f, v)
	def __repr__(self):
		p = self._parent
		return "non-existent .%s field of <%s.%s instance at 0x%08X>" % (self._fieldname, p.__class__.__module__,p.__class__.__name__,id(p))
		

class sstruct(object):
	"""
	An object which keeps track of the order of its attributes, and
	which is displayed using reportstruct().
	
	Also supports various hierarchical operations when some of its
	attributes are themselves of this class:
	
	_setfield(field, val)       # field name may be dot-delimited
	_getfield(field[, default]) # field name may be dot-delimited
	_allfields()                # returns all field and subfield names
	_allitems()                 # analogous to dict.items()
	
	Attributes whose names begin with '_' are not considered to be
	"fields", and behave entirely normally.
	"""###
	def __init__(self, *pargs, **kwargs):
		self._fields = []
		self._allowedfields = None
		self._maxstrlen = 24
		for d in pargs + (kwargs,):
			if d == None: continue
			if hasattr(d, '_allitems'): d = d._allitems()
			if hasattr(d, 'items'):
				for f,v in sorted(d.items()): setattr(self, f, v)
			else:
				for f,v in d: setattr(self, f, v)
	def __getattr__(self, f):
		allowed = self.__dict__.get('_allowedfields', None)
		if allowed == None: allowed = True
		else: allowed = (f in allowed)
		if not allowed or f.startswith('_'):
			raise AttributeError("'%s' object has no attribute '%s'" % (self.__class__.__name__, f))
		return ghostsstruct(self, f)
	def __setattr__(self, f, v):
		allowed = self.__dict__.get('_allowedfields', None)
		if allowed == None: allowed = True
		else: allowed = (f in allowed)
		if not allowed and f.split('.')[0] not in self._fields: 
			raise AttributeError("attachment of new field '%s' is not allowed" % f)
		if '.' in f: self._setfield(f, v); return
		if not f.startswith('_') and f not in self._fields: self._fields.append(f)
		self.__dict__[f] = v
	def __delattr__(self, f): return self._rmfield(f)
	def _getAttributeNames(self):
		return tuple(self._fields)
	def __repr__(self):
		return self._report()
	def _report(self):
		maxstrlen = getattr(self, '_maxstrlen', 24)
		return reportstruct(self, maxstrlen=maxstrlen)
	def _reorder_fields(self, fields, position=0):
		"""
		Promote the field specified by <fields> (if it is a string) or
		multiple fields (if it is a sequence of strings) to the top of
		the list of fields, or to the position in the list specified
		by <position>.
		"""###
		if not isinstance(fields, (tuple, list)): fields = (fields,)
		if position < 0: position += len(self._fields)
		for k in fields:
			self._fields.remove(k)
			self._fields.insert(position, k)
			position += 1
		return self
	def _reverse(self):
		"""
		Reverse the order of fields and subfields.
		"""###
		self._fields.reverse()
		for k,v in list(self.__dict__.items()):
			if not k.startswith('_') and hasattr(v, '_reverse'): v._reverse()
	def _inherit(self, _baseobj=None, _recursive=False, **kwargs):
		"""
		A helper method for your sstruct subclasses.
		
		This is a handy way to write your subclass constructor:
		
		class foo(sstruct):
			def __init__(self, _baseobj=None, **kwargs):

				self._inherit(_baseobj, False, **kwargs)
		
		_baseobj, if not None, is assumed to be some kind of
		sstruct subclass itself, and its content is copied to self.
		Extra fields can be set with the kwargs.
		
		With _recursive=False, only the immediate fields of _baseobj
		are copied, without any class conversion. With _recursive=True,
		all subfields are copied, entailing the automatic creation of
		sub-level objects of the same class as self, where appropriate.
		
		"""###
		sstruct.__init__(self)
		if _baseobj != None:
			if not isinstance(_baseobj, sstruct): _baseobj = sstruct(_baseobj)
			if _recursive:
				for f,v in _baseobj._allitems(): self._setfield(f, v)
			else:
				for f in _baseobj._fields: setattr(self, f, getattr(_baseobj,f))
		for f,v in list(kwargs.items()): setattr(self, f, v)
	def copy(self, deep=False):
		"""
		Creates a hierarchical copy of the sstruct, with a deep copy of all
		internal/administrative elements (attributes starting with "_").
		
		Actual data is only deepcopy'd if <deep> is True---otherwise just
		the object pointers are copied to the respective fields and subfields.
		"""###
		c = self.__class__()
		for f,v in self._allitems():
			if deep: v = copy.deepcopy(v)
			c._setfield(f, v)
		[c.__dict__.__setitem__(f,copy.deepcopy(v)) for f,v in list(self.__dict__.items()) if f.startswith('_')]
		return c
	def __eq__(self, other):
		if other == None: return False
		if not isinstance(other, sstruct):
			try: other = sstruct(other)
			except: return False
		sf = self._allfields()
		of = other._allfields()
		if not isequal(sorted(sf), sorted(of)): return False
		for f in sf:
			if not isequal(self._getfield(f), other._getfield(f)): return False
		return True
	def _update(self, other):
		if not isinstance(other, sstruct): other = sstruct(other)
		for k,v in other._allitems(): self._setfield(k,v)
	def __iadd__(self, other): self._update(other); return self
	def __add__(self, other): return self.copy().__iadd__(other)
	def __getitem__(self, i):
		if isinstance(i, int): return self._fields[i]
		else: return self._getfield(i)
	def __setitem__(self, i, val): return self._setfield(i, val)
	def __delitem__(self, i): return self._rmfield(i)
	def __contains__(self, f): return f in self._fields
	def __len__(self): return len(self._fields)
	def _allitems(self):
		"""
		Returns [(key, value)] for all (sub)fields.
		"""###
		return [(f, self._getfield(f)) for f in self._allfields()]
	def _allfields(self):
		"""
		Returns a list of all field and subfield names.
		"""###
		result = []
		for f in self._fields:
			v = self.__dict__[f]
			if isinstance(v, sstruct): result += [f + '.' + vf for vf in v._allfields()]
			else: result.append(f)
		return tuple(result)
	def _setfield(self, field=None, val=None, **kwargs):
		"""
		Set one or more fields/subfields.
		
		Use either of two syntaxes:
		   e._setfield(field, val)   or
		   e._setfield(fieldname1=value1, fieldname2=value2)
		
		Field names may be (dot-delimited) sub-field names.
		"""###
		if field != None:
			if isinstance(field, str): field = field.strip('.').split('.')
			if not isinstance(field, (tuple,list)): raise TypeError('invalid field type')
			if len(field) == 1: setattr(self, field[0], val);  return self
			if field[0] not in self._fields: setattr(self, field[0], sstruct())
			sub = self.__dict__[field[0]]
			if not isinstance(sub, sstruct): raise AttributeError("field '%s' cannot accept subfields" % field[0])
			sub._setfield(field[1:], val)
		for field,val in list(kwargs.items()): self._setfield(field,val)
		return self
	def _getfield(self, field, *pargs):
		"""
		Get one field/subfield value.
		
		   v = e._getfield(fieldname)   or
		   v = e._setfield(fieldname, default_val)
		
		The field name may be (dot-delimited) sub-field names.
		"""###
		if len(pargs) > 1: raise TypeError("_getfield() takes at most 3 arguments (%d given)" % len(pargs)+2)
		if isinstance(field, str): field = field.strip('.').split('.')
		if not isinstance(field, (tuple,list)): raise TypeError('invalid field type')
		if field[0] not in self._fields:
			if len(pargs): return pargs[0]
			raise AttributeError("no such field '%s'" % field[0])
		val = self.__dict__[field[0]]
		if len(field) > 1:
			if not hasattr(val, '_getfield'):
				if len(pargs): return pargs[0]
				raise AttributeError("field '%s' has no subfields" % field[0])
			val = val._getfield('.'.join(field[1:]), *pargs)
		return val
	def _rmfield(self, field):
		"""
		Remove one field/subfield
		"""###
		parts = field.split('.', 1)
		if len(parts) == 1:
			allowed = self.__dict__.get('_allowedfields', None)
			if allowed == None: allowed = True
			else: allowed = (field in allowed)
			if not allowed: raise AttributeError('the "%s" field cannot be removed' % field)
			if field in self._fields: self._fields.remove(field)
			if field in self.__dict__: del self.__dict__[field]
		else:
			self.__dict__[parts[0]]._rmfield(parts[1])
		return self
	def _lock(self, recursive=True):
		self._allowedfields = []
		for field in self._fields:
			self._allowedfields.append(field)
			v = getattr(self, field)
			if isinstance(v, sstruct): v._lock()
		
def ssdiff( obj, *objs ):
	"""
	Find differences between sstruct objects.
	Both syntaxes are equivalent:  ssdiff(s1, s2, s3)
	                         and:  ssdiff( [s1, s2, s3] )
	In either case, s1 and friends are sstruct instances.
	A tuple of sstruct objects is returned: each one contains only the fields
	that distinguished the corresponding input object from the other objects.
	
	See also sscommon()
	"""###
	return_common = False
	if obj is 'inverse':
		return_common = True
		obj, objs = objs[ 0 ], objs[ 1: ]
	if not isinstance( obj, ( tuple, list ) ): obj = [ obj ]
	objs = list( obj ) + list( objs )
	objs = [ sstruct( obj ) for obj in objs ]
	fields = []
	rm = []
	allfields_eachobj = [ obj._allfields() for obj in objs ]
	for allfields_thisobj in allfields_eachobj:
		fields += [ field for field in allfields_thisobj if field not in fields ]
	for field in fields:
		for obj, allfields_thisobj in zip( objs[ 1: ], allfields_eachobj[ 1: ] ):
			if field not in allfields_thisobj or field not in allfields_eachobj[ 0 ] or obj[ field ] != objs[ 0 ][ field ]:
				if return_common: rm.append( field )
				else: break
		else:
			if not return_common: rm.append( field )
				
	def prune( s ):
		rm = []
		for field in s._fields:
			sub = s.__dict__[ field ]
			if isinstance( sub, sstruct ):
				if len( prune( sub )._fields ) == 0:
					rm.append( field )
		for field in rm:
			s._rmfield( field )
		return s

	for field in rm:
		for obj in objs:
			if hasattr( obj, field ): delattr( obj, field )
	for obj in objs:
		prune( obj )
	return tuple( objs )
				

def sscommon( obj, *objs ):
	"""
	Find commonalities between sstruct objects.
	Both syntaxes are equivalent:  sscommon(s1, s2, s3)
	                         and:  sscommon( [s1, s2, s3] )
	In either case, s1 and friends are sstruct instances.
	An sstruct object is returned containing only those fields whose values are identical
	across all input objects.
	
	See also ssdiff()
	"""###
	return ssdiff( 'inverse', obj, *objs )[ 0 ]
	
def loadmat(filename, **kwargs):
	"""
	Wraps scipy.io.loadmat to understand '/' as a universal
	file-separator, and to yield easier-to-inspect output (sdict
	class).
	"""###
	import os,scipy.io
	if isinstance(filename,str): filename = filename.replace('/', os.path.sep)
	if 'struct_as_record' not in kwargs: kwargs['struct_as_record'] = False
	if 'squeeze_me' not in kwargs: kwargs['squeeze_me'] = False
	return sdict(scipy.io.loadmat(filename, **kwargs))

def savemat(filename, d, append=False, oned_as='row'):
	"""
	Wraps scipy.io.savemat to understand '/' as a universal
	file-separator, to save d.__dict__ if d is not itself a dict,
	and to filter out (and warn about) any unsaveable items.
	"""###
	import os,scipy,scipy.io
	if isinstance(filename,str): filename = filename.replace('/', os.path.sep)
	if not filename.lower().endswith('.mat'): filename += '.mat'
	if not isinstance(d, dict): d = d.__dict__
	
	before = set(d.keys())
	selectval = lambda x: isinstance(x, (str,int,float,numpy.ndarray)) or (isinstance(x,(tuple,list)) and not False in [isinstance(y,(float,int)) for y in x])
	selectkey = lambda x: isinstance(x, str) and not x.startswith('_')
	dd = dict([x for x in list(d.items()) if selectkey(x[0]) and selectval(x[1])])
	gone = before.difference(list(dd.keys()))
	for k in gone: print("warning: unable to save item  %s: %s" % (repr(k), summarize(d[k])))
	if len(dd) == 0: raise TypeError('nothing to save')
	if scipy.__version__ >= '0.7.0':
		scipy.io.savemat(filename, dd, appendmat=append, oned_as=oned_as)
	else:
		scipy.io.savemat(filename, dd, appendmat=append)

def pickle(obj, targetfile, protocol=2, append=False):
	import pickle, gzip
	if isinstance(targetfile, str):
		if targetfile.lower().endswith('.gz'): openfn = gzip.open
		else: openfn = open
		targetfile = openfn(targetfile, {False:'wb', True:'ab'}[append])
	if not isinstance(targetfile, (file, gzip.GzipFile)):
		raise TypeError("targetfile must be a filename or a file handle")
	pickle.dump(obj, targetfile, protocol)

def unpickle(targetfile):
	import pickle, gzip
	if isinstance(targetfile, str):
		if targetfile.lower().endswith('.gz'): openfn = gzip.open
		else: openfn = open
		targetfile = openfn(targetfile, 'rb')
	if not isinstance(targetfile, (file, gzip.GzipFile)):
		raise TypeError("targetfile must be a filename or a file handle")
	return pickle.load(targetfile)
