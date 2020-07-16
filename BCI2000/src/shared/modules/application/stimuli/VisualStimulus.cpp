////////////////////////////////////////////////////////////////////////////////
// $Id: VisualStimulus.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual base class for stimuli that are also graphic objects.
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

#include "VisualStimulus.h"
#include "GraphObject.h"
#include "Target.h"

using namespace std;
using namespace GUI;

VisualStimulus::VisualStimulus()
: mPresentationMode( ShowHide ),
  mDimFactor( 0.5 ),
  mBeingPresented( false )
{
}

VisualStimulus::~VisualStimulus()
{
}

VisualStimulus&
VisualStimulus::SetPresentationMode( Mode m )
{
  mPresentationMode = m;
  GraphObject* pGraphObject = dynamic_cast<GraphObject*>( this );
  if( pGraphObject )
    pGraphObject->Change();
  return *this;
}

VisualStimulus::Mode
VisualStimulus::PresentationMode() const
{
  return mPresentationMode;
}

VisualStimulus&
VisualStimulus::SetDimFactor( float f )
{
  mDimFactor = f;
  GraphObject* pGraphObject = dynamic_cast<GraphObject*>( this );
  if( pGraphObject )
    pGraphObject->Change();
  return *this;
}

float
VisualStimulus::DimFactor() const
{
  return mDimFactor;
}

void
VisualStimulus::OnPresent()
{
  mBeingPresented = true;
  GraphObject* pGraphObject = dynamic_cast<GraphObject*>( this );
  if( pGraphObject )
    pGraphObject->Invalidate();
}

void
VisualStimulus::OnConceal()
{
  mBeingPresented = false;
  GraphObject* pGraphObject = dynamic_cast<GraphObject*>( this );
  if( pGraphObject )
    pGraphObject->Invalidate();
}

bool
VisualStimulus::BeingPresented() const
{
  return mBeingPresented;
}

