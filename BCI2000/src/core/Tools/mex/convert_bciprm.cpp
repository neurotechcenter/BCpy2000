///////////////////////////////////////////////////////////////////////////////
// $Id: convert_bciprm.cpp 5265 2016-03-02 20:23:27Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A Matlab (mex) subroutine that converts BCI2000 parameters
//  from Matlab struct into string representation and back.
//
//   parameter_lines = convert_bciprm( parameter_struct );
//
//  converts a BCI2000 parameter struct (as created by load_bcidat)
//  into a cell array of strings containing valid BCI2000 parameter
//  definition strings.
//
//  When the input is a cell array rather than a Matlab struct, convert_bciprm
//  will interpret the input as a list of BCI2000 parameter definition strings:
//
//   parameter_struct = convert_bciprm( parameter_lines );
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
///////////////////////////////////////////////////////////////////////////////
#include "mex.h"
#include "mexutils.h"
#include "Param.h"
#include "ParamList.h"
#include "BCIStream.h"
#include "BCIException.h"
#include <sstream>

using namespace std;

mxArray* StringsToStruct( const mxArray* );
mxArray* StructToStrings( const mxArray* );

void
BCIMexFunction( int nargout, mxArray* varargout[],
                int nargin,  const mxArray* varargin[] )
{
  if( PrintVersion( __FILE__, nargin, varargin ) )
    return;

  if( nargin < 1 )
    throw bciexception << "No input given.";

  switch( mxGetClassID( varargin[ 0 ] ) )
  {
    case mxCELL_CLASS:
      varargout[ 0 ] = StringsToStruct( varargin[ 0 ] );
      break;

    case mxSTRUCT_CLASS:
      varargout[ 0 ] = StructToStrings( varargin[ 0 ] );
      break;

    default:
      throw bciexception << "Expected a cell array or a struct as input.";
  }
}

mxArray*
StringsToStruct( const mxArray* inStrings )
{
  ParamList params;
  mwSize numStrings = mxGetNumberOfElements( inStrings );
  for( mwSize i = 0; i < numStrings; ++i )
  {
    const mxArray* cell = mxGetCell( inStrings, i );
    if( mxGetClassID( cell ) == mxCHAR_CLASS )
    {
      char* line = mxArrayToString( cell );
      params.Add( Param( line ) );
      mxFree( line );
    }
    else
      throw bciexception << "Expected a cell array of strings as input.";
  }
  return ParamlistToStruct( params );
}

mxArray*
StructToStrings( const mxArray* inStruct )
{
  ParamList params;
  StructToParamlist( inStruct, params );
  size_t numParams = params.Size();
  mxArray* strings = mxCreateCellMatrix( numParams, 1 );
  for( size_t i = 0; i < numParams; ++i )
  {
    ostringstream oss;
    oss << params[ i ];
    mxSetCell( strings, i, mxCreateString( oss.str().c_str() ) );
  }
  return strings;
}
