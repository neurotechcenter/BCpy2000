#ifndef _stepwisefit_h
#define _stepwisefit_h

#include "ap.h"
#include "stepcalc.h"
#include "stepnext.h"

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

typedef void CALLBACK_STATUS(string message);

void stepwisefit(const ap::real_2d_array& X, const ap::real_1d_array& y, const double penter, const double premove,
				 const int max_iter, ap::real_1d_array& B, ap::real_1d_array& SE, ap::real_1d_array& PVAL, ap::boolean_1d_array& in,
				 CALLBACK_STATUS callback_status=NULL);

#endif
