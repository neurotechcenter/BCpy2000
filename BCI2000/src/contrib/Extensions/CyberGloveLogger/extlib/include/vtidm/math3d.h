#ifndef VT_MATH3D_H
#define VT_MATH3D_H

#ifdef __cplusplus
extern "C" {
#endif
#include "types.h"

#include <math.h>

/*-----------------------*/
/* some useful constants */
/*-----------------------*/
#undef  M_PI
#define M_PI                3.14159265358979323846

#undef M_2PI
#define M_2PI               6.28318530717959

#undef M_PI_2
#define M_PI_2              1.57079632679489661923

#undef M_PI_4
#define M_PI_4              0.78539816339744830962

#undef ABS
#define ABS(x) ((x<0.0)?(-(x)):(x))

#define epsilon 0.000001
#define EPSILON 0.01

#undef RAD2DEG
#define RAD2DEG 57.28578

#undef DEG2RAD
#define DEG2RAD 0.017453293

#undef VX
#define VX 0

#undef VY
#define VY 1

#undef VZ
#define VZ 2

#undef AZ 
#define AZ 0

#undef EL 
#define EL  1

#undef ROLL
#define ROLL 2

enum {
    VT_EULER_XYZ, VT_EULER_ZYX
};

/* somehow the system version always return NaN!!!*/
#define COPYSIGN(x, y)         (fabs(x) * (((y) > 0)?1.0:-1.))

typedef double vt_m3[3][3];	/* 3x3 matrix */
typedef double vt_m4[4][4];	/* 4x4 matrix */
typedef double vt_p3[3];	/* 3d points */
typedef double vt_p2[2];	/* 2d points */
typedef double vt_p6[6];	/* 6d points */
typedef double vt_q[4];		/* quaternion */
typedef struct {
    vt_p3 p;
    vt_q q;
} vt_pq;

VT_DLL_PROC vt_m4 VT_M4IDENTITY;
VT_DLL_PROC vt_m4 VT_M4ZERO;

#define _ISZERO(exp)   ((exp) == 0.)

/* set */
#define VT_Pos3Set(p3, a, b, c)  p3[0] = a; p3[1] = b; p3[2] = c

/* copy operations: vector, quaternion, and matrix */
#define VT_Pos3Copy(pin, pout) memcpy(pout, pin, sizeof(vt_p3))
#define VT_Pos2Copy(pin, pout) memcpy(pout, pin, sizeof(vt_p2))
#define VT_Mat3Copy(min, mout) memcpy(mout, min, sizeof(vt_m3))
#define VT_Mat4Copy(min, mout) memcpy(mout, min, sizeof(vt_m4))
#define VT_QuatCopy(qin, qout) memcpy(qin, qout, sizeof(vt_q))
#define VT_PQCopy(in, out)     memcpy(out, in, sizeof(vt_pq))

/* test for vector/quaternion equality */
#define VT_Pos2Equal(p1, p2) ( _ISZERO((p1)[VX]-(p2)[VX]) && _ISZERO((p1)[VY]-(p2)[VY]) )

#define VT_Pos3Equal(p1, p2) ( _ISZERO((p1)[VX]-(p2)[VX]) && \
			_ISZERO((p1)[VY]-(p2)[VY]) && _ISZERO((p1)[VZ]-(p2)[VZ]) )

#define VT_QuatEqual(q1, q2) (( _ISZERO((q1)[0]-(q2)[0) && \
            _ISZERO((q1)[1]-(q2)[1]) && _ISZERO((q1)[2]-(q2)[2]) && \
            _ISZERO((q1)[3]-(q2)[3]) )   ||   \
            ( _ISZERO((q1)[0]+(q2)[0]) &&   \
            _ISZERO((q1)[1]+(q2)[1]) && _ISZERO((q1)[2]+(q2)[2]) && \
            _ISZERO((q1)[3]+(q2)[3]) ))

/* scalar-vector multiplication */
#define VT_Pos3Scale(v,s)  ( (v)[0]*=(s), (v)[1]*=(s), (v)[2]*=(s) )
#define VT_QuatScale(q, s) ((q)[0]*=(s),(q)[1]*=(s),(q)[2]*=(s),(q)[3] *= (s))


/* vector dot product */
#define VT_Pos3Dot(a,b)  ( a[VX]*b[VX] + a[VY]*b[VY] + a[VZ]*b[VZ] )
#define VT_Pos2Dot(a,b)   ( a[VX]*b[VX] + a[VY]*b[VY] )
#define VT_QuatDot(a,b) (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3])


/* initialization */
#define VT_Pos3Init(pin) ( (pin)[0] = (pin)[1] = (pin)[2] = 0.0 )
#define VT_Pos2Init(pin) ( (pin)[0] = (pin)[1] = 0.0 )
#define VT_QuatInit(q)   ( (q)[0]=(q)[1]=(q)[2]=0., (q)[3]=1.)
#define VT_PQInit(pq)    ( VT_Pos3Init(pq->p), VT_QuatInit(pq->q))

#define VT_Mat3Init(m) ( 	\
    m[0][0] = m[1][1] = m[2][2] = 1.0,  \
    m[0][1] = m[0][2] = m[1][2] = 0.0,	\
    m[1][0] = m[2][0] = m[2][1] = 0.0 ) 

#define VT_Mat4Init(m) ( 	\
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0,    \
    m[0][1] = m[0][2] = m[0][3] = 0.0,              \
    m[1][0] = m[1][2] = m[1][3] = 0.0,              \
    m[2][0] = m[2][1] = m[2][3] = 0.0,              \
    m[3][0] = m[3][1] = m[3][2] = 0.0 )
 
/* Vector addition */
#define VT_Pos3Add(p1, p2, pout) (			\
	(pout)[0] = (p1)[0] + (p2)[0],	\
	(pout)[1] = (p1)[1] + (p2)[1],	\
	(pout)[2] = (p1)[2] + (p2)[2] )
#define VT_QuatAdd(q1, q2, qout)(\
        (qout)[0] = (q1)[0] + (q2)[0], \
        (qout)[1] = (q1)[1] + (q2)[1], \
        (qout)[2] = (q1)[2] + (q2)[2], \
        (qout)[3] = (q1)[3] + (q2)[3])

/* Vector vt_p3_subtraction */
#define VT_Pos3Subtract(p1, p2, pout) (	\
	(pout)[0] = (p1)[0] - (p2)[0],  \
	(pout)[1] = (p1)[1] - (p2)[1],  \
	(pout)[2] = (p1)[2] - (p2)[2] )

/* Vector vt_p2_subtraction */
#define VT_Pos2Subtract(p1, p2, pout) (	\
	(pout)[0] = (p1)[0] - (p2)[0],  \
	(pout)[1] = (p1)[1] - (p2)[1])

/* Vector inversion */ 
#define VT_Pos3Invert(pin, pout) (\
    (pout)[0] = -(pin)[0],  \
    (pout)[1] = -(pin)[1],  \
    (pout)[2] = -(pin)[2] )

#define VT_QuatInvert(qin, qout) (\
   (qin)[0] = -(qin)[0], \
   (qin)[1] = -(qin)[1], \
   (qin)[2] = -(qin)[2], \
   (qin)[3] = (qin)[3])

/* magnitudes - vector and quaternion */
#define VT_Pos3Length(p) (sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) 
#define VT_Pos2Length(p) (sqrt((p[0]*p[0]+p[1]*p[1])))
#define VT_QuatLength(q) (sqrt((q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]))) 

#define VT_Pos2Norm(p)   \
    {               \
    double m;        \
    m = VT_Pos2Length(p);   \
	if (m > 0.0) {	\
    	p[0] /= m;	\
    	p[1] /= m;	\
	} \
    }

/* vector normalization */
#define VT_Pos3Norm(p)        \
    {                   \
    double m;			\
    m =  VT_Pos3Length(p); \
	if (m > 0.0) {	\
    	p[0] /= m;	\
    	p[1] /= m;	\
    	p[2] /= m;	\
	} \
    }
#define VT_QuatNorm(q) \
   {                   \
    double m;		    \
    m = VT_QuatLength(q);	\
	if (m > 0.0f) {	\
    	q[0] /= m;	\
    	q[1] /= m;	\
    	q[2] /= m;	\
    	q[3] /= m;	\
	}		\
    }

#define VT_Mat4SetTranslation(mat, p) \
( (mat)[0][3] = (p)[0], (mat)[1][3] = (p)[1], (mat)[2][3] = (p)[2] )

#define VT_Mat4GetTranslation(mat, p) \
( (p)[0] = (mat)[0][3], (p)[1] = (mat)[1][3], (p)[2] = (mat)[2][3])

#define VT_Mat4SetRotation(m, rotm) \
{(m)[0][0] = (rotm)[0][0]; (m)[0][1] = (rotm)[0][1]; (m)[0][2] = (rotm)[0][2];\
 (m)[1][0] = (rotm)[1][0]; (m)[1][1] = (rotm)[1][1]; (m)[1][2] = (rotm)[1][2];\
 (m)[2][0] = (rotm)[2][0]; (m)[2][1] = (rotm)[2][1]; (m)[2][2] = (rotm)[2][2];}

/*------------------------------------*/
/*            functions               */
/*------------------------------------*/
VT_DLL_PROC void VT_Pos3Cross(vt_p3 p1, vt_p3 p2, vt_p3 pout);
VT_DLL_PROC double VT_Pos3Distance( vt_p3 p1, vt_p3 p2);
VT_DLL_PROC void VT_Pos3Print( vt_p3 pos, char *string);
VT_DLL_PROC char * VT_Pos3Out( vt_p3 pos, char *str);
VT_DLL_PROC char * VT_Pos3In( vt_p3 pos, char *str);

VT_DLL_PROC void VT_Mat3MultPos3( vt_m3 m, vt_p3 v1, vt_p3 v2);
VT_DLL_PROC void VT_Mat3MultMat3( vt_m3 m1, vt_m3 m2, vt_m3 mout);
VT_DLL_PROC void VT_Mat3ToEuler( vt_m3 m, int seq, vt_p3 euler);
VT_DLL_PROC void VT_Mat3Transpose( vt_m3 min, vt_m3 mout);
VT_DLL_PROC void VT_Mat3Rotate(vt_m3 inm, char axis, double theta, vt_m3 outm);
VT_DLL_PROC void VT_Mat3ToQuat(vt_m3 m, vt_q q);
VT_DLL_PROC void VT_Mat3Print(vt_m3 mat, char *string);
VT_DLL_PROC char * VT_Mat3Out( vt_m3 mat, char *str);
VT_DLL_PROC char * VT_Mat3In( vt_m3 mat, char *str);

VT_DLL_PROC void VT_Mat4Transpose( vt_m4 min, vt_m4 mout);
VT_DLL_PROC void VT_Mat4MultMat4( vt_m4 m1, vt_m4 m2, vt_m4 mout);
VT_DLL_PROC void VT_Mat4XformPos3( vt_m4 m4, vt_p3 pin, vt_p3 pout);
VT_DLL_PROC void VT_Mat4MultPos3( vt_m4 m4, vt_p3 vecin, vt_p3 vecout);
VT_DLL_PROC void VT_Mat4ToEuler( vt_m4 m, int seq, vt_p3 euler);
VT_DLL_PROC void VT_Mat4Rotate(vt_m4 inm, char axis, double theta, vt_m4 outm);
VT_DLL_PROC void VT_Mat4Translate(vt_m4 inm, vt_p3 pos, vt_m4 outm);
/* for m4 with orthogonal rotation matrix */
VT_DLL_PROC void VT_Mat4OrthoRotInverse(vt_m4 inm, vt_m4 outm);
VT_DLL_PROC void VT_Mat4MultMat3(vt_m4 m1, vt_m3 m2, vt_m4 outm);
VT_DLL_PROC void VT_Mat4ToPQ(vt_m4 m,  vt_pq pq);
VT_DLL_PROC void VT_Mat4Print(vt_m4 mat, char *string);
VT_DLL_PROC char * VT_Mat4Out( vt_m4 mat, char *str);
VT_DLL_PROC char * VT_Mat4In( vt_m4 mat, char *str);

VT_DLL_PROC void VT_EulerToMat4( double rx, double ry, double rz,
				 int seq, vt_m4 m );
VT_DLL_PROC void VT_EulerToMat3( double rx, double ry, double rz, 
				 int seq, vt_m3 m );
VT_DLL_PROC void VT_EulerToQuat( vt_p3 euler, vt_q qout);

VT_DLL_PROC void VT_QuatMultQuat(vt_q q1, vt_q q2, vt_q qout);
VT_DLL_PROC void VT_QuatSlerp(vt_q q1, vt_q q2, double u, vt_q qout);
VT_DLL_PROC void VT_QuatPrint(vt_q q, char * str);
VT_DLL_PROC void VT_QuatToEuler(vt_q q, int seq, vt_p3 euler);
VT_DLL_PROC void VT_QuatToAxisAngle(vt_q q, vt_p3 dir, double ang);

VT_DLL_PROC void VT_AxisAngleToQuat(vt_p3 dir, double ang, vt_q q);

VT_DLL_PROC void VT_PQToMat4(vt_pq pq, vt_m4 m);
VT_DLL_PROC void VT_PQPrint(vt_pq pq, char * str);



#ifdef __cplusplus
}
#endif

#endif				/* VT_MATH3D_H */





