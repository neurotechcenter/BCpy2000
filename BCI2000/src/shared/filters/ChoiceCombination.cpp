////////////////////////////////////////////////////////////////////////////////
// $Id: ChoiceCombination.cpp 4981 2015-08-26 17:04:38Z mellinger $
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
#include "ChoiceCombination.h"
#include "BCIStream.h"

ChoiceCombination::ChoiceCombination( const std::string& inParamName )
: mChoice( 0 ),
  mParamName( inParamName )
{
}

ChoiceCombination::~ChoiceCombination()
{
  for( size_t i = 0; i < mFilters.size(); ++i )
    delete mFilters[i];
}

void
ChoiceCombination::Publish()
{
  GenericFilter::Publish();
  for( size_t i = 0; i < mFilters.size(); ++i )
    mFilters[i]->CallPublish();
}

void
ChoiceCombination::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  bool configOK = true;
  if( mFilters.empty() )
  {
    bcierr << "No filter available" << std::endl;
    configOK = false;
  }
  for( size_t i = 0; i < mFilters.size(); ++i )
  {
    if( mFilters[i] == NULL )
    {
      bcierr << "No filter specified for choice " << i << std::endl;
      configOK = false;
    }
  }
  if( configOK )
  {
    int choice = Parameter( mParamName );
    mFilters[choice]->CallAutoConfig( Input );
    mFilters[choice]->CallPreflight( Input, Output );
  }
}

void
ChoiceCombination::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mChoice = Parameter( mParamName );
  mFilters[mChoice]->CallInitialize( Input, Output );
}

void
ChoiceCombination::Process( const GenericSignal& Input, GenericSignal& Output )
{
  mFilters[mChoice]->CallProcess( Input, Output );
}

void
ChoiceCombination::StartRun()
{
  mFilters[mChoice]->CallStartRun();
}

void
ChoiceCombination::StopRun()
{
  mFilters[mChoice]->CallStopRun();
}

void
ChoiceCombination::Resting()
{
  mFilters[mChoice]->CallResting();
}

void
ChoiceCombination::Halt()
{
  mFilters[mChoice]->CallHalt();
}

