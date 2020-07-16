////////////////////////////////////////////////////////////////////////////////
// $Id: FilePlaybackADC.h 4609 2013-10-16 18:43:30Z mellinger $
// Author: Adam Wilson, Jeremy Hill
// Description: An ADC class for testing purposes.
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
#ifndef SIGNAL_GENERATOR_ADC_H
#define SIGNAL_GENERATOR_ADC_H

#include "GenericADC.h"
#include "Clock.h"
#include "BCI2000FileReader.h"
#include <string>
#include <vector>


class FilePlaybackADC : public GenericADC
{
	public:
				   FilePlaybackADC();
		virtual      ~FilePlaybackADC();

		virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
		virtual void Initialize( const SignalProperties&, const SignalProperties& );
		virtual void StartRun();
		virtual void Process( const GenericSignal&, GenericSignal& );
		virtual void Halt();

		virtual bool IsRealTimeSource() const { return false; } // permits --EvaluateTiming=0, to launch without realtime checking

	private:
		void MatchChannels( const BCI2000FileReader& dataFile, std::vector<int>& chList ) const;
		void CheckFile( std::string& fname, BCI2000FileReader& dataFile ) const;

		// Configuration
		float  mSamplingRate;
		int mBlockSize;
		float mSpeedup;
		std::string mFileName;
		std::string mTemplateFileName;
		std::vector<int> mChList;
		int mBlockDuration;
		BCI2000FileReader *mDataFile;
		int mCurBlock;
		int mMaxBlock;
		bool mSuspendAtEnd;
		int mStartTime;
		long long mNumSamples;
		bool mReverse;

		class StateMapping {
			public:
				StateMapping(size_t srcLoc, size_t srcLen, size_t dstLoc, size_t dstLen) {mSrcLoc=srcLoc; mSrcLen=srcLen; mDstLoc=dstLoc; mDstLen=dstLen; mPrevVal=0;}
				void Reset() { mPrevVal = 0; }
				void Copy(const StateVector* srcVec, size_t srcSampleOffset, StateVector* dstVec, size_t dstSampleOffset )
				{
					State::ValueType val = srcVec->StateValue( mSrcLoc, mSrcLen, srcSampleOffset );
					if( val == mPrevVal ) return;
					mPrevVal = val;
					dstVec->SetStateValue( mDstLoc, mDstLen, dstSampleOffset, val );
				}
			private:
				size_t mSrcLoc;
				size_t mSrcLen;
				size_t mDstLoc;
				size_t mDstLen;
				State::ValueType mPrevVal;
		};
		std::vector<StateMapping> mStateMappings;
    Clock mClock;
};

#endif // SIGNAL_GENERATOR_ADC_H

