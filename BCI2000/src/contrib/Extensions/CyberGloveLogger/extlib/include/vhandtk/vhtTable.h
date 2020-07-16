#ifndef VHT_TABLE_H
#define VHT_TABLE_H
/********************************************************************
 FILE: $Id: vhtTable.h,v 1.11 2002/08/28 23:16:25 jsilver Exp $
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

#include <vhtArray.h>

//: A resizable table class optimized for fast access.
// A two dimensional resizable table that trades off resizing
// speed for fast queries.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtTable
{
	public:
		vhtTable(void);
		//: Construct a default table.
		// Default size is 10x10.
		vhtTable( int w, int h);
		//: Construct a table.
		//!param: w - Width of table.
		//!param: h - Height of table.
		// Builds a table of size w x h.
		vhtTable( vhtTable *table );
		//: Copy constructor.
		~vhtTable(void);
		//: Destructor.

		inline void setNumRows( int numRows ) { setSize( numRows, m_columns); }
		//: Set number of rows.
		//!param: numRows - Set the number of table rows.
		// Must be greater than the current number of rows.
		inline int  getNumRows(void) { return m_rows; }
		//: Get the number of rows.

		inline void setNumColumns( int numColumns ) { setSize( m_rows, numColumns); }
		//: Set the number of columns.
		//!param: numColumns - Set the number of table columns.
		// Must be greater than the current number of columns.
		inline int  getNumColumns(void) { return m_columns; }
		//: Get the number of columns.

		inline void *getElement( int row, int col ) { return m_table[ col+row*m_columns]; }
		//: Get a table element.
		//!param: row - Row of element to retrieve.
		//!param: col - Column of element to retrieve.
		// Indices outside the table range are undefined.
		void  setElement( void *element, int row, int col );
		//: Set a particular element.
		//!param: element - Element to store.
		//!param: row - Row of element to set.
		//!param: col - Column of element to set.
		// Sets the element at row and column to element. If row > numRows or col > numColumns, the
		// table is resized to accommodate the new entry. The resize algorithm will double the table
		// size in either the rows or columns or both. For more precise control, use the setNumColumns
		// or setNumRows methods.

		void setAllElements( void *element, void *reserved = NULL );
		//: Set all elements to given value except reserved fields.
		//!param: element - Element to set in all table positions.
		//!param: reserved - Ignore elements already set to this value.
		// Sets all table entries to element. Entries that already have the
		// reserved value are not set.
    
		void deleteAll(void);
		//: Deletes all entries.
		// Sets the table size to 0x0 and removes all entries.

		/*
		  Table management.
		 */
		void freezeCurrent(void);
		//: Freeze (copy) the current table.
		// The table will make a backup of the current state and store it internally.
		void thawBackup(void);
		//: Replace the current table with the frozen version.
		// Note that if the table is thawed before it has been frozen, the result
		// is undefined.

	protected:

		int         m_rows;
		int         m_columns;

		void      **m_table;
		void      **m_freezeTable;
		/*
		// resize the table (expensive!)
		// 
		// note: growth only
		*/
		void setSize( int newRows, int newCols );
		//: Set the table size.
		//!param: newRows - New # of rows.
		//!param: newCols - New # of columns.
};

#endif


