//////////////////////////////////////////////////////////////////////
// $Id: Thread.h 5248 2016-02-19 17:47:34Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A thread class similar to the VCL's TThread.
//   To implement your own thread, create a class that inherits from
//   Thread, and put your own functionality into its
//   OnExecute() function.
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
#ifndef TINY_THREAD_H
#define TINY_THREAD_H

#include "Uncopyable.h"
#include "Waitable.h"
#include <memory>

namespace Tiny
{

class Runnable;

class Thread : public Waitable, Uncopyable
{
 public: // autoDelete is declared int rather than bool to avoid unintended cast from pointer
  Thread( int autoDelete = false, Runnable* = 0, const std::string& = "" );
  Thread( Runnable*, const std::string& );
  Thread( const std::string& );
  virtual ~Thread();

  const std::string& Name() const;

  void Start();
  bool StartIfNotRunning();
  bool Terminate( Time::Interval timeout = Time::Forever );

  bool Terminating() const;
  bool Running() const;
  int Result() const;

  static Thread* Current();
  static Thread* AdoptAs( const std::string& );

  enum
  {
    None = 0,
    WhenTerminating,
    WhenFinished,

    NumberOfEvents
  };
  static bool Notify( int event, class Runnable* );
  static int RegisterWait( const Waitable::ConditionPtr*, int, Runnable* );

 protected:
  void TerminateAndWait();
  virtual int OnExecute();

 private:
  // undefined function to detect incompatible code, use Terminate( Time::FromMilliseconds( timeout_ms ) ) instead
  bool Terminate( int timeout_ms );

  void Init( int, Runnable*, const std::string& );
  int RunThread();

  struct Starter;
  friend struct Starter;

  Waitable::ConditionPtr mConditionRef;
  struct Private;
  std::shared_ptr<Private> p;
};

} // namespace

using Tiny::Thread;

#endif // TINY_THREAD_H
