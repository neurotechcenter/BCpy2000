//////////////////////////////////////////////////////////////////////
// $Id: Waitable.h 4813 2015-04-24 23:01:17Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A mix-in class that provides support for synchronous
//   events to be sent to a set of handlers.
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
#ifndef TINY_BROADCASTER_H
#define TINY_BROADCASTER_H

namespace Tiny
{

class Runnable;

class Broadcaster
{
public:
  Broadcaster& SetEnabled( bool );
  bool Enabled() const;
  Broadcaster& AddListener( int message, Runnable* );
  Broadcaster& RemoveListener( int message, Runnable* );

  class Message
  {
    friend class Broadcaster;
  public:
    Message() : sender( nullptr ) {}
    Broadcaster* Sender() const { return sender; }
    int Id() const { return id; }
    template<class T> T Argument() const { return static_cast<T>( Argument() ); }
    void* Argument() const { return arg; }
  private:
    Broadcaster* sender; int id; void* arg;
  };
  static const Message& Message();

protected:
  Broadcaster();
  ~Broadcaster();
  Broadcaster( const Broadcaster& );
  Broadcaster& operator=( const Broadcaster& );
  int Emit( unsigned short message, void* = nullptr );

private:
  struct Data;
  Data* d;
};

}

using Tiny::Broadcaster;

#endif // TINY_BROADCASTER_H
