////////////////////////////////////////////////////////////////////////////////
//  $Id: ViewSource.h 3798 2012-02-01 18:07:06Z mellinger $
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A data source that combines multiple data sources into a
//    single one, and that provides its output if form of a GenericSignal,
//    suitable for visualization, or as a filter output.
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
#ifndef VIEW_SOURCE_H
#define VIEW_SOURCE_H

#include "DataSource.h"
#include <vector>

class ViewSource : public DataSource
{
 public:
  ViewSource( const std::string& name );
  void Add( DataSource* p ) { DataSource::Add( p ); }
  const GenericSignal& Signal() const { return mSignal; }

 protected:
  void OnInitialize( const Context& );
  void OnProcess( const Context& );
  Value OnData( int );

 private:
  static void AppendElements( SignalProperties&, const SignalProperties& );

 private:
  std::vector<int> mElements;
  GenericSignal mSignal;
};

#endif // VIEW_SOURCE_H
