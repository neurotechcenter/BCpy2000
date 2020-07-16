////////////////////////////////////////////////////////////////////////////////
// $Id: ProtocolVersion.h 5291 2016-03-17 18:26:36Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for protocol version matching.
//   Compatibility-breaking modifications to the protocol will be reflected
//   by an increment of the VersionID constant.
//   Upon connecting to the operator module, core modules report their
//   protocol version, and incompatible versions are rejected.
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
#ifndef PROTOCOL_VERSION_H
#define PROTOCOL_VERSION_H

#include <iostream>

class ProtocolVersion
{
  struct Version { int major, minor; const char* desc; };
  static const Version* History()
  {
    static const Version v[] =
    {
      { 2, 3, "ListeningAddress parameter" },
      { 2, 2, "Shared signal storage" },
      { 2, 1, "NextModuleInfo from Operator" },
      { 2, 0, "Multi-sample state vector" },
      { 1, 0, "2.0 Release" },
    };
    return v;
  }

 public:
   enum
   {
     MultiSampleStateVector = 1,
     Negotiation,
     NextModuleInfo,
     SharedSignalStorage,
     ListeningAddressParameter,
   };

   ProtocolVersion()
     : mMajor( 0 ), mMinor( 0 )
     {}
   ~ProtocolVersion()
     {}
   ProtocolVersion Major() const
     { return ProtocolVersion( mMajor, 0 ); }

  private:
   ProtocolVersion( int maj, int min ) : mMajor( maj ), mMinor( min ) {}

  public:
   bool operator==( const ProtocolVersion& p ) const
     { return mMajor == p.mMajor && mMinor == p.mMinor; }
   bool Matches( const ProtocolVersion& p ) const
     { return mMajor == p.mMajor; }
   bool MoreRecentThan( const ProtocolVersion& p ) const
     { if( mMajor > p.mMajor ) return true; return mMinor > p.mMinor; }
   bool AtLeast( const ProtocolVersion& p ) const
     { if( mMajor > p.mMajor ) return true; return mMajor == p.mMajor && mMinor >= p.mMinor; }
   bool Provides( int ) const;

   std::ostream& InsertInto( std::ostream& os ) const
     { return os << mMajor << "." << mMinor; }
   std::istream& ExtractFrom( std::istream& is );
   std::ostream& Serialize( std::ostream& os ) const;
   std::istream& Unserialize( std::istream& is )
     { return ExtractFrom( is ).ignore(); }

   static ProtocolVersion None()
     { return ProtocolVersion( 0, 0 ); }
   static ProtocolVersion Current()
     { return ProtocolVersion( History()[0].major, History()[0].minor ); }

  private:
   int mMajor, mMinor;
};

inline
bool ProtocolVersion::Provides( int feature ) const
{
  switch( feature )
  {
    case MultiSampleStateVector:
      return AtLeast( ProtocolVersion( 2, 0 ) );
    case Negotiation:
    case NextModuleInfo:
      return AtLeast( ProtocolVersion( 2, 1 ) );
    case SharedSignalStorage:
      return AtLeast( ProtocolVersion( 2, 2 ) );
    case ListeningAddressParameter:
      return AtLeast( ProtocolVersion( 2, 3 ) );
  }
  return false;
}

inline
std::ostream& ProtocolVersion::Serialize( std::ostream& os ) const
{
  os << mMajor;
  if( mMinor )
    os << "." << mMinor;
  return os.put( '\0' );
}

inline
std::istream& ProtocolVersion::ExtractFrom( std::istream& is )
{
  mMinor = 0;
  is >> mMajor;
  if( is.peek() == '.' )
    is.ignore() >> mMinor;
  return is;
}

inline
std::ostream& operator<<( std::ostream& os, const ProtocolVersion& s )
{ return s.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, ProtocolVersion& s )
{ return s.ExtractFrom( is ); }

#endif // PROTOCOL_VERSION_H
