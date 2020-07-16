////////////////////////////////////////////////////////////////////////////////
// $Id: Localization.h 4327 2012-11-30 17:02:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An Environment Extension class that provides facilities for
//         localizing (translating) application modules.
//         This class is instantiated by itself at initialization time.
//         There is at most one instance of this class.
//
//         To add translations to an existing application module, take care of the
//         following:
//         - There are two parameters that govern the behavior of the
//           Localization class:
//           "Language" defines the language to translate the strings
//           into; if its value matches one of the "LocalizedStrings" row
//           labels, translations will be taken from that row; otherwise,
//           strings will not be translated.
//           "LocalizedStrings" defines string translations. Strings that
//           don't appear as a column label will not be translated.
//           Also, strings with an empty translation entry in "LocalizedStrings"
//           will not be translated.
//         - To provide your translations in a filter constructor, add a line
//             LANGUAGES "Italian", "French",
//           to enumerate the languages you provide translations for,
//           followed by
//             BEGIN_LOCALIZED_STRINGS
//           and a list of translated strings where each entry has the form
//             "Original string", "Italian translation", "French translation",
//           and finally a line
//             END_LOCALIZED_STRINGS
//           to end the enumeration.
//           Even if you don't provide translations this way, operator users may
//           still add their own translations for any language by editing
//           the LocalizedStrings parameter.
//         - Mark localizable string constants by the symbol
//           LocalizableString, i.e.
//             TellUser( LocalizableString( "Well done!" ) );
//           instead of
//             TellUser( "Well done!" );
//         - For GUI elements (e.g. VCL forms) visible to the subject, put a call
//             ApplyLocalizations( pointerToTheGUIElement );
//           inside the Initialize() method of the form's associated
//           GenericFilter.
//         - You should not use LocalizableString on string constants used
//           before the first call to GenericFilter::Initialize() or for
//           initializing static objects of any kind because localization
//           information used will not be valid, or the string may not be
//           updated appropriately then.
//         - Language names are case-insensitive. You may use any legal C
//           identifier for a language name but as a convention we suggest
//           its most common English name, as in
//             Italian, Dutch, French, German
//           with international country abbreviations as optional regional
//           qualifiers as in
//             EnglishUS, EnglishGB, GermanD, GermanCH
//           if that should ever be necessary.
//         - For now, encoding of non-ASCII characters follows the UTF8
//           convention. To ensure platform independent readability of source
//           code files, please use the macros that define HTML character names
//           to their UTF8 encoded string as in
//             "Dansk Sm" oslash "rrebr" oslash "d".
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
#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <map>
#include <vector>
#include <string>

#include "Environment.h"

// Some non-ASCII characters with their HTML names defined to their ISO Latin-1
// encodings.
#define Agrave "\xc0"
#define Aacute "\xc1"
#define Acirc  "\xc2"
#define Atilde "\xc3"
#define Auml   "\xc4"
#define Aring  "\xc5"
#define AElig  "\xc6"
#define Ccedil "\xc7"
#define Egrave "\xc8"
#define Eacute "\xc9"
#define Ecirc  "\xca"
#define Euml   "\xcb"
#define Igrave "\xcc"
#define Iacute "\xcd"
#define Icirc  "\xce"
#define Iuml   "\xcf"
#define ETH    "\xd0"
#define Ntilde "\xd1"
#define Ograve "\xd2"
#define Oacute "\xd3"
#define Ocirc  "\xd4"
#define Otilde "\xd5"
#define Ouml   "\xd6"

#define Oslash "\xd8"
#define Ugrave "\xd9"
#define Uacute "\xda"
#define Ucirc  "\xdb"
#define Uuml   "\xdc"
#define Yacute "\xdd"
#define THORN  "\xde"
#define szlig  "\xdf"
#define agrave "\xe0"
#define aacute "\xe1"
#define acirc  "\xe2"
#define atilde "\xe3"
#define auml   "\xe4"
#define aring  "\xe5"
#define aelig  "\xe6"
#define ccedil "\xe7"
#define egrave "\xe8"
#define eacute "\xe9"
#define ecirc  "\xea"
#define euml   "\xeb"
#define igrave "\xec"
#define iacute "\xed"
#define icirc  "\xee"
#define iuml   "\xef"
#define eth    "\xf0"
#define ntilde "\xf1"
#define ograve "\xf2"
#define oacute "\xf3"
#define ocirc  "\xf4"
#define otilde "\xf5"
#define ouml   "\xf6"

#define oslash "\xf8"
#define ugrave "\xf9"
#define uacute "\xfa"
#define ucirc  "\xfb"
#define uuml   "\xfc"
#define yacute "\xfd"
#define thorn  "\xfe"
#define yuml   "\xff"

// Macros that allow for convenient addition of translations.
#define LANGUAGES \
        { const char* languages_[] = {
#define BEGIN_LOCALIZED_STRINGS \
        }; const char* localizations_[] = {
#define END_LOCALIZED_STRINGS \
        }; Localization::AddLocalizations(                                     \
             languages_,                                                       \
             sizeof( languages_ ) / sizeof( *languages_ ),                     \
             localizations_,                                                   \
             sizeof( localizations_ ) / sizeof( *localizations_ )              \
        );                                                                     \
        }

class Localization: public EnvironmentExtension
{
  private: // This class instantiates itself once.
    Localization()          {}
    virtual ~Localization() {}

  // The EnvironmentExtension interface called from EnvironmentBase.
  protected:
    virtual void Publish();
    virtual void Preflight() const;
    virtual void Initialize();
    virtual void Process() {}

  // The public interface for filter programmers.
  public:
    static const char* LocalizableString( const char* );
    static void        ApplyLocalizations( void* );
    static void        AddLocalizations( const char**, int, const char**, int );

  private:
    static Localization sInstance;
    void AddLocalizations_( const char**, int, const char**, int );
    typedef std::map<void*,std::vector<std::string> > GUIObjectStringsContainer;
    static GUIObjectStringsContainer& GUIObjectStrings();
    typedef std::map<std::string, std::string> LocalizedStringsContainer;
    static LocalizedStringsContainer& LocalizedStrings();
};

// The Borland compiler appears to have difficulties with the using directive.
// The inline function definitions should have the same effect.
//#ifndef __BORLANDC__
//using Localization::LocalizableString;
//using Localization::ApplyLocalizations;
//#else
inline
const char* LocalizableString( const char* s )
{ return Localization::LocalizableString( s ); }

inline
void ApplyLocalizations( void* p )
{ Localization::ApplyLocalizations( p ); }
//#endif // Borland workaround

#endif // LOCALIZATION_H
