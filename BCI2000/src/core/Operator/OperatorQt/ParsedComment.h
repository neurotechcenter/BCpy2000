//////////////////////////////////////////////////////////////////////
// $Id: ParsedComment.h 4874 2015-06-26 12:49:58Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that handles parsing a parameter's comment
//       for display purposes.
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
///////////////////////////////////////////////////////////////////////
#ifndef PARSED_COMMENT_H
#define PARSED_COMMENT_H

#include <string>
#include <vector>

class Param;

class ParsedComment
{
  // Type declarations.
  public:
    // Possible interpretation results.
    typedef enum
    {
      unknown = 0,

      shapePos = 0,
      shapeBit = 1 << shapePos,
      singleEntry = 1 << (shapePos + 1) | shapeBit,
      list = 2 << (shapePos + 1) | shapeBit,
      matrix = 3 << (shapePos + 1) | shapeBit,
      shapeMask = singleEntry | list | matrix,

      typePos = 3,
      typeBit = 1 << typePos,
      enumeration = 1 << (typePos + 1) | typeBit, // Possible parameter values are from a pre-defined set.
      boolean = 2 << (typePos + 1) | typeBit,   // The parameter represents an on/off switch.
      inputfile = 3 << (typePos + 1) | typeBit, // Parameter values are paths to files.
      outputfile = 4 << (typePos + 1) | typeBit,
      directory = 5 << (typePos + 1) | typeBit, // Parameter values are directory paths.
      color = 6 << (typePos + 1) | typeBit,     // Parameter values are RGB colors in hex string notation.
      blob = 7 <<  (typePos + 1) | typeBit,      // Open parameter value in external editor.
      typeMask = enumeration | boolean | inputfile | outputfile | directory | color | blob,

      accessPos = 8,
      accessBit = 1 << accessPos,
      noedit = 1 << (accessPos + 1 ) | accessBit, // Not user-configurable, but may be written by AutoConfig.
      readonly = 2 << (accessPos + 1 ), // | accessBit, // May not be modified.
      accessMask = noedit /*| readonly*/, // ignore readonly flag while it's broken

    } Kind_type;
  private:
    typedef std::vector<std::string> Values_type;

  // The public interface.
  public:
    ParsedComment( const Param& );
    // The parameter name.
    const std::string& Name() const        { return mName; }
    // A parameter's help context, typically derived from its subsection.
    const std::string& HelpContext() const { return mHelpContext; }
    // The parameter comment after interpretation.
    const std::string& Comment() const     { return mComment; }

    int                Kind() const        { return mKind; }
    const Values_type& Values() const      { return mValues; }

    // This is only relevant for the singleValuedEnum type and represents
    // the numerical parameter value of the first enumeration entry.
    int                IndexBase() const   { return mIndexBase; }

  // Private members.
  private:
    bool ExtractEnumValues( const Param& p );
    bool IsBooleanEnum() const;

    std::string mName,
                mHelpContext,
                mComment;
    int         mKind;
    Values_type mValues;
    int         mIndexBase;
};

#endif // PARSED_COMMENT_H
