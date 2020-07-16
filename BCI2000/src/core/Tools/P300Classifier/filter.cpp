#include "filter.h"
///////////////////////////////////////////////////////////////////////
/// Filter a data sequence using a digital filter. The filter is a \e direct
/// \e form \e II \e transposed implementation of the standard difference 
/// equation.
/// @param [in] ord		order of the filter
/// @param [in] a		denominator coefficient vector \e a
/// @param [in] b		numerator coefficient vetor \e b
/// @param [in] np		number of points
/// @param [in] x		data sequence
/// @param [out] y		filtered data
/// \author Cristhian Potes
/// \date June 3, 2009

void filter(const int ord, const ap::template_1d_array<float, true>& a, 
			const ap::template_1d_array<float, true>& b, 
			const int np, const ap::template_1d_array<float, true>& x, 
			ap::template_1d_array<float, true>& y)
{
	/////////////////////////////////////////////////////////////
	// Section: Define variables
    int i,j;
	float val;
	ap::template_1d_array<float, true> tmp;
	tmp.setbounds(0, np);
	
	/////////////////////////////////////////////////////////////
	// Section: Perform feedback and feedforward filter
	
	// Create a vector of zeros
	for(i=0; i<np+1; i++)
		tmp(i) = (float) 0;
	
	y(0) = b(0)*x(0);

	for (i=1; i<ord+1; i++)
	{
        y(i) = 0.0;
        for (j=0; j<i+1; j++)
        	y(i) = y(i) + b(j) * x(i-j);

        for (j=0; j<i; j++)
        	y(i) = y(i) - a(j+1) * y(i-j-1);
	}

	for (i=ord+1; i<np+1; i++)
	{
		y(i) = 0.0;
			for (j=0; j<ord+1; j++)
				y(i) = y(i) + b(j) * x(i-j);
			for (j=0; j<ord; j++)
				y(i) = y(i) - a(j+1) * y(i-j-1);
	}

	// cumulative sum 
	val = 0;
	for(i=0; i<np+1; i++)
	{
		if (i<ord+1)
		{
			val += y(i);
			tmp(i) = val;
		}
		else
		{
			val += y(i);
			val = val - y(i-(ord+1));
			tmp(i) = val;
		}
	}
	y = tmp;
} 
