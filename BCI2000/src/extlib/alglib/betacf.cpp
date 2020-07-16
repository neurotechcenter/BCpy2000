#include "betacf.h"
#define MAXIT 100
#define EPS 3.0e-7f
#define FPMIN 1.0e-30f

float betacf(float a, float b, float x)
{
	int m, m2;
	float aa, c, d, del, h, qab, qam, qap;

	qab=a+b;
	qap=a+1.0f;
	qam=a-1.0f;
	c=1.0f;
	d=1.0f-qab*x/qap;
	if (fabs(d) < FPMIN) d=FPMIN;
	d=1.0f/d;
	h=d;
	for (m=1; m<MAXIT; m++)
	{
		m2=2*m;
		aa=m*(b-m)*x/((qam+m2)*(a+m2));
		d=1.0f+aa*d;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=1.0f+aa/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0f/d;
		h*=d*c;
		aa=-(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
		d=1.0f+aa*d;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=1.0f+aa/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0f/d;
		del=d*c;
		h*=del;
		if (fabs(del-1.0f) < EPS) break;
	}
	if (m>MAXIT)
		nrerror("a or b too big, or MAXIT too small in betacf");

	return h;
}
