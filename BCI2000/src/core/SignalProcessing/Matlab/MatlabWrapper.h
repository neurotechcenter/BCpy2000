////////////////////////////////////////////////////////////////////////////////
// $Id: MatlabWrapper.h 4570 2013-09-06 16:49:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
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
#ifndef MatlabWrapperH
#define MatlabWrapperH

#include <string>
#include <vector>
#include "GenericSignal.h"
#include "Param.h"

// BCB2009 and MSVC10 define char16_t but not __STDC_UTF_16__.
// This leads to problems with recent Matlab headers.
#if ( __BORLANDC__ > 0x0600 ) || ( _MSC_VER >= 1600 )
# ifndef __STDC_UTF_16__
#  define __STDC_UTF_16__ true
# endif // __STDC_UTF_16__
#endif // ( __BORLANDC__ > 0x0600 ) || ( _MSC_VER >= 1600 )

#ifdef _WIN32
# define MX_COMPAT_32 1
#endif // _WIN32
#include "engine.h"

class MatlabEngine
{
 public:
  class DoubleMatrix : public std::vector<std::vector<double> >
  {
   public:
    DoubleMatrix() {}
    DoubleMatrix( double d )
    : std::vector<std::vector<double> >( 1, std::vector<double>( 1, d ) ) {}

    DoubleMatrix( const GenericSignal& );
    DoubleMatrix( const SignalProperties& );
    operator GenericSignal() const;
    operator SignalProperties() const;
  };

  class StringMatrix : public std::vector<std::vector<std::string> >
  {
   public:
    StringMatrix() {}
    StringMatrix( const std::string& s )
    : std::vector<std::vector<std::string> >( 1, std::vector<std::string>( 1, s ) ) {}

    StringMatrix( const Param& );
    operator Param() const;
  };

 protected:
  MatlabEngine();
  virtual ~MatlabEngine();

 public:
  static void         Open();
  static bool         IsOpen() { return spEngineRef != NULL; }
  static void         Close();

  static bool         Execute( const std::string& commands );
  static bool         Execute( const std::string& commands, std::string& error );

  static bool         CreateGlobal( const std::string& name );
  static bool         ClearVariable( const std::string& name );
  static bool         ClearObject( const std::string& name );

  static std::string  GetString( const std::string& expr );
  static bool         PutString( const std::string& expr, const std::string& value );

  static double       GetScalar( const std::string& expr );
  static bool         PutScalar( const std::string& expr, double value );

  static DoubleMatrix GetMatrix( const std::string& expr );
  static bool         PutMatrix( const std::string& expr, const DoubleMatrix& value );


  static StringMatrix GetCells(  const std::string& expr );
  static bool         PutCells(  const std::string& expr, const StringMatrix& value );

 private:
  static mxArray* GetMxArray( const std::string& expr );
  static bool     PutMxArray( const std::string& expr, const mxArray* value );

 protected:
  static Engine* spEngineRef;
  static int sNumInstances;
};

class MatlabFunction : private MatlabEngine
{
 public:
  MatlabFunction( const std::string& );
  ~MatlabFunction();

 private:
  MatlabFunction( const MatlabFunction& );
  MatlabFunction& operator=( const MatlabFunction& );

 public:
  MatlabFunction& InputArgument( const std::string& );
  MatlabFunction& OutputArgument( const std::string& );
  MatlabFunction& CodePre( const std::string& );
  MatlabFunction& CodePost( const std::string& );
  const std::string& Name() const { return mName; }
  bool Exists() const             { return mExists; }
  std::string Execute() const;

 private:
  std::vector<std::string> mInputArguments,
                           mOutputArguments;
  std::string mName, mCodePre, mCodePost;
  bool mExists;
};

#endif // MatlabWrapperH
