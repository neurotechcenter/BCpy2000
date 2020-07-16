#include <cmath>

/* If you want something to work on all platforms and all compilers, you have to program it yourself... */
#include "Gaussian.h"

const double pi = 3.1415926535897932;

double polynomial(double x, double *coeffs, unsigned short order)
{
    unsigned short i;
    double result;
    for(result = coeffs[0], i = 1; i <= order; i++) result = result * x + coeffs[i];
    return result;
}

double erf(double x)
{
    double y, coeffs[6] = {1.061405429e+00, -1.453152026e+00, 1.421413741e+00, -2.844967366e-01, 2.548295918e-01, 0.0};

    if(x == 0.0) return 0.0;
    y = exp(-x * x) * polynomial(1.0 / (1.0 + 3.275911166e-01 * fabs(x)), coeffs, 5);
    return ((x < 0.0) ? y - 1.0 : 1.0 - y);
}

double erf_inv(double y)
{
    double ya, ys, x, z;
    double nc1[4] = {-0.140543331, 0.914624893, -1.64534962, 0.886226899};
    double dc1[5] = {0.012229801, -0.329097515, 1.44271046, -2.11837773, 1.0};
    double nc2[4] = {1.64134531, 3.4295678, -1.62490649, -1.97084045};
    double dc2[3] = {1.6370678, 3.5438892, 1.0};

    if(y == 0.0) return 0.0;
    ya = fabs(y);
    ys = ((y < 0.0) ? -1.0 : (y > 0.0) ? 1.0 : 0.0);
    // if(ya == 1.0) return ys * INF;  // need INF definition (major headache to make cross-platform/cross-compiler)
    // if(ya > 1.0) return NAN; // need NAN definition (major headache to make cross-platform/cross-compiler)

    if(ya <= 0.7) {
        z = y * y;
        x = y * polynomial(z, nc1, 3) / polynomial(z, dc1, 4);
    }
    else {
        z = sqrt(-log((1.0 - ya) / 2.0));
        x = ys * polynomial(z, nc2, 3) / polynomial(z, dc2, 2);
    }
    x -= (erf(x) - y) / (exp(-x * x) * 2.0 / sqrt(pi));
    x -= (erf(x) - y) / (exp(-x * x) * 2.0 / sqrt(pi));

    return x;
}

double InverseCumulativeGaussian(double x)
{
	return sqrt(2.0) * erf_inv(2.0 * x - 1.0);
}
