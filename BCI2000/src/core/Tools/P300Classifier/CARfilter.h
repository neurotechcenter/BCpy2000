#ifndef _CARfilter_h
#define _CARfilter_h

#include <iostream>
#include <algorithm>
#include <vector>
#include "ap.h"
#include "blas.h"

using namespace std;

void CARfilter(ap::template_2d_array<float,true>& signal);

#endif
