/////////////////////////////////////////////////////////////////////////////
//
// File: TextProperties.cpp
//
// Date: Jan 7, 2002
//
// Author: Juergen Mellinger
//
// Description: A mix-in class for views that handle text properties.
//
// Changes:
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

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "TextProperties.h"
#include "ParamList.h"
#include <list>
#include <string>
#include <algorithm>

#define TEXT_PROP_SEPARATOR "_"

using namespace std;

TTextProperties::TTextProperties()
{
    SetFontName( GUIDefaultFont );
    SetFontSize( GUIDefaultFontSize );
    SetFontStyle( defaultGUIFontStyle );
    SetFontColor( defaultGUIFGColor );
    SetAlignment( defaultGUIAlignment );
    SetVAlignment( defaultGUIVAlignment );
    SetBiDi( defaultGUIBiDi );
}

TPresError
TTextProperties::SetTextProperties( const Param *inParamPtr )
{
    TPresError  err;
    const char* valPtr;
    if( inParamPtr == NULL )
        return presParamInaccessibleError;
    valPtr = inParamPtr->Value().c_str();
    if( valPtr == NULL || *valPtr == '\0' )
        return presParamOutOfRangeError;
    if( ( err = TTextProperties::ParseTextProperties( valPtr ) ) != presNoError )
        return err;
    return presNoError;
}

TPresError
TTextProperties::ParseTextProperties( const char* inProperties )
{
    string          properties( inProperties );
    list<string>    propList;
    unsigned int    lastBegin = 0,
                    curBegin = properties.find( TEXT_PROP_SEPARATOR );
    while( curBegin != string::npos )
    {
        propList.push_back( properties.substr( lastBegin, curBegin - lastBegin ) );
        lastBegin = curBegin + 1;
        curBegin = properties.find( TEXT_PROP_SEPARATOR, lastBegin );
    }
    propList.push_back( properties.substr( lastBegin ) );

    if( *propList.begin() == "" )
        return presParamOutOfRangeError;

    for( list<string>::iterator i = propList.begin(); i != propList.end(); i++ )
    {
        bool    foundMatch = false;
        if( !foundMatch )
            // Is the list entry a biDi?
            for( int f = firstGUIBiDi; f < numGUIBiDis; f++ )
                if( *i == GUIBiDiStrings[ f ] )
                {
                    SetBiDi( TGUIBiDi( f ) );
                    foundMatch = true;
                    break;
                }
        if( !foundMatch )
            // Is the list entry an alignment?
            for( int f = firstGUIAlignment; f < numGUIAlignments; f++ )
                if( *i == GUIAlignmentStrings[ f ] )
                {
                    SetAlignment( TGUIAlignment( f ) );
                    foundMatch = true;
                    break;
                }
        if( !foundMatch )
            // Is the list entry a vAlignment?
            for( int f = firstGUIVAlignment; f < numGUIVAlignments; f++ )
                if( *i == GUIVAlignmentStrings[ f ] )
                {
                    SetVAlignment( TGUIVAlignment( f ) );
                    foundMatch = true;
                    break;
                }
        if( !foundMatch )
            // Is the list entry a style?
            for( int f = firstGUIFontStyle; f < numGUIFontStyles; f++ )
                if( *i == GUIFontStyleStrings[ f ] )
                {
                    SetFontStyle( TGUIFontStyle( f ) );
                    foundMatch = true;
                    break;
                }
        if( !foundMatch )
            // Is the list entry a color?
            for( int c = firstGUIColor; c < numGUIColors; c++ )
                if( *i == GUIColorStrings[ c ] )
                {
                    SetFontColor( TGUIColor( c ) );
                    foundMatch = true;
                    break;
                }
        if( !foundMatch )
        {
            // Is the list entry a point size?
            bool    isNumber = true;
            for( string::iterator s = i->begin(); s != i->end(); s++ )
            {
                if( ( *s < '0' || *s > '9' ) && *s != '.' )
                {
                    isNumber = false;
                    break;
                }
            }
            if( isNumber )
            {
                float   size = atof( i->c_str() );
                if( size <= 0.0 || size > 100.0 )
                    return presParamOutOfRangeError;
                SetFontSize( size / 100.0 );
                foundMatch = true;
            }
        }
        if( !foundMatch )
        {
            // If we didn't find a match until now, we either have a
            // font name or something illegal.
            // Font names must be first in the list, so we return an error
            // when this is not the case.
            if( i == propList.begin() )
            {
                // We can't have spaces in font names. Add a space before
                // each uppercase character in the name.
                if( i->length() > 1 )
                {
                    string  fontName;
                    fontName += *i->begin();
                    for( string::iterator j = i->begin() + 1; j != i->end(); ++j )
                    {
                        if( isupper( *j ) )
                            fontName += ' ';
                        fontName += *j;
                    }
                    SetFontName( fontName.c_str() );
                }
                else
                    SetFontName( i->c_str() );
            }
            else
                return presParamOutOfRangeError;
        }
    }
    return presNoError;
}

#pragma warn -8104
const TInputSequence&
TTextProperties::DisplayToInput( const TDisplaySequence& inDisplaySequence ) const
{
    static std::string stringBuffer;
    std::string& resultRef = stringBuffer;
    switch( biDi )
    {

      case bd: // to do: - bd should discriminate between numbers and letters
               //        - bd and rl should use context dependent character
               //          replacements from a language table.
      case rl:
        stringBuffer.resize( inDisplaySequence.size() );
        reverse_copy( inDisplaySequence.begin(), inDisplaySequence.end(), stringBuffer.begin() );
        break;
      case lr:
      default:
        resultRef = inDisplaySequence;
    }
    return resultRef;
}

const TDisplaySequence&
TTextProperties::InputToDisplay( const TInputSequence& inInputSequence ) const
{
    static std::string stringBuffer;
    std::string& resultRef = stringBuffer;
    switch( biDi )
    {

      case bd: // to do: - bd should discriminate between numbers and letters
               //        - bd and rl should use context dependent character
               //          replacements from a language table.
      case rl:
        stringBuffer.resize( inInputSequence.size() );
        reverse_copy( inInputSequence.begin(), inInputSequence.end(), stringBuffer.begin() );
        break;
      case lr:
      default:
        resultRef = inInputSequence;
    }
    return resultRef;
}

