#ifndef VHT_QUATERNION_H
#define VHT_QUATERNION_H
/********************************************************************
 FILE: $Id: vhtQuaternion.h,v 1.13 2002/08/28 23:16:23 jsilver Exp $
 AUTHOR: Chris Ullrich.
 DATE: July 1999.

   DESCRIPTION:
    -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998, 1999 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#include <vhtMath.h>
class vhtVector3d;

//: A three dimensional rotation.
// The VHT uses quaternions to store three dimensional rotations.  Quaternions
// provide a compact, non-singular and numerically stable representation of
// rotations.  In addition, quaternion inversion is accomplished with a single
// operation. 
// Quaternions are represented by 4 double values (s,x,y,z).  To convert from
// a axis-angle (theta,x,y,z)->(sin(theta/2),x*cos(theta/2),y*cos(theta/2),z*cos(theta/2)).
// Note that this class only handles normalized quaternions.  
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtQuaternion
{
	public:
		vhtQuaternion( void );
		//: Construct a default quaternion (1,0,0,0).

		vhtQuaternion( double theta, const vhtVector3d &axis );
		//: Construct a quaternion using given axis, angle.
		//!param: theta - Angle of rotation.
		//!param: axis - Axis about which rotation takes place.
		// To convert from a axis-angle 
		// (theta,x,y,z)->(sin(theta/2),x*cos(theta/2),y*cos(theta/2),z*cos(theta/2)).

		~vhtQuaternion( void );
		//: Destructor.

		static const vhtQuaternion Identity;
		//: Identity quaternion.
		// q = (1,0,0,0).

		void add( const vhtQuaternion &q1 );
		//: Add q1 to current quaternion.
		//!param: q1 - Quaternion to add to this.
		// This is normalized after addition.

		void add( const vhtQuaternion &q1, const vhtQuaternion &q2 );
		//: Add q1 and q2, store result in current quaternion.
		//!param: q1 - First quaternion.
		//!param: q2 - Second quaternion.
		// This is normalized after addition.

		void conjugate( void );
		//: Make this into it's conjugate.
		// Conjugation reverses the sign of the x,y,z components.

		void conjugate( const vhtQuaternion &q1 );
		//: Make this into q1's conjugate.
		//!param: q1 - Quaternion to conjugate.
		// Conjugation reverses the sign of the x,y,z components.

		double getAngle( void );
		//: Get rotation angle of quaternion.

		void getAxis( vhtVector3d &axis ) const;
		//: Get axis of rotation.
		//!param: axis - Sets axis to correspond to the axis of rotation.

		void setAxisAngle( double theta, const vhtVector3d &axis );
		//: Set the rotation angle and axis.
		//!param: theta - Angle of rotation.
		//!param: axis - Axis about which rotation takes place.
		// To convert from a axis-angle 
		// (theta,x,y,z)->(sin(theta/2),x*cos(theta/2),y*cos(theta/2),z*cos(theta/2)).

		void getDerivative( vhtVector3d &omega );
		//: Sets this to the corresponding quaternion derivative.
		//!param: omega - Angular velocity to use for derivative calculation.
		// Use omega to calculate dq/dt and store in this.  Note that after this
		// call, this is no longer a quaternion but it's derivative.
		// The object is not normalized after this operation.  It is recommended to use
		// the propagate method.

		void propagate(vhtVector3d &omega, double time);
		//: Propagates the orientation.
		//!param: omega - Angular velocity to use for derivative calculation.
		//!param: time - Delta time to integrate.
		// Use a forward Euler integration step to propagate the quaternion
		// with the given angular velocity.

		void identity( void );
		//: Set this to the identiy quaternion (1,0,0,0).

		void invert( void );
		//: Invert the current quaternion.
		// This is accomplished by setting s = -s.

		void invert( const vhtQuaternion &q1 );
		//: Invert q1 and store the result in this.
		//!param: q1 - Quaternion to invert.

		void mul( const vhtQuaternion &q1, vhtRotationOrder o=VHT_POSTMULT );
		//: Multiply this with q1 using the given rotation order.
		//!param: q1 - Quaternion to multiply.
		//!param: o - Order of multiplcation.
		// For VHT_POSTMULT, this = this*q1, for VHT_PREMULT, this = q1*this.

		void mul( const vhtQuaternion &q1, const vhtQuaternion &q2 );
		//: Multiply q1 with q2 and store in this.
		//!param: q1 - First quaternion to multiply.
		//!param: q2 - Second quaternion to multiply.
		// Set this = q1*q2;

		void normalize( void );
		//: Normalize this quaternion.
		// The norm of a quaternion is sqrt( s*s + x*x + y*y + z*z ).

		void normalize( const vhtQuaternion &q1 );
		//: Set this = q1 and normalize.
		// The norm of a quaternion is sqrt( s*s + x*x + y*y + z*z ).

		void rotate( vhtVector3d &v1 ) const; 
		//: Apply the rotation of this to v1.
		//!param: v1 - Vector to rotate.
		// Rotates v1 in place.

		void getTransform( double xform[4][4] ) const;
		//: Get corresponding homogeneous transformation matrix
		//!param: xform - Double array in which to store the rotation matrix.
		// The resulting rotation matrix xform will have unit determinant and zero translation.
		// Only the upper 3x3 components will be non-zero.

		void setTransform( double xform[4][4] );
		//: Set the quaternion to the roation given by xform.
		//!param: xform - Double array containing a rotation matrix.
		// The matrix xform should be stored in [row][column] order and have unit determinant.
		// Any translation component will be ignored.  The matrix should be homogeneous, i.e. xform[3][3] = 1.
		// Non-homogeneous matrices will be handled but may produce undefined quaternions.

		void print( void ) const;
		//: Print the quaternion to cout.

		vhtQuaternion& operator*( const vhtQuaternion& q);
		//: Quaternion multiplication.
		//!param: q - Quaternion to multiply.
		// result = this.mul(q).

		vhtQuaternion& operator+( const vhtQuaternion& q);
		//: Quaternion addition.
		//!param: q - Quaternion to add.
		// result = this.add(q).
    
		inline void getComponents(double comps[4]) const
		{ 
			comps[0] = qs; comps[1] = qx; comps[2] = qy; comps[3] = qz; 
		} 
		//: Get the component of the quaternion.
		//!param: comps - Array of 4 doubles to contain the components.

		inline void setComponents(double comps[4]) 
		{ 
			qs = comps[0]; qx = comps[1];  qy = comps[2]; qz = comps[3]; 
		} 

	private:
		double qs, qx, qy, qz;
};

#endif
