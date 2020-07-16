/////////////////////////////////////////////////////////////////////////////
//
// File: ParamDecl.h
//
// Date: Oct 17, 2001
//
// Author: Juergen Mellinger
//
// Description: Use the macros in this file for declaring and defining
//      TTD/BCI2000 parameters in a centralized resource-like fashion.
//
//      The cpp file that _defines_ the _TParamDef globals
//      must #define DEFINE_PARAMS before including this file.
//
//      Each parameter declared has a global variable of type
//      _TParamDef associated whose name is the parameter's name
//      prefixed with '_param_'. This way the compiler will complain
//      about parameters with identical names (an error difficult
//      to track down otherwise).
//      By accessing a parameter's name via an element
//      of its associated _TParamDef variable
//      (i.e. _not_ by just converting a given name into a
//      string using '#') we make the compiler notify us of typos in
//      parameter names (another error difficult to track down
//      otherwise).
//
// Changes: Jan 31, 2004: Introduced function calls, moved definitions into
//      a new file ParamDecl.cpp.
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
//////////////////////////////////////////////////////////////////////////////

#ifndef PARAMDECLH
#define PARAMDECLH

// #includes for functions used in macros.
#include <cassert>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "PresErrors.h"

#ifndef DEFINE_PARAMS

# define PARAM_DECL( \
        Name,        \
        Type,        \
        Section,     \
        Definition,  \
        Comment,     \
        loBound,     \
        upBound      \
    )           \
    extern _TParamDef _param_##Name;

// This is for parameter names with a runtime suffix.
# define PARAM_DECL_SUFFIX  PARAM_DECL

#else // DEFINE_PARAMS

# define PARAM_DECL( \
        Name,        \
        Type,        \
        Section,     \
        Definition,  \
        Comment,     \
        loBound,     \
        upBound      \
    )                \
    _TParamDef _param_##Name(                   \
        #Name,                                  \
        STRING( Section ) " " #Type " "         \
        #Name "= " Definition " \/\/ " Comment, \
        ( loBound ),                            \
        ( upBound )                             \
    );

// This is for parameter names with a runtime suffix.
# define PARAM_DECL_SUFFIX(     \
        Name,       \
        Type,       \
        Section,    \
        Definition, \
        Comment,    \
        loBound,    \
        upBound     \
    )               \
    _TParamDef _param_##Name(                                       \
        #Name RUNTIME_SUFFIX,                                       \
        STRING( Section ) " " #Type " " #Name RUNTIME_SUFFIX "= "   \
        Definition " \/\/ " Comment,                                \
        ( loBound ),                                                \
        ( upBound )                                                 \
    );

#endif // DEFINE_PARAMS

// Use this with the 'Definition' parameter if the definition
// needs to be built at run time.
#define RUNTIME_ELEMENT "__RE__"
#define RUNTIME_SUFFIX  "__RS__"

// By accessing a parameter's name via an element
// of its associated _TParamDef variable
// (i.e. _not_ by just converting a given name into a
// string using '#'), we make the compiler notify us of typos in
// parameter names.
#define PARAM_GET_PTR( List, Name, Ptr )                \
    { Ptr = List->Exists( _param_##Name.name() ) ? &( *List )[ _param_##Name.name() ] : NULL; }

#define PARAM_GET_PTR_SUFFIX( List, Name, Suffix, Ptr )                             \
    { Ptr = _TParamDef::param_get_ptr_suffix( List, _param_##Name.name(), Suffix ); }

// The following macros may behave differently if there is an "active" flag
// available for parameters.
# define PARAM_ENABLE( List, Name )                { PARAM_LINE( Name ); } // Just check for existence.
# define PARAM_ENABLE_SUFFIX( List, Name, Suffix ) { PARAM_LINE( Name ); }
# define PARAM_ENABLE_RT( List, Name, RtElem )     { PARAM_LINE( Name ); }

#define PARAM_ENABLE_ALL        PARAM_ADD_ALL

#define PARAM_DISABLE( List, Name )                 { PARAM_LINE( Name ); } // just to check for existence
#define PARAM_DISABLE_SUFFIX( List, Name, Suffix )  { PARAM_LINE( Name ); }

# define ADD_PARAM_TO_LIST( List, Name, Len ) List->Add( Name )
# define PARAM_EXISTS( List, Name )           List->Exists( Name )

// Don't add the parameter if it's already there.
# define PARAM_ADD( List, Name )                                    \
    {                                                               \
        Param* _p = NULL;                                           \
        PARAM_GET_PTR( List, Name, _p );                            \
        if( _p == NULL )                                            \
          ADD_PARAM_TO_LIST( List,                                  \
                        _param_##Name.definitionLine(),             \
                        ::strlen( _param_##Name.definitionLine() )  \
                        );                                          \

#define PARAM_ADD_SUFFIX( List, Name, Suffix )                                          \
    {                                                                                   \
        std::string  _def = PARAM_LINE( Name ),                                         \
                     _name = PARAM_NAME( Name );                                        \
        std::ostringstream _suffix;                                                     \
        _suffix << Suffix;                                                              \
        unsigned int _replacePos = _def.find( RUNTIME_SUFFIX );                         \
        while( _replacePos != std::string::npos )                                       \
        {                                                                               \
            _def.replace( _replacePos, sizeof( RUNTIME_SUFFIX ) - 1, _suffix.str() );   \
            _replacePos = _def.find( RUNTIME_SUFFIX );                                  \
        }                                                                               \
        _replacePos = _name.find( RUNTIME_SUFFIX );                                     \
        if( _replacePos != std::string::npos )                                          \
            _name.replace( _replacePos, sizeof( RUNTIME_SUFFIX ) - 1, _suffix.str() );  \
        if( !PARAM_EXISTS( List, ( char* )_name.c_str() ) )                             \
             ADD_PARAM_TO_LIST( List, ( char* )_def.c_str(), _def.length() );           \
    }

#define PARAM_ADD_RT( List, Name, RuntimeElement )                                          \
    {                                                                                       \
        if( List->GetParamPtr( _param_##Name.name() ) == NULL )                             \
        {                                                                                   \
            std::string _defLine = PARAM_LINE( Name );                                      \
            unsigned int _replacePos = _defLine.find( RUNTIME_ELEMENT );                    \
            assert( _replacePos != std::string::npos );                                     \
            _defLine.replace( _replacePos, sizeof( RUNTIME_ELEMENT ) - 1, RuntimeElement ); \
            ADD_PARAM_TO_LIST( List, ( char* )_defLine.c_str(), _defLine.length() );        \
        }                                                                                   \
    }

#define PARAM_ADD_ALL( SectionPrefix )  \
    _TParamDef::param_add_all( Parameters, #SectionPrefix, MAX_NUM_TARGETS )

#define PARAM_DELETE( List, Name )              \
    List->DeleteParam( _param_##Name.name() )

#define PARAM_DELETE_SUFFIX( List, Name, Suffix )  \
    _TParamDef::param_delete_suffix( List, _param_##Name.name(), Suffix )

#define PARAM_NAME( Name )                      \
    ( _param_##Name.name() )

#define PARAM_NAME_SUFFIX( Name, Suffix )       \
    ( _TParamDef::param_name_suffix( _param_##Name.name(), Suffix ) )

#define PARAM_LINE( Name )                      \
    ( _param_##Name.definitionLine() )

#ifdef PARAM_NO_RANGECHECK
# pragma warn -8008 // condition always true
# pragma warn -8066 // unreachable code
# define PARAM_IN_RANGE( Name, Value )              (true)
#else
# define PARAM_IN_RANGE( Name, Value )                \
    ( ( _param_##Name.loBound() <= ( Value ) )        \
        && ( _param_##Name.upBound() >= ( Value ) ) )
#endif // PARAM_NO_RANGECHECK

#define PARAM_GET_TIME( List, Name, Variable )       \
        {                                            \
          float _temp;                               \
          PARAM_GET_NUM( List, Name, _temp );        \
          Variable = Util::Units::UnitTime( _temp ); \
        }

#define PARAM_GET_NUM( List, Name, Variable )                       \
        {                                                           \
            Param *_paramPtr;                                       \
            const char  *_valuePtr;                                 \
            PARAM_GET_PTR( List, Name, _paramPtr );                 \
            if( _paramPtr == NULL ||                                \
                ( ( _valuePtr = _paramPtr->Value().c_str() ) == NULL ) ||   \
                ( *_valuePtr == '\0' ) )                            \
            {                                                       \
                gPresErrors.AddError( presParamInaccessibleError,   \
                                            _param_##Name.name() ); \
                Variable = _param_##Name.loBound();                 \
            }                                                       \
            else                                                    \
            {                                                       \
                Variable = ::atof( _valuePtr );                     \
                if( !PARAM_IN_RANGE( Name, Variable ) )             \
                {                                                   \
                    gPresErrors.AddError( presParamOutOfRangeError, \
                                            _param_##Name.name() ); \
                    Variable = _param_##Name.loBound();             \
                }                                                   \
            }                                                       \
        }

#define PARAM_GET_NUM_BY_INDEX( List, Name, Index, Variable )       \
        {                                                           \
            Param *_paramPtr;                                       \
            const char  *_valuePtr;                                 \
            PARAM_GET_PTR( List, Name, _paramPtr );                 \
            if( _paramPtr == NULL ||                                \
                ( ( _valuePtr = _paramPtr->Value( Index ).c_str() ) == NULL ) || \
                ( *_valuePtr == '\0' ) )                            \
            {                                                       \
                gPresErrors.AddError( presParamInaccessibleError,   \
                                            _param_##Name.name() ); \
                Variable = _param_##Name.loBound();                 \
            }                                                       \
            else                                                    \
            {                                                       \
                Variable = ::atof( _valuePtr );                     \
                if( !PARAM_IN_RANGE( Name, Variable ) )             \
                {                                                   \
                    gPresErrors.AddError(presParamOutOfRangeError,  \
                                            _param_##Name.name() ); \
                    Variable = _param_##Name.loBound();             \
                }                                                   \
            }                                                       \
        }

#define PARAM_GET_TIME_BY_INDEX( List, Name, Index, Variable )       \
        {                                                            \
          float _temp;                                               \
          PARAM_GET_NUM_BY_INDEX( List, Name, Index, _temp );        \
          Variable = Util::Units::UnitTime( _temp );                 \
        }

#define PARAM_GET_NUM_SUFFIX( List, Name, Suffix, Variable )        \
        {                                                           \
            Param *_paramPtr;                                       \
            const char  *_valuePtr;                                 \
            PARAM_GET_PTR_SUFFIX( List, Name, Suffix, _paramPtr );  \
            if( _paramPtr != NULL )                                 \
              _valuePtr = _paramPtr->Value().c_str();               \
            else                                                    \
            {                                                       \
              PARAM_GET_PTR( List, Name, _paramPtr );               \
              if( _paramPtr != NULL )                               \
                _valuePtr = _paramPtr->Value( Suffix ).c_str();     \
            }                                                       \
            if( _paramPtr == NULL ||                                \
                _valuePtr == NULL || *_valuePtr == '\0' )           \
            {                                                       \
                gPresErrors.AddError( presParamInaccessibleError,   \
                                    _param_##Name.name(), Suffix ); \
                Variable = _param_##Name.loBound();                 \
            }                                                       \
            else                                                    \
            {                                                       \
                Variable = ::atof( _valuePtr );                     \
                if( !PARAM_IN_RANGE( Name, Variable ) )             \
                {                                                   \
                    gPresErrors.AddError( presParamOutOfRangeError, \
                                    _param_##Name.name(), Suffix ); \
                    Variable = _param_##Name.loBound();             \
                }                                                   \
            }                                                       \
        }

#define PARAM_GET_NUM_SUFFIX_BY_INDEX( List, Name, Suffix, Index, Variable ) \
        {                                                             \
            Param *_paramPtr;                                         \
            const char  *_valuePtr;                                   \
            PARAM_GET_PTR_SUFFIX( List, Name, Suffix, _paramPtr );    \
            if( _paramPtr != NULL )                                   \
              _valuePtr = _paramPtr->Value( Index ).c_str();          \
            else                                                      \
            {                                                         \
              PARAM_GET_PTR( List, Name, _paramPtr );                 \
              if( _paramPtr != NULL )                                 \
                _valuePtr = _paramPtr->Value( Suffix, Index ).c_str();\
            }                                                         \
            if( _paramPtr == NULL ||                                  \
                _valuePtr == NULL || *_valuePtr == '\0' )             \
            {                                                         \
                gPresErrors.AddError( presParamInaccessibleError,     \
                                    _param_##Name.name(), Suffix );   \
                Variable = _param_##Name.loBound();                   \
            }                                                         \
            else                                                      \
            {                                                         \
                Variable = ::atof( _valuePtr );                       \
                if( !PARAM_IN_RANGE( Name, Variable ) )               \
                {                                                     \
                    gPresErrors.AddError( presParamOutOfRangeError,   \
                                    _param_##Name.name(), Suffix );   \
                    Variable = _param_##Name.loBound();               \
                }                                                     \
            }                                                         \
        }

#define PARAM_GET_STRING( List, Name, Variable )                    \
        {                                                           \
            Param *_paramPtr;                                       \
            PARAM_GET_PTR( List, Name, _paramPtr );                 \
            if( _paramPtr == NULL )                                 \
            {                                                       \
                gPresErrors.AddError( presParamInaccessibleError,   \
                                            _param_##Name.name() ); \
                Variable = NULL;                                    \
            }                                                       \
        }

#define PARAM_GET_STRING_BY_INDEX( List, Name, Index, Variable )    \
        {                                                           \
            Param *_paramPtr;                                       \
            const char  *_valuePtr;                                 \
            PARAM_GET_PTR( List, Name, _paramPtr );                 \
            if( _paramPtr == NULL )                                 \
            {                                                       \
                gPresErrors.AddError( presParamInaccessibleError,   \
                                            _param_##Name.name() ); \
                Variable = NULL;                                    \
            }                                                       \
            else                                                    \
                Variable = _valuePtr;                               \
        }

#define PARAM_GET_STRING_SUFFIX( List, Name, Suffix, Variable )     \
        {                                                           \
            Param *_paramPtr;                                       \
            PARAM_GET_PTR_SUFFIX( List, Name, Suffix, _paramPtr );  \
            if( _paramPtr != NULL )                                 \
              Variable = _paramPtr->Value().c_str();                \
            else                                                    \
            {                                                       \
              PARAM_GET_PTR( List, Name, _paramPtr );               \
              if( _paramPtr != NULL )                               \
                Variable = _paramPtr->Value( Suffix ).c_str();      \
            }                                                       \
            if( _paramPtr == NULL ||                                \
                Variable == NULL || *Variable == '\0' )             \
            {                                                       \
                gPresErrors.AddError( presParamInaccessibleError,   \
                                    _param_##Name.name(), Suffix ); \
                Variable = NULL;                                    \
            }                                                       \
        }

#define PARAM_OPTIONAL_GET_NUM( List, Name, Default, Variable )     \
        {                                                           \
          Param* _paramPtr;                                         \
          PARAM_GET_PTR( List, Name, _paramPtr );                   \
          if( _paramPtr != NULL )                                   \
            PARAM_GET_NUM( List, Name, Variable );                  \
          else                                                      \
            Variable = Default;                                     \
        }

#define STRING( X )         #X


class _TParamDef
{
 public:
  _TParamDef( const char* inName,
              char*       inDefinitionLine,
              float       inLoBound,
              float       inUpBound )
  : _name( inName ),
    _definitionLine( inDefinitionLine ),
    _loBound( inLoBound ),
    _upBound( inUpBound )
  {
    beautify_name();
    params().push_back( this );
  }
  const char* name() const           { return _name; }
  const char* definitionLine() const { return _definitionLine; }
  float       loBound() const        { return _loBound; }
  float       upBound() const        { return _upBound; }

  static const char* param_name_suffix( const char* inName, long inSuffix );
  static class Param* param_get_ptr_suffix( class ParamList* inList, const char* inName, long inSuffix );
  static void param_delete_suffix( class ParamList* inList, const char* inName, long inSuffix );
  static void param_add_all( ParamList* inList, const char* inSectionPrefix, long inDefaultDimension );

 private:
  void        beautify_name();
  const char* _name;
  char*       _definitionLine;
  float       _loBound;
  float       _upBound;
  static std::list<_TParamDef*>& params();
};


#endif // PARAMDECLH
