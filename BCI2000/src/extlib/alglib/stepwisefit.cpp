#include "stepwisefit.h"
#include <iostream>
#include <limits>
#include <cassert>

#define TRUE 1
#define FALSE 0

using namespace std;

static void Normalize( const ap::real_2d_array& inX, const ap::real_1d_array& inY, ap::real_1d_array& ioB )
{ // LDA coefficients are only determined up to a multiplication constant.
  // This function rescales the coefficient vector such that within-class variance of the training data
  // is unity, allowing an interpretation of classification scores in terms of a log-likelihood ratio
  // ("evidence ratio").
  const ap::real_1d_array& Y = inY;
  ap::real_1d_array X;
  X.setbounds( inX.getlowbound(1), inX.gethighbound(1) );
  assert( inX.getlowbound(2) == ioB.getlowbound() && inX.gethighbound(2) == ioB.gethighbound() );
  for( int i = inX.getlowbound(1); i <= inX.gethighbound(1); ++i )
    for( int j = inX.getlowbound(2); j <= inX.gethighbound(2); ++j )
      X(i) += inX(i,j) * ioB(j);

  double P0 = 0,
         P1X = 0, P1Y = 0,
         P2XX = 0, P2XY = 0, P2YY = 0;
  assert( X.getlowbound() == Y.getlowbound() && X.gethighbound() == Y.gethighbound() );
  for( int i = X.getlowbound(); i <= X.gethighbound(); ++i )
  {
    P0 += 1;
    P1X += X(i);
    P1Y += Y(i);
    P2XX += X(i)*X(i);
    P2XY += X(i)*Y(i);
    P2YY += Y(i)*Y(i);
  }
  static const double eps = numeric_limits<double>::epsilon();
  if( P0 > eps )
  {
    double covXX = (P2XX - P1X*P1X/P0)/P0,
           covXY = (P2XY - P1X*P1Y/P0)/P0,
           covYY = (P2YY - P1Y*P1Y/P0)/P0,
           withinClassVar = covYY < eps ? 0 : covXX - covXY * covXY / covYY;
    if( withinClassVar > eps )
    {
      double sdev = ::sqrt( withinClassVar );
      for( int i = ioB.getlowbound(); i <= ioB.gethighbound(); ++i )
        ioB(i) /= sdev;
    }
  }
}

///////////////////////////////////////////////////////////////////
/// Apply the Stepwise Linear Discriminant Analysis (SWLDA) classifier
/// to a given data. SWLDA models a response variable as a function of
/// the predictor variables represented by the columns of the input data.
/// The provided data must have ROWS > COLUMS.
/// @param [in] X         Given data
/// @param [in] y         Response variable
/// @param [in] penter    parameter penter
///	@param [in] premove   parameter premove
/// @param [in] maxiter   parameter Maximum number of features
/// @param [out] B        Vector of estimated coefficient values for all columns of X
/// @param [out] SE       Vector os standard errors of B
/// @param [out] PVAL     Vector of p-values for testing if B is 0
/// @param [out] in       Logical vector indicating which predictors are in the final model
/// \author Cristhian Potes
/// \date May 30, 2009
/// Reference: Draper, N. R., and H. Smith. Applied Regression Analysis, John Wiley & Sons, 1966. pp. 173-216

void stepwisefit(const ap::real_2d_array& X, const ap::real_1d_array& y, const double penter, const double premove,
                 const int max_iter, ap::real_1d_array& B, ap::real_1d_array& SE, ap::real_1d_array& PVAL,
                 ap::boolean_1d_array& in, CALLBACK_STATUS callback_status)
{
  ///////////////////////////////////////////////////////////////////
  // Section: Define variables
  bool FLAG = TRUE;
  int swap;
  int iter=1;
  ostringstream oss;
  ///////////////////////////////////////////////////////////////////
  // Section: Compute B, SE, PVAL, and the next step
  while(FLAG==TRUE)
  {
    if (callback_status != NULL)
    {
      oss << "Added feature... " << "[" << iter << "/" << max_iter << "]";
      callback_status(oss.str());
      oss.str("");
    }
    else
      printf("Added feature... %d\n", iter);

    stepcalc(X, y, in, B, SE, PVAL);
    swap = stepnext(in, PVAL, penter, premove);
    if (swap==-1)
      FLAG=FALSE;
    else
      in(swap)=(!(in(swap)));

    iter++;
    if (iter>max_iter)
      FLAG = FALSE;
  }
  Normalize( X, y, B );
  printf("\n");
}
