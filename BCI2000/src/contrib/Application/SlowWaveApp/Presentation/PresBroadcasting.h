/////////////////////////////////////////////////////////////////////////////
//
// File: PresBroadcasting.h
//
// Date: Oct 26, 2001
//
// Author: Juergen Mellinger
//
// Description: This purely virtual class (TListener) declares a common method
//              interface for views and models. The listeners are kept in a list
//              inside a broadcaster (TBroadcaster), and the resp. functions are
//              called via the broadcaster's Broadcast...() functions at
//              appropriate times.
//
// Changes: Feb 2, 2004, jm: Introduced TBroadcaster to generalize the
//              broadcaster concept to adapt to BCI2000 needs.
//              Changed the name from PresListener.h to PresBroadcasting.h
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
//////////////////////////////////////////////////////////////////////////////

#ifndef PRES_BROADCASTING_H
#define PRES_BROADCASTING_H

#include <list>
#include <queue>

class GenericSignal;

struct TEventArgs
{
    unsigned short          targetCode,
                            resultCode;
    const GenericSignal     *signal;
    float                   timeOffset;
};

class TPresListener
{
  protected:
                 TPresListener() {}
    virtual      ~TPresListener() {}

  public:
    virtual void ProcessTrialActive(   const TEventArgs& ) = 0;
    virtual void ProcessBeginOfTrial(  const TEventArgs& ) = 0;
    virtual void ProcessTaskBegin(     const TEventArgs& ) = 0;
    virtual void ProcessFeedbackBegin( const TEventArgs& ) = 0;
    virtual void ProcessFeedbackEnd(   const TEventArgs& ) = 0;
    virtual void ProcessFeedback(      const TEventArgs& ) = 0;
    virtual void ProcessEndOfClass(    const TEventArgs& ) = 0;
    virtual void ProcessSuccess(       const TEventArgs& ) = 0;
    virtual void ProcessItiBegin(      const TEventArgs& ) = 0;
    virtual void ProcessStopBegin(     const TEventArgs& ) = 0;
};

class TPresBroadcaster
{
  public:
                 TPresBroadcaster() {}
    virtual      ~TPresBroadcaster() {}

  protected:
            void BroadcastTrialActive(   const TEventArgs& ) const;
            void BroadcastBeginOfTrial(  const TEventArgs& ) const;
            void BroadcastTaskBegin(     const TEventArgs& ) const;
            void BroadcastFeedbackBegin( const TEventArgs& ) const;
            void BroadcastFeedbackEnd(   const TEventArgs& ) const;
            void BroadcastFeedback(      const TEventArgs& ) const;
            void BroadcastEndOfClass(    const TEventArgs& ) const;
            void BroadcastSuccess(       const TEventArgs& ) const;
            void BroadcastItiBegin(      const TEventArgs& ) const;
            void BroadcastStopBegin(     const TEventArgs& ) const;

            typedef void ( TPresBroadcaster::*BroadcastFunction )( const TEventArgs& ) const;
            void Queue( BroadcastFunction );
            void PerformQueuedBroadcasts( const TEventArgs& inArgs );

            void DetachListeners();

  public:
            void AttachListener( TPresListener*  inListener );

  private:
    typedef std::list<TPresListener*> ListenerList;
    ListenerList listeners;

    std::queue<BroadcastFunction> mBroadcastQueue;

};

inline
void
TPresBroadcaster::BroadcastTrialActive( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessTrialActive( inArgs );
}

inline
void
TPresBroadcaster::BroadcastBeginOfTrial( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessBeginOfTrial( inArgs );
}

inline
void
TPresBroadcaster::BroadcastTaskBegin( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessTaskBegin( inArgs );
}

inline
void
TPresBroadcaster::BroadcastFeedbackBegin( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessFeedbackBegin( inArgs );
}

inline
void
TPresBroadcaster::BroadcastFeedbackEnd( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessFeedbackEnd( inArgs );
}

inline
void
TPresBroadcaster::BroadcastFeedback( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessFeedback( inArgs );
}

inline
void
TPresBroadcaster::BroadcastEndOfClass( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessEndOfClass( inArgs );
}

inline
void
TPresBroadcaster::BroadcastSuccess( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessSuccess( inArgs );
}

inline
void
TPresBroadcaster::BroadcastItiBegin( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessItiBegin( inArgs );
}

inline
void
TPresBroadcaster::BroadcastStopBegin( const TEventArgs& inArgs ) const
{
  for( ListenerList::const_iterator i = listeners.begin(); i != listeners.end(); ++i )
    ( *i )->ProcessStopBegin( inArgs );
}

inline
void
TPresBroadcaster::AttachListener( TPresListener*  inListener )
{
  listeners.push_back( inListener );
}

inline
void
TPresBroadcaster::DetachListeners()
{
  listeners.clear();
}

inline
void
TPresBroadcaster::Queue( BroadcastFunction f )
{
  mBroadcastQueue.push( f );
}

inline
void
TPresBroadcaster::PerformQueuedBroadcasts( const TEventArgs& inArgs )
{
  while( !mBroadcastQueue.empty() )
  {
    ( this->*mBroadcastQueue.front() )( inArgs );
    mBroadcastQueue.pop();
  }
}
#endif // PRES_BROADCASTING_H
