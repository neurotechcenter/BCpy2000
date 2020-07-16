#include "downsampling.h"
///////////////////////////////////////////////////////////////////////
/// Downsample a data sequence.
/// @param [in]  x			data sequence
/// @param [in]	 ds_factor	downsampling factor
///	@param [out] y			downsampled data
/// \author Cristhian Potes
/// \date June 3, 2009
void downsampling(const ap::template_1d_array<float, true>& x,
				  const int ds_factor,
				  ap::template_1d_array<float, true>& y)
{
	for(int i=0; i<y.gethighbound(0)+1; i++)
		y(i) = x(i*ds_factor);
}
