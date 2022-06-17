# -*- coding: utf-8 -*-
# 
#   $Id: $
#   
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
# 
#   Copyright (C) 2007-12  Jeremy Hill, Thomas Schreiner,
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

"""
This file implements the minimal subset of matlab API definitions required to read .mat files written by bci_stream2mat.exe
ReadMatFile() will read only certain types of matfile (version 5 uncompressed), only non-sparse 2-dimensional matrices, and
only real numeric types of class single or uint32, although it *will* cope with unicode character arrays, cell arrays and
structs.

This is not normally used, if the rest of the BCI2000 Tools are present:  in the normal case SigTools.loadmat, which wraps
scipy.io.loadmat, is used instead. However, this file is used as a fallback if either SigTools or scipy is absent. This
potentially frees BCI2000Tools.Chain from the unwieldy dependency on scipy.
"""
__all__ = [ 'ReadMatFile', 'Bunch' ]

matTextHeaderLength = 116
matVersionInfoOffset = 124
matVersionInfoLength = 4
matPadding = 8 # padding to 64 bit boundaries

miMATRIX = 14

miINT8 = 1
miINT32 = 5
miUINT8 = 2
miUINT32 = 6
miSINGLE = 7
miUTF16 = 17

mxCELL_CLASS   = 1
mxSTRUCT_CLASS = 2
mxCHAR_CLASS   = 4
mxDOUBLE_CLASS = 6
mxSINGLE_CLASS = 7
mxUINT32_CLASS = 13

NUMERIC_TYPES = {
	mxSINGLE_CLASS : ( miSINGLE, '<f' ),
	mxUINT32_CLASS : ( miUINT32, '<L' ),
	# TODO: could flesh this out with more definitions
}

class Bunch( dict ):
	"""
	A class like a dict but in which you can de-reference members like.this as well as like['this'] .
	"""
	def __init__( self, d=(), **kwargs ): dict.__init__( self, d ); dict.update( self, kwargs )
	def update( self, d=(), **kwargs ): dict.update( self, d ); dict.update( self, kwargs ); return self
	def __getattr__( self, key ):
		if key in self.__dict__: return self.__dict__[ key ]
		elif key in self: return self[ key ]
		else: raise AttributeError( "'%s' object has no attribute or item '%s'" % ( self.__class__.__name__, key ) )
	def __setattr__( self, key, value ):
		if key in self.__dict__: self.__dict__[ key ] = value
		else: self[ key ] = value
	def _getAttributeNames( self ): return list(self.keys())


CONTAINER = Bunch

import struct
try: import numpy
except ImportError: numpy = None
	
def chomp( file, fmt ):
	size = struct.calcsize( fmt )
	raw = file.read( size )
	if len( raw ) < size: return None
	return struct.unpack( fmt, raw )
def ReadMatFile( file ):
	if isinstance( file, str ): file = open( file, 'rb' )
	container = CONTAINER()
	container[ '__header__' ]  = file.read( matTextHeaderLength ).strip()
	file.seek( matVersionInfoOffset )
	container[ '__version__' ] = file.read( matVersionInfoLength )
	while ReadArray( file, container ): pass
	return container
def ReadArray( file, container, index=None ):
	arrayStartFlag = chomp( file, '<L' )
	if arrayStartFlag == None: return False
	if arrayStartFlag != ( miMATRIX, ): raise ValueError( "array header started with unexpected code" )
	nBytes, flagType, flagLengthBytes, arrayType, flagPadding = chomp( file, '<LLLLL' ) # NB: nBytes cannot be relied upon for structs and cell arrays written by bci_stream2mat
	if ( flagType, flagLengthBytes, flagPadding ) != ( miUINT32, 8, 0 ): raise ValueError( "array header contained unexpected data" )
	dimType, dimLengthBytes, nRows, nCols = chomp( file, '<LLll' )	
	numel = nRows * nCols
	shape = [ nRows, nCols ]
	if ( dimType, dimLengthBytes ) != ( miINT32, 8 ): raise ValueError( "array dimension info contained unexpected data" )
	data = [ [ None for i in range( nCols ) ] for i in range( nRows ) ]
	nameType, nameLengthBytes = chomp( file, '<LL' )
	if nameType != miINT8: raise ValueError( "array name had unexpected name type" )
	if nameLengthBytes:
		name = file.read( nameLengthBytes )
		while file.tell() % matPadding: file.read( 1 )
		if index == None: index = name
	if index == None: raise ValueError( "no name recorded in file, and none supplied in call" )
	#print "reading ", repr( index )
	if arrayType == mxSTRUCT_CLASS:
		fieldNameDataFlags, fieldNameLength, fieldNameDataType, fieldNameDataSize = chomp( file, '<LLLL' )
		if fieldNameDataFlags != ( ( 4 << 16 ) | miINT32 ): raise ValueError( "unexpected data flags for struct fieldname record" )
		if fieldNameDataType != miINT8: raise ValueError( "unexpected data type for struct fieldname record" )
		if fieldNameDataSize % fieldNameLength: raise ValueError( "fieldname data block size is not an integer number of fieldNameLengths" )
		fields = [ chomp( file, str( fieldNameLength ) + 's' )[ 0 ].strip( '\x00' ) for i in range( fieldNameDataSize / fieldNameLength ) ]
		for iCol in range( nCols ):
			for iRow in range( nRows ):
				data[ iRow ][ iCol ] = CONTAINER()
				for field in fields:
					ReadArray( file, container=data[ iRow ][ iCol ], index=field )
	elif arrayType == mxCELL_CLASS:
		for iCol in range( nCols ):
			for iRow in range( nRows ):
				ReadArray( file, container=data[ iRow ], index=iCol )
	elif arrayType == mxCHAR_CLASS:
		dataType, dataLengthBytes = chomp( file, '<LL' )
		if dataType != miUTF16: raise ValueError( "unexpected data type for payload in character array" )
		if dataLengthBytes != numel * 2: raise ValueError( "unexpected size for payload in character array" )
		for iCol in range( nCols ):
			for iRow in range( nRows ):
				data[ iRow ][ iCol ] = file.read( 2 )
		data = [ ''.join( row ).decode( 'utf-16' ) for row in data ]
	elif arrayType in NUMERIC_TYPES:
		expectedDataType, dataElementFormat = NUMERIC_TYPES[ arrayType ]
		dataType, moreFlags = chomp( file, '<HH' )
		if dataType != expectedDataType: raise ValueError( "payload for numeric array of type %d has unexpected data type %d" % ( arrayType, dataType ) )
		if moreFlags not in [ 4, 0 ]: raise ValueError( "unexpected flags in numeric array" )
		if not ( moreFlags & 4 ):
			dataSizeBytes, = chomp( file, '<L' )
			if dataSizeBytes != numel * struct.calcsize( dataElementFormat ): raise ValueError( "payload for %s numeric array of type %d has unexpected block size %d bytes" % ( 'x'.join( [ str( x ) for x in shape ] ), arrayType, dataSizeBytes ) )
		for iCol in range( nCols ):
			for iRow in range( nRows ):
				data[ iRow ][ iCol ], = chomp( file, dataElementFormat )
	else:
		raise ValueError( "unrecognized Matlab array type %d" % arrayType )
	if numpy: data = numpy.array( data )
	container[ index ] = data
	while file.tell() % matPadding: file.read( 1 )
	return True
