#include "in_out_variable.h"
#define TRUE 1
#define FALSE 0

///////////////////////////////////////////////////////////////////////
/// This program extracts variables (represented as column vectors) from a 
/// matrix X. These variables are extracted using a logical vector 'in'
/// indicating which variables are 'in' or 'out'. The extracted variables
/// are represented in a matrix x. 
/// @param [in] in	A logical vector of size nx1 indicating which variables are 
///					'in' or 'out' according to the boolean input io. e.g in = [0 1 0 1].
/// @param [in]	X	A model matrix X of m rows and n columns with m > n.
/// @param [in] io	If io = 1, then the algorithm extracts only the variables 
///					that are reprensented in 'in' with 1's. If io = 0, then the 
///					algorithm extracts only the variables that are represented 
///					in 'in' with 0's.
/// @param [out] x	A matrix x of m rows and l columns representing the extracted
///					variables.

/// \author	Cristhian Potes	
///	\date June 16, 2009
/// \e-mail:	cmpotes@gmail.com
		
bool in_out_variable(const ap::boolean_1d_array& in, const ap::real_2d_array& X, ap::real_2d_array& x, bool io)
{
	//////////////////////////////////////////////////////////////////
	// Section: Define variables
	int rows = in.gethighbound(0) + 1;
	bool flag;
	vector<int> stdVector;
	//////////////////////////////////////////////////////////////////
	// Section: Identify how many variables are in or out

	for (int i=0; i<rows; i++)
	{
		if (in(i)==io) 
			stdVector.push_back(i);
	}
	if (stdVector.size()>0)
	{
		// Routine to extract the in/out variables
		x.setbounds(0,X.gethighbound(1),0,static_cast<int>(stdVector.size())-1);
		for (size_t i=0; i<stdVector.size(); i++)
			ap::vmove(x.getcolumn(static_cast<int>(i),0,X.gethighbound(1)), X.getcolumn(stdVector[i],0,X.gethighbound(1)));
	
        flag=TRUE;
	}
	else
		flag=FALSE;

	return flag;
}
/**********************************************************************************************************/
bool in_out_variable_1D(const ap::boolean_1d_array& in, const ap::real_1d_array& X, ap::real_1d_array& x, ap::real_1d_array& vector,  bool io)
{
// Routine to know the number of variables in/out
	int rows = in.gethighbound(0) + 1;
	int n_invar=0;
	bool flag;
	
	//ap::real_1d_array vector;
	vector.setbounds(0,rows-1);
	unsigned int k=0;
	for (int i=0; i<rows; i++)
	{
		if (in(i)==io) //to know how many variables are in/out
		{
			vector(k) = i;
			k++;
			n_invar++;
		}
	}
	if (n_invar>0)
	{
		// Routine to extract the in/out variables
		x.setbounds(0,n_invar-1);
		for (int i=0; i<n_invar; i++)
		x(i) = X(static_cast<int>(vector(i)));
	
        flag=TRUE;
	}
	else
		flag=FALSE;

	return flag;
}
