/********************************************************************
 FILE: $Id: vhtTransform3D.h,v 1.12 2002/08/28 23:16:26 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTTRANSFORM3D_H
#define __VHTTRANSFORM3D_H

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
#include <vhtVector3d.h>
#include <vhtQuaternion.h>

//: A three dimensional homogeneous transformation.
// The vhtTransform3D class implements a highly optimized homogeneous transformation matrix.  
// Although this class does not store an explicit matrix representation, most standard 
// matrix operations are supported including muliply, add, transpose and invert.  Operations on
// vhtVector3d, including transform, rotate and translate, are also supported.
// Transformations are stored in a factored form, and can be expressed as 
// H = scale*Translation*Rotation.
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtTransform3D {

	public:
		vhtTransform3D(void);
		//: Default constructor.
		// Default, sets matrix to the identity.               
    
		vhtTransform3D( double x, double y, double z );
		//: Construct a identity transform with given translation.
		//!param: x - The x component of the translation.
		//!param: y - The y component of the translation.
		//!param: z - The z component of the translation.
		// Construct a default transform and apply provided translation.
    
		vhtTransform3D( const vhtVector3d &v );
		//: Construct a identity transform with given translation.
		//!param: v - The translation component represented as a vector.
		// Construct a default transform and apply provided translation.

		vhtTransform3D( const vhtVector3d &v, const vhtQuaternion &rot, const double scale = 1.0 );
		//: Construct a identity transform with given translation.
		//!param: v - Translation component.
		//!param: rot - Rotation component.
		//!param: scale - Scale.
		// Construct a default transform and apply provided translation.
    
		virtual ~vhtTransform3D( void );
		//: Destructor.

		void add(const vhtTransform3D &t1);
		//: Add given transform to this.
		//!param: t1 - Transform to add.
		// Add given transform to this.

		void add(const vhtTransform3D &t1, const vhtTransform3D &t2);
		//: Add two transforms and store result in this.
		//!param: t1 - First transform to add.
		//!param: t1 - Second transform to add.
		// Add two transforms and store result in this.

		void getRotation( vhtQuaternion &q ) const;
		//: Get the current rotation.
		//!param: q - Quaternion to store rotation in.
		// Get the current rotation matrix in terms of a normalized axis and an angle of rotation
		// about that axis.

		void getTranslation(vhtVector3d &trans) const ;
		//: Get the current translation vector.
		//!param: trans - Vector to store translation in.
		// Sets the vector trans to the current translation.  This is equivalent to getColumn with c=3.

		void getTransform( double xform[4][4] ) const;
		//: Sets the values of xform to the current matrix entries.
		//!param: xform - Double array in which to store matrix representation.
		// The double array is filled from the matrix entries.

		void invert(const vhtTransform3D &t1);
		//: Invert t1 and store in this.
		//!param: t1 - Transform to invert.
		// Inverts the matrix t1 and stores the result in this.

		void invert(void);
		//: Invert this in place.
		// Inverts this and stores the result in place.

		void mul( const vhtTransform3D &T, vhtRotationOrder order = VHT_POSTMULT );
		//: Multiply this by T, and store the result in this.
		//!param: T - Transform to multiply by.
		//!param: order - Multiplication order.
		// Multiply this by T.  If order == VHT_POSTMULT, this = this*T, otherwise this = T*this.

		void mul( const vhtTransform3D &A, const vhtTransform3D &B, bool isIgnoreScale = true);
		//: Multiply A by B, and store the result in this.
		//!param: A - First transform.
		//!param: B - Second transform.
		// this = A*B.
    
		void rotate( vhtVector3d axis, double theta, vhtRotationOrder order = VHT_POSTMULT );
		//: Rotate this about axis by amount theta.
		//!param: axis - Axis about which to rotate.
		//!param: theta - Angle to rotate by.
		//!param: order - Multiplication order.
		// Rotate this about axis by amount theta. The rotation matrix is multipled by this using
		// the indicated order. The translation component is not affected.

		void rotX(double angle, vhtRotationOrder order = VHT_POSTMULT );
		//: Rotate this about the X axis by amount angle.
		//!param: angle - Angle to rotate by.
		//!param: order - Multiplication order.
		// Rotate this about X axis by amount theta. The rotation matrix is multipled by this using
		// the indicated order. The translation component is not affected.

		void rotY(double angle, vhtRotationOrder order = VHT_POSTMULT );
		//: Rotate this about the Y axis by amount angle.
		//!param: angle - Angle to rotate by.
		//!param: order - Multiplication order.
		// Rotate this about Y axis by amount theta. The rotation matrix is multipled by this using
		// the indicated order. The translation component is not affected.

		void rotZ(double angle, vhtRotationOrder order = VHT_POSTMULT );
		//: Rotate this about the Z axis by amount angle.
		//!param: angle - Angle to rotate by.
		//!param: order - Multiplication order.
		// Rotate this about Z axis by amount theta. The rotation matrix is multipled by this using
		// the indicated order. The translation component is not affected.

		void scale(double scale);
		//: Apply a uniform scale to this.
		//!param: scale - Scale to use.
		// Rescales this transform by amount scale. 

		void getColumn( vhtLocalAxes c, vhtVector3d &col ) const ;
		//: Get indicated column.
		//!param: c - Column to retrieve.
		//!param: col - Vector to store column in.
		// Sets col to the values of column c.  
		void getRow( vhtAxes r, vhtVector3d &row ) const ;
		//: Get indicated row from the rotation component.
		//!param: r - Rot to retrieve.
		//!param: row - Vector to store row in.
		// Sets row to the values of row r in the matrix representation of the rotation.

		void setRotation( const vhtQuaternion &a1 );
		//: Set the rotation to indicated quaternion.
		//!param: a1 - Quaterion to use for rotation.

		void setRotation( vhtAxes axis, double theta );
		//: Set the rotation to angle theta about axis.
		//!param: axis - Axis to rotate about.
		//!param: theta - Angle to rotate.
		// Set the rotation to angle theta about axis.

		void setRotation(const vhtVector3d &a, const vhtVector3d &b);
		//: Set the rotation to rotate a into b.
		//!param: a - Source vector.
		//!param: b - Destination vector.
		// Constructs a rotation that takes the vector a into the vector b.  The rotation will
		// be about an axis perpendicular to both vectors.  If the angle between a and b is small,
		// this method may be poorly conditioned.

		void setScale(double scale);
		//: Set uniform scaling of this transform.
		//!param: scale - Scaling for the transform.

		void setTransform( double xform[4][4] );
		//: Set the transform to the homogeneous matrix xform.
		//!param: xform - Double array containing a homogeneous transformation.
		// Use the homoegeneous matrix xform to generate a transform object.  The
		// xform matrix must be arranged in [row][column] order.  Any embedded scaling may cause
		// undefined effects.

		void setTranslation(double xTrans, double yTrans, double zTrans);
		//: Set the translation component.
		//!param: xTrans - X Component of translation.
		//!param: yTrans - Y Component of translation.
		//!param: zTrans - Z Component of translation.
		// Sets the translation of this to the indicated values.

		void setTranslation(const vhtVector3d &translate);
		//: Set the translation component.
		//!param: translate - Translation vector.
		// Sets the translation of this to the given vector.

		void translate(double xTrans, double yTrans, double zTrans);
		//: Translate by indicated values.
		//!param: xTrans - X Component of translation.
		//!param: yTrans - Y Component of translation.
		//!param: zTrans - Z Component of translation.
		// Adds the arguments to the entries of the translation of this.

		void translate(const vhtVector3d &translate);
		//: Translate by indicated value.
		//!param: translate - Translation vector.
		// Adds the argument to the entries of the translation of this.

		void setIdentity( void );
		//: Set this to the identity transform.
 
		void transform(const vhtVector3d &vIn, vhtVector3d &vOut) const;
		//: Transform vector.
		//!param: vIn - Input vector.
		//!param: vOut - Output vector.
		// Calculates vOut = this.transform(vIn).

		void transform(vhtVector3d &v) const ;
		//: Transform vector.
		//!param: v - Vector to transform.
		// Cacluates normal = this*normal;

		void rotate( vhtVector3d &v ) const;
		//: Rotate vector.
		//!param: v - Vector to rotate.
		// Applies only rotation component to the vector.

		void print( void ) const;
		//: Print the current transform to cout.

		static const vhtTransform3D m_ident;
		//: Construct an identity matrix constant for frequent use

		vhtTransform3D& operator*( const vhtTransform3D& q);
		//: transform multiplication
		//!param: q - Transform to multiply by.
		// result = this*q.
    
		vhtTransform3D& operator+( const vhtTransform3D& q);
		//: transform addition
		//!param: q - Transform to add to.
		// result = this+q.
    
	private:
		vhtVector3d         m_trans;
		vhtQuaternion       m_quat;
		double				m_scale;
};
#endif
