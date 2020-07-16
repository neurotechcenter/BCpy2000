#ifndef _downsampling_h
#define _downsampling_h

#include "ap.h"

void downsampling(const ap::template_1d_array<float, true>& x,
				  const int ds_factor,
				  ap::template_1d_array<float,true>& y);

#endif
