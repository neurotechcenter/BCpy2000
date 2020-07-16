#include "CARfilter.h"
/// Apply common average reference to all the channels of the BCI data file.
/// @param [in] signal Signal to apply common average reference. 
/// Signal is overwritten.
/// \author Cristhian Potes
/// \date June 02, 2009

void CARfilter(ap::template_2d_array<float,true>& signal)
{
///////////////////////////////////////////////////////////////////
// Section: Define variables
int rows, cols, i, j;
float val;
ap::template_2d_array<float, true> matrix_copy;
ap::template_2d_array<float, true> matrix_r;
ap::template_2d_array<float, true> matrix_rem;
ap::template_1d_array<float, true> vect_sum;
ap::template_1d_array<float, true> vect_temp; 


///////////////////////////////////////////////////////////////////
// Section: Substract mean from all the channels
rows = signal.gethighbound(1)+1;
cols = signal.gethighbound(0)+1;
vect_sum.setbounds(0,rows-1);
vect_temp.setbounds(0,cols-1);
matrix_rem.setbounds(0, rows-1, 0, cols-1);

// Sum over all columns
for (i=0; i<rows; i++)
	{
		val = 0;
		ap::vmove(vect_temp.getvector(0,cols-1), signal.getrow(i,0,cols-1));
		for (j=0; j<cols; j++)
			val += vect_temp(j);
		
		vect_sum(i) = val;
	}

val = ((float)1)/cols;
ap::vmul(vect_sum.getvector(0,rows-1),val);

matrix_copy.setbounds(0,rows-1,0,cols-1);
matrix_r.setbounds(0,rows-1,0,cols-1);
matrix_r = signal;

for (i=0; i<cols; i++)
	ap::vmove(matrix_copy.getcolumn(i,0,rows-1), vect_sum.getvector(0,rows-1));
	
for (i=0; i<cols; i++)
{
	ap::vsub(matrix_r.getcolumn(i,0,rows-1), matrix_copy.getcolumn(i,0,rows-1));
	ap::vmove(matrix_rem.getcolumn(i,0,rows-1), matrix_r.getcolumn(i,0,rows-1));
}
signal = matrix_rem;
}

