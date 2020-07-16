//////////////////////////////////////////////////////////////////////
// $Id: StateRef.h 5428 2016-07-14 00:39:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that holds references to state values, and
//         allows for convenient automatic type
//         conversions when accessing state values.
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
#ifndef STATE_REF_H
#define STATE_REF_H

#include "State.h"

class StateRef
{
  class SampleRef;
 public:
  StateRef();
  StateRef( const State*,
            StateVector*,
            int sample,
            uint32_t defaultValue = 0 );
  StateRef& operator=( const StateRef& );
  const StateRef& operator=( uint32_t );
  operator uint32_t() const;
  StateRef operator()( size_t offset ) const;
  const State* operator->() const;

  StateRef& Reset( const StateRef& );

  class StateRefFloat  AsFloat();
  class StateRefSigned AsSigned();
  StateRef&            AsUnsigned();

  SampleRef Sample( int );

 private:
  const State* mpState;
  StateVector* mpStateVector;
  int          mSample;
  uint32_t     mDefaultValue;
};

class StateRefFloat
{
 public:
  StateRefFloat( StateRef& );
  const StateRefFloat& operator=( float );
  operator float() const;

 private:
  StateRef& mrStateRef;
};

class StateRefSigned
{
 public:
  StateRefSigned( StateRef& );
  const StateRefSigned& operator=( int32_t );
  operator int32_t() const;

 private:
  StateRef& mrStateRef;
};

class StateRef::SampleRef
{
 public:
  SampleRef( StateRef&, int );
  const SampleRef& operator=( uint32_t );
  operator uint32_t() const;

 private:
  StateRef& mrStateRef;
  int mSample;
};

#endif // STATE_REF_H
