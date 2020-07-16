#ifndef FILTER_FILTER_H
#define FILTER_FILTER_H

#include "CustomIIR.h"
#include "GenericFilter.h"

class FilterFilter : public GenericFilter
{
	public:
		FilterFilter();
		virtual ~FilterFilter();
		virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
		virtual void Initialize( const SignalProperties&, const SignalProperties& );
		virtual void StartRun();
		virtual void Process( const GenericSignal&, GenericSignal& );
		virtual void Halt();

	private:
		bool      mEnabled;
		float     mGain;
		CustomIIR mFilter;
};

#endif // FILTER_FILTER_H
