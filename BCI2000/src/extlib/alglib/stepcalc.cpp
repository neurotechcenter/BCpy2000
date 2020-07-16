#include "stepcalc.h"
#define TRUE 1
#define FALSE 0

///////////////////////////////////////////////////////////////////
/// Apply the Stepwise Linear Discriminant Analysis (SWLDA) classifier
/// to a given data. SWLDA models a response variable as a function of 
/// the predictor variables represented by the columns of the input data.
/// The provided data must have ROWS > COLUMS.
/// @param [in] X			Given data
/// @param [in] y			Response variable
/// @param [out] in			Logical vector indicating which predictors are in the final model
/// @param [out] B			Vector of estimated coefficient values for all columns of X
/// @param [out] SE			Vector os standard errors of B
/// @param [out] PVAL		Vector of p-values for testing if B is 0
/// \author Cristhian Potes
/// \date May 30, 2009
/// Reference: Draper, N. R., and H. Smith. Applied Regression Analysis, Jhon Wiley & Sons, 1966. pp. 173-216 

void stepcalc(const ap::real_2d_array& X, 
			  const ap::real_1d_array& y, 
			  ap::boolean_1d_array& in,
			  ap::real_1d_array& B,
			  ap::real_1d_array& SE,
			  ap::real_1d_array& PVAL)
{
////////////////////////////////////////////////////////////////////////
// Section: Define variables 
int dfe, dfo, rows_in, cols_in, rows_out, cols_out, df_out, c_in, 
	c_out, m, n, i;
bool fin, fout, io, trans;
double	SS_total, SS_res, SS_reg, rmse, alpha, beta, ptemp_in, 
		ptemp_out, tval;

ap::real_2d_array x;
ap::real_2d_array x_in;
ap::real_2d_array x_in_temp;
ap::real_2d_array x_out;
ap::real_2d_array x_out_temp;
ap::real_2d_array Q;
ap::real_2d_array QT;
ap::real_2d_array I;
ap::real_2d_array R;
ap::real_2d_array Rinv;
ap::real_2d_array xr;
ap::real_2d_array xr_;
ap::real_2d_array vect;
ap::real_2d_array xx_;
ap::real_2d_array res_out;
ap::real_1d_array tau;
ap::real_1d_array yr;
ap::real_1d_array res;
ap::real_1d_array y_;
ap::real_1d_array b_in;
ap::real_1d_array ones;
ap::real_1d_array se_in;
ap::real_1d_array se_out;
ap::real_1d_array work;
ap::real_1d_array vect_temp;
ap::real_1d_array y_temp;
ap::real_1d_array yr_;
ap::real_1d_array yr_copy;
ap::real_1d_array xx;
ap::real_1d_array b_out;
	

////////////////////////////////////////////////////////////////////////
// Section: Get dimensions of the data
m = X.gethighbound(1)+1;
n = X.gethighbound(0)+1;

////////////////////////////////////////////////////////////////////////
// Section: Find in variables and allocate memory space in x_in. Fill the first 
//			column of x_in with ones

// Create a vector of ones
ones.setbounds(0,m-1);
for (i=0; i<m; i++)
	ones(i) = 1;

fin = in_out_variable(in, X, x, io=TRUE);
if (fin)
{
	x_in.setbounds(0,m-1,0,x.gethighbound(0)+1);
	ap::vmove(x_in.getcolumn(0,0,m-1), ones.getvector(0,m-1));
	for (i=0; i<x_in.gethighbound(0)+1; i++)
		ap::vmove(x_in.getcolumn(i+1,0,m-1), x.getcolumn(i,0,m-1));
}	
else
{
	x_in.setbounds(0,m-1,0,0);
	ap::vmove(x_in.getcolumn(0,0,m-1), ones.getvector(0,m-1));
}
////////////////////////////////////////////////////////////////////////
// Section: Solving a linear system using QR factorization 
rows_in = x_in.gethighbound(1);
cols_in = x_in.gethighbound(0);
x_in_temp = x_in;
Q.setbounds(0,rows_in,0,cols_in);
QT.setbounds(0, cols_in, 0, rows_in);
R.setbounds(0,rows_in,0,cols_in);
tau.setbounds(0, ap::minint(rows_in,cols_in));
// Decompose the matrix x_in_temp using QR factorization
rmatrixqr(x_in_temp, rows_in+1, cols_in+1, tau);
// Unpack Q from the overwritten matrix x_in_temp
rmatrixqrunpackq(x_in_temp,rows_in+1, cols_in+1, tau,cols_in+1, Q);
// Unpack R from the overwritten matrix x_in_temp
rmatrixqrunpackr(x_in_temp,rows_in+1, cols_in+1, R);
copyandtranspose(Q,0,rows_in,0,cols_in,QT,0,cols_in,0,rows_in);
b_in.setbounds(0,cols_in);
y_.setbounds(0,cols_in);
matrixvectormultiply(QT,0,cols_in,0,rows_in,trans=FALSE,y,0,rows_in,alpha=1,y_,0,cols_in,beta=0);
// Solve the linear system b_in = R\(Q'*y) 
solve_linear_equation_1D(R, y_, b_in);
////////////////////////////////////////////////////////////////////////
// Section: Compute the residual res = y-X*b 
res.setbounds(0,rows_in);
matrixvectormultiply(x_in,0,rows_in,0,cols_in,trans,b_in,0,cols_in,alpha,res,0,rows_in,beta);
ap::vsub(res.getvector(0,rows_in), y.getvector(0,rows_in));
////////////////////////////////////////////////////////////////////////
// Section: Compute Sum of Squares SS
yr.setbounds(0,rows_in);
remmean_col_vector(y, yr);
SS_total = pow(vectornorm2(yr, 0, rows_in),2);
SS_res = pow(vectornorm2(res,0,rows_in),2);
SS_reg = SS_total-SS_res;
////////////////////////////////////////////////////////////////////////
// Section: Compute degrees of freedom 
dfe = m - (cols_in + 1);
dfo = cols_in;
////////////////////////////////////////////////////////////////////////
// Section: Compute the rmse 
rmse = sqrt(SS_res/dfe);
////////////////////////////////////////////////////////////////////////
// Section: Compute the standard error of in variables 
I.setbounds(0,rows_in, 0, cols_in);
Rinv.setbounds(0, cols_in, 0, cols_in);
se_in.setbounds(0,cols_in);
matrixidentity(I);
solve_linear_equation_2D(R, I, Rinv);
matrix_pow(Rinv, Rinv, 2);
sum_matrix(Rinv, se_in, 2);
vector_sqrt(se_in, se_in);
ap::vmul(se_in.getvector(0,cols_in),rmse);
////////////////////////////////////////////////////////////////////////
// Section: Find out variables and allocate memory space in x_out
fout = in_out_variable(in, X, x, io=FALSE);	
if (fout)
	{
		x_out.setbounds(0,m-1,0,x.gethighbound(0));
		for (i=0; i<x_out.gethighbound(0)+1; i++)
			ap::vmove(x_out.getcolumn(i,0,m-1), x.getcolumn(i,0,m-1));

		// Compute separate added-variable coeffs and their standard errors 
		rows_out = x_out.gethighbound(1);
		cols_out = x_out.gethighbound(0);
		x_out_temp = x_out;
		xr.setbounds(0,rows_out,0,cols_out);
		vect.setbounds(0,cols_in,0,cols_out);
		xr_.setbounds(0,rows_out,0,cols_out);
		se_out.setbounds(0,cols_out);
		work.setbounds(0,rows_out+1);
		// remove effect of "in" predictors on "out" predictors 	
		matrixmatrixmultiply(QT, 0, cols_in, 0, rows_in, FALSE,
							x_out, 0, rows_out, 0, cols_out, FALSE, alpha=1,
							vect, 0, cols_in, 0, cols_out, beta=0, work);
		matrixmatrixmultiply(Q, 0, rows_in, 0, cols_in,  FALSE,
							vect, 0, cols_in, 0, cols_out,FALSE,alpha=1,
							xr_, 0, rows_out, 0, cols_out, beta=0, work);
		for (i=0; i<cols_out+1; i++)
		{
			ap::vsub(x_out_temp.getcolumn(i,0,rows_out), xr_.getcolumn(i,0,rows_out));
			ap::vmove(xr.getcolumn(i,0,rows_out), x_out_temp.getcolumn(i,0,rows_out));
		}
		// remove effect of "in" predictors on response 
		vect_temp.setbounds(0,cols_in);
		y_temp.setbounds(0,rows_out);
		yr_.setbounds(0,rows_out);
		yr_ = y;
		matrixvectormultiply(QT,0, cols_in, 0, rows_in, FALSE, y, 0, rows_out, alpha=1, vect_temp, 0, cols_in, beta=0);
		matrixvectormultiply(Q,0, rows_in, 0, cols_in, FALSE, vect_temp, 0, cols_in, alpha=1, y_temp, 0, rows_out, beta=0);
		ap::vsub(yr_.getvector(0,rows_out), y_temp.getvector(0,rows_out));

		xx_.setbounds(0,rows_out,0,cols_out);
		xx.setbounds(0,cols_out);
		matrix_pow(xr,xx_,2);
		sum_matrix(xx_, xx, 1);
		// compute b_out
		b_out.setbounds(0,cols_out);
		work.setbounds(0,cols_out);
		matrixvectormultiply(x_out,0, rows_out, 0, cols_out,TRUE,yr_,0,rows_out,alpha=1,work,0,cols_out,beta=0);
		
		for (i=0; i<cols_out+1; i++)
			b_out(i) = work(i)/xx(i);

		// compute res_out 
		yr_copy = yr_;
		res_out.setbounds(0,rows_out,0,cols_out);
		for (i=0; i<cols_out+1; i++)
		{
			ap::vmul(xr.getcolumn(i,0,rows_out),b_out(i));
			ap::vsub(yr_copy.getvector(0,rows_out),xr.getcolumn(i,0,rows_out));
			ap::vmove(res_out.getcolumn(i,0,rows_out),yr_copy.getvector(0,rows_out));
			yr_copy = yr_;
		}
		// Compute the degree of freedom out variables 
		df_out = ap::maxint(0, dfe-1);

		// Compute the standard error out variables 
		matrix_pow(res_out, res_out, 2);
		sum_matrix(res_out, work, 1);
		ap::vmul(work.getvector(0, cols_out), ((double) 1)/df_out);
		vector_sqrt(work, work);
		for (i=0; i<cols_out+1; i++)
			se_out(i) = work(i)/sqrt(xx(i));											   
	}
////////////////////////////////////////////////////////////////////////
// Section: Combine in/out coefficients and standard error 
c_in = 0; 
c_out = 0;
for (i=0; i<n; i++)
{
	if (in(i)==TRUE)
	{
		B(i) = b_in(c_in + 1);
		SE(i) = se_in(c_in + 1);
		if (dfe>0)
		{
			tval = B(i)/SE(i);
			ptemp_in = tcdf(static_cast<float>(tval),dfe);
			ptemp_in = 2*ap::minreal(ptemp_in, 1-ptemp_in);				
			PVAL(i) = ptemp_in;
		}
		c_in++;
	}
	else
	{
		B(i) = b_out(c_out);
		SE(i) = se_out(c_out);
		if (dfe>1)
		{
			tval = B(i)/SE(i);
			ptemp_out = tcdf(static_cast<float>(tval),dfe-1);
			ptemp_out = 2*ap::minreal(ptemp_out, 1-ptemp_out);
			PVAL(i) = ptemp_out;
		}
		c_out++;			
	}
}
}
