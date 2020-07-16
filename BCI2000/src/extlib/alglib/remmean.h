#ifndef _remmean_h_
#define _remmean_h_

#include <math.h>
#include "blas.h"
#include "ap.h"


extern void remmean_matrix(const ap::real_2d_array& matrix, ap::real_2d_array& matrix_rem);
extern void remmean_col_vector(const ap::real_1d_array &vect, ap::real_1d_array& vect_rem);
extern double sum_col_vector(const ap::real_1d_array &vect);
extern double sum_row_vector(const ap::real_1d_array &vect);
extern void sum_matrix(const ap::real_2d_array &matrix, ap::real_1d_array &vect_sum, unsigned int dim);
extern void matrix_pow(const ap::real_2d_array& src, ap::real_2d_array& dst, int power);
extern void matrix_sqrt(const ap::real_2d_array& src, ap::real_2d_array& dst);
extern void vector_pow(const ap::real_1d_array& src, ap::real_1d_array& dst, int power);
extern void vector_sqrt(const ap::real_1d_array& src, ap::real_1d_array& dst);
extern void vector_abs(const ap::real_1d_array& src, ap::real_1d_array& dst);
extern double matrix_get_max_element(const ap::real_2d_array& src);
extern unsigned int vect_get_max_element(const ap::real_1d_array& src, ap::real_1d_array& indx_in);
extern unsigned int vect_get_min_element(const ap::real_1d_array& src, ap::real_1d_array& indx_in);
extern unsigned int find_element(const ap::real_1d_array& src, const double element);

#endif

