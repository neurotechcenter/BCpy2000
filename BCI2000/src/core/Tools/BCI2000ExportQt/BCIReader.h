//////////////////////////////////////////////////////////////////////////////
// $Id: BCIReader.h 4981 2015-08-26 17:04:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An base class for converting a BCI2000 file into different
//   formats. Output formats are represented as descendants implementing
//   BCIReader's purely virtual output functions.
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
#ifndef BCI_READER_H
#define BCI_READER_H

#include <set>
#include <vector>
#include <string>

#include "BCI2000FileReader.h"

struct iless
{
  bool operator()( const std::string& a, const std::string& b ) const
  { return ::stricmp( a.c_str(), b.c_str() ) < 0; };
};

typedef std::set<std::string, iless> StringSet;
typedef std::vector<std::string>     StringList;

class BCIReader
{
 public:
  BCIReader()
    {}
  virtual ~BCIReader()
    {}
  void Open( const char* inFileName );
  void Close();
  void Process( const StringList& inChannelNames,
                const StringSet&  inIgnoreStates,
                bool              inScanOnly = false );
  const StringSet& GetStates() const
    { return mStatesInFile; }

 protected:
  struct OutputInfo
  {
          const char*       name;
          unsigned long     numChannels;
          const StringList* channelNames;
          int               blockSize;
          long long         numSamples;
          double            samplingRate;
          const StringList* stateNames;
  };

  virtual void InitOutput( OutputInfo& ) = 0;
  virtual void ExitOutput() = 0;
  virtual void OutputSignal( const GenericSignal&, long long inSamplePos ) = 0;
  virtual void OutputStateValue( const State&, State::ValueType, long long inSamplePos ) {}
  virtual void OutputStateChange( const State&, State::ValueType, long long inSamplePos ) {}
  virtual void OutputStateRange( const State&, State::ValueType, long long inBeginPos, long long inEndPos ) {}

 private:
  std::string       mFileName;
  BCI2000FileReader mInputData;
  StringSet         mStatesInFile;
};

#endif // BCI_READER_H

