#include "stepnext.h"
#define TRUE 1
#define FALSE 0
///////////////////////////////////////////////////////////////////
/// Figure out the next step for the SWLDA. This function returns an
/// integer indicating which variable should be added or removed from
/// the model.
/// @param [in] in        Logical vector indicating which predictors are in model
/// @param [in] PVAL      Vector of p-values
/// @param [in] penter    parameter penter
///	@param [in] premove   parameter premove
/// \author Cristhian Potes
/// \date June 05, 2009

int stepnext(const ap::boolean_1d_array& in, const ap::real_1d_array& PVAL, const double penter, const double premove)
{
  ///////////////////////////////////////////////////////////////////
  // Section: Define variables
  ap::real_1d_array pval_out;
  ap::real_1d_array pval_in;
  ap::real_1d_array indx_in;
  ap::real_1d_array indx_out;
  unsigned int index_pval;
  bool fout, fin, io;
  int swap = -1;

  // Section: Extract the indices of in and out variables
  fin = in_out_variable_1D(in, PVAL, pval_in, indx_in, io=TRUE);
  fout = in_out_variable_1D(in, PVAL, pval_out, indx_out, io=FALSE);

  // Section: Figure out which variables must be added or removed from the model
  if(fout)
  {
    index_pval = vect_get_min_element(pval_out, indx_out);

    if (PVAL(index_pval)<penter)
      swap = index_pval;
  }
  if (swap==-1)
  {
    if (fin)
    {
      index_pval = vect_get_max_element(pval_in, indx_in);
      if (PVAL(index_pval)>premove)
        swap=index_pval;
    }
  }
  return swap;
}

