////////////////////////////////////////////////////////////////////////////////
// $Id: Speller.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A speller provides an Enter() method, and keeps a collection of
//   speller targets.
//   When a speller target is selected, it calls the speller's Enter() method
//   with the value of its EntryText property.
//   The speller's NextTarget() method returns the target best suited to modify
//   its SpelledText() property into its GoalText() property.
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

#include "Speller.h"
#include <map>

using namespace std;

// Speller members
Speller::Speller()
{
}

Speller::~Speller()
{
}

// Properties
Speller&
Speller::Add( SpellerTarget* s )
{
  mTargets.insert( s );
  return *this;
}

Speller&
Speller::Remove( SpellerTarget* s )
{
  mTargets.erase( s );
  return *this;
}

Speller&
Speller::DeleteObjects()
{
  for( SetOfSpellerTargets::iterator i = mTargets.begin(); i != mTargets.end(); ++i )
    delete *i;
  mTargets.clear();
  return *this;
}

const SetOfSpellerTargets&
Speller::Targets() const
{
  return mTargets;
}

// Target suggestion
SpellerTarget*
Speller::SuggestTarget( const string& inFrom, const string& inTo ) const
{
  // This default method uses the number of characters to delete, plus the number
  // of characters to enter, as a distance metric between strings to determine
  // the optimum target.
  struct
  {
    int operator()( const string& from, const string& to )
    {
      int result = static_cast<int>( from.length() + to.length() );
      string::const_iterator p1 = from.begin(),
                             p2 = to.begin();
      while( p1 != from.end() && p2 != to.end() && *p1++ == *p2++ )
        --result;
      return result;
    };
  } Distance;

  map<int, SpellerTarget*> sortedTargets;
  for( SetOfSpellerTargets::const_iterator i = mTargets.begin(); i != mTargets.end(); ++i )
    sortedTargets[ Distance( inFrom + ( *i )->EntryText(), inTo ) ] = *i;
  return sortedTargets.empty() ? NULL : sortedTargets.begin()->second;
}

bool
Speller::TrySpelling( const string& inText, SequenceOfSpellerTargets* pOutSequence )
{ 
  // Check whether it is possible to spell a given text, 
  // using targets available in the speller.
  if( pOutSequence )
    pOutSequence->clear();

  if( inText.empty() )
    return true;

  string remainingText = inText;
  SpellerTarget* pSuggestedTarget = NULL;
  do
  { pSuggestedTarget = SuggestTarget( "", remainingText );
    if( pSuggestedTarget != NULL )
    {
      const string& entryText = pSuggestedTarget->EntryText();
      if( remainingText.find( entryText ) != 0 )
        pSuggestedTarget = NULL;
      else
        remainingText = remainingText.substr( entryText.length() );
    }
    if( pOutSequence )
      pOutSequence->push_back( pSuggestedTarget );
  } while( pSuggestedTarget != NULL && remainingText.length() > 0 );

  return ( pSuggestedTarget != NULL );
}


// Event triggering
Speller&
Speller::Enter( const std::string& s )
{
  OnEnter( s );
  return *this;
}

// SpellerTarget members
SpellerTarget::SpellerTarget( Speller& s )
: mSpeller( s )
{
  mSpeller.Add( this );
}

SpellerTarget::~SpellerTarget()
{
}

// Properties
SpellerTarget&
SpellerTarget::SetEntryText( const std::string& s )
{
  mEntryText = s;
  return *this;
}

const std::string&
SpellerTarget::EntryText() const
{
  return mEntryText;
}

void SpellerTarget::OnSelect()
{
  mSpeller.Enter( mEntryText );
}

