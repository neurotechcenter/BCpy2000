////////////////////////////////////////////////////////////////////////////////
// $Id: EDFOutputFormat.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Output into an EDF data file.
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
#ifndef EDF_OUTPUT_FORMAT_H
#define EDF_OUTPUT_FORMAT_H

#include "EDFOutputBase.h"

class EDFOutputFormat: public EDFOutputBase
{
 public:
          EDFOutputFormat();
  virtual ~EDFOutputFormat();

  virtual void Publish() const;
  virtual void Preflight( const SignalProperties&, const StateVector& ) const;
  virtual void Initialize( const SignalProperties&, const StateVector& );
  virtual void StartRun( std::ostream&, const std::string& );
  virtual void StopRun( std::ostream& );
  virtual void Write( std::ostream&, const GenericSignal&, const StateVector& );

  virtual const char* DataFileExtension() const { return ".edf"; }
};

#endif // EDF_OUTPUT_FORMAT_H
