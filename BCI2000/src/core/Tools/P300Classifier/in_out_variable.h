#ifndef _in_out_variable_h
#define _in_out_variable_h

#include "ap.h"
#include <vector>
#include <iostream>

using namespace std;

bool in_out_variable(const ap::boolean_1d_array& in, const ap::real_2d_array& X, ap::real_2d_array& x, bool io);
bool in_out_variable_1D(const ap::boolean_1d_array& in, const ap::real_1d_array& X, ap::real_1d_array& x, ap::real_1d_array& vector, bool io);

#endif
