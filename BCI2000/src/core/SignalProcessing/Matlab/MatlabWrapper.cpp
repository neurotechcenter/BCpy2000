////////////////////////////////////////////////////////////////////////////////
// $Id: MatlabWrapper.cpp 5265 2016-03-02 20:23:27Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
//         jeremy.hill@tuebingen.mpg.de
// Description: Wrapper classes for convenient creation and manipulation of
//              Matlab workspace variables, and calling of Matlab functions.
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
////////////////////////////////////////////////////////////////////////////////
#include "MatlabWrapper.h"
#include "matlab/engine.h"
#include <sstream>

using namespace std;

namespace {
const string cErrorVariable = "bci_Error";
const string cAnsVariable = "bci_Ans";
} // anonymous namespace

////////////////////////////////////////////////////////////////////////////////
// MatlabEngine::DoubleMatrix definitions                                     //
////////////////////////////////////////////////////////////////////////////////
MatlabEngine::DoubleMatrix::DoubleMatrix( const GenericSignal& inSignal )
: vector<vector<double> >( inSignal.Channels(), vector<double>( inSignal.Elements() ) )
{
  for( size_t channel = 0; channel < size(); ++channel )
    for( size_t sample = 0; sample < ( *this )[ channel ].size(); ++sample )
      ( *this )[ channel ][ sample ] = inSignal( channel, sample );
}

MatlabEngine::DoubleMatrix::operator GenericSignal() const
{
  GenericSignal signal( size(), empty() ? 0 : ( *this )[ 0 ].size(), SignalType::float32 );
  for( size_t channel = 0; channel < size(); ++channel )
    for( size_t sample = 0; sample < ( *this )[ channel ].size(); ++sample )
      signal( channel, sample ) = ( *this )[ channel ][ sample ];
  return signal;
}

MatlabEngine::DoubleMatrix::DoubleMatrix( const SignalProperties& inProperties )
: vector<vector<double> >( 1, vector<double>( 2 ) )
{
  ( *this )[ 0 ][ 0 ] = inProperties.Channels();
  ( *this )[ 0 ][ 1 ] = inProperties.Elements();
}

MatlabEngine::DoubleMatrix::operator SignalProperties() const
{
  size_t channels = 0,
         elements = 0;
  if( !empty() )
  {
    if( !( *this )[ 0 ].empty() )
      channels = static_cast<size_t>( ( *this )[ 0 ][ 0 ] );
    if( ( *this )[ 0 ].size() > 1 )
      elements = static_cast<size_t>( ( *this )[ 0 ][ 1 ] );
  }
  return SignalProperties( channels, elements, SignalType::float32 );
}

////////////////////////////////////////////////////////////////////////////////
// MatlabEngine::StringMatrix definitions                                     //
////////////////////////////////////////////////////////////////////////////////
MatlabEngine::StringMatrix::StringMatrix( const Param& p )
: vector<vector<string> >( p.NumRows(), vector<string>( p.NumColumns() ) )
{
  for( size_t row = 0; row < size(); ++row )
    for( size_t column = 0; column < ( *this )[ row ].size(); ++column )
      ( *this )[ row ][ column ] = p.Value( row, column );
}

////////////////////////////////////////////////////////////////////////////////
// MatlabEngine definitions                                                   //
////////////////////////////////////////////////////////////////////////////////
int MatlabEngine::sNumInstances = 0;
Engine* MatlabEngine::spEngineRef = NULL;

MatlabEngine::MatlabEngine()
{
  sNumInstances++;
  if( !MatlabEngine::IsOpen() )
    MatlabEngine::Open();
}

MatlabEngine::~MatlabEngine()
{
  sNumInstances--;
}

void
MatlabEngine::Open()
{
  if( !IsOpen() )
  {
    spEngineRef = engOpen( nullptr );
    if( !spEngineRef )
      bcierr << "Could not open Matlab engine";
    else
      PutString( cErrorVariable, "" );
  }
}

void
MatlabEngine::Close()
{
  if( sNumInstances > 0 )
  {
    bcierr << "Cannot close Matlab engine while client instances are still present" << endl;
  }
  else if( spEngineRef )
  {
    ClearVariable( cErrorVariable );
    engClose( spEngineRef );
    spEngineRef = NULL;
  }
}

bool
MatlabEngine::Execute( const string& inCommands )
{
  return ( spEngineRef && ( 0 == engEvalString( spEngineRef, inCommands.c_str() ) ) );
}

bool
MatlabEngine::Execute( const string& inCommands, string& outError )
{
    outError.clear();
    string command = cErrorVariable + "=''; try " + inCommands + "; catch " + cErrorVariable + "=lasterr; end;";
    if( !MatlabEngine::Execute( command ) )
      outError = "Could not execute Matlab command:\n" + command;
    else
    {
      outError = MatlabEngine::GetString( cErrorVariable );
      MatlabEngine::PutString( cErrorVariable, "" );
    }
    return outError.empty();
}

bool
MatlabEngine::CreateGlobal( const string& inName )
{
  return Execute( "global " + inName + ";" );
}

bool
MatlabEngine::ClearVariable( const string& inName )
{
  return Execute( "clear " + inName + "; clear global " + inName + ";" );
}

bool
MatlabEngine::ClearObject( const string& inName )
{
  return Execute( "if( exist('" + inName + "', 'var') ); delete(" + inName + "); end;" )
    && ClearVariable( inName );
}

string
MatlabEngine::GetString( const string& inExp )
{
  string result;
  mxArray* ans = GetMxArray( inExp );
  if( ans )
  {
    const char* s = mxArrayToString( ans );
    if( s == NULL )
      bcierr << "Could not read \"" << inExp << "\" from Matlab workspace" << endl;
    else
      result = s;
    mxFree( ( void* )s );
    mxDestroyArray( ans );
  }
  return result;
}

bool
MatlabEngine::PutString( const string& inExp, const string& inValue )
{
  mxArray* val = mxCreateString( inValue.c_str() );
  bool success = PutMxArray( inExp, val );
  mxDestroyArray( val );
  return success;
}

double
MatlabEngine::GetScalar( const string& inExp )
{
  DoubleMatrix result = GetMatrix( inExp );
  if( result.empty() || result[ 0 ].empty() )
  {
    bcierr << "Could not get scalar value \"" << inExp << "\"" << endl;
    result.resize( 1, vector<double>( 1 ) );
  }
  return result[ 0 ][ 0 ];
}

bool
MatlabEngine::PutScalar( const string& inExp, double inValue )
{
  return PutMatrix( inExp, inValue );
}

MatlabEngine::DoubleMatrix
MatlabEngine::GetMatrix( const string& inExp )
{
  DoubleMatrix result;
  mxArray* ans = GetMxArray( inExp );
  if( ans )
  {
    int numDims = mxGetNumberOfDimensions( ans );
    const mwSize* dims = mxGetDimensions( ans );
    if( numDims != 2 )
      bcierr << "Can only handle two dimensions" << endl;
    result.resize( dims[ 0 ], vector<double>( dims[ 1 ] ) );
    double* value = mxGetPr( ans );
    if( value )
    {
      mwIndex indices[] = { 0, 0 };
      for( mwIndex i = 0; i < static_cast<mwIndex>( result.size() ); ++i )
      {
        indices[ 0 ] = i;
        for( mwIndex j = 0; j < static_cast<mwIndex>( result[ i ].size() ); ++j )
        {
          indices[ 1 ] = j;
          result[ i ][ j ] = value[ mxCalcSingleSubscript( ans, 2, indices ) ];
        }
      }
    }
    mxDestroyArray( ans );
  }
  return result;
}

bool
MatlabEngine::PutMatrix( const string& inExp, const DoubleMatrix& inValue )
{
  mwSize sizeDim2 = static_cast<mwSize>( inValue.empty() ? 0 : inValue[ 0 ].size() );
  mxArray* val = mxCreateNumericMatrix( static_cast<mwSize>( inValue.size() ), sizeDim2, mxDOUBLE_CLASS, mxREAL );
  double* data = mxGetPr( val );
  if( data )
  {
    mwIndex indices[] = { 0, 0 };
    for( mwIndex i = 0; i < static_cast<mwIndex>( inValue.size() ); ++i )
    {
      indices[ 0 ] = i;
      for( mwIndex j = 0; j < static_cast<mwIndex>( inValue[ i ].size() ); ++j )
      {
        indices[ 1 ] = j;
        data[ mxCalcSingleSubscript( val, 2, indices ) ] = inValue[ i ][ j ];
      }
    }
  }
  bool success = PutMxArray( inExp, val );
  mxDestroyArray( val );
  return success;
}

MatlabEngine::StringMatrix
MatlabEngine::GetCells( const string& inExp )
{
  StringMatrix result;
  mxArray* ans = GetMxArray( inExp );
  if( ans )
  {
    int numDims = mxGetNumberOfDimensions( ans );
    const mwSize* dims = mxGetDimensions( ans );
    if( numDims != 2 )
      bcierr << "Can only handle two dimensions" << endl;
    result.resize( dims[ 0 ], vector<string>( dims[ 1 ] ) );
    mwIndex indices[] = { 0, 0 };
    for( mwIndex i = 0; i < static_cast<mwIndex>( result.size() ); ++i )
    {
      indices[ 0 ] = i;
      for( mwIndex j = 0; j < static_cast<mwIndex>( result[ i ].size() ); ++j )
      {
        indices[ 1 ] = j;
        int idx = mxCalcSingleSubscript( ans, 2, indices );
        mxArray* cell = mxGetCell( ans, idx );
        if( cell )
        {
          const char* s = mxArrayToString( cell );
          if( s == NULL )
            bcierr << "Could not read string value \"" << inExp << "\"" << endl;
          else
          {
            result[ i ][ j ] = s;
            mxFree( ( void* )s );
          }
        }
      }
    }
    mxDestroyArray( ans );
  }
  return result;
}

bool
MatlabEngine::PutCells( const string& inExp, const StringMatrix& inValue )
{
  mwSize sizeDim2 = static_cast<mwSize>( inValue.empty() ? 0 : inValue[ 0 ].size() );
  mxArray* mat = mxCreateCellMatrix( static_cast<mwSize>( inValue.size() ), sizeDim2 );
  mwIndex indices[] = { 0, 0 };
  for( mwIndex i = 0; i < static_cast<mwIndex>( inValue.size() ); ++i )
  {
    indices[ 0 ] = i;
    for( mwIndex j = 0; j < static_cast<mwIndex>( inValue[ i ].size() ); ++j )
    {
      indices[ 1 ] = j;
      int idx = mxCalcSingleSubscript( mat, 2, indices );
      mxDestroyArray( mxGetCell( mat, idx ) );
      mxArray* val = mxCreateString( inValue[ i ][ j ].c_str() );
      mxSetCell( mat, idx, val );
    }
  }
  bool success = PutMxArray( inExp, mat );
  mxDestroyArray( mat );
  return success;
}

::mxArray*
MatlabEngine::GetMxArray( const string& inExp )
{
  mxArray* ans = NULL;
  if( Execute( cAnsVariable + "=" + inExp + ";" ) )
    ans = engGetVariable( spEngineRef, cAnsVariable.c_str() );
  if( !ans )
    bcierr << "Could not read \"" << inExp << "\" from Matlab workspace" << endl;
  ClearVariable( cAnsVariable );
  return ans;
}

bool
MatlabEngine::PutMxArray( const string& inExp, const mxArray* inArray )
{
  bool success = ( 0 == engPutVariable( spEngineRef, cAnsVariable.c_str(), inArray ) );
  if( success )
    success = Execute( inExp + "=" + cAnsVariable + ";" );
  if( !success )
    bcierr << "Could not put value into \"" << inExp << "\"" << endl;
  ClearVariable( cAnsVariable );
  return success;
}

////////////////////////////////////////////////////////////////////////////////
// MatlabFunction definitions                                                 //
////////////////////////////////////////////////////////////////////////////////
MatlabFunction::MatlabFunction( const string& inName )
: mName( inName ),
  mExists( false )
{
  if( spEngineRef )
  {
    // Check whether there exists a function with the required name in
    // the Matlab search path.
    string command = "rehash;";
    command += cAnsVariable + "=exist('" + inName + "');";
    if( MatlabEngine::Execute( command ) )
    {
      mxArray* ans = engGetVariable( spEngineRef, cAnsVariable.c_str() );
      if( ans )
      {
        int result = static_cast<int>( mxGetPr( ans )[ 0 ] );
        enum
        {
          mfile = 2,
          mexfile = 3,
          mbuiltin = 5,
          pfile = 6,
        };
        switch( result )
        {
          case mfile:
          case mexfile:
          case mbuiltin:
          case pfile:
            mExists = true;
            break;
          default:
            mExists = false;
        }
        mxDestroyArray( ans );
      }
      ClearVariable( cAnsVariable );
    }
  }
}

MatlabFunction::~MatlabFunction()
{
}

MatlabFunction&
MatlabFunction::InputArgument( const string& inArg )
{
  mInputArguments.push_back( inArg );
  return *this;
}

MatlabFunction&
MatlabFunction::OutputArgument( const string& inArg )
{
  mOutputArguments.push_back( inArg );
  return *this;
}

MatlabFunction&
MatlabFunction::CodePre( const string& inCode )
{
  mCodePre = inCode;
  return *this;
}

MatlabFunction&
MatlabFunction::CodePost( const string& inCode )
{
  mCodePost = inCode;
  return *this;
}

string
MatlabFunction::Execute() const
{
  string result;
  if( !mExists )
    result = "Function " + mName + " does not exist in the Matlab search path.";
  else
  {
    ostringstream command;
    command << mCodePre;
    if( !mOutputArguments.empty() )
    {
      command << "[" << mOutputArguments[ 0 ];
      for( size_t i = 1; i < mOutputArguments.size(); ++i )
        command << "," << mOutputArguments[ i ];
      command << "]=";
    }
    command << mName;
    if( !mInputArguments.empty() )
    {
      command << "(" << mInputArguments[ 0 ];
      for( size_t i = 1; i < mInputArguments.size(); ++i )
        command << "," << mInputArguments[ i ];
      command << ")\n";
    }
    command << mCodePost;
    MatlabEngine::Execute( command.str(), result );
  }
  return result;
}
