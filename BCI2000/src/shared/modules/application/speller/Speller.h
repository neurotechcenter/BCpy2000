////////////////////////////////////////////////////////////////////////////////
// $Id: Speller.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A speller provides an Enter() method, and keeps a collection of
//   speller targets.
//   When a speller target is selected, it calls the speller's Enter() method
//   with the value of its EntryText property.
//   The speller's SuggestTarget() method returns the target best suited to modify
//   its first argument string into its second argument string.
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
#ifndef SPELLER_H
#define SPELLER_H

#include "Target.h"
#include <string>
#include <set>
#include <vector>

class SpellerTarget;
typedef std::set<SpellerTarget*> SetOfSpellerTargets;
typedef std::vector<SpellerTarget*> SequenceOfSpellerTargets;

class Speller
{
 public:
  Speller();
  virtual ~Speller();
  // Properties
  Speller& Add( SpellerTarget* );
  Speller& Remove( SpellerTarget* );
  Speller& DeleteObjects();
  const SetOfSpellerTargets& Targets() const;

  // Target suggestion
  virtual SpellerTarget* SuggestTarget( const std::string& from,
                                        const std::string& to ) const;
  bool TrySpelling( const std::string& inText, 
                    SequenceOfSpellerTargets* pOutSequence = 0 );
  // Event triggering
  Speller& Enter( const std::string& );

 protected:
  virtual void OnEnter( const std::string& ) = 0;

 private:
  SetOfSpellerTargets mTargets;
};


class SpellerTarget : public Target
{
 public:
  SpellerTarget( Speller& );
  virtual ~SpellerTarget();
  // Properties
  SpellerTarget& SetEntryText( const std::string& );
  const std::string& EntryText() const;

 protected:
  virtual void OnSelect();

 private:
  Speller&    mSpeller;
  std::string mEntryText;
};

#endif // SPELLER_H

