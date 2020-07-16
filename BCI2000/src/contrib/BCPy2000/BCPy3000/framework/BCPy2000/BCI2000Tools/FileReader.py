# -*- coding: utf-8 -*-
# 
#   $Id: FileReader.py 5752 2018-05-09 16:38:35Z jhill $
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
__all__ = ['ListDatFiles', 'bcistream', 'ParseState', 'ParseParam', 'ReadPrmFile', 'FormatPrmList', 'unescape']

import os
import sys
import struct
import time
try: import numpy
except: pass

class DatFileError(Exception): pass

def Warn( msg ):
	sys.stderr.write( 'WARNING: %s\n' % msg )
	try: sys.stderr.flush()
	except: pass

def nsorted(x, width=20):
	if len(x) == 0: return []
	import re; return list(zip(*sorted(zip([re.sub('[0-9]+', lambda m: m.group().rjust(width,'0'), xi) for xi in x], x))))[1]

def ListDatFiles(d='.'):
	return nsorted([os.path.realpath(os.path.join(d,f)) for f in os.listdir(d) if f.lower().endswith('.dat')]);

class bcistream(object):
	
	def __init__(self, filename, ind=-1):
		import numpy
		
		if os.path.isdir(filename):
			filename = ListDatFiles(filename)[ind]
			sys.stderr.write("file at index %d is %s\n" % (ind, filename))

		self.filename        = filename
		self.headerlen       = 0
		self.stateveclen     = 0
		self.nchan           = 0
		self.bytesperchannel = 0
		self.bytesperframe   = 0
		self.framefmt        = ''
		self.unpacksig       = ''
		self.unpackstates    = ''
		self.paramdefs       = {}
		self.statedefs       = {}
		self.samplingfreq_hz = 0
		self.gains           = None
		self.offsets         = None
		self.params          = {}
		
		self.file = open(self.filename, 'r')
		self.readHeader()
		self.file.close()
				
		self.bytesperframe = self.nchan * self.bytesperchannel + self.stateveclen

		self.gains = self.params.get('SourceChGain')
		if self.gains is not None:
			self.gains = numpy.array([DecodeUnits(x)[2] for x in self.gains[:self.nchan]], dtype=numpy.float32)
			self.gains.shape = (self.nchan,1)
		self.offsets = self.params.get('SourceChOffset')
		if self.offsets is not None:
			self.offsets = numpy.array([float(x) for x in self.offsets[:self.nchan]], dtype=numpy.float32)
			self.offsets.shape = (self.nchan,1)
		
		for k,v in list(self.statedefs.items()):
			a = numpy.zeros((self.stateveclen,1), dtype=numpy.uint16)
			startbyte = int(v['bytePos'])
			startbit  = int(v['bitPos'])
			nbits     = int(v['length'])
			nbytes    = int( (startbit+nbits) / 8 )
			if (startbit+nbits)%8: nbytes += 1
			extrabits = nbytes*8 - nbits - startbit;
			startmask = 255 & (255 << startbit)
			endmask   = 255 & (255 >> extrabits)
			div       = (1 << startbit);
			v['slice'] = slice(startbyte, startbyte+nbytes)
			v['mask'] = numpy.array([255]*nbytes, dtype=numpy.uint8)
			v['mask'][0]  &= startmask
			v['mask'][-1] &= endmask
			v['mask'].shape = (nbytes,1)
			v['mult'] = numpy.asmatrix(256.0 ** numpy.arange(nbytes, dtype=numpy.float64) / float(div))
			self.statedefs[k] = v

		self.open()
		
	def open(self):
		if self.file.closed:
			self.file = open(self.filename, 'rb')
		self.file.seek(self.headerlen)
	
	def close(self):
		if not self.file.closed:
			self.file.close()
		
	def __str__(self):
		nsamp = self.samples()
		s = ["<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))]
		s.append('file ' + self.filename.replace('\\', '/'))
		s.append('recorded ' + self.date())
		s.append('%d samples @ %gHz = %s' % (nsamp, self.samplingfreq_hz, self.sample2time(nsamp),) )
		s.append('%d channels, total %.3g MB' % (self.nchan, self.datasize()/1024.0**2,) )
		if not self.file.closed:
			s.append('open for reading at sample %d  (%s)' % (self.tell(), self.sample2time(self.tell()),) )			
		return '\n    '.join(s)
		
	def __repr__(self):
		return self.__str__()
	
	def channels(self):
		return self.nchan

	def samplingrate(self):
		return self.samplingfreq_hz

	def datasize(self):
		return os.stat(self.filename)[6] - self.headerlen
		
	def samples(self):
		return int( self.datasize() / self.bytesperframe )

	def date(self):
		d = self.datestamp
		if not isinstance(d, str): d = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(self.datestamp))
		return d
	
	def readHeader(self):
		line = self.file.readline().split()
		k = [x.rstrip('=') for x in line[::2]]
		v = line[1::2]
		self.headline = dict(list(zip(k,v)))
		self.headerlen = int(self.headline['HeaderLen'])
		self.nchan = int(self.headline['SourceCh'])
		self.stateveclen = int(self.headline['StatevectorLen'])
		fmtstr = self.headline.get('DataFormat','int16')
		fmt = {'int16':'h', 'int32':'l', 'float32':'f'}.get(fmtstr)
		if fmt is None: raise DatFileError( 'unrecognized DataFormat "%s"' % fmtstr )
		self.bytesperchannel = struct.calcsize(fmt)
		self.framefmt     = fmt * self.nchan + 'B' * self.stateveclen
		self.unpacksig    = fmt * self.nchan + 'x' * self.stateveclen
		self.unpackstates = 'x' * self.bytesperchannel * self.nchan + 'B' * self.stateveclen
		
		line = self.file.readline()
		if line.strip() != '[ State Vector Definition ]':
			raise DatFileError( 'failed to find state vector definition section where expected' )
		while True:
			try: line = self.file.readline()
			except: break
			if len(line) == 0 or line[0] == '[': break
			rec = ParseState(line)
			name = rec.pop('name')
			self.statedefs[name] = rec
		
		if line.strip() != '[ Parameter Definition ]':
			raise DatFileError( 'failed to find parameter definition section where expected' )
			
		while True:
			try: line = self.file.readline()
			except: break
			if self.file.tell() >= self.headerlen: break
			if not len(line.strip()): continue
			rec = ParseParam(line)
			name = rec.pop('name')
			self.paramdefs[name] = rec
			self.params[name] = rec.get('scaled', rec['val'])

		self.samplingfreq_hz = float(str(self.params['SamplingRate']).rstrip('Hz'))

		def fixdate(d):
			try: import datetime; d = time.mktime(time.strptime(d, '%a %b %d %H:%M:%S %Y'))
			except: pass
			else: return d

			try: import datetime; d = time.mktime(time.strptime(d, '%Y-%m-%dT%H:%M:%S'))
			except: pass
			else: return d
			
			return d

		self.datestamp = fixdate(self.params.get('StorageTime'))
	
	def read(self, nsamp=1, apply_gains=True):
		if nsamp==-1:
			nsamp = self.samples() - self.tell()
		if nsamp=='all':
			self.rewind(); nsamp = self.samples()
		if isinstance(nsamp, str):
			nsamp = self.time2sample(nsamp)

		raw = self.file.read(self.bytesperframe*nsamp)
		nsamp = int( len(raw) / self.bytesperframe )
			
		sig = numpy.zeros((self.nchan,nsamp),dtype=numpy.float32)
		rawstates = numpy.zeros((self.stateveclen,nsamp), dtype=numpy.uint8)
		S1 = struct.Struct('<' + self.framefmt[:self.nchan]); n1 = S1.size
		S2 = struct.Struct('<' + self.framefmt[self.nchan:]); n2 = S2.size
		xT = sig.T; rT = rawstates.T; start = 0
		for i in range(nsamp):
			mid = start + n1; end = mid + n2
			xT[i].flat = S1.unpack(raw[start:mid])   # multiple calls to precompiled Struct.unpack seem to be better than
			rT[i].flat = S2.unpack(raw[mid:end])     # a single call to struct.unpack('ffffff...bbb') on the whole data
			start = end                              # since time efficiency is <= and the latter caused *massive* memory leaks on the apple's python 2.6.1 (snow leopard 64-bit)
			
		if apply_gains:
			if self.offsets is not None: sig -= self.offsets
			if self.gains is not None: sig *= self.gains

		sig = numpy.asmatrix(sig)
		return sig,rawstates
	
	def decode(self, nsamp=1, states='all', apply_gains=True):
		sig,rawstates = self.read(nsamp, apply_gains=apply_gains)
		states,statenames = {},states
		if statenames == 'all':
			statenames = list(self.statedefs.keys())
		for statename in statenames:
			sd = self.statedefs[statename]
			states[statename] = numpy.array(sd['mult']*numpy.asmatrix(rawstates[sd['slice'],:] & sd['mask']), dtype=numpy.int32)
		return sig,states
				
	def tell(self):
		if self.file.closed: raise IOError( 'dat file is closed' )
		return int( (self.file.tell() - self.headerlen) / self.bytesperframe )
		
	def seek(self, value, wrt='bof'):
		if self.file.closed: raise IOError( 'dat file is closed' )
		if isinstance(value, str):
			value = self.time2sample(value)
		
		if wrt in ('bof',-1):
			wrt = 0
		elif wrt in ('eof',+1):
			wrt = self.samples()
		elif wrt in ('cof',0):
			wrt = self.tell()
		else:
			raise IOError( 'unknown origin "%s"' % str(wrt) )
		
		value = min(self.samples(), max(0, value + wrt))
		self.file.seek(value * self.bytesperframe + self.headerlen)
				
	def rewind(self):
		self.file.seek(self.headerlen)
		
	def time2sample(self, value):
		t = value.split(':')
		if len(t) > 3:
			raise DatFileError( 'too many colons in timestamp "%s"' % value )
		t.reverse()
		t = [float(x) for x in t] + [0]*(3-len(t))
		t = t[0] + 60.0 * t[1] + 3600.0 * t[2]
		return int(round(t * self.samplingfreq_hz))
		
	def sample2time(self, value):
		msecs = round(1000.0 * float(value) / self.samplingfreq_hz)
		secs,msecs = divmod(int(msecs), 1000)
		mins,secs  = divmod(int(secs), 60)
		hours,mins  = divmod(int(mins), 60)
		return '%02d:%02d:%02d.%03d' % (hours,mins,secs,msecs)

	def msec2samples(self, msec):
		if msec is None: return None
		return numpy.round(self.samplingfreq_hz * msec / 1000.0)

	def samples2msec(self, samples):
		if samples is None: return None
		return 1000.0 * samples / float(self.samplingfreq_hz)

	def plotstates(self, states, whichstates=None, **kwargs):
		labels = list(states.keys())
		if whichstates is not None:
			if isinstance(whichstates, str): whichstates = whichstates.split()
			labels = whichstates
		
		v = numpy.matrix(numpy.concatenate([states[k] for k in labels], axis=0), dtype=numpy.float32)
		ntraces,nsamp = v.shape
		#v = v - numpy.min(v,1)
		sc = numpy.max(v,1)
		sc[numpy.where(sc==0.0)] = 1.0
		v = v / sc
		offsets = numpy.asmatrix(numpy.arange(1.0,ntraces+1.0)).A
		v = v.T.A * -0.7 + offsets
		t = numpy.matrix(list(range(nsamp)), dtype=numpy.float32).T.A / self.samplingfreq_hz

		plt = load_plt()
		plt.cla()
		ax = plt.gca()
		h = plt.plot(t, v, **kwargs)
		ax.set_xlim(0,nsamp/self.samplingfreq_hz)
		ax.set_yticks(offsets.flatten())
		ax.set_yticklabels(labels)
		ax.set_ylim(ntraces+1,0)
		ax.grid(True)
		plt.draw()
		return h
	
	def plotsig(self, sig, fac=3.0, ch=None):
		ntraces,nsamp = sig.shape
		labels = self.params.get('ChannelNames', '')
		if len(labels)==0: labels = [str(x) for x in range(1,ntraces+1)]

		# Plotting of subsets of channels (GM)
		if ch is not None:
			ch = [c if isinstance(c, int) else labels.index(c) for c in ch]
			sig = sig[ch, :]
			ntraces,nsamp = sig.shape
			labels = numpy.array( labels )[ch]

		v = numpy.asmatrix(sig).T
		v = v - numpy.median(v, axis=0)
		offsets = numpy.asmatrix(numpy.arange(-1.0,ntraces+1.0))
		offsets = offsets.A * max(v.A.std(axis=0)) * fac
		v = v.A + offsets[:,1:-1]
		
		t = numpy.matrix(list(range(nsamp)), dtype=numpy.float32).T.A / self.samplingfreq_hz

		plt = load_plt()
		plt.cla()
		ax = plt.gca()
		h = plt.plot(t, v)
		ax.set_xlim(0,nsamp/self.samplingfreq_hz)
		ax.set_yticks(offsets.flatten()[1:-1])
		ax.set_yticklabels(labels)
		ax.set_ylim(offsets.flatten()[-1],offsets.flatten()[0])
		ax.grid(True)
		plt.draw()
		return h
		
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
	
def ParseState(state):
	state = state.split()
	return {
		'name'    : state[0],
		'length'  : int(state[1]),
		'startVal': int(state[2]),
		'bytePos' : int(state[3]),
		'bitPos'  : int(state[4])
	}

def ReadPrmFile(f):
	open_here = isinstance(f, str)
	if open_here: f = open(f)
	f.seek(0)
	p = [ParseParam(line) for line in f.readlines() if len(line.strip())]
	if open_here: f.close()
	return p

def ParseParam(param):
	param = param.strip().split('//', 1)
	comment = ''
	if len(param) > 1:
		comment = param[1].strip()
		
	param = param[0].split()
	category = [unescape(x) for x in param.pop(0).split(':')]
	param = [unescape(x) for x in param]
	category += [''] * (3-len(category))
	if len(category)>3: # this shouldn't happen, but some modules seem to register parameters with the string '::' inside one of the category elements. Let's assume this only happens in the third element
		category = category[:2] + [':'.join(category[2:])]
	datatype = param.pop(0)
	name = param.pop(0).rstrip('=')
	rec = {
		'name':name, 'comment':comment, 'category':category, 'type':datatype,
		'defaultVal':'', 'minVal':'', 'maxVal':'',
	}

	scaled = None
	if datatype in ('int', 'float'):
		datatypestr = datatype
		datatype = {'float':float, 'int':int}.get(datatype)
		val = param[0]
		unscaled,units,scaled = DecodeUnits(val, datatype)
		if isinstance(unscaled, (str,type(None))): Warn('failed to interpret "%s" as type %s in parameter "%s"' % (val,datatypestr,name))
		rec.update({
			'valstr'     : val,
			'val'        : unscaled,
			'units'      : units,
		})

	elif datatype in ('string', 'variant'):
		val = param.pop(0)
		rec.update({
			'valstr'     : val,
			'val'        : val,
		})
		
	elif datatype.endswith('list'):
		valtype = datatype[:-4]
		valtypestr = valtype
		valtype = {'float':float, 'int':int, '':str, 'string':str, 'variant':str}.get(valtype, valtype)
		if isinstance(valtype,str): raise DatFileError( 'Unknown list type "%s"' % datatype	)
		numel,labels,labelstr = ParseDim(param)
		val = param[:numel]
		valstr = ' '.join(filter(len, [labelstr]+val))
		if valtype==str:
			unscaled = val
			units = [''] * len(val)
		else:
			eachstr = val
			val = [DecodeUnits(x,valtype) for x in eachstr]
			if len(val): unscaled,units,scaled = list(zip(*val))[:3]
			else: unscaled,units,scaled = [],[],[]
			for u,v,s in zip(unscaled,val,eachstr):
				if isinstance(u, (str,type(None))): Warn( 'failed to interpret "%s" as type %s in parameter "%s"' % (s,valtypestr,name) )
		rec.update({
			'valstr'     : valstr,
			'valtype'    : valtype,
			'len'        : numel,
			'val'        : unscaled,
			'units'      : units,
		})
	
	elif datatype.endswith('matrix'):
		valtype = datatype[:-6]
		valtype = {'float':float, 'int':int, '':str, 'string':str, 'variant':str}.get(valtype, valtype)
		if isinstance(valtype,str): raise DatFileError( 'Unknown matrix type "%s"' % valtype )
		nrows,rowlabels,rowlabelstr = ParseDim(param)
		ncols,collabels,collabelstr = ParseDim(param)
		valstr = ' '.join(filter(len, [rowlabelstr, collabelstr] + param[:nrows*ncols]))
		val = []
		for i in range(nrows):
			val.append([])
			for j in range(ncols):
				val[-1].append(param.pop(0))
		rec.update({
			'valstr'     : valstr,
			'valtype'    : valtype,
			'val'        : val,
			'shape'      : (nrows,ncols),
			'dimlabels'  : (rowlabels,collabels),
		})
				
	else:
		Warn( "unsupported parameter type %r" % datatype )
		rec.update({
			'valstr'     : ' '.join(param),
			'val'        : param,
		})

	param.reverse()
	if len(param): rec['maxVal'] = param.pop(0)
	if len(param): rec['minVal'] = param.pop(0)
	if len(param): rec['defaultVal'] = param.pop(0)

	if scaled is None:
		rec['scaled'] = rec['val']
	else:
		rec['scaled'] = scaled
	return rec

def ParseDim(param):
	extent = param.pop(0)
	labels = []
	if extent == '{':
		while True:
			p = param.pop(0)
			if p == '}': break
			labels.append(p)
		extent = len(labels)
		labelstr = ' '.join(['{'] + labels + ['}'])
	else:
		labelstr = extent
		extent = int(extent)
		labels = [str(x) for x in range(1,extent+1)]
	return extent,labels,labelstr

def DecodeUnits(s, datatype=float):
	s = str(s)
	if s.lower().startswith('0x'): return int(s,16),None,None
	units = ''
	while len(s) and not s[-1] in '0123456789.':
		units = s[-1] + units
		s = s[:-1]
	if len(s) == 0: return None, None, None
	try: unscaled = datatype(s)
	except:
		try: unscaled = float(s)
		except: return s,None,None
	scaled = unscaled * {
		                 'hz':1, 'khz':1000, 'mhz':1000000,
		                'muv':1,  'mv':1000,   'v':1000000,
		'musec':0.001, 'msec':1, 'sec':1000, 'min':60000,
		                 'ms':1,   's':1000,
	}.get(units.lower(), 1)
	return unscaled,units,scaled

def FormatPrmList(p, sort=False):
	max_element_width = 6
	max_value_width = 20
	max_treat_string_as_number = 1
	def escape(s):
		s = s.replace('%', '%%')
		s = s.replace(' ', '%20')
		if len(s) == 0: s = '%'
		return s
		
	def FormatDimLabels(p):
		dl = p.get('dimlabels', None)
		if dl is None:
			if isinstance(p['val'], (tuple,list)): return ['', str(len(p['val']))]
			else: return ['', '']
		sh = p['shape']
		dl = list(dl)
		for i in range(len(dl)):
			if len(dl[i]):
				t = ' '.join([escape(x) for x in dl[i]])
				td = ' '.join([str(j+1) for j in range(len(dl[i]))])
				if t == td: dl[i] = str(len(dl[i]))
				else: dl[i] = '{ ' + t + ' }'
			else: dl[i] = str(sh[i])
		return dl
	
	def FormatVal(p):
		if isinstance(p, dict): p = p['val']
		if isinstance(p, (tuple,list)): return ' '.join([FormatVal(x) for x in p])
		return escape(str(p))
	
	vv = []
	pp = [{} for i in range(len(p))]
	for i in range(len(p)):
		pp[i]['category'] = ':'.join([x for x in p[i]['category'] if len(x.strip())])
		pp[i]['type'] = p[i]['type']
		pp[i]['name'] = p[i]['name'] + '='
		pp[i]['rows'],pp[i]['cols'] = FormatDimLabels(p[i])
		v = FormatVal(p[i]).split()
		if len(v) == 0: v = ['%']
		pp[i]['val'] = list(range(len(vv), len(vv)+len(v)))
		vv += v
		pp[i]['comment'] = '// ' + p[i]['comment']
	align = [len(v) <= max_element_width for v in vv]
	numalign = [len(v) <= max_treat_string_as_number or v[0] in '+-.0123456789' for v in vv]
	n = [(vv[i]+' ').replace('.', ' ').index(' ') * int(align[i] and numalign[i]) for i in range(len(vv))]
	maxn = max(n)
	for i in range(len(vv)):
		if align[i] and numalign[i]: vv[i] = ' ' * (maxn-n[i]) + vv[i]
		if align[i]: n[i] = len(vv[i])
	maxn = max(n)
	for i in range(len(vv)):
		if align[i] and numalign[i]: vv[i] = vv[i].ljust(maxn, ' ')
		elif align[i]: vv[i] = vv[i].rjust(maxn, ' ')
	for i in range(len(pp)):
		pp[i]['val'] = ' '.join([vv[j] for j in pp[i]['val']])
	align = [len(pp[i]['val']) <= max_value_width for i in range(len(pp))]
	maxn = max([0]+[len(pp[i]['val']) for i in range(len(pp)) if align[i]])
	for i in range(len(pp)):
		if align[i]: pp[i]['val'] = pp[i]['val'].ljust(maxn, ' ')
	for x in list(pp[0].keys()):
		n = [len(pp[i][x]) for i in range(len(pp))]
		n = max(n)
		for i in range(len(pp)):
			if x in ['rows','cols']: pp[i][x] = pp[i][x].rjust(n, ' ')
			elif not x in ['comment', 'val']: pp[i][x] = pp[i][x].ljust(n, ' ')
			if not x in ['rows', 'cols', 'category']: pp[i][x] = '  ' + pp[i][x]
	if sort: pp = sorted(pp, cmp=lambda x,y:cmp((x['category'],x['name']), (y['category'],y['name'])) )
	fields = 'category type name rows cols val comment'.split()
	for i in range(len(pp)): pp[i] = ' '.join([pp[i][x] for x in fields])
		
	return pp
	

def load_plt():
	try:
		import matplotlib
		if not 'matplotlib.backends' in sys.modules: matplotlib.interactive(True)
		import matplotlib.pyplot as plt
		return plt
	except ImportError:
		Warn( "%s module failed to import matplotlib.pyplot: plotting methods will not work" % __name__ )

