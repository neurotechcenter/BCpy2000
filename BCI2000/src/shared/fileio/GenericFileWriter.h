////////////////////////////////////////////////////////////////////////////////
// $Id: GenericFileWriter.h 4202 2012-06-28 15:44:46Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual class interface for data output filters.
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
#ifndef GENERIC_FILE_WRITER_H
#define GENERIC_FILE_WRITER_H

#include "GenericFilter.h"

class GenericFileWriter : public GenericFilter
{
 protected: // No instantiation outside derived classes.
  GenericFileWriter() {}
 public:
  virtual ~GenericFileWriter() {}

 protected:
  // GenericFilter inherited functions.
  virtual void Publish() = 0;
  virtual void Preflight( const SignalProperties&,
                                SignalProperties& ) const = 0;
  virtual void Initialize( const SignalProperties&,
                           const SignalProperties& ) = 0;
  virtual void StartRun() {}
  virtual void StopRun() {}
  // The write function takes as argument the state vector
  // that existed at the time of the signal argument's time stamp.
  virtual void Write( const GenericSignal&, const StateVector& ) = 0;
  virtual void Halt() {}

  virtual bool AllowsVisualization() const { return false; }

 public:
  void CallWrite( const GenericSignal&, const StateVector& );

 private:
  // The Process() function should not be called for the
  // GenericFileWriter class.
  virtual void Process( const GenericSignal&,
                              GenericSignal& ) {};
};

#endif // GENERIC_FILE_WRITER_H

