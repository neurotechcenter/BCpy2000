////////////////////////////////////////////////////////////////////////////////
// $Id: Target.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual base class defining a calling interface for "targets".
//   A target is anything that can be selected by the user.
//   Descendants of Target implement actions in their OnSelect event handler.
//   A speller target, for example, will enter its entry text in its OnSelect
//   method.
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
#ifndef TARGET_H
#define TARGET_H

#include <set>

class Target
{
 public:
  Target()
    : mTag( 0 )
    {}
  virtual ~Target()
    {}
  // Properties
  Target& SetTag( int inTag )
    { mTag = inTag; return *this; }
  int Tag() const
    { return mTag; }
  // Event calling
  Target& Select()
    { OnSelect(); return *this; }

 protected:
  virtual void OnSelect()
    {}

 private:
  int mTag;
};

class SetOfTargets : public std::set<Target*>
{
 public:
  SetOfTargets()
    {}
  virtual ~SetOfTargets()
    {}
  // Householding
  SetOfTargets& Add( Target* t )
    { insert( t ); return *this; }
  SetOfTargets& Remove( Target* t )
    { erase( t ); return *this; }
  SetOfTargets& Clear()
    { clear(); return *this; }
  SetOfTargets& DeleteObjects()
    { for( iterator i = begin(); i != end(); ++i ) delete *i; clear(); return *this; }

  bool Contains( Target* t ) const
    { return find( t ) != end(); }
  bool Intersects( const SetOfTargets& s ) const
    {
      for( const_iterator i = begin(); i != end(); ++i )
        if( s.Contains( *i ) ) return true;
      return false;
    }

  // Events
  void Select()
    { for( iterator i = begin(); i != end(); ++i ) ( *i )->Select(); }
};


#endif // TARGET_H

