#ifndef _stepcalc_h_
#define _stepcalc_h_

#include <math.h>
#include "in_out_variable.h"
#include "solve_linear_equation.h"
#include "ap.h"
#include "qr.h"
#include "blas.h"
#include "remmean.h"
#include "tcdf.h"

void stepcalc(const ap::real_2d_array& X,
              const ap::real_1d_array& y,
              ap::boolean_1d_array& in,
              ap::real_1d_array& B,
              ap::real_1d_array& SE,
              ap::real_1d_array& PVAL);
#endif

