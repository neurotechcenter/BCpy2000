# -*- coding: utf-8 -*-
# 
#   $Id: TreeStructure.py 3445 2011-08-08 15:46:38Z jhill $
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
# -*- coding: utf-8 -*-
__all__ = ['trie']

import struct
import md5
import time
import os
try: from .ProgressMonitoring import progress
except ImportError: progress = None

packedfmt_header    = '<HLL'    # number of children, parent index, count of occurrences
packedfmt_link      = '<ccL'    # key + node number
packedsize_header   = struct.calcsize(packedfmt_header)
packedsize_link     = struct.calcsize(packedfmt_link)
packedsize_key      = struct.calcsize(packedfmt_link[:-1])

class TrieError(Exception): pass

class trienode:
	def __init__(self, parent=0, string=''):
		#self.string = string  # can save a bit of memory by commenting this out and hence not storing the strings explicitly
		self.parent = parent
		self.children = {}
		self.count = 0
		
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		if hasattr(self, 'string'): content = self.string
		else: content = ''
		s += "\n    (from %02d, count %d) %s\n" % (self.parent, self.count, content)
		for key,val in sorted(self.children.items()): s += "    %s -> %02d\n" % (repr(key), val)
		return s

	def packed_fmt(self):
		return packedfmt_header + packedfmt_link[1:] * len(self.children)
		
	def packed_size(self):
		return struct.calcsize(self.packed_fmt())
	
def packnode(node, lut=None):
	if lut == None: trans = lambda x: x
	else: trans = lambda x: lut[x]
	parent = node.parent
	vals = [len(node.children), trans(parent), node.count]
	packedsize_link
	for key,val in list(node.children.items()):
		key = key.encode('utf-8').rjust(packedsize_key, '\x00')
		if len(key) > packedsize_key: raise TrieError("encoded key '%s' overflows the number of bytes allocated for keys (%d)" % (key,packedsize_key))
		vals.extend(key)
		vals.extend([trans(val)])
	return struct.pack(node.packed_fmt(), *vals)

def unpacknode(x, ind=0):
	node = trienode()
	nchildren,node.parent,node.count = list(map(int, struct.unpack(packedfmt_header, x[ind:ind+packedsize_header])))
	ind += packedsize_header
	for i in range(nchildren):
		seq = struct.unpack(packedfmt_link, x[ind:ind+packedsize_link])
		key = ''.join(seq[:-1]).strip('\x00').decode('utf-8')
		val = int(seq[-1])
		node.children[key] = val
		ind += packedsize_link
	return node,ind


class trie:
	"""
	A trie structure, either unpacked (python lists, extensible) or packed
	(binary, inextensible). If packed, either on disk or in memory.
	
	Construct with trie()  or trie(filename)  or trie(binary_blob).
	
	trie() creates an empty trie: use the add() method to train it one substring
	at a time.
	
	Predictive distributions:
	
	    t.dist(' th')  # returns unnormalized counts
	    t.dist(' th', normalized=True)  # returns prob. distribution normalized
	                                    # by the ' th' node's own count
	
	The latter syntax effectively does this:

		node = t.retrieve(' th')
		distrib = t.dist(' th')
		for letter in distrib: distrib[letter] /= float(node.count)
	

	The read() and write() methods allow conversion between a packed trie on disk
	and a packed trie in memory.
	
	The pack() and unpack() methods allow conversion between an unpacked and a
	packed trie in memory.
	
	"""###

	def __init__(self, packed=None):
		if packed == None: self.nodes = [trienode(parent=0)]
		else:
			self.nodes = packedtriedata(packed)
			try: self.nodes[0].children.get('a')
			except:
				# Nasty heuristic to determine whether raw string was intended to be a filename or raw binary content
				if len(packed) < 1024 and len(os.path.basename(packed)) < 64: raise IOError("file %s not found" % packed) 
				else: raise TrieError("invalid content in <%s instance at 0x%08x>" % (self.nodes.__class__.__name__, id(self.nodes)))
						
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		if self.ispacked():
			s += ('\n'+repr(self.nodes)).replace('\n', '\n    ')
		else:
			s += '\n    %d nodes (unpacked)' % len(self.nodes)
		return s
		
	def walk(self, string):
		ind = 0
		found = ''
		for letter in string:
			newind = self.nodes[ind].children.get(letter)
			if newind==None: break
			ind = newind
			found += letter
		return (ind,found)
		
	def retrieve(self, string):
		(ind,found) = self.walk(string)
		node = self.nodes[ind]
		if found == string: return node
		else: return None

	def ispacked(self):
		return not isinstance(self.nodes, list)

	def isondisk(self):
		if not self.ispacked(): return False
		return isinstance(self.nodes.content, filestr)
		
	def add(self, string):
		if self.ispacked(): raise TrieError('cannot add strings to a packed trie')
		if not isinstance(string, str): string = string.decode('utf-8')
		(ind,found) = self.walk(string)
		node = self.nodes[ind]
		for i in range(len(found), len(string)):
			newnode = trienode(parent=ind, string=string[:i+1])
			ind = len(self.nodes)
			self.nodes.append(newnode)
			node.children[string[i]] = ind
			node = newnode
		while ind != None:
			self.nodes[ind].count += 1
			parent = self.nodes[ind].parent
			if parent == ind: ind = None  # stop after dealing with node 0, which is its own parent
			else: ind = parent

	def dist(self, node, normalize=False):
		distrib = {}
		if isinstance(node, str): node = self.retrieve(node)
		if node == None: return distrib
		if isinstance(node, int): node = self.nodes[node]
		for letter,ind in list(node.children.items()):
			distrib[letter] = self.nodes[ind].count
			if normalize: distrib[letter] /= float(node.count)
		return distrib

	def pack(self):
		return trie(packtrie(self))
	
	def unpack(self):
		if not self.ispacked(): raise TrieError('not packed')
		return unpacktrie(self.nodes.content)

	def gethash(self, length=16):
		if not self.ispacked(): raise TrieError('not packed')
		if self.isondisk(): raise TrieError('cannot hash: not in memory')
		m = md5.md5()
		m.update(self.nodes.content)
		return m.hexdigest()[:length]
		
	def write(self, filestem='', suffix=None, extension='.trie'):
		if not self.ispacked(): raise TrieError('not packed')
		if self.isondisk(): raise TrieError('already on disk')
		if suffix == None:
			suffix = self.gethash(8)
			suffix = suffix + '-%04d%02d%02d%02d%02d%02d'%time.localtime()[:6]
			if len(filestem): suffix = '-' + suffix
		filename = filestem + suffix + extension
		filename = os.path.abspath(filename)
		a = open(filename, 'wb')
		a.write(self.nodes.content)
		a.close()
		return filename
	
	def read(self):
		if not self.isondisk(): raise TrieError('already in memory')
		fp = self.nodes.content.handle
		t = fp.tell()
		fp.seek(0)
		content = fp.read()
		fp.seek(t)
		return trie(content)
	
	def count(self, string):
		node = self.retrieve(string)
		if node == None: return 0
		return node.count
	
	def nchildren(self, string):
		node = self.retrieve(string)
		if node == None: return 0
		return len(node.children)
		
	def kncount(self, string, prefixes=None):
		"""
			N_1+ ( .  string )
		"""###
		if prefixes==None: prefixes = list(self.dist('').keys())
		return sum([self.count(p+string)>=1 for p in prefixes])
	
	def kndist(self, string):
		"""
			N_1+ ( .  string w )  for each w that follows string
		"""###
		d = self.retrieve(string)
		if d == None: d = {}
		else: d = dict(d.children)
		prefixes = list(self.dist('').keys())
		for w in list(d.keys()): d[w] = self.kncount(string+w, prefixes)
		return d

	def __getitem__(self, ind):
		if isinstance(ind, int):
			try: return self.nodes[ind]
			except IndexError: return None
		return self.retrieve(ind)

def packtrie(t):
	"""
	Helper function for trie.pack()
	"""###
	if t.ispacked(): raise TrieError('already packed')
	x = ''
	lut = [0]
	for node in t.nodes[:-1]:
		lut.append(lut[-1]+node.packed_size())
	if progress != None: pr = progress(len(t.nodes), ' packing')
	for i in range(len(t.nodes)):
		x += packnode(t.nodes[i], lut=lut)
		if i%5000 == 0 and progress != None: pr.update(i, '(%d nodes)'%i)
	if progress != None: pr.done()
	return x

def unpacktrie(x):
	"""
	Helper function for trie.unpack()
	"""###
	t = trie()
	t.nodes = []
	ind = 0
	rlut = {}
	if progress != None: pr = progress(len(x), 'unpacking')
	while ind < len(x):
		rlut[ind] = len(t.nodes)
		node,ind = unpacknode(x,ind)
		t.nodes.append(node)
		nnodes = len(t.nodes)
		if nnodes % 5000 == 0 and progress != None: pr.update(ind, '(%d nodes)'%nnodes)
	for node in t.nodes:
		node.parent = rlut[node.parent]
		for key,val in list(node.children.items()): node.children[key] = rlut[val]
	return t

class packedtriedata:
	"""
	Helper class:  a virtual list of trienode objects, indexible like [this]
	"""###
	def __init__(self, string):
		try: isfile = os.path.isfile(string)
		except: isfile = False
		if isfile: self.content = filestr(string)
		else: self.content = string
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		if isinstance(self.content, str): s += '\n    %.3g MB in memory (packed)' % (float(len(self.content)) / 1024**2)
		elif isinstance(self.content, filestr): s += '\n    %s' % repr(self.content.handle)
		return s
	def __getitem__(self, i):
		if not isinstance(i, int): raise TypeError('invalid subscript type')
		return unpacknode(self.content, i)[0]

class filestr:
	"""
	Helper class
	"""###
	def __init__(self, filename):
		self.filename = filename
		self.handle = open(filename, 'rb')
	def __getitem__(self, i):
		if not isinstance(i, slice) or i.step != None or i.start==None or i.stop==None or i.start<0 or i.stop<0:
			raise TypeError('invalid subscript type')
		self.handle.seek(i.start, 0)
		return self.handle.read(max(0,i.stop-i.start))
	def __del__(self):
		self.close()
	def close(self):
		self.handle.close()
