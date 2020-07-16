////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Generalized interface for reading data from sockets, files,
//   pipes, etc.
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
#ifndef TINY_IO_OBJECTS_H
#define TINY_IO_OBJECTS_H

#include "Waitable.h"
#include "Lockable.h"
#include <iostream>

namespace Tiny
{

class Streambuf;

class BasicIOObject
{
  public:
    virtual ~BasicIOObject();

    Streambuf* AttachTo( Streambuf* );
    Streambuf* AttachedTo()
      { return mpStreambuf; }
    const Streambuf* AttachedTo() const
      { return mpStreambuf; }

    bool Good() const // ios::good()
      { return IOState() == std::ios_base::goodbit; }
    bool Bad() const // ios::bad()
      { return IOState() & std::ios_base::badbit; }
    bool Failed() const // ios::fail()
      { return IOState() & std::ios_base::failbit; }
    bool Eof() const // ios::eof()
      { return IOState() & std::ios_base::eofbit; }
    const char* DescribeIOState() const
      { return OnDescribeIOState(); }
    std::ios_base::iostate IOState() const // ios::rdstate()
      { return mIOState; }
    void ClearIOState() // ios::clear( 0 )
      { mIOState = std::ios_base::goodbit; }

    bool MightBlock() const
      { return OnMightBlock(); }

  protected:
    BasicIOObject()
      : mpStreambuf( 0 ), mBeingAttached( false ), mIOState( std::ios_base::goodbit )
      {}
    void SetIOState( std::ios_base::iostate state ) // ios::clear( iostate )
      { mIOState = state; }
    void SetIOStateBits( std::ios_base::iostate state ) // ios::setstate()
      { SetIOState( IOState() | state ); }

    virtual void OnAttachTo( Streambuf* );
    virtual void OnDestruct()
      { AttachTo( 0 ); }
    virtual bool OnMightBlock() const
      { return false; }
    virtual void OnIOState()
      {}
    virtual const char* OnDescribeIOState() const;

  private:
    Streambuf* mpStreambuf;
    bool mBeingAttached;
    std::ios_base::iostate mIOState; // std::ios_base::iostate
};

class SeekableObject : public BasicIOObject
{
public:
  int64_t SeekTo( int64_t pos ) { return OnSeekTo( pos ); }
protected:
  virtual int64_t OnSeekTo( int64_t ) = 0;
};

class InputObject
  : public SeekableObject, public Waitable, public LockableObject
{
  public:
    static InputObject& Default();

    int64_t Read( void* p, int64_t n );
    int64_t Available()
      { ScopedLock( this ); return OnAvailable(); }

  protected:
    void OnAttachTo( Streambuf* );
    bool OnMightBlock() const
      { ScopedLock( this ); return !this->Wait( Time::Interval( 0 ) ); }

    // OnRead() must return 0 to indicate EOF, and -1 to indicate failure.
    virtual int64_t OnRead( char*, int64_t ) = 0;
    virtual int64_t OnAvailable() = 0;
};

class OutputObject
  : public SeekableObject, public Waitable, public LockableObject
{
  public:
    static OutputObject& Default();

    int64_t Write( const void* p, int64_t );

  protected:
    void OnAttachTo( Streambuf* );
    bool OnMightBlock() const
      { return !this->Wait( Time::Interval( 0 ) ); }

    // OnWrite() must return 0 to indicate EOF, and -1 to indicate failure.
    virtual int64_t OnWrite( const char*, int64_t ) = 0;
};

class IOObject : public InputObject
{
  public:
    IOObject()
      { mOutput.pIO = this; }
    virtual ~IOObject()
      {}

    InputObject& Input()
      { return *this; }
    const InputObject& Input() const
      { return *this; }
    OutputObject& Output()
      { return mOutput; }
    const OutputObject& Output() const
      { return mOutput; }
    operator OutputObject&()
      { return Output(); }
    operator const OutputObject&() const
      { return Output(); }

    int64_t Write( const void* p, int64_t n )
      {
        int64_t r = mOutput.Write( p, n );
        SetIOStateBits( mOutput.IOState() );
        return r;
      }

  protected:
    void OnDestruct()
    {
      InputObject::OnDestruct();
      mOutput.OnDestruct();
    }
    void AssociateWith( const Waitable& read, const Waitable& write )
    {
      Waitable::AssociateWith( read );
      mOutput.AssociateWith( write );
    }
    void AssociateWith( const Waitable::ConditionPtr& read, const Waitable::ConditionPtr& write )
    {
      Waitable::AssociateWith( read );
      mOutput.AssociateWith( write );
    }
    // OnWrite() must return 0 to indicate EOF, and -1 to indicate failure.
    virtual int64_t OnWrite( const char*, int64_t ) = 0;
    using SeekableObject::OnSeekTo;

  private:
    using Waitable::AssociateWith;
    struct : OutputObject
    {
      public:
        using Waitable::AssociateWith;
        using OutputObject::OnDestruct;
      bool OnLock() const override
        { pIO->Lock(); return true; }
      bool OnUnlock() const override
        { return pIO->Unlock(); }
      bool OnTryLock() const override
        { return pIO->TryLock(); }
      int64_t OnWrite( const char* p, int64_t n ) override
        { return pIO->OnWrite( p, n ); }
      int64_t OnSeekTo( int64_t pos ) override
        { return pIO->OnSeekTo( pos ); }
      IOObject* pIO;
    } mOutput;
};

} // namespace

#endif // TINY_IO_OBJECTS_H
