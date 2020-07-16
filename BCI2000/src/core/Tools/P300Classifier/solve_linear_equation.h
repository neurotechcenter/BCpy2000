#ifndef _solve_linear_equation_h_
#define _solve_linear_equation_h_

#include "blas.h"
#include "ap.h"

using namespace std;

void solve_linear_equation_2D(const ap::real_2d_array&r, const ap::real_2d_array& b, ap::real_2d_array& result);
void solve_linear_equation_1D(const ap::real_2d_array&r, const ap::real_1d_array& b, ap::real_1d_array& result);
#endif
