# -*- coding: utf-8 -*-
# 
#   $Id: Parameters.py 4734 2014-07-15 01:57:34Z jhill $
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
	'read_parameter_lines', 'make_param',
	'Param', 'ParamList',
	'make_bciprm', 'read_bcidate',
]

import numpy, os
import re
import copy
import time

def escape(s):
	if isinstance(s, Param): return s.report(verbosity=-1)
	if isinstance(s, bool): s = int(s)
	if s == None: s = ''
	elif not isinstance(s, str): s = str(s)
	if len(s) == 0: return '%'
	out = ''
	for c in s:
		v = ord(c)
		if not 32 < v < 127 or c in '%{}[]': out += '%%%02x' % v
		else: out += str(c)
	return out

def unescape(s):
	# unfortunately there are two slight difference between the BCI2000 standard and urllib.unquote
	if s in ['%', '%0', '%00']: return ''  # here's one (empty string)
	out = ''
	s = list(s)
	while len(s):
		c = s.pop(0)
		if c == '%':
			c = ''.join(s[:2])
			if c.startswith('%'):  # here's the other ('%%' maps to '%')
				out += '%'
				s = s[1:]
			else:
				try: c = int(c,16)
				except: pass
				else:
					out += chr(c)
					s = s[2:]
		else:
			out += c
	return out

def unspace(x):
	x = x.strip()
	x = x.replace('\t', ' ')
	while True:
		y = x.replace('  ', ' ')
		if len(y) == len(x): break
		else: x = y
	return x

opener,closer = '{ ',' }'

def read_bracketed(x, brackets=('{ ',' }')):
	x = unspace(x)
	opener,closer = brackets
	if not x.startswith(opener):
		return '', '', x
	counter = 0
	for pos,c in enumerate(x):
		if x[pos:pos+len(opener)] == opener: counter += 1
		if x[pos:pos+len(closer)] == closer: counter -= 1
		if counter == 0: break
	cutoff = pos+len(closer)
	element = x[:cutoff]
	x = x[cutoff:].lstrip()
	stripped = element[len(opener):].lstrip()
	if stripped.endswith(closer): stripped = stripped[:-len(closer)].rstrip()
	return element,stripped,x

def read_element(x):
	head,deeper,tail = read_bracketed(x, ('{ ', ' }'))
	if not len(head): head,sep,tail = tail.partition(' ')
	return head,deeper,tail

def split_elements(x):
	elements = []
	while len(x):
		xi,deeper,x = read_element(x)
		if len(deeper): elements.append(split_elements(deeper))
		elif len(xi): elements.append(unescape(xi))
	return elements

def chomp(x, key, d=None, insist=True, lower=False, name=None):
	val = ''
	if len(x):
		val = x.pop(0)
		if d != None: d[key] = val
	elif insist:
		if name in (None,''): name=''
		else: name = ' ' + name
		raise ValueError('parameter%s definition has no %s element' % (name,key))
	if lower: val = val.lower()
	return val

knownUnits = [
	('Hz', 1, 'Hz'),   ('kHz', 1000, 'Hz'), ('MHz', 1000000, 'Hz'),
	('mus',   1e-3, 'ms'), ('us',   1e-3, 'ms'), ('ms',   1, 'ms'), ('s',   1000, 'ms'), ('min',   60000, 'ms'),
	('musec', 1e-3, 'ms'), ('usec', 1e-3, 'ms'), ('msec', 1, 'ms'), ('sec', 1000, 'ms'),
	('muV', 1, 'muV'), ('uV', 1, 'muV'), ('mV', 1000, 'muV'), ( 'V', 1000000, 'muV'),
]
knownUnits.sort(cmp=lambda x,y:-cmp(len(x[0]),len(y[0])))

def wrangle_units(val):
	valstr = str(val).lower().strip()
	if valstr.endswith('sec'): valstr = valstr[:-2]
	for units,scaling,base_units in knownUnits:
		if valstr.lower().endswith(units.lower()):
			try: float(valstr[:-len(units)])
			except: pass
			else: valstr = valstr[:-len(units)]; break
	else:
		units = ''
		base_units = ''
		scaling = 1
	return valstr, units, base_units, scaling
	
def convert(val, type, name='', check_nesting=False, mode='Value', opts=()):
	if check_nesting and isinstance(val, list):
		p = make_param(val, parent=name)
		p.verbosity = -1
		return p
	if name in (None,''): namestr = ''
	else: namestr = ' "%s"' % name
	type = type.lower()
	if type.endswith('list'):
		val = [convert(x, type[:-len('list')], name, check_nesting=True, mode=mode) for x in val]
	elif type.endswith('matrix'):
		val = numpy.array([[convert(x, type[:-len('matrix')], name, check_nesting=True, mode=mode) for x in row] for row in val])
	else: # basic Param type: int, float or string.  So we must wrangle PhysicalUnits
		valstr, units, base_units, scaling = wrangle_units(val)
		numval = numpy.nan
		floatval = intval = None
		
		try: numval = floatval = float(valstr)
		except: pass
		try: numval = intval = int(valstr)
		except:
			if floatval != None and float(int(floatval)) == floatval: intval = int(floatval)
			
		if type == 'float':
			if floatval != None: numval = floatval
			elif mode=='NumericValue' and 'q' not in opts: print(('WARNING: invalid float value "%s" in parameter%s' % (str(val),namestr)))
		if type == 'int':
			if intval != None: numval = intval
			elif mode=='NumericValue' and 'q' not in opts: print(('WARNING: invalid int value "%s" in parameter%s' % (str(val),namestr)))		
		if type == 'string':
			val = str(val)
		
		if   mode == 'Value':        pass
		elif mode == 'NumericValue': val = numval
		elif mode == 'Units':        val = units
		elif mode == 'BaseUnits':    val = base_units
		elif mode == 'ScaledValue':  val = numval * scaling
		elif mode == 'StringValue':  val = str(val)
			
	return val


enummatch = re.compile('^\s*(?P<val>.*?)[\s:]+(?P<key>[0-9]+)[\s,]+')   # match backwards from the end of a comment

def make_param(x, parent=None, opts=()):
	"""Construct a Param object from a single BCI2000 parameter definition string.
	"""###
	d = {}
	nExpected = 1
	if isinstance(x, Param): x = x.report(verbosity=2)
	if isinstance(x, str):
		x,sep,comment = x.partition(' //')
		x = split_elements(x)
		comment = comment.strip()
	else:
		x = list(x)
		comment = ''
	if parent==None:
		location = chomp(x, 'location').split(':')
		chomp(location, 'Section', d)
		chomp(location, 'Subsection', d, insist=False)
		if len(location): d['Filter'] = ':'.join(location)
	type = chomp(x, 'Type', d, lower=True)
	if parent == None:
		name = chomp(x, 'Name', d)
		if not name.endswith('='): raise ValueError('parameter name %s must end with =' % name)
		d['Name'] = name = name.rstrip('=')
	else:
		name = parent + ' sub-parameter'
	
	if comment.endswith('(enumeration)'):
		c = comment[:-len('(enumeration)')].rstrip()
		c = c[::-1] + ' '
		d['Enumeration'] = edict = {}
		while len(c):
			m = enummatch.match(c)
			if not m: break
			c = c[len(m.group()):]
			edict[int(m.groupdict()['key'][::-1])] = m.groupdict()['val'][::-1]
		comment = c[::-1].strip()
		
	d['Comment'] = comment
	
	islist = type.endswith('list')
	ismatrix = type.endswith('matrix')

	if islist:
		length = chomp(x, 'length', name=name)
		try: length = nExpected = int(length)
		except: raise ValueError('invalid length "%s" in parameter %s' % (length, name))
	elif ismatrix:
		rows = chomp(x, 'rows', name=name)
		cols = chomp(x, 'cols', name=name)
		if isinstance(rows, list): d['RowLabels'],rows = rows,len(rows)
		if isinstance(cols, list): d['ColumnLabels'],cols = cols,len(cols)
		try: rows = int(rows)
		except: raise ValueError('invalid number of rows "%s" in parameter %s' % (rows, name))
		try: cols = int(cols)
		except: raise ValueError('invalid number of columns "%s" in parameter %s' % (cols, name))
		nExpected = rows * cols
		if rows == 0 and cols > 0 and 'ColumnLabels' not in d: d['ColumnLabels'] = cols
		
	if len(x) < nExpected:
		raise ValueError('expected %d elements in parameter %s - found only %d' % (nExpected,name,len(x)))
	
	if islist:
		val,x = x[:nExpected],x[nExpected:]
	elif ismatrix:
		val,x = x[:nExpected],x[nExpected:]
		val = [[val.pop(0) for j in range(cols)] for i in range(rows)]
	else:
		val = x.pop(0)
	d['Value'] = convert(val, type, name, mode='Value', opts=opts)
	d['NumericValue'] = convert(val, type, name, mode='NumericValue', opts=opts)
	d['Units'] = convert(val, type, name, mode='Units', opts=opts)
	d['BaseUnits'] = convert(val, type, name, mode='BaseUnits', opts=opts)
	d['ScaledValue'] = convert(val, type, name, mode='ScaledValue', opts=opts)
	
	x = x[-3:]
	d['DefaultValue'] = chomp(x, 'DefaultValue', insist=False, name=name)
	d['LowRange'] = chomp(x, 'LowRange', insist=False, name=name)
	d['HighRange'] = chomp(x, 'HighRange', insist=False, name=name)
	p = Param(d)
	return p
	
def write_to(self, file, append=False):
	"""Write str(self) to the text file whose name or handle is given in <file>.
	If the file already exists, overwrite it from the beginning unless append=True."""###
	if isinstance(file, str):
		mode = {True:'a', False:'w'}[append]
		file = open(file, mode + 't')
	if not append: file.seek(0)
	file.write(str(self)+'\n')
def append_to(self, file):
	"""Append str(self) to the text file whose name or handle is given in <file>.
	Create the file if it doesn't already exist."""###
	return self.write_to(file, append=True)		

class Param(object):
	"""
	A representation of a single BCI2000 parameter. Can be created from a definition string
	using make_param(), or created directly from a value, e.g.  Param(5, Name='ModelOrder')
	When print(), .report()ed, .write_to(file)ed or otherwise converted to str(),  the object
	will try to make the best sense it can out of whatever information it has, and create a
	BCI2000 definition string.  Note that the Param object passively stores whatever
	.NumericValue, .Units, .BaseUnits and .ScaledValue are given to it, and does not attempt
	to generate or update these when the .Value changes.  The .Value determines the string
	representation.
	
	See also ParamList()
	"""###
	def __init__(self, Value, NumericValue=None, Units=None, BaseUnits=None, ScaledValue=None,
	                   Section='Nowhere', Subsection=None, Filter=None, Type='auto', Name='Unnamed',
	                   RowLabels=None, ColumnLabels=None,
	                   DefaultValue=None, LowRange=None, HighRange=None, Comment='', Enumeration=None):
		"""
		Store any of the named arguments as attributes of the Param object.  Value is the only
		mandatory argument.
		"""###
		self.Value = Value
		self.NumericValue = NumericValue
		self.Units = Units
		self.BaseUnits = BaseUnits
		self.ScaledValue = ScaledValue
		self.Section = Section
		self.Subsection = Subsection
		self.Filter = Filter
		self.Type = Type
		self.Name = Name
		self.RowLabels = RowLabels
		self.ColumnLabels = ColumnLabels
		self.DefaultValue = DefaultValue
		self.LowRange = LowRange
		self.HighRange = HighRange
		self.Comment = Comment
		self.Enumeration = Enumeration
		self.verbosity = None
		if isinstance(Value, dict):
			x = Value.pop('Value')
			self.__init__(x, **Value)
	
	def copy(self):
		return copy.deepcopy(self)
	
	def dbreport(self):
		print('%s (%s) :' % (self.Name, self.Type))
		for f in ['Value', 'NumericValue', 'Units', 'ScaledValue', 'BaseUnits']:
			v = getattr(self, f)
			s = repr(v)
			head = (' ' * (20-len(f))) + f + ' = '
			tail = ('\n' + ' ' * len(head)).join(s.split('\n'))
			if '\n' in tail: tail += '\n'
			print(head + tail)
		print()
	
	def Elements(self):
		return self.Rows() * self.Columns()
		
	def Rows(self):
		type = self.Type.lower()
		if type == 'auto': type = self.determine_type()
		if type.endswith('matrix'): return numpy.asarray(self.Value).shape[0]
		else: return 1
	
	def Columns(self):
		type = self.Type.lower()
		if type == 'auto': type = self.determine_type()
		if type.endswith('list'): return len(self.Value)
		elif type.endswith('matrix'):
			if self.Rows() == 0:
				if isinstance(self.ColumnLabels, int): return self.ColumnLabels
				elif self.ColumnLabels != None: return len(self.ColumnLabels)
				else: return 0
			else:
				shape = numpy.asarray(self.Value).shape
				if len(shape) < 2: return 1
				else: return shape[1]
		else: return 1
	
	def merge(self, newer):
		"""
		Return a copy of the Param, with the Value imported from the <newer> Param.
		Keep as much of the older meta-info as is compatible with the new Value.
		"""###
		out = self.copy()
		out.Value = newer.Value
		out.NumericValue = None
		out.ScaledValue = None
		out.Units = None
		out.BaseUnits = None
		if out.RowLabels != None:
			if isinstance(out.RowLabels, int): oldRows = out.RowLabels
			else: oldRows = len(out.RowLabels)
			if oldRows != newer.Rows(): out.RowLabels = None
		if out.ColumnLabels != None:
			if isinstance(out.ColumnLabels, int): oldColumns = out.ColumnLabels
			else: oldColumns = len(out.ColumnLabels)
			if oldColumns != newer.Columns(): out.ColumnLabels = None
			
		if out.RowLabels == None or newer.RowLabels != None: out.RowLabels = newer.RowLabels
		if out.ColumnLabels == None or newer.ColumnLabels != None: out.ColumnLabels = newer.ColumnLabels
		return make_param(out)

	def determine_type(self, type=None):
		if type == None: type = self.Type
		if type not in (None, 'auto'): return type.lower()
		
		x = self.Value
		if isinstance(x, numpy.ndarray):
			if len(x.shape) == 0: x = x.flat[0]
			elif len(x.shape) in (1,2): x = x.tolist()
			else: raise ValueError("don't know how to deal with >2-D arrays")
		if isinstance(x, bool): return 'bool'
		if isinstance(x, int): return 'int'
		if isinstance(x, float): return 'float'
		if isinstance(x, str): return 'string'
		if isinstance(x, (tuple,list)):
			if False not in [isinstance(xi, int) for xi in x]: return 'intlist'
			if False not in [isinstance(xi, (int,float)) for xi in x]: return 'floatlist'
			if False not in [isinstance(xi, (int,float,str)) for xi in x]: return 'list'
			if False not in [isinstance(xi, (tuple,list)) for xi in x]: return 'matrix'
		raise ValueError("don't know how to deal with this data type")
	
	def format(self, field, type=None, verbosity=1, width=0, widths_out=None):
				
		def getval(self, type):
			val = self.Value
			if type.endswith('list'):
				if numpy.asarray(val).ndim != 1: val = numpy.asarray(val).flatten()
			elif type.endswith('matrix'):
				val = numpy.asarray(val)
				while len(val.shape) < 2: val = numpy.expand_dims(val, -1)
			return val

		def justify(txt, mode, width, field, widths_out, trailingspace):
			if mode == 'already done': return txt
			if mode == 'numeric':
				# TODO: make more sophisticated
				txt = (' '  * (width-len(txt)-trailingspace)) + txt
			if len(txt): txt += ' ' * trailingspace
			if widths_out != None:
				widths_out[field] = widths_out.get(field, [])
				widths_out[field].append(len(txt))
			txt += ' ' * (width-len(txt))
			return txt
		
		just = 'left'
		trailingspace = 1
	
		if field == 'Location':
			if verbosity < 0:
				txt = ''
			else:
				section    = {None:''}.get(self.Section, self.Section)
				subsection = {None:''}.get(self.Subsection, self.Subsection)
				filter     = {None:''}.get(self.Filter, self.Filter)
				txt = escape(section)
				extra = ''
				if len(subsection) or len(filter): extra += ':' + escape(subsection)
				if len(filter): extra += ':' + escape(filter)
				#if verbosity == 0 and len(extra) > 10: extra = ':...'
				if verbosity == 0: extra = ''
				txt += extra
				txt = txt.replace('::', ':%:')
				
		elif field == 'Type':
			txt = self.determine_type(type)
			if txt in ('bool', 'enum'): txt = 'int'
			
		elif field == 'Name':
			txt = self.Name
			if verbosity < 0: txt = ''
			elif len(txt) == 0: txt = 'Unnamed'
			if len(txt) and not txt.endswith('='): txt += '='
		
		elif field == 'RowLabels':
			type = self.determine_type(type)
			if type.endswith('list'):
				val = getval(self, type)
				txt = str(len(val))
				just = 'numeric'
				trailingspace = 4
			elif type.endswith('matrix'):
				nrows = self.Rows()
				if self.RowLabels == None or isinstance(self.RowLabels, int): txt = str(nrows); just = 'numeric'
				elif len(self.RowLabels) != nrows:    raise ValueError("wrong number of row labels (got %d, expected %d)" % (len(self.RowLabels), nrows))
				else:
					txt = ' '.join([escape(lab) for lab in self.RowLabels])
					if verbosity == 0 and len(txt) > 10: txt = '...'
					txt = '{ ' + txt + ' }'
			else:
				txt = ''
				
		elif field == 'ColumnLabels':
			type = self.determine_type(type)
			if type.endswith('matrix'):
				ncols = self.Columns()
				if self.ColumnLabels == None or isinstance(self.ColumnLabels, int): txt = str(ncols); just = 'numeric'
				elif len(self.ColumnLabels) != ncols: raise ValueError("wrong number of column labels (got %d, expected %d)" % (len(self.ColumnLabels), ncols))
				else:
					txt = ' '.join([escape(lab) for lab in self.ColumnLabels])
					if verbosity == 0 and len(txt) > 10: txt = '...'
					txt = '{ ' + txt + ' }'
				trailingspace = 4
			else:
				txt = ''
		
		elif field == 'Value':
			type = self.determine_type(type)
			val = getval(self, type)
			just = 'numeric' # TODO: type-sensitive??
			if type.endswith('list'):
				txt = ' '.join([justify(escape(element), just, width, field, widths_out, trailingspace=1) for element in val])
				just = 'already done'
			elif type.endswith('matrix'):
				txt = '    '.join([' '.join([justify(escape(element), just, width, field, widths_out, trailingspace=1) for element in row]) for row in val])
				just = 'already done'
			else:
				txt = escape(self.Value)
			if verbosity == 0 and len(txt) > 10: txt = '...'
		
		elif field == 'DefaultValue':
			if verbosity < 2: txt = ''
			else: txt = '    ' + escape(self.DefaultValue); just = 'numeric'
		
		elif field == 'LowRange':
			if verbosity < 2: txt = ''
			else: txt = escape(self.LowRange); just = 'numeric'
		
		elif field == 'HighRange':
			if verbosity < 2: txt = ''
			else: txt = escape(self.HighRange); just = 'numeric'
		
		elif field == 'Comment':
			mark_empty_comments = 0
			if verbosity == 0: txt = ''
			else: txt = self.Comment.strip()
			type = self.determine_type(type)
			if type == 'bool' and not txt.endswith('(boolean)'): txt += ' (boolean)'
			elif type in ('int', 'intlist', 'enum') and isinstance(self.Enumeration, dict) and len(self.Enumeration) and not txt.endswith('(enumeration)'):
				if verbosity > 0: txt += ' ' + (', '.join([str(k)+': '+str(v) for k,v in sorted(self.Enumeration.items())]))
				if verbosity >= 0: txt += ' (enumeration)'
			if verbosity >= 0 and (mark_empty_comments or len(txt)): txt = '// ' + txt
		
		else:
			raise ValueError("unknown field '%s'" % field)
			
		return justify(txt, just, width, field, widths_out, trailingspace=trailingspace)

	def report(self, verbosity=1, widths_out=None, widths_in=None, reprtxt=False):
		"""
		Return a string defining the parameter in BCI2000 format.  Use verbosity >= 2
		to include the DefaultValue, LowRange and HighRange fields.
		"""###
		type = self.determine_type()
		txt = ''; width = 0
		fields = 'Location Type Name RowLabels ColumnLabels Value DefaultValue LowRange HighRange Comment'
		for field in fields.split():
			if widths_in != None: width = widths_in.get(field, 0)
			txt += self.format(field=field, type=type, verbosity=verbosity, width=width, widths_out=widths_out)
		if verbosity < 0: txt = '{ ' + txt.strip() + ' }'
		if reprtxt: txt = '<%s object at 0x%08X>: %s' % (self.__class__.__name__, id(self), txt)
		return txt
	
	def write_to(self, file, append=False, **kwargs): return write_to(self.report(**kwargs), file=file, append=append)
	def append_to(self, file, **kwargs): return write_to(self.report(**kwargs), file=file, append=True)
			
	def __getslice__(self, s, e):
		return self.__getitem__(slice(s,e,None))

	def __getitem__(self, sub):
		def conv(self, i, x): # helper function for converting subscripts
			if isinstance(x, (tuple, list)):
				if i == None: return x.__class__([conv(self,i,xi) for i,xi in enumerate(x)])
				else: return x.__class__([conv(self, i, xi) for xi in x])
			if i == None: i = 0
			if isinstance(x, slice): return slice(conv(self,i,x.start), conv(self,i,x.stop), conv(self,i,x.step))
			if i == 0: lab = self.RowLabels; labname = 'row'
			elif i == 1: lab = self.ColumnLabels; labname = 'column'
			else: raise TypeError("too many subscripts")
			if isinstance(lab, int): lab = None
			if isinstance(x, int):
				if x < 0:  x += numpy.asarray(self.Value).shape[i]
				return x
			if not isinstance(x, str): return x
			if lab == None or x not in lab: raise ValueError("%s label '%s' not found" % (labname, x))
			return lab.index(x)
		sub = conv(self, None, sub)
		if not hasattr(sub, '__len__') or len(sub) == 1: result = numpy.asarray(self.Value).__getitem__(sub)
		elif len(sub) == 2: result = numpy.asarray(numpy.asmatrix(self.Value).__getitem__(sub))
		else: result = numpy.asarray(self.Value).__getitem__(sub)
		if isinstance(result, numpy.ndarray):
			if len(result.shape) < 1: result = result.tolist()
			elif len(result.shape) == 1 and result.dtype.kind not in 'fib': result = result.tolist()
		return result

	def __repr__(self):
		return self.report(verbosity=0, reprtxt=True)
		
	def __str__(self):
		return self.report(verbosity={None:1}.get(self.verbosity, self.verbosity))

	def __cmp_name__(self, other):
		if not isinstance(other, Param): return 1
		return cmp(self.Name, other.Name)
	def __cmp_location__(self, other):
		if not isinstance(other, Param): return 1
		def s(x): return {None:''}.get(x,x)
		def f(x): return '%s:%s: %s' % (s(x.Section), s(x.Subsection), s(x.Name))
		return cmp(f(self), f(other))
	__cmp__ = __cmp_name__
			
def read_parameter_lines(f):
	if isinstance(f, str): f = open(f, 'rt')
	start = f.tell()
	line = f.readline()
	if len(line) < 1000 and 'HeaderLen=' in line:
		hl = line.split(); hd = {}
		while len(hl) >= 2: k = hl.pop(0).strip().rstrip('='); v = hl.pop(0).strip(); hd[k] = v
		lines = []
		headerlen = int(hd['HeaderLen'])
		stop = start + headerlen
		while f.tell() < stop and len(line) and not line.startswith('[ Parameter Definition ]'): line = f.readline()
		while f.tell() < stop and len(line): lines.append(f.readline().strip())
	else:
		f.seek(start, 0)
		lines = [x.strip() for x in f.readlines() if len(x.strip())]
	return lines


class ParamList(list):
	def __init__(self, x=(), opts=()):
		"""
		Uses make_param() to convert a file, or a string, or a list of strings, into a list of
		Param objects. Manage the result like a regular list, .collate() it, .report() it,
		print(it) or .write_to(file) as desired.
		"""###
		if isinstance(x, str) and len(x) < 1024 and not '\n' in x and os.path.isfile(x): x = open(x, 'rt')
		if isinstance(x, file): x = read_parameter_lines(x)
		if isinstance(x, str): x = x.replace('\r\n', '\n').replace('\r', '\n').split('\n')
		if not isinstance(x, (tuple,list)): x = [x]
		for xi in x:
			if xi == None: continue
			if isinstance(xi, str) and len(xi.strip()) == 0: continue
			if not isinstance(xi, Param): xi = make_param(str(xi), opts=opts)
			self.append(xi)

	def dbreport(self): [p.dbreport() for p in self if hasattr(p, 'dbreport')]
	
	def write_to(self, file, append=False, **kwargs): return write_to(self.report(**kwargs), file=file, append=append)
	def append_to(self, file, **kwargs): return write_to(self.report(**kwargs), file=file, append=True)

	def collate(self, merge_meta=False):
		"""
		Prune the list in place:  later elements are moved so that they overwrite earlier
		elements with the same .Name; elements without a .Name attribute are removed.
		"""###
		i,d = 0,{}
		while i < len(self):
			name = getattr(self[i], 'Name', None)
			if name == None: self.pop(i)
			elif name in d:
				if merge_meta: self[d[name]] = self[d[name]].merge(self.pop(i))
				else: self[d[name]] = self.pop(i)
			else: d[name] = i; i += 1

	def report(self, verbosity=1, delim='\n', widths_out=None, widths_in=None, maxalign=None, pretty=False, reprtxt=False):
		"""Return a string representation of the parameter definintions. Use verbosity >= 2
		to include the DefaultValue, LowRange and HighRange fields.
		"""###
		if pretty:
			if widths_in == None:
				if widths_out == None: widths_out = {}
				self.report(verbosity=verbosity, delim=delim, widths_out=widths_out, widths_in=None, maxalign=maxalign, pretty=False, reprtxt=reprtxt)
				widths_in = widths_out
				
		if widths_in != None:
			ma = {'Location':100, 'Type':100, 'Name':100, 'RowLabels': 7, 'ColumnLabels': 7, 'Value':7, 'DefaultValue':7, 'LowRange':7, 'HighRange':7, 'Comment':0}
			if maxalign != None: ma.update(maxalign)
			widths_in = dict(widths_in)
			for k,v in list(widths_in.items()):
				if not isinstance(v, (tuple,list)): v = [v]
				v = list(v)
				if len(v) == 0: v.append(0)
				widths_in[k] = min(ma[k], max(v))
		
		txt = [xi.report(verbosity=verbosity, widths_out=widths_out, widths_in=widths_in, reprtxt=reprtxt) for xi in self]
		if reprtxt: txt = '<%s object at 0x%08X>: [%s\n]\n' % (self.__class__.__name__, id(self), '\n  '.join(['']+txt))
		else: txt = delim.join(txt)
		return txt
		
	def electrodes(self):
		if 'ChannelNames' not in self: raise KeyError('no ChannelNames parameter')
		try: from SigTools.Electrodes import ChannelSet
		except ImportError: from BCPy2000.SigTools.Electrodes import ChannelSet
		c = ChannelSet(self.ChannelNames.Value)
		if 'ReferenceChannelName' in self: # despite the singular name, should be a list
			ref = self.ReferenceChannelName.Value
			if isinstance(ref, str): ref = ref.replace('+', ' ').replace(',', ' ').split()
			ref = ChannelSet(ref)
			ref = numpy.sum(ref) / float(len(ref))
			c = c - ref
		if 'GroundChannelName' in self: # despite the singular name, should be a list
			gnd = self.GroundChannelName.Value
			if isinstance(gnd, str): gnd = gnd.replace('+', ' ').replace(',', ' ').split()
			gnd = tuple(gnd)
			for ci in c.flat: ci.gnd = gnd
		return c
		
	def __str__(self):
		return '\n'.join([str(xi) for xi in self])
	def __repr__(self):
		return self.report(verbosity=0, reprtxt=True, pretty=True)
	
	def __names(self): return [p.Name for p in self if getattr(p, 'Name', None) != None] # a handy helper for __getitem__, __getattr__ and __contains__
	def _getAttributeNames(self): return self.__names()  # for IPython
	def __contains__(self, x): return x in self.__names()
	def __getitem__(self, sub): # de-reference Param elements dict-like, by their .Name
		if isinstance(sub, str):
			names = self.__names()
			if sub in names: sub = len(self) - 1 - names[::-1].index(sub)
			else: raise KeyError(sub)
		result = list.__getitem__(self, sub)
		if isinstance(result, list) and not isinstance(result, self.__class__):
			result = self.__class__(result)
		return result
	def __getattr__(self, name): # de-reference Param elements lazily as virtual attributes of the list
		try: return self[name]
		except KeyError: raise AttributeError("'%s' object has no attribute '%s'" % (self.__class__.__name__, name))

	# boilerplate stuff required just to make a list subclass keep its identity following common operations
	def __getslice__(self, *pargs, **kwargs):
		result = list.__getslice__(self, *pargs, **kwargs)
		if isinstance(result, list) and not isinstance(result, self.__class__): result = self.__class__(result)
		return result
	def __iadd__(self, other):
		if not isinstance(other, self.__class__): other = self.__class__(other)
		list.__iadd__(self, other); return self
	def __imul__(self, fac): list.__imul__(self, fac); return self
	def __add__(self, other): result = self.__class__(self); result += other; return result
	def __mul__(self, other): result = self.__class__(self); result *= other; return result
	def __rmul__(self, other): return self.__mul__(other)
	
def make_bciprm(*pargs, **kwargs):
	"""
	Merge parameters from multiple sources.
	
	    make_bciprm('Example.dat', 'Example.prm', 'WindowLength', '20s', SpatialFilterType=3)
	
	Arguments may be .dat filenames, .prm filenames, handles to open files, dicts,  Param objects,
	ParamList objects, pairs of   ... parametername, parametervalue, ... arguments,  and tuples or
	lists of any of the above. Parameters are read from left to right, with later parameters
	overwriting previous ones of the same name (Note, as always, that considered individually each
	dict input, including the **kwargs, delivers its fields in an arbitrary order).
	
	Arguments may also be interspersed with output directives, to write or append the state of the
	collated parameters so far to a file---e.g.:
	
	    make_bciprm('Example.dat', 'Example.prm', '>output.prm', SpatialFilterType=3)
	    make_bciprm('Example.dat', 'Example.prm', '>>output.prm', SpatialFilterType=3)
	    
	"""###
	out = ParamList()
	pl = list(pargs)
	pl.append(kwargs)
	def isfilename(x): return isinstance(x, str) and 0 < len(x) < 1024 and '\n' not in x and os.path.isfile(x)
	def islegalkey(x): return isinstance(x, str) and 0 < len(x) < 256 and False not in [c in 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789' for c in x] and x[0] not in '0123456789'
	key = None
	opts = []
	while len(pl):
		p = pl.pop(0)
		if key in ['>', '>>']:
			if not isinstance(p, str) or not len(p.strip()):
				raise ValueError("a filename is expected after '>' or '>>'")
			p = key + p
			key = None		
		if isinstance(p, str) and p.startswith('>>') and len(p.lstrip(' >')):
			a = ParamList(out, opts=opts); a.collate(merge_meta=True); a.append_to(p.lstrip(' >'))
		elif isinstance(p, str) and p.startswith('>') and len(p.lstrip(' >')):
			a = ParamList(out, opts=opts); a.collate(merge_meta=True); a.write_to(p.lstrip(' >'))
		elif isinstance(p, str) and p.startswith('-'): opts.append(p.lstrip('-'))
		elif (islegalkey(p) or p in ['>', '>>']) and key == None: key = p; continue
		elif isfilename(p) and key == None: out += ParamList(p, opts=opts)
		elif isinstance(p, dict):
			if key != None: raise ValueError('did not expect dict object to follow parameter name "%s"' % str(key) )
			for k,v in sorted(p.items()): pl = [k,v] + pl
		elif isinstance(p, Param):
			if key != None: raise ValueError('did not expect Param object to follow parameter name "%s"' % str(key) )
			out.append(p)
		elif isinstance(p, (tuple,list)) and key == None:
			pl = list(p) + pl
		elif isinstance(p, str) and key == None:
			out += ParamList(p, opts=opts)
		elif key == None:
			raise ValueError('%s argument received without a preceding parameter name' % p.__class__.__name__)
		else:
			out.append(make_param(Param(p, Name=key), opts=opts))
			key = None
	if key != None:
		raise ValueError('extraneous key "%s" was passed without accompanying value' % key)
	out.collate(merge_meta=True)
	return out

def read_bcidate(x, fmt=None):
	"""
	Decode the timestamp from a BCI2000 StorageTime parameter and return it as seconds since the
	epoch, or as a strftime-formatted string if <fmt> is specified (hint: a good shorthand is
	fmt='ISO'). x may be a .dat or .prm filename, a handle to an open file, a ParamList,
	a Param, a parameter definition string or the string value itself. Or it may be a list of
	such things, in which case a list of datestamps is returned.
	"""###
	def tryparse(x):
		if not isinstance(x, str): return None
		try: return time.strptime(' '.join(x.split()[1:]), '%b %d %H:%M:%S %Y')
		except: pass
		try: return time.strptime(x.lower().replace('t', ' '), '%Y-%m-%d %H:%M:%S')
		except: pass

	d = tryparse(x)
	if d == None:
		if isinstance(x, str): x = ParamList(x)
		if isinstance(x, ParamList): x = getattr(x, 'StorageTime', None)
		if isinstance(x, Param): x = x.Value
		if isinstance(x, (tuple,list)):
			return [read_bcidate(xi, fmt=fmt) for xi in x]
		if isinstance(x, (float, time.struct_time)): d = x
		else: d = tryparse(x)
	if fmt != None:
		if fmt.lower() == 'iso': fmt = '%Y-%m-%d %H:%M:%S'
		if d == None: d = 'invalid date'
		else: d = time.strftime(fmt, d)
	elif d != None:
		if not isinstance(d, float): d = time.mktime(d)		
	return d
	
def test(filename, zarg=False, width=2000):
	"""
	"zarg" means ignore lines with " % % %" in them on the assumption that the original may have
	omitted a set of empty DefaultValue, LowRange, HighRange
	"""###
	ParamList(filename).write_to('reported.prm', verbosity=2)
	write_to('\n'.join(read_parameter_lines(filename)), 'original.prm')
	if zarg: zarg = " | grep -v ' % % %'"
	else: zarg = ''
	os.system("diff -wyW%d original.prm reported.prm | grep '|'%s | less -S" % (width, zarg))

