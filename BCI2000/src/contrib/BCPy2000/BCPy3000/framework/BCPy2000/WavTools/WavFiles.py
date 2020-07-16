# -*- coding: utf-8 -*-
# 
#   $Id: WavFiles.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = []

from . import Base
import wave
import os.path
import io
import sys

little_endian = sys.byteorder == 'little'

def endianswap(s, nbytes):
	if little_endian or nbytes == 1: return s
	fmt = {2:'H', 4:'L'}
	if not nbytes in fmt: raise ValueError("failed to swap endianness for %d-byte values"%nbytes)
	fmt = str(len(s)/nbytes) + fmt[nbytes]
	return struct.pack('<'+fmt, *struct.unpack('>'+fmt, s))

def read(self, filename):
	wf = wave.open(filename, 'rb')
	nbytes = wf.getsampwidth()
	nchan = wf.getnchannels()
	nsamp = wf.getnframes()
	fs = wf.getframerate()
	comptype = (wf.getcomptype(),wf.getcompname())
	strdat = wf.readframes(nsamp)
	wf.close()
	strdat = endianswap(strdat, nbytes)
	self.set_bitdepth(nbytes*8)
	self.fs = fs
	if isinstance(filename, io.StringIO):
		self.filename = "(StringStream)"
	else:
		self.filename = os.path.abspath(filename)
	self.comptype = comptype
	self.y = self.str2dat(strdat, nsamp, nchan)
	if strdat != self.dat2str():
		print("warning: data mismatch in",self)
Base.wav.read = read

def write(self, filename=None):
	if filename==None: filename = self.filename
	if filename==None: raise TypeError('no filename supplied')		
	wf = wave.open(filename, 'wb')
	wf.setsampwidth(self.nbytes)
	wf.setnchannels(self.channels())
	wf.setnframes(self.samples())
	wf.setframerate(self.fs)
	wf.setcomptype(*self.comptype)
	s = self.dat2str()
	s = endianswap(s, self.nbytes)
	wf.writeframes(s)
	wf.close()
	self.filename = filename
Base.wav.write = write
