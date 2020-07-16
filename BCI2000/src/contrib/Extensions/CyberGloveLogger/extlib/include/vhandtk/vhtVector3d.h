#ifndef VHT_VECTOR3D_H
#define VHT_VECTOR3D_H
/********************************************************************
 FILE: $Id: vhtVector3d.h,v 1.13 2002/08/28 23:16:27 jsilver Exp $
 AUTHOR: C. Ullrich
 DATE: 1999

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998, 1999 --
********************************************************************/
//: A 3 dimensional vector qunatity.
// Represents a point or vector in R3.

//!vhtmodule: Device

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

class VH_DEVICE_DLL_EXPORT vhtVector3d 
{
	public:

		double x, y, z;
		//: Component values.

		vhtVector3d( void );
		//: Null constructor.
		// All components set to zero.
		vhtVector3d( const double *v );
		//: Construct a vector using the indicated values.
		//!param: v - An array of 3 doubles to use for initialization.
		vhtVector3d( double a, double b, double c );
		//: Construct a vector using the indicated values.
		//!param: a - The x component.
		//!param: b - The y component.
		//!param: c - The z component.

		~vhtVector3d( void );
		//: Destructor

		void add( const vhtVector3d &v1 );
		//: Add to current vector.
		//!param: v1 - Vector to add.
		// this = this + v1.

		void add( const vhtVector3d &v1, const vhtVector3d &v2 );
		//: Add and store in this.
		//!param: v1 - First vector to add.
		//!param: v2 - Second vector to add.
		// this = v1 + v2.

		double angle( const vhtVector3d &v1 ) const;
		//: Compute angle between this and v1.
		//!param: v1 - Vector to find angle with.
		// Return value is limited by to the range [0,Pi].
		void cross( const vhtVector3d &v1 );
		//: Compute the cross product.
		//!param: v1 - Vector to take cross product with.
		// this = this X v1.
    
		void cross( const vhtVector3d &v1, const vhtVector3d &v2 );
		//: Compute the cross product.
		//!param: v1 - First Vector to take cross product with.
		//!param: v2 - Second Vector to take cross product with.
		// this = v1 X v2.

		double dot( const vhtVector3d &v1 ) const;
		//: Compute dot product.
		//!param: v1 - Vector to compute dot product with.
		// Compute dot product between this and v1.

		double length( void ) const;
		//: Compute length of this vector.
		// Uses the eucledian norm.
		double lengthSquared( void ) const;
		//: Compute the squared length.
		// This is equivalent to the dot product of this with itself.

		void normalize( void );
		//: Normalize this vector.
		// May throw a vhtBadLogicExcp if the length of this is 0.0.
		void normalize( const vhtVector3d &v1 );
		//: Normalize v1 and store in this.
		//!param: v1 - Vector to normalize.
		// May throw a vhtBadLogicExcp if the length of v1 is 0.0.

		void scale( double length );
		//: Scale this vector by length.
		//!param: length - Scalar multiple.
		// this = length*this.

		void sub( const vhtVector3d &v1 );
		//: Subtract v1 from this.
		//!param: v1 - Vector to subtract from this.
		// this = this - v1.
		void sub( const vhtVector3d &v1, const vhtVector3d &v2 );
		//: Subtract v2 from v1, store in this.
		//!param: v1 - Frist vector.
		//!param: v2 - Second vector.
		// this = v1 - v2.

		const double &operator[](int i) const;
		//: Access elements.
		// Read only.
		double &operator[](int i);
		//: Access elements.

		/* Constants */
		static const vhtVector3d m_ihat;
		//: X unit vector.
		static const vhtVector3d m_jhat;
		//: Y unit vector.
		static const vhtVector3d m_khat;
		//: Z unit vector.

		/* Operators */
		double operator*( const vhtVector3d& q);
		//: vector dot product
		//!param: q - Vector to take dot product with.
		// result = this.dot( q ).

		vhtVector3d& operator+( const vhtVector3d& q);
		//: vector addition
		//!param: q - Vector to add to this.
		// result = this + q.

		vhtVector3d& operator-( const vhtVector3d& q);
		//: vector subtraction
		//!param: q - Vector to subtract from this.
		// result = this - q.

		vhtVector3d& operator-( void );
		//: unary minus
		// result = this.scale( -1.0 ).

		vhtVector3d& operator^( const vhtVector3d& q);
		//: vector cross product
		//!param: q - Vector to take cross product with.
		// result = this X q.
};

#endif
