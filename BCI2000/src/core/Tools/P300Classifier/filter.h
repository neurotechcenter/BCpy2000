#ifndef _filter_h
#define _filter_h

#include "ap.h"

void filter(const int ord, const ap::template_1d_array<float, true>& a,
			const ap::template_1d_array<float, true>& b, const int np,
			const ap::template_1d_array<float, true>& x,
			ap::template_1d_array<float, true>& y);

#endif
