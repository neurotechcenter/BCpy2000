////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A general purpose tree class.
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
#include "Tree.h"
#include "Brackets.h"
#include "EncodedString.h"

static Tree Nulltree;

Tree&
Tree::Child( int i )
{
  if( i >= mChildren.size() )
    mChildren.resize( i + 1 );
  return mChildren[i];
}

const Tree&
Tree::Child( int i ) const
{
  if( i >= mChildren.size() )
    return Nulltree;
  return mChildren[i];
}

bool
Tree::Parse( std::string s )
{
  std::istringstream iss(s);
  return !!ExtractFrom( iss );
}

std::string
Tree::ToString() const
{
  std::ostringstream oss;
  InsertInto( oss );
  return oss.str();
}

std::istream&
Tree::ExtractFrom( std::istream& is )
{
  mChildren.clear();
  is >> std::ws;
  char closingBracket = '\0';
  if( Brackets::IsOpening( is.peek() ) )
  {
    closingBracket = Brackets::ClosingMatch( is.get() );
    while( is && ( is >> std::ws ).peek() != closingBracket )
    {
      mChildren.push_back( Tree() );
      mChildren.back().ExtractFrom( is );
    }
    is.get();
  }
  else
  {
    EncodedString s;
    if( s.ExtractFrom( is ) )
      mData = s;
  }
  return is;
}

std::ostream&
Tree::InsertInto( std::ostream& os ) const
{
  if( mChildren.empty() )
    return os << EncodedString( mData );
  os << Brackets::OpeningDefault << " ";
  for( const auto& child : mChildren )
    child.InsertInto( os ) << " ";
  return os << Brackets::ClosingDefault;
}
