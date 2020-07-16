////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class representing an IEEE 1284 parallel port
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
#ifndef PARALLEL_PORT_H
#define PARALLEL_PORT_H

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class ParallelPort
{
private:
  ParallelPort();
  ParallelPort( const ParallelPort& );// = delete;
  ParallelPort& operator=( const ParallelPort& );// = delete;

public:
  ~ParallelPort();

  typedef std::shared_ptr<ParallelPort> Ptr;
  struct List : std::vector<Ptr> { Ptr Find( const std::string& ); };

  const std::string& Name() const { return mName; }
  bool Open();
  void Close();
  bool IsOpen() const;
  std::ostream& WriteToStream( std::ostream& ) const;

  typedef enum
  {
    Data = 0,
    DSR = 1,
    DCR = 2,

    CRA = 0x10,
    CRB = CRA + 1,
    ECR = CRA + 2
  } Register;
  bool Write( Register, uint8_t value );
  bool Read( Register, uint8_t& value );

  static bool Enumerate( List& );

private:
  uint16_t Address( Register ) const;

  std::string mName;
  struct IORange { uint16_t base, size; };
  std::vector<IORange> mIORanges;
  void* mHandle;
};

inline
std::ostream&
operator<<( std::ostream& os, const ParallelPort& p )
{
  return p.WriteToStream( os );
}

#endif // PARALLEL_PORT_H
