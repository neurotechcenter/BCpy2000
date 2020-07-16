////////////////////////////////////////////////////////////////////////////////
// $Id: Stimulus.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The Stimulus class is a virtual base class which defines a
//   calling interface for stimuli.
//   A "Stimulus" is defined as "an object that can present and conceal itself."
//
//   Descendants of the Stimulus class may be members of one or more sets of
//   stimuli (class SetOfStimuli), and may be associated with stimulus
//   codes (class Association).
//   The StimulusTask framework class uses an AssociationMap to keep track of
//   the association between stimulus codes, stimuli, and targets.
//   Examples:
//   - For the P3Speller application, stimulus codes represent individual rows
//     or columns, and thus associations contain stimuli (matrix elements) from
//     rows and columns.
//   - For the StimulusPresentation application, stimulus codes are associated
//     with single stimuli, thus associations contain one stimulus each.
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
#ifndef STIMULUS_H
#define STIMULUS_H

#include <set>
#include <map>

class Stimulus
{
 public:
  Stimulus()
    : mTag( 0 )
    {}
  virtual ~Stimulus()
    {}
  // Properties
  Stimulus& SetTag( int inTag )
    { mTag = inTag; return *this; }
  int Tag() const
    { return mTag; }
  // Event calling interface
  Stimulus& Present()
    { OnPresent(); return *this; }
  Stimulus& Conceal()
    { OnConceal(); return *this; }

 protected:
  // Event handling interface
  //  In its OnPresent event handler, a stimulus is supposed to present itself
  //  (e.g., to make itself visible, play itself if it is a sound or a movie,
  //  or highlight itself if it is a P300 matrix element).
  virtual void OnPresent() = 0;
  //  In its OnConceal event handler, a stimulus is supposed to conceal itself
  //  (e.g., make itself invisible, stop playback, or switch back to normal mode).
  //  This event is called Conceal rather than Hide because "Hide" is already
  //  used for making a graphic element invisible.
  virtual void OnConceal() = 0;

 private:
  int mTag;
};

class SetOfStimuli : public std::set<Stimulus*>
{
 public:
  SetOfStimuli()
    {}
  virtual ~SetOfStimuli()
    {}
  // Householding
  SetOfStimuli& Add( Stimulus* s )
    { insert( s ); return *this; }
  SetOfStimuli& Remove( Stimulus* s )
    { erase( s ); return *this; }
  SetOfStimuli& Clear()
    { clear(); return *this; }
  SetOfStimuli& DeleteObjects()
    { for( iterator i = begin(); i != end(); ++i ) delete *i; clear(); return *this; }

  bool Contains( Stimulus* s ) const
    { return find( s ) != end(); }
  bool Intersects( const SetOfStimuli& s ) const
    {
      for( const_iterator i = begin(); i != end(); ++i )
        if( s.Contains( *i ) ) return true;
      return false;
    }

  // Events
  void Present()
    { for( iterator i = begin(); i != end(); ++i ) ( *i )->Present(); }
  void Conceal()
    { for( iterator i = begin(); i != end(); ++i ) ( *i )->Conceal(); }
};

#endif // STIMULUS_H

