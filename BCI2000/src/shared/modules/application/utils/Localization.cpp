////////////////////////////////////////////////////////////////////////////////
// $Id: Localization.cpp 4349 2013-01-10 19:33:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An Environment Extension class that provides facilities for
//         localizing (translating) application modules.
//         This class is instantiated by itself at initialization time.
//         There is at most one instance of this class.
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
#include "PCHIncludes.h"
#pragma hdrstop

#ifdef __BORLANDC__
#define UI_VCL
#else //__BORLANDC__
#define UI_QT
#endif // __BORLANDC__

#include "Localization.h"
#include "LabelIndex.h"
#include "BCIStream.h"

#ifdef UI_VCL
# include <vcl.h>
# include <typinfo.hpp>
#endif // UI_VCL

#ifdef UI_QT
# include <QObject>
# include <QVariant>
#endif

// Name of the parameter that holds the localized strings.
#define STRINGS_PARAM    "LocalizedStrings"
#define LANG_PARAM       "Language"
#define SECTION          "Application:Localization"
#define DEFAULT_LANGUAGE "Default" // A language ID that means "don't localize".

using namespace std;

Localization Localization::sInstance;

#ifdef __BORLANDC__
# pragma warn -8104 // No warning about local statics.
#endif // __BORLANDC__

Localization::GUIObjectStringsContainer&
Localization::GUIObjectStrings()
{
  static Localization::GUIObjectStringsContainer GUIObjectStrings_;
  return GUIObjectStrings_;
}

Localization::LocalizedStringsContainer&
Localization::LocalizedStrings()
{
  static Localization::LocalizedStringsContainer localizedStrings;
  return localizedStrings;
}

void
Localization::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    SECTION " string " LANG_PARAM "= " DEFAULT_LANGUAGE " " DEFAULT_LANGUAGE " % % //"
      " Language for user messages",
    SECTION " matrix " STRINGS_PARAM "= 0 0 % % % //"
      " Localized user messages",
  END_PARAMETER_DEFINITIONS
}

void
Localization::Preflight() const
{
  // The localizer depends only on the parameters defined inside its
  // constructor. There are no range limitations, and a missing language
  // entry does not constitute an error.
  // However, we emit a warning if the user requires a language that is
  // not present in the LocalizedStrings parameter.
  string userLanguage = OptionalParameter( LANG_PARAM );
  if( userLanguage != "" && userLanguage != DEFAULT_LANGUAGE )
  {
    const LabelIndex& labels = Parameter( STRINGS_PARAM )->RowLabels();
    size_t numLanguages = Parameter( STRINGS_PARAM )->NumRows();
    bool foundLanguage = false;
    for( size_t i = 0; i < numLanguages && !foundLanguage; ++i )
      foundLanguage = ( userLanguage == labels[ i ] );
    if( !foundLanguage )
      bciwarn << "Language requested in the \"" LANG_PARAM "\" parameter is not "
              << "present in the \"" STRINGS_PARAM "\" parameter."
              << endl;
  }
}

void
Localization::Initialize()
{
  LocalizedStrings().clear();
  string userLanguage = OptionalParameter( LANG_PARAM );
  if( userLanguage != ""
      && userLanguage != DEFAULT_LANGUAGE
      && Parameter( STRINGS_PARAM )->RowLabels().Exists( userLanguage ) )
  {
    size_t numStrings = Parameter( STRINGS_PARAM )->NumColumns();
    const LabelIndex& labels = Parameter( STRINGS_PARAM )->ColumnLabels();
    int languageIndex = Parameter( STRINGS_PARAM )->RowLabels()[ userLanguage ];
    for( size_t i = 0; i < numStrings; ++i )
      LocalizedStrings()[ labels[ i ] ] = ( string )Parameter( STRINGS_PARAM )( languageIndex, i );
  }
}

void
Localization::AddLocalizations( const char** inLanguages, int inNumLanguages,
                                const char** inStrings,   int inNumStrings )
{
  sInstance.AddLocalizations_( inLanguages, inNumLanguages, inStrings, inNumStrings );
}

void
Localization::AddLocalizations_( const char** inLanguages, int inNumLanguages,
                                 const char** inStrings,   int inNumStrings )
{
  int numLocalizationEntries = inNumStrings / ( inNumLanguages + 1 );
  Param& p = *( Parameter( STRINGS_PARAM ).operator->() );
  if( p.NumValues() == 0 && inNumStrings > 0 )
  {
    p.SetDimensions( 0, 1 );
    p.ColumnLabels()[ 0 ] = inStrings[ 0 ];
  }
  if( p.NumRows() == 0 && inNumLanguages > 0 )
  {
    p.SetDimensions( 1, 1 );
    p.RowLabels()[ 0 ] = inLanguages[ 0 ];
    p.Value() = "";
  }
  for( int i = 0; i < numLocalizationEntries; ++i )
  {
    size_t dim1 = p.NumRows();
    const char* originalText = inStrings[ i * ( inNumLanguages + 1 ) ];
    if( !p.ColumnLabels().Exists( originalText ) )
    {
      size_t dim2 = p.NumColumns();
      p.SetDimensions( dim1, dim2 + 1 );
      p.ColumnLabels()[ dim2 ] = originalText;
      for( size_t k = 0; k < dim1; ++k )
        p.Value( k, dim2 ) = "";
    }
  }
  for( int j = 0; j < inNumLanguages; ++j )
  {
    size_t dim2 = p.NumColumns();
    const char* languageName = inLanguages[ j ];
    if( !p.RowLabels().Exists( languageName ) )
    {
      size_t dim1 = p.NumRows();
      p.SetDimensions( dim1 + 1, dim2 );
      p.RowLabels()[ dim1 ] = languageName;
      for( size_t k = 0; k < dim2; ++k )
        p.Value( dim1, k ) = "";
    }
  }
  for( int i = 0; i < numLocalizationEntries; ++i )
    for( int j = 0; j < inNumLanguages; ++j )
    {
      const char* pLang = inLanguages[j],
                * pOrig = inStrings[i * ( inNumLanguages + 1 )],
                * pTrans = inStrings[i * ( inNumLanguages + 1 ) + j + 1];
      bcidbg << pOrig << " in " << pLang << ": " << pTrans;
      Parameter( STRINGS_PARAM )( pLang, pOrig ) = pTrans;
    }
}

const char*
Localization::LocalizableString( const char* inString )
{
  const char* result = inString;
  if( inString && *inString != '\0' )
  {
    const string& localization = LocalizedStrings()[ inString ];
    if( !localization.empty() )
      result = localization.c_str();
  }
  return result;
}

void
Localization::ApplyLocalizations( void* inObject )
{
#ifdef UI_VCL
  TObject* vclObject = reinterpret_cast<TObject*>( inObject );
  if( dynamic_cast<TObject*>( vclObject ) )
  {
    const char* localizableProperties[] =
    {
      "Caption",
      "Text",
    };
    const int numLocalizableProperties
              = sizeof( localizableProperties ) / sizeof( *localizableProperties );

    // We use the VCL Typinfo routines to check whether a given object has one
    // of the properties we consider localizable.
    if( GUIObjectStrings()[ inObject ].size() < numLocalizableProperties )
    {
      GUIObjectStrings()[ inObject ].resize( numLocalizableProperties );
      for( int i = 0; i < numLocalizableProperties; ++i )
        if( Typinfo::IsPublishedProp( vclObject, localizableProperties[ i ] ) )
          GUIObjectStrings()[ inObject ][ i ] = AnsiString( Typinfo::GetStrProp( vclObject,
                                                            localizableProperties[ i ] ) ).c_str();
    }
    for( int i = 0; i < numLocalizableProperties; ++i )
      if( Typinfo::IsPublishedProp( vclObject, localizableProperties[ i ] ) )
        Typinfo::SetStrProp( vclObject, localizableProperties[ i ],
                   LocalizableString( GUIObjectStrings()[ inObject ][ i ].c_str() ) );

    // If the object is a TWinControl, it has sub-controls.
    TWinControl* vclWinControl = dynamic_cast<TWinControl*>( vclObject );
    if( vclWinControl )
      for( int i = 0; i < vclWinControl->ControlCount; ++i )
        ApplyLocalizations( vclWinControl->Controls[ i ] );
  }
#endif // UI_VCL

#ifdef UI_QT
  QObject* obj = reinterpret_cast<QObject*>( inObject );
  if( dynamic_cast<QObject*>( obj ) )
  {
    // Make a list of all properties which may contain localizations
    const char* localizableProperties[] =
    {
      "text",
      "title",
      "accessableDescription",
      "accessableName",
      "statusTip",
      "toolTip",
      "windowIconText",
      "windowTitle"
    };
    const int numLocalizableProperties
              = sizeof( localizableProperties ) / sizeof( *localizableProperties );

    // Change all localizable properties to their local variants
    for( int i = 0; i < numLocalizableProperties; i++ )
    {
      if( obj->property( localizableProperties[i] ).isValid() )
      {
        QVariant val( QString::fromLocal8Bit( LocalizableString( obj->property( localizableProperties[i] ).toString().toLocal8Bit() ) ) );
        obj->setProperty( localizableProperties[i], val );
      }
    }

    // Apply localizations to all children
    QObjectList childList = obj->children();
    for( int i = 0; i < childList.size(); i++ )
      ApplyLocalizations( ( void* )childList[i] );

  }
#endif // UI_QT
}





