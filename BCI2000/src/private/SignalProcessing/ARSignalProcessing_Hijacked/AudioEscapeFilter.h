////////////////////////////////////////////////////////////////////////////////
//  $Id: AudioEscapeFilter.h 3163 2011-03-18 03:19:34Z jhill $
//  Author:      jezhill@gmail.com
//  Description: A (working) tutorial low pass filter demonstrating
//               parameter access, visualization, and unit conversion.
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_ESCAPE_FILTER_H
#define AUDIO_ESCAPE_FILTER_H

#include "GenericFilter.h"

class AudioEscapeFilter : public GenericFilter
{
	public:
		AudioEscapeFilter();
		~AudioEscapeFilter();

		void Preflight( const SignalProperties&, SignalProperties& ) const;
		void Initialize( const SignalProperties&, const SignalProperties& );
		void Process( const GenericSignal&, GenericSignal& );

	private:
		int mLeftChIndex;
		int mRightChIndex;
		bool mWarned;
};
#endif // AUDIO_ESCAPE_FILTER_H
