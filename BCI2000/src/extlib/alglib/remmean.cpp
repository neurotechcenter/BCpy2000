#include "remmean.h"


extern void remmean_matrix(const ap::real_2d_array& matrix, ap::real_2d_array& matrix_rem)
{
	unsigned int rows, cols, i;
	double val;
	ap::real_2d_array matrix_copy;
	ap::real_2d_array matrix_r;
	ap::real_1d_array vect_sum;
	rows = matrix.gethighbound(1)+1;
	cols = matrix.gethighbound(0)+1;
	vect_sum.setbounds(0,cols-1);
	sum_matrix(matrix, vect_sum, 1);
	val = ((double)1)/rows;
	matrix_copy.setbounds(0,rows-1,0,cols-1);
	matrix_r.setbounds(0,rows-1,0,cols-1);
	matrix_r = matrix;
	ap::vmul(vect_sum.getvector(0,cols-1),val);
	for (i=0; i<rows; i++)
		ap::vmove(matrix_copy.getrow(i,0,cols-1), vect_sum.getvector(0,cols-1));
	
	for (i=0; i<cols; i++)
	{
		ap::vsub(matrix_r.getcolumn(i,0,rows-1), matrix_copy.getcolumn(i,0,rows-1));
		ap::vmove(matrix_rem.getcolumn(i,0,rows-1), matrix_r.getcolumn(i,0,rows-1));
	}
}
/**********************************************************************************************************/
extern void remmean_col_vector(const ap::real_1d_array &vect, ap::real_1d_array& vect_rem)
{
	double val;
	unsigned int rows, i;
	ap::real_1d_array vect_copy;
	rows = vect.gethighbound(1)+1;
	vect_copy.setbounds(0,rows-1);
	vect_copy = vect;
	val = sum_col_vector(vect)/rows;
	for (i=0; i<rows; i++)
		vect_rem(i) = (double)1;
	ap::vmul(vect_rem.getvector(0,rows-1),val);
	ap::vsub(vect_copy.getvector(0,rows-1),vect_rem.getvector(0,rows-1));
	vect_rem = vect_copy;
}
/**********************************************************************************************************/
extern double sum_col_vector(const ap::real_1d_array &vect)
{
	unsigned int i, rows;
	double sum = 0;
	rows = vect.gethighbound(1)+1;
	for (i=0; i<rows; i++)
		sum += vect(i);

	return sum;
}
/**********************************************************************************************************/
extern double sum_row_vector(const ap::real_1d_array &vect)
{
	unsigned int i, rows;
	double sum = 0;
	rows = vect.gethighbound(0)+1;
	for (i=0; i<rows; i++)
		sum += vect(i);

	return sum;
}
/**********************************************************************************************************/
extern void sum_matrix(const ap::real_2d_array &matrix, ap::real_1d_array &vect_sum, unsigned int dim)
{
	unsigned int i, rows, cols;
	double sum = 0;
	cols = matrix.gethighbound(0)+1;
	rows = matrix.gethighbound(1)+1;
	ap::real_1d_array vect_temp; 
	if (dim==1)
	{
		vect_temp.setbounds(0,rows-1);
		for (i=0; i<cols; i++)
		{
			ap::vmove(vect_temp.getvector(0,rows-1), matrix.getcolumn(i,0,rows-1));
			vect_sum(i) = sum_col_vector(vect_temp);
		}
	}
	if (dim==2)
	{
		vect_temp.setbounds(0,cols-1);
		for (i=0; i<rows; i++)
		{
			ap::vmove(vect_temp.getvector(0,cols-1), matrix.getrow(i,0,cols-1));
			vect_sum(i) = sum_row_vector(vect_temp);
		}
	}
}
/**********************************************************************************************************/
extern void matrix_pow(const ap::real_2d_array& src, ap::real_2d_array& dst, int power)
{
	unsigned int i, j, rows, cols;
	cols = src.gethighbound(0)+1;
	rows = src.gethighbound(1)+1;

	for (i=0; i<rows; i++)
	{
		for (j=0; j<cols; j++)
			dst(i,j) = pow(src(i,j),power);
	}
}

/**********************************************************************************************************/
extern void vector_pow(const ap::real_1d_array& src, ap::real_1d_array& dst, int power)
{
	unsigned int i, rows;
	rows = src.gethighbound(0)+1;

	for (i=0; i<rows; i++)
			dst(i) = pow(src(i),power);
}
/**********************************************************************************************************/
extern void matrix_sqrt(const ap::real_2d_array& src, ap::real_2d_array& dst)
{
	unsigned int i, j, rows, cols;
	cols = src.gethighbound(0)+1;
	rows = src.gethighbound(1)+1;

	for (i=0; i<rows; i++)
	{
		for (j=0; j<cols; j++)
			dst(i,j) = sqrt(src(i,j));
	}
}
/**********************************************************************************************************/
extern void vector_sqrt(const ap::real_1d_array& src, ap::real_1d_array& dst)
{
	unsigned int i, rows;
	rows = src.gethighbound(0)+1;

	for (i=0; i<rows; i++)
		dst(i) = sqrt(src(i));
}
/**********************************************************************************************************/
extern void vector_abs(const ap::real_1d_array& src, ap::real_1d_array& dst)
{
	unsigned int i, rows;
	rows = src.gethighbound(0)+1;

	for (i=0; i<rows; i++)
		dst(i) = abs(src(i));
}
/**********************************************************************************************************/
extern double matrix_get_max_element(const ap::real_2d_array& src)
{
    unsigned int row, col, rows, cols;
    double max_value = -1e100;
    double element_value;
	cols = src.gethighbound(0)+1;
	rows = src.gethighbound(1)+1;

    /* find the maximum value in the matrix */
    for (row=0; row<rows; row++)
    {
        for (col=0; col<cols; col++)
        {
            /* get element value */
            element_value = src(row, col);
            
            /* make absolute value */
            /*if (element_value < 0.0)
                element_value = -element_value;*/

            /* compare with max */
            if (element_value > max_value)
                max_value = element_value;
        }
    }

    /* return the maximum element value (absolute value) */
    return max_value;
}

/**********************************************************************************************************/
extern unsigned int vect_get_max_element(const ap::real_1d_array& src, ap::real_1d_array& indx_in)
{
    unsigned int row, rows;
    unsigned int index;
    double max_value = -1e100;
    double element_value;
    rows = src.gethighbound(0)+1;

    /* find the maximum value in the matrix */
    for (row=0; row<rows; row++)
    {
        /* get element value */
        element_value = src(row);
            
        /* make absolute value */
        //if (element_value < 0.0)   // correct it
        //    element_value = -element_value;

        /* compare with max */
        if (element_value > max_value)
        {
            max_value = element_value;
            //index = row;
            index = static_cast<unsigned int>( indx_in(row) );
        }        
    }
	return index;
}
/**********************************************************************************************************/
extern unsigned int vect_get_min_element(const ap::real_1d_array& src, ap::real_1d_array& indx_in)
{
    unsigned int row, rows;
    unsigned int index = 0;
    double min_value = 1e+100;
    double element_value;
    rows = src.gethighbound(0)+1;

    /* find the minimum value in the matrix */
    for (row=0; row<rows; row++)
    {
        /* get element value */
        element_value = src(row);
            
        /* make absolute value */
        /*if (element_value < 0.0)
            element_value = -element_value;*/

        /* compare with min */
        if (element_value < min_value)
        {
            min_value = element_value;
            //index = row;
            index = static_cast<unsigned int>( indx_in(row) );
        }        
    }
	return index;
}
