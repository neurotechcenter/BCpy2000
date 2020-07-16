////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000SessionReader.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Class that provides an interface to the data stored in BCI2000
//   data files belonging to a session.
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
#ifndef BCI2000_SESSION_READER_H
#define BCI2000_SESSION_READER_H

#include "BCI2000FileReader.h"

class BCI2000SessionReader: public BCI2000FileReader
{
 public:
  static const int cDefaultBufSize = BCI2000FileReader::cDefaultBufSize;
  enum
  {
    InconsistentFiles = BCI2000FileReader::NumErrors,

    NumErrors
  };

 public:
  BCI2000SessionReader();
  explicit BCI2000SessionReader( const char* fileName );
  ~BCI2000SessionReader();

 private:
  BCI2000SessionReader( const BCI2000SessionReader& );
  BCI2000SessionReader& operator=( const BCI2000SessionReader& );

 public:
  // State
  virtual int   ErrorState() const;
  virtual bool  IsOpen() const
                { return mInitialized; }

  // File access
  virtual BCI2000SessionReader&
                Open( const char* fileName, int bufferSize = cDefaultBufSize );
  virtual long  NumSamples() const
                { return mNumSamples.empty() ? 0 : *mNumSamples.rbegin(); }

  // Data access
  virtual GenericSignal::ValueType
               RawValue( int channel, long sample );
  virtual BCI2000SessionReader&
               ReadStateVector( long sample );

 private:
  void Reset();
  long SetSampleRun( long sample );

 private:
  bool               mInitialized;
  int                mBufSize;
  std::vector<std::string> mRunNames;
  std::vector<long>  mNumSamples;  // samples in a particular run
  int                mCurRun;
  int                mErrorState;
};

#endif // BCI2000_SESSION_READER_H
