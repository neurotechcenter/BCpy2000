#ifndef VHT_MATRIX_4X4_H
#define VHT_MATRIX_4X4_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

class vhtVector3d;

class VH_CORE_DLL_EXPORT vhtMatrix4x4
{
	public:
		vhtMatrix4x4(void);

		~vhtMatrix4x4(void);
		
		void getColumn( int c, vhtVector3d &col ) const ;
		//: Get indicated column.
		// Sets col to the values of column c.  The range of c is 0 to 3.

		void getRow( int r, vhtVector3d &row ) const ;
		//: Get indicated row.
		// Sets row to the values of row r.  The range of r is 0 to 3.

		void setColumn( int c, const vhtVector3d &col );
		//: Set the column c to the values in col.
		// Set the column c to the values in col.

		void setRow( int r, const vhtVector3d &row );
		//: Set the row r to the values in row.
		// Set the row r to the values in row.

		void transpose();
		//: Transpose this in place.
		// Sets all the columns of this to the rows.
  
		void  transpose(const vhtTransform3D &t1);
	  //: Transpose t1.
	  // Sets all the columns of this to the rows of t1.

	private:
		double m[4][4];
};

#endif
