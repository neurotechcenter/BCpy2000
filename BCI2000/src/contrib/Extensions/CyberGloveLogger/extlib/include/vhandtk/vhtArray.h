/********************************************************************
 FILE: $Id: vhtArray.h,v 1.8 2002/08/28 23:16:05 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998
<header>

   DESCRIPTION:
    -
 HISTORY:
  -
</header>
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTARRAY_H
#define __VHTARRAY_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
	#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
	#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#define VHTARRAY_DEFAULT_SIZE 15
// NOTE: must be greater than 0

#include <stdlib.h>

//: A resizable array.
// A generic resizable array for storing void pointers.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtArray{
public:
    vhtArray(void);
    //: Construct an empty array.
    vhtArray(int ziSize);
    //: Construct an array of size ziSize.
    vhtArray(const vhtArray &zcArray);
    //: Copy constructor.
    ~vhtArray(void);
    //: Destructor.

    inline int  getSize(void) const {return m_size;}
    //: Get the current array size.
    void        setSize(int newSize);
    //: Set the array to at least newSize.


    inline int  getNumEntries(void) const { return m_endOfData; }
    //: Get the number of entries used.
    // Returns the greater of the number of entries added with the add method
    // or the highest index acessed directly.

    void add( void *newEntry );
    //: Append a new entry to the array.
    // Adds newEntry to the end of the number of entries.

	void crop(void);
    //: Crop the array.

    void remove(int index);
    //: Remove the entry at location index.
    // Shifts all entries 1 cell left, overwriting startIndex

    void insert(int index, void *newEntry ); 
    //: Insert newEntry before index.

    vhtArray &operator = (vhtArray &zcArray);
    //: copy constructor
   
    void *&operator [](int ziIndex);
    //: Allow direct array indexing.

    bool  contains(void *entry);
    //: True if entry is present in the array.
    // Uses a (linear search).

    int   getIndex( void *entry );
    //: Return index of entry.
    // Returns -1 if entry is not found.
    inline void *getEntry(int index) const 
	{
		if (index<m_endOfData && index>=0){
			return m_data[index];
		}
		else {
			return NULL;
		}
    }
    //: Get entry at location index.

    void deleteAll(void);
    //: Delete all entries.
    // Deletes all objects keep in internal array, and
    // set size of array to 0.

private:

    int         m_size;
    int         m_endOfData;
    void      **m_data;
    
    int         m_increment;
};

#endif


