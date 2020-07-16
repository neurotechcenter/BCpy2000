#ifndef VHT_SYMMETRIC_TABLE_H
#define VHT_SYMMETRIC_TABLE_H
/********************************************************************
 FILE: $Id: vhtSymmetricTable.h,v 1.6 2002/08/28 23:16:24 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: 1998

 TODO:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#include <vhtTable.h>

//: Square symmetric table.
// A resizable table with symmetric entries.  This table enforces 
// symmetric entries. For example, getElement( i, j ) == getElement( j, i).
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtSymmetricTable : public vhtTable
{
	public:
		vhtSymmetricTable(void);
		//: Construct a default symmetric table.
		// The default size is 10x10.
	    vhtSymmetricTable( int size);
		//: Construct a symmetric table.
		//!param: size - Size of table.
		// The table will have dimensions size x size.
		vhtSymmetricTable( vhtSymmetricTable &table);
		//: Copy constructor.
		~vhtSymmetricTable(void);
		//: Destructor.

		void setSize( int size );
		//: Set the table size.
		//!param: size - Size of table.
		// Currently only support size increases.
		inline int  getSize(void) { return m_size; }
		//: Get the current table size.

		void *getElement( int row, int column );
		//: Get an element, order is unimportant.
		//!param: row - Row of element to retrieve.
		//!param: column - Column of element to retrieve.
		// The table is symmetric, i.e. getElement( row, col ) == getElement( col, row).

		void  setElement( void *element, int row, int col );
		//: Set an element, order is unimportant.
		//!param: element - Element to set.
		//!param: row - Row of element to set.
		//!param: col - Column of element to set.
		// The table is symmetric, i.e. getElement( row, col ) == getElement( col, row).

	private:
		int m_size;
};

#endif


