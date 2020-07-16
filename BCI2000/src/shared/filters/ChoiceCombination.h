////////////////////////////////////////////////////////////////////////////////
// $Id: ChoiceCombination.h 4586 2013-09-30 14:14:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A Filter combination that allows a choice between filters.
//   To use, derive a new filter class from it, and call
//   AddChoice<Filter1>( choice1 ); ...
//   from the derived class' constructor.
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
#ifndef CHOICE_COMBINATION_H
#define CHOICE_COMBINATION_H

#include "GenericFilter.h"

class ChoiceCombination : public GenericFilter
{
 public:
  ChoiceCombination( const std::string& inParamName );
  ~ChoiceCombination();

 protected:
   // Call this function from your derived class' constructor.
   template<class T> void AddChoice( size_t inChoiceIdx )
   {
     if( mFilters.size() <= inChoiceIdx )
       mFilters.resize( inChoiceIdx + 1 );
     mFilters[inChoiceIdx] = ( new GenericFilter::FilterRegistrar<T>( this ) )->NewInstance();
   }

   void Publish();
   void Preflight( const SignalProperties& Input, SignalProperties& Output ) const;
   void Initialize( const SignalProperties& Input, const SignalProperties& Output );
   void Process( const GenericSignal& Input, GenericSignal& Output );
   void StartRun();
   void StopRun();
   void Resting();
   void Halt();

 private:
   std::vector<GenericFilter*> mFilters;
   int mChoice;
   std::string mParamName;
};

#endif // CHOICE_COMBINATION_H
