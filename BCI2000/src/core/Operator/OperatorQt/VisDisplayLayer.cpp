////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayLayer.cpp 5112 2015-11-18 14:35:10Z mellinger $
// Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: Defines a layer of rendering into a VisDisplayWindow
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
#include "VisDisplayLayer.h"
#include "Debugging.h"
#include <QLayout>
#include <map>

using namespace std;

VisDisplayLayer::VisDisplayLayer( const std::string& inVisID )
: VisDisplayBase( inVisID )
{
  // Set widget properties and add it to the window
  this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
  this->parentWidget()->layout()->addWidget( this );

  // Arrange sibling layers according to their layer IDs.
  typedef map<string, VisDisplayLayer*> LayerMap;
  LayerMap layers;
  const QObjectList& children = this->parentWidget()->children();
  for( QObjectList::const_iterator i = children.begin(); i != children.end(); ++i )
  {
    VisDisplayLayer* pLayer = dynamic_cast<VisDisplayLayer*>( *i );
    if( pLayer != NULL )
      layers[pLayer->mVisID.LayerID()] = pLayer;
  }
  // A layer with an empty layer ID will be moved to the bottom of the layout.
  for( LayerMap::iterator i = layers.begin(); i != layers.end(); ++i )
    i->second->raise();
  // The bottom layer should get keyboard focus.
  Assert( !layers.empty() );
  layers.begin()->second->setFocus();
}

VisDisplayLayer::~VisDisplayLayer()
{
}
