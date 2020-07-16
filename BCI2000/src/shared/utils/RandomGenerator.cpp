////////////////////////////////////////////////////////////////////////////////
// $Id: RandomGenerator.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Authors: mcfarland@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates a random number generator.
//   Each class instance maintains its own internal state, ensuring identical
//   number sequences for identical seeds across software elements that use
//   random numbers.
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

#include "RandomGenerator.h"
#include "FileUtils.h"

using namespace std;

// For backward compatibility, allow for the instance count to serve as an ID
int
RandomGenerator::NextUnnamedInstance()
{
  static int count = 0;
  return ++count;
}

RandomGenerator::RandomGenerator()
{
  int count = NextUnnamedInstance();
  mID.resize( 1 );
  if( count >= 256 )
    throw std_runtime_error <<
      "Number of unnamed RandomGenerator instances exceeds limit, "
      "use constructor arguments to name RandomGenerators";
  mID[0] = static_cast<string::value_type>( count );
  mID += FileUtils::ApplicationTitle();
}

void
RandomGenerator::Preflight() const
{
  OptionalParameter( "RandomSeed" );
}

void
RandomGenerator::Initialize()
{
  SeedType seed = OptionalParameter( "RandomSeed", 0 );
  if( seed == 0 )
    SetSeed( DefaultSeed() );
  else
    SetSeed( seed );
  // Use the ID string to modify the seed in a way that is both unique
  // and robust against configuration changes such as addition of filters,
  // or change of endianness.
  while( mID.length() % sizeof( SeedType ) )
    mID += " ";
  for( size_t i = 0; i < mID.length() / sizeof( SeedType ); ++i )
  {
    SeedType value = 0;
    for( size_t j = 0; j < sizeof( SeedType ); ++j )
    {
      SeedType c = static_cast<uint8_t>( mID[i * sizeof( SeedType ) + j] );
      value |= c << ( 8 * j );
    }
    SetSeed( Seed() ^ value );
  }
  bcidbg( 1 ) << "RandomGenerator ID: " << mID << ", Seed: " << hex << Seed() << endl;
}

void
RandomGenerator::StartRun()
{
  Initialize();
}

