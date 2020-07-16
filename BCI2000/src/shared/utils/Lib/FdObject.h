////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IO wrapper for OS objects based on file descriptors.
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
#ifndef TINY_FDOBJECT_H
#define TINY_FDOBJECT_H

#include "Uncopyable.h"
#include "IOObjects.h"

namespace fdio { class fd_object; }

namespace Tiny
{

class FdObject : public IOObject, Uncopyable
{
  public:
    FdObject();
    virtual ~FdObject();

    FdObject& SetBlockingMode( bool );
    bool BlockingMode() const;

    bool IsOpen() const;
    std::ptrdiff_t Fd() const;
    FdObject& Close();

  protected:
    void Open( fdio::fd_object* );
    
    int64_t OnAvailable() override;
    int64_t OnRead( char*, int64_t ) override;
    int64_t OnWrite( const char*, int64_t ) override;
    int64_t OnSeekTo( int64_t ) override;
    const char* OnDescribeIOState() const override;
    bool OnLock() const override;
    bool OnUnlock() const override;
    bool OnTryLock() const override;

  private:
    struct Private;
    Private* p;
};

} // namespace

using Tiny::FdObject;

#endif // TINY_FDOBJECT_H
