#include "solve_linear_equation.h"
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Solve a linear system Ax=b. The matrix A must be decomposed using QR factorization.
/// This function solve Rx = Q'b using back substitution.
/// @param [in] r		R matrix obtained from the QR factorization
/// @param [in] b		b matrix
/// @param [out] result	x matrix
/// \author Cristhian Potes
/// \date May 15, 2009
void solve_linear_equation_2D(const ap::real_2d_array&r, const ap::real_2d_array& b, ap::real_2d_array& result)
 //Implement a routine to solve a linear system Rx=Q'b (QR factorization and A(m,n) where m<n). 
 //xi = (bi - sum<i=j+1,n> (R(i,j)x(j)))/R(i,i) 
{
	double total=0, tol=1e-6;
	bool FlagWarning = 0;
	int rows_r, cols_r, cols_b, row, i, j;
	rows_r = r.gethighbound(1)+1;
	cols_r = r.gethighbound(0)+1;
	cols_b = b.gethighbound(0)+1;

	for (i=0; i<cols_b; i++)
	{
		for (row=(cols_r-1); row>=0; row--)
		{
			if (row==(cols_r-1))
			{
				if ((r(row,row) < tol) && (r(row,row)>-tol))
				{
					if (r(row,row) == 0)
						result(row,i) = b(row,i) / tol;	

					FlagWarning = 1;					
				}
				else
					result(row,i) = b(row,i) / r(row,row);	
			}
			else
			{
				for (j=row+1; j<cols_r; j++)
					total += r(row,j) * result(j,i);
				
				if ((r(row,row) < tol) && (r(row,row)>-tol))
				{
					if (r(row,row) == 0)
						result(row,i) = b(row,i) / tol;	

					FlagWarning = 1;						
				}
				else
					result(row,i) = (b(row,i)-total) / r(row,row);
				
				total=0;
			}
		}
	}
	if (FlagWarning)
		printf("Warning: Matrix close to singular. Results might be innacurate. \n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Solve a linear system Ax=b. The matrix A must be decomposed using QR factorization.
/// This function solve \f$Rx = Q^{T}b\f$ using back substitution.
/// @param [in] r		R matrix obtained from the QR factorization
/// @param [in] b		b vector
/// @param [out] result	x vector
/// \author Cristhian Potes
/// \date May 15, 2009
void solve_linear_equation_1D(const ap::real_2d_array&r, const ap::real_1d_array& b, ap::real_1d_array& result)
 /*Implement a routine to solve a linear system Rx=Q'b (QR factorization and A(m,n) where m<n). 
 xi = (bi - sum<i=j+1,n> (R(i,j)x(j)))/R(i,i) */
{
	double total=0, tol=1e-6;
	bool FlagWarning = 0;
	int rows_r, cols_r, cols_b, row, i, j;
	rows_r = r.gethighbound(1)+1;
	cols_r = r.gethighbound(0)+1;
	cols_b = b.getlowbound(1)+1;

	for (i=0; i<cols_b; i++)
	{
		for (row=(cols_r-1); row>=0; row--)
		{
			if (row==(cols_r-1))
			{
				if ((r(row,row) < tol) && (r(row,row)>-tol))
				{
					if (r(row,row) == 0)
						result(row) = b(row) / tol;	

					FlagWarning = 1;					
				}
				else
					result(row) = b(row) / r(row,row);	
			}
			else
			{
				for (j=row+1; j<cols_r; j++)
					total += r(row,j) * result(j);
						
				if ((r(row,row) < tol) && (r(row,row)>-tol))
				{
					if (r(row,row) == 0)
						result(row) = (b(row)-total) / tol;	

					FlagWarning = 1;					
				}
				else
					result(row) = (b(row)-total) / r(row,row);
				
				total=0;
			}
		}
	}
	if (FlagWarning)
		printf("Warning: Matrix close to singular. Results might be innacurate. \n");
}
	

