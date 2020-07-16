/////////////////////////////////////////////////////////////////////////////
//
// File: TargetSeq.h
//
// Date: Nov 16, 2001
//
// Author: Juergen Mellinger
//
// Description: A list of target codes and associated auditory/visual
//              stimulus files that can read itself from a file.
//
// Changes:
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

#ifndef TARGETSEQH
#define TARGETSEQH

#include <string>
#include <list>

#include "PresParams.h"
#include "PresErrors.h"

class Param;

class TTargetSeqEntry
{
  public:
        TTargetSeqEntry() : targetCode( 0 ) {}
        bool        operator<(  const TTargetSeqEntry &inEntry ) const;

        int         targetCode;
        std::string audioFile;
        std::string visFile;
};


class TTargetSeq: public std::list< TTargetSeqEntry >
{
  public:
    static const int    randomSeqLength = 100; // Length of sequence when created from probabilities.
    TPresError  ReadFromFile(               const char  *inSequenceFileName, bool inCheckFiles = false );
    TPresError  ReadFromParam(              const Param *inParamPtr );
    TPresError  CreateFromProbabilities(    const Param *inParamPtr );
};

inline
bool
TTargetSeqEntry::operator<( const TTargetSeqEntry   &inEntry ) const
{
    return  TRUE_TARGET_CODE( targetCode ) < TRUE_TARGET_CODE( inEntry.targetCode );
}

#endif // TARGETSEQH


