# -*- coding: utf-8 -*-
# 
#   $Id: Coords.py 4065 2012-05-27 05:58:17Z jhill $
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
	'Point', 'Size', 'Box',
	'union', 'union_of_boxes',  # synonymous
	'intersection', 'intersection_of_boxes',  # synonymous
]

import operator

class Point(list):
	def __init__(self, position=(), x=None, y=None, z=None, step=None, box=None, default_val=0.0):
		list.__init__(self, [float(i) for i in position])
		
		self._default = float(default_val)
		self.__step = step
		self.__box = box

		if x != None: self.x = x
		if y != None: self.y = y
		if z != None: self.z = z

	# @apply

	def _step_fget(self): return self.__step
	def _step_fset(self, val):
		if not isinstance(val, self.__class__): val = self.__class__(val)
		self.__step = val
	step= property(_step_fget, _step_fset)
	# @apply

	def _box_fget(self): return self.__box
	def _box_fset(self, val):
		if not isinstance(val, Box): val = Box(val)
		self.__box = val
	box= property(_box_fget, _box_fset)
	# @apply
	

	def _x_fget(self): return self[0]
	def _x_fset(self, val): self[0] = val
	x = property(_x_fget, _x_fset, doc="the x coordinate: the first element of the sequence")
	# @apply

	def _y_fget(self): return self[1]
	def _y_fset(self, val): self[1] = val
	y = property(_y_fget, _y_fset, doc="the y coordinate: the second element of the sequence")
	# @apply
	
	def _z_fget(self): return self[2]
	def _z_fset(self, val): self[2] = val
	z = property(_z_fget, _z_fset, doc="the z coordinate: the third element of the sequence")
	# @apply
	
	def _A_fget(self): import numpy; return numpy.array(self)
	def _A_fset(self, val):
		import numpy
		val = numpy.asarray(val)
		if val.size != max(val.shape):
			raise ValueError("arrays for assignment to a Point may only have non-singular extent in one dimension")
		it = val.flat
		for i in range(len(it)): self[i] = it[i]
		for i in range(len(it), len(self)): self[i] = self._default
	A = property(_A_fget, _A_fset, doc="get or set the Point's content as a numpy array")


	def _M_fget(self): import numpy; return numpy.matrix(self).T
	def _M_fset(self, val):
		import numpy
		val = numpy.asarray(val)
		if val.size != max(val.shape):
			raise ValueError("arrays for assignment to a Point may only have non-singular extent in one dimension")
		it = val.flat
		for i in range(len(it)): self[i] = it[i]
		for i in range(len(it), len(self)): self[i] = self._default
	M = property(_M_fget, _M_fset, doc="get or set the Point's content as a numpy matrix")
	
	def up(self):      self.y += self.__step.y; return self
	def down(self):    self.y -= self.__step.y; return self
	def left(self):    self.x -= self.__step.x; return self
	def right(self):   self.x += self.__step.x; return self
	def towards(self): self.z -= self.__step.z; return self
	def away(self):    self.z += self.__step.z; return self

	def __getitem__(self, pos):
		pos = int(pos)
		if pos < -len(self):
			raise IndexError("sequence index out of range")
		if pos < len(self): return list.__getitem__(self, pos)
		return self._default
		
	def __setitem__(self, pos, val):
		if isinstance(pos,slice):
			for i,v in enumerate(val):
				self[i]=v
			return
				
		pos = int(pos)
		if pos < -len(self):
			raise IndexError("sequence index out of range")
		val = float(val)
		if pos >= len(self): list.__iadd__(self, [self._default] * (pos - len(self) + 1))
		return list.__setitem__(self, pos, val)
	
	def __repr__(self): return '%s(%s)' % (self.__class__.__name__, list.__repr__(self))
	def copy(self, cls=None):
		if cls == None: cls = self.__class__
		return cls(self, step=self.__step, box=self.__box)
	def map(self, f): return self.__class__([f(x) for x in self], step=self.__step, box=self.__box)
	
	def through(self, box=None):
		"""
		Map the Point or Size from the specified <box>'s internal coordinates to its external
		coordinates. If <box> is not supplied, use the optional self.box attribute. Return a new Point.
		"""###
		if box==None: box = self.box
		if box==None: return self.copy()
		p = self.copy()
		src = box.internal.lims
		dst = box.lims
		for dim,x in enumerate(p):
			if dim >= len(src) or dim >= len(dst):
				x = 0.0
			elif isinstance(p, Size):
				x = x / (src[dim][1] - src[dim][0])
				x = x * (dst[dim][1] - dst[dim][0])
			else:
				x = (x - src[dim][0]) / (src[dim][1] - src[dim][0])
				x = dst[dim][0]   + x * (dst[dim][1] - dst[dim][0])
			p[dim] = x
		return p
	
	def set(self, *pargs, **kwargs):
		for k,v in list(pargs)+list(kwargs.items()): setattr(self, k, v)
		return self
			
		
	def __op(self, op, other, swap=False):
		try: len(other)
		except: other = [other] * len(self)
		other = list(other)
		other += [self._default] * (len(self) - len(other))
		list.__iadd__(self, [self._default] * (len(other)-len(self)))
		if swap: a,b = other,self
		else:    a,b = self,other
		for i in range(len(self)): self[i] = op(a[i], b[i])
		return self

	def __iadd__(self, other): return self.__op(operator.add, other)
	def __isub__(self, other): return self.__op(operator.sub, other)
	def __imul__(self, other): return self.__op(operator.mul, other)
	def __idiv__(self, other): return self.__op(operator.div, other)
	def __add__(self, other): return self.copy().__op(operator.add, other)
	def __sub__(self, other): return self.copy().__op(operator.sub, other)
	def __mul__(self, other): return self.copy().__op(operator.mul, other)
	def __div__(self, other): return self.copy().__op(operator.div, other)
	def __radd__(self, other): return self.copy().__op(operator.add, other)
	def __rsub__(self, other): return self.copy().__op(operator.sub, other, swap=True)
	def __rmul__(self, other): return self.copy().__op(operator.mul, other)
	def __rdiv__(self, other): return self.copy().__op(operator.div, other, swap=True)
	def __neg__(self): return self * -1

class Size(Point):
	"""
	A subclass of Point which behaves exactly the same as Point, except when
	it gets mapped from one coordinate frame to another using the .through()
	method:  then, the offset of the coordinate frame is disregarded (the size
	of an object does not change if it is being mapped between coordinate
	frames that have the same scaling but different origins, whereas the
	position of an object does).
	"""
	pass


class Box(object):
	
	_alignment_dic={}
	
	def __init__(self, arg=None, **k):		
		self.__size = Size(default_val=2.0)
		self.__position = Point(default_val=0.0)
		self.__anchor = Point(default_val=0.0)
		self.__anchorstr = 'center'
		self.__sticky = False
		self.__internal = None
		keys = [
			'rect', 'lims',
			'position', 'x', 'y', 'z',
			'size', 'width', 'height', 'depth',
			'anchor', 'anchorstr',
			'left', 'right', 'top', 'bottom', 'near', 'far',
			'internal',
		]
		
		if not Box._alignment_dic: #class dict is empty .. lets initialize it once for all instances
			ccccc = dict([(i, 0.5) for i in range(2)])
			Box._alignment_dic = {
				'center' : [ccccc],
				'middle': [{}],
				'left':   [{0:0}],
				'right':  [{0:1}],
				'bottom': [{1:0}],
				'top':    [{1:1}],
				'lower':  [{1:0}],
				'upper':  [{1:1}],
				'near':   [{2:0}],
				'far':    [{2:1}],
				
#				'topleft': [{1:1},{0:0}],
#				'topright': [{1:1},{0:1}],
#				'bottomleft': [{1:0},{0:0}],
#				'bottomright': [{1:0},{0:1}],
#				'upperleft': [{1:1},{0:0}],
#				'upperright': [{1:1},{0:1}],
#				'lowerleft': [{1:0},{0:0}],
#				'lowerright':[{1:0},{0:1}],
#				'lefttop': [{0:0},{1:1}],
#				'righttop': [{0:1},{1:1}],
#				'leftbottom': [{0:0},{1:0}],
#				'rightbottom': [{0:1},{1:0}],
#				'leftupper': [{0:0},{1:1}],
#				'rightupper': [{0:1},{1:1}],
#				'leftlower': [{0:0},{1:0}],
#				'rightlower':[{0:1},{1:0}]				
				}
			from itertools import permutations
			c1 = list(permutations(Box._alignment_dic.keys(),2))
			c2 = set(list(permutations(Box._alignment_dic.keys(),3)))
			for c in c1:
				nkey=''.join(c)
				coords=[]
				for i,ckey in enumerate(c):
					coords.append(Box._alignment_dic[ckey][0])
				Box._alignment_dic[nkey]=coords

			for c in c2:
				nkey=''.join(c)
				coords=[]
				for i,ckey in enumerate(c):
					coords.append(Box._alignment_dic[ckey][0])
				Box._alignment_dic[nkey]=coords			
				

		
		if isinstance(arg, (tuple,list)):
			if 'rect' not in k: k['rect'] = arg
			arg = None
			
		for key in keys:
			if arg != None:
				if key == 'internal' and hasattr(arg, 'internal_initialized'): gotit = arg.internal_initialized()
				else: gotit = hasattr(arg, key)
				if gotit: setattr(self, key, getattr(arg, key))
			if key in k: setattr(self, key, k.pop(key))
				
		if 'sticky' not in k and not hasattr(arg, 'sticky'):
			self.sticky = True
			
		for key,val in list(k.items()):
			setattr(self, key, val)
			
		self.__equalize_len()
	
	def copy(self, cls=None):
		if cls == None: cls = self.__class__
		b = cls(position=self.position.copy(), size=self.size.copy(), anchor=self.anchor.copy(), sticky=self.sticky)
		if self.internal_initialized(): b.internal = self.internal.copy()
		return b
	
	def through(self, other):
		"""
		Map the Box from the specified <other> box's internal coordinates to its external coordinates.
		Return a new Box.
		"""###
		b = self.copy()
		b.__position = self.__position.through(other)
		b.__size = self.__size.through(other)
		return b
		
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		nd = len(self)
		if nd == 2: s += "\n    " + "rectangle (left=%g,bottom=%g,right=%g,top=%g)" % self.rect
		if nd == 1: s += "\n    " + "range (left=%g,right=%g)" % self.lims[0]
		anchorstr = self.anchorstr
		if anchorstr == None: anchorstr = 'custom'
		s += "\n    " + "size %s, %s anchor point at (%s), %s" % (' x '.join(map(str,self.size)), anchorstr.replace(' ','-'), ', '.join(map(str,self.position)), (self.sticky and "sticky" or "non-sticky"))
		if self.__internal != None:
			ib = self.__internal
			ndi = len(ib)
			if ndi == 2: s += "\n    " + "internal coordinates X [%+g %+g] by Y [%+g %+g]" % (ib.__getlim(0) + ib.__getlim(1))
			else:        s += "\n    " + "internal coordinates " + " by ".join(["[%+g %+g]" % ib.__getlim(i) for i in range(len(ib))])
		return s

	def __len__(self): return max(len(self.__size), len(self.__position), len(self.__anchor))

	def __equalize_len(self):
		n = len(self)
		for i in range(len(self.__size), n):     self.__size[i]     = self.__size[i]
		for i in range(len(self.__position), n): self.__position[i] = self.__position[i]
		for i in range(len(self.__anchor), n):   self.__anchor[i]   = self.__anchor[i]

	def __i2n(self, val, dim=None): # internal to normalized
		if isinstance(val, (tuple,list)): 
			if dim == None: return val.__class__([self.__i2n(val[i], i) for i in range(len(val))])
			else: val = val[dim]
		if self.__internal == None: return (float(val) - -1.0) / 2.0
		else:  lo, hi = self.__internal.__getlim(dim); return (float(val) - lo) / (hi - lo)

	def __n2i(self, val, dim=None): # normalized to internal
		if isinstance(val, (tuple,list)): 
			if dim == None: return val.__class__([self.__n2i(val[i], i) for i in range(len(val))])
			else: val = val[dim]
		if self.__internal == None: return -1.0 + float(val) * 2.0
		else:  lo, hi = self.__internal.__getlim(dim); return lo + float(val) * (hi - lo)

	def __getlim(self, dim):
		size, position, anchor = self.__size[dim], self.__position[dim], self.__get_anchor_coords()[dim]
		anchor = self.__i2n(anchor, dim)
		lo = position - anchor * size
		return lo, lo + size
		
	def __setlim(self, dim, vals):
		anchor = self.__get_anchor_coords()[dim]
		anchor = self.__i2n(anchor, dim)
		lo, hi = vals
		if lo == None or hi == None:
			size, position = self.__size[dim], self.__position[dim]
			oldlo = position - anchor * size
			if self.__sticky:
				if lo == None:  lo = oldlo
				if hi == None:  hi = oldlo + size
			else:
				if lo == None:  lo = hi - size
				if hi == None:  hi = lo + size
			
		self.__size[dim] = size = float(hi) - float(lo)
		self.__position[dim] = float(lo) + size * anchor
			
	def set(self, *pargs, **kwargs):
		for k,v in list(pargs)+list(kwargs.items()): setattr(self, k, v)
		return self
		

	def _lims_fget(self): return tuple([self.__getlim(i) for i in range(len(self))])							
	def _lims_fset(self, val):
		n = len(val)
		for i in range(n): self.__setlim(i, val[i])
		del self.__size[n:]; del self.__position[n:]; del self.__anchor[n:]
	lims = property(_lims_fget, _lims_fset, doc="for each dimension, a tuple of (left/low/near, right/high/far) limits, expressed in external coordinates")
	# @apply


	def _rect_fget(self): l,r = self.__getlim(0); d,u = self.__getlim(1); return l, d, r, u
	def _rect_fset(self, val): l, d, r, u = val; self.__setlim(0, (l,r)); self.__setlim(1, (d,u))
	rect =  property(_rect_fget, _rect_fset, doc="the (left, bottom, right, top) external coordinates of the Box")

	def _left_fget(self): return self.__getlim(0)[0]
	def _left_fset(self, val):   self.__setlim(0, (val, None))
	left = property(_left_fget, _left_fset, doc="the position of the left edge of the Box in external coordinates")
	# @apply

	def _right_fget(self): return self.__getlim(0)[1]
	def _right_fset(self, val):   self.__setlim(0, (None, val))
	right = property(_right_fget, _right_fset, doc="the position of the right edge of the Box in external coordinates")
	# @apply

	def _bottom_fget(self): return self.__getlim(1)[0]
	def _bottom_fset(self, val):   self.__setlim(1, (val, None))
	bottom = property(_bottom_fget, _bottom_fset, doc="the position of the lower edge of the Box in external coordinates")
	# @apply

	def _top_fget(self): return self.__getlim(1)[1]
	def _top_fset(self, val):   self.__setlim(1, (None, val))
	top = property(_top_fget, _top_fset, doc="the position of the upper edge of the Box in external coordinates")
	# @apply

	def _near_fget(self): return self.__getlim(2)[0]
	def _near_fset(self, val):   self.__setlim(2, (val, None))
	near = property(_near_fget, _near_fset, doc="the position of the near edge of the Box in external coordinates")
	# @apply

	def _far_fget(self): return self.__getlim(2)[1]
	def _far_fset(self, val):   self.__setlim(2, (None, val))
	far = property(_far_fget, _far_fset, doc="the position of the far edge of the Box in external coordinates")
	# @apply

	def _size_fget(self):
		a = self.__size; n, nmax = len(a), len(self)
		if n < nmax: list.__iadd__(a, [a._default] * (nmax-n))
		return a
	def _size_fset(self, val):
		if not isinstance(val, Size): val = Size(val)
		self.__size = val
	size = property(_size_fget, _size_fset, doc="the extent of the Box in each dimension, in external coordinates")
	# @apply

	def _width_fget(self): return self.__size.x
	def _width_fset(self, val): self.__size.x = val
	width = property(_width_fget, _width_fset, doc="the extent of the Box in the x dimension, in external coordinates")
	# @apply

	def _height_fget(self): return self.__size.y
	def _height_fset(self, val): self.__size.y = val
	height = property(_height_fget, _height_fset, doc="the extent of the Box in the y dimension, in external coordinates")
	# @apply

	def _depth_fget(self): return self.__size.z
	def _depth_fset(self, val): self.__size.z = val
	depth = property(_depth_fget, _depth_fset, doc="the extent of the Box in the z dimension, in external coordinates")
	# @apply

	def _position_fget(self):
		a = self.__position; n, nmax = len(a), len(self)
		if n < nmax: list.__iadd__(a, [a._default] * (nmax-n))
		return a
	def _position_fset(self, val):
		if not isinstance(val, Point): val = Point(val)
		self.__position = val
	position = property(_position_fget, _position_fset, doc="the external coordinate of the Box's anchor point")
	# @apply

	def _x_fget(self): return self.__position.x
	def _x_fset(self, val): self.__position.x = val
	x = property(_x_fget, _x_fset, doc="the x coordinate of the Box's anchor point in external coordinates")
	# @apply

	def _y_fget(self): return self.__position.y
	def _y_fset(self, val): self.__position.y = val
	y = property(_y_fget, _y_fset, doc="the y coordinate of the Box's anchor point in external coordinates")
	# @apply

	def _z_fget(self): return self.__position.z
	def _z_fset(self, val): self.__position.z = val
	z = property(_z_fget, _z_fset, doc="the z coordinate of the Box's anchor point in external coordinates")
	
	def __get_anchor_coords(self):
		a = self.__anchorstr2internal()
		if a != None:		
			for i,x in enumerate(a): self.__anchor[i] = x
		return self.__anchor
		
	def __anchorstr2internal(self, val=None):
		if val == None: val = self.__anchorstr
		if val == None: return None
		out = Point(self.__anchor)
#		ccccc = dict([(i, 0.5) for i in range(len(self))])
#		words = (
#			('center', ccccc),
#			('middle', {}),
#			('left',   {0:0}),
#			('right',  {0:1}),
#			('bottom', {1:0}),
#			('top',    {1:1}),
#			('lower',  {1:0}),
#			('upper',  {1:1}),
#			('near',   {2:0}),
#			('far',    {2:1}),
#		)
		
		s = val.lower()
		s =val.replace('-','')
		s =val.replace(' ','')
		w=Box._alignment_dic[s]
		for v in w:
			for dim,prop in list(v.items()):
				out[dim] = self.__n2i(prop, dim)
				
		return out
#		for k,v in words:
#			oldlen = len(s)
#			s = s.replace(k, '')
#			if len(s) < oldlen or k == 'center':
#				for dim,prop in list(v.items()):
#					out[dim] = self.__n2i(prop, dim)
#		s = s.replace(' ', '').replace('-', '')
#		if len(s): raise ValueError("could not interpret anchor specification '%s'" % val)
		return out
	
	# @apply

	def _anchor_fget(self):
		a = self.__anchor; n, nmax = len(a), len(self)
		if n < nmax: list.__iadd__(a, [a._default] * (nmax-n))
		return a
	def _anchor_fset(self, val):
		oldorigin = self.__position - self.__size * self.__i2n(self.__get_anchor_coords())
		if isinstance(val, str):
			self.__anchor = self.__anchorstr2internal(val)
			self.__anchorstr = val
		else:
			if not isinstance(val, Point): val = Point(val)
			self.__anchor = val
			self.__anchorstr = None
		if self.__sticky:
			neworigin = self.__position - self.__size * self.__i2n(self.__get_anchor_coords())
			self.__position += oldorigin - neworigin
	anchor = property(_anchor_fget, _anchor_fset)

	# @apply

	def _anchorstr_fget(self): return self.__anchorstr
	def _anchorstr_fset(self, val): self.anchor = val
	anchorstr = property(_anchorstr_fget, _anchorstr_fset, doc="the box's anchor expressed as a string (or None if there is no particular name for the anchor point)")


	# @apply

	def _sticky_fget(self): return self.__sticky
	def _sticky_fset(self, val): self.__sticky = bool(val)
	sticky = property(_sticky_fget, _sticky_fset, doc=""""
	The sticky property may be True or False.  When the internal coordinates of
	a sticky Box's anchor point are changed, the bounding-box stays where it is
	in external space, and its nominal position is updated. A non-sticky box, on
	the other hand, moves in external space so that the numerical values of its
	its anchor point's external coordinates do not change.

	  >>> b = Box(position=(0,0), anchor='center', sticky=True)
	  >>> b.left
	  -1.0
	  >>> b.anchor = 'bottom left'
	  >>> b.position
	  Point([-1.0, -1.0])   # nominally, the box's .position has now changed
	  >>> b.left            # to denote where the new anchor is in external space,
	  -1.0                  # but .left hasn't changed: the box has not moved

	  >>> b = Box(position=(0,0), anchor='center', sticky=False)
	  >>> b.left
	  -1.0
	  >>> b.anchor = 'bottom left'
	  >>> b.position
	  Point([0.0, 0.0])    # nominally, the box's .position is unchanged
	  >>> b.left           # and in order to place the box's new .anchor there
	  0.0                  # the box has moved in external space

	Similarly, if a box is sticky, moving one edge resizes it (the other edges
	are stuck at their previous positions in external space). If the box is
	non-sticky, moving one edge moves the whole box.

	""")

	# @apply


	def _internal_fget(self):
		if self.__internal == None:
			ib = self.__internal = Box()
			ib.sticky = True
			ib.anchor = [0.0] * len(self)
			ib.lims = [(-1.0,1.0)] * len(self) # do not change these: these absolute defaults are also assumed elsewhere
		return self.__internal
	def _internal_fset(self, val): self.__internal = val
	internal = property(_internal_fget, _internal_fset, doc="""
	Any Box may optionally use another Box to specify its "internal" coordinates.
	Box objects are initially created without an explicit internal Box, but an
	internal Box is created for them automatically the first time the .internal
	property is explicitly accessed. By default, a Box's internal dimensions run
	from -1.0 to +1.0 in all dimensions. So, for a two-dimensional Box, the point
	(-1,-1) in internal coordinates maps to Box's bottom left, and (+1,+1) maps
	to its top right. One can easily move away from the default convention:  for
	example, with newly-created Box object A,

		A.internal.size /= 2

	would set the internal ranges of box A to (-0.5,+0.5) instead. Subsequently,

		A.internal.position += 0.5

	would then set the internal ranges of box A to (0.0, 1.0).

	The motivation for .internal boxes is to allow easy coordinate transformations.
	An object X (a Point, Size or Box) that is defined in the internal coordinates
	of box A can easily be mapped to the external (ordinary) coordinates of A using
	either
		X.through(A)
	or
		A.int2ext(X)

	This allows easy mappings from relative to absolute coordinates in the context
	of windows-within-screens, panes-within-windows, etc.

	""")

	def int2ext(self, obj, attr=None):
		"""
		Map the coordinates of <obj> from this Box's internal to its external
		coordinate frame, the same as obj.through(self)
		"""###
		if not isinstance(obj, (Point,Box)) and attr != None and attr.lower() in ['position', 'point']: obj = Point(obj)
		if not isinstance(obj, (Size, Box)) and attr != None and attr.lower() in ['size']: obj = Size(obj)
		
		if not isinstance(obj, (Point,Size,Box)): obj = Point(obj)
		return obj.through(self)
	
	map = int2ext # transitional duck-typing form old AppTools.Boxes.box
		
	def internal_initialized(self):
		return self.__internal != None
	
	def union(self, other):
		"""
		Return a new Box object whose coordinates encompass those of <self> and <other>.
		"""###
		newself = self.copy(cls=Box)
		for dim in range(max(len(newself), len(other))):
			lself = newself.__getlim(dim)
			lother = other.__getlim(dim)
			newself.__setlim(dim, [min(lself[0], lother[0]), max(lself[1], lother[1])])
		return newself
		
	def intersects(self, other):
		return True not in [x <= 0 for x in self.intersect(other).size]
			
	def intersect(self, other):
		"""
		Return a new Box object whose coordinates indicate the intersection of <self> and <other>.
		If <self> and <other> do not intersect, the result will have a 0 or negative extent in
		one or more dimensions.  Use self.intersects(other) to test for this condition.
		"""###
		newself = self.copy(cls=Box)
		for dim in range(max(len(newself), len(other))):
			lself = self.__getlim(dim)
			lother = other.__getlim(dim)
			newself.__setlim(dim, [max(lself[0], lother[0]), min(lself[1], lother[1])])
		return newself
	
	def scale(self, xyz=None, x=None, y=None, z=None):
		# TODO: doc
		if xyz == None:  xyz = [x,y,z]
		if not isinstance(xyz, (list,tuple)): xyz = [xyz] * len(self)
		xyz = list(xyz)
		print(xyz)
		for i,val in enumerate(xyz):
			if val == None: xyz[i] = 1.0
		xyz += [1.0] * (len(self) - len(xyz))
		xyz = xyz[:len(self)]
		self.size *= xyz
	
def union(b, *pargs):
	for other in pargs:
		b = b.union(other)
	return b
union_of_boxes = union

def intersection(b, *pargs):
	for other in pargs:
		b = b.intersect(other)
	return b
intersection_of_boxes = intersection

def argh():
	# sanity-destroying sanity check
	newanchor = 'left'	
	print("sticky")
	b = Box(position=(2,), anchor='center', sticky=True);
	print('before: left = %s, x = %s' % (str(b.left), str(b.x)))
	b.anchor=newanchor
	print(' after: left = %s, x = %s' % (str(b.left), str(b.x)))
	print('target: left = %s, x = %s' % (str(1.0), str(1.0)))
	# sticky: left should still be 1.0, nominal position should be updated to 1.0
	print()
	print("non-sticky")
	b = Box(position=(2,), anchor='center', sticky=False);
	print('before: left = %s, x = %s' % (str(b.left), str(b.x)))
	b.anchor=newanchor
	print(' after: left = %s, x = %s' % (str(b.left), str(b.x)))
	print('target: left = %s, x = %s' % (str(2.0), str(2.0)))
	#non-sticky: left should have been moved to 2.0, nominal position is held at 2.0
