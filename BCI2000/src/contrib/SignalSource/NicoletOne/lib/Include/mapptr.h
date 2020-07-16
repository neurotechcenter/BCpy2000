
#pragma once

#include <crtdbg.h>
#include <windows.h>

struct __POSITION2 { };
typedef __POSITION2* POSITION2;
#define BEFORE_START_POSITION2 ((POSITION2)-1L)

struct CPlex     // warning variable length structure
{
	CPlex* pNext;
#if (_AFX_PACKING >= 8)
	DWORD dwReserved[1];    // align on 8 byte boundary
#endif
	// BYTE data[maxNum*elementSize];

	void* data() { return this+1; }

	static CPlex* PASCAL Create(CPlex*& head, UINT nMax, UINT cbElement);
			// like 'calloc' but no zero fill
			// may throw memory exceptions

	void FreeDataChain();       // free this one and links
};


class CMapPtrToPtr1
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;

		void* key;
		void* value;
	};

public:

// Construction
	CMapPtrToPtr1(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(void* key, void*& rValue) const;

// Operations
	// Lookup and add if not there
	void*& operator[](void* key);

	// add a new (key, value) pair
	void SetAt(void* key, void* newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(void* key);
	void RemoveAll();

	// iterating all (key, value) pairs
	POSITION2 GetStartPosition() const;
	void GetNextAssoc(POSITION2& rNextPosition, void*& rKey, void*& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Overridables: special non-virtual (see map implementation for details)
	// Routine used to user-provided hash keys
	UINT HashKey(void* key) const;

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CAssoc* NewAssoc();
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(void*, UINT&) const;

public:
	~CMapPtrToPtr1();

	void* GetValueAt(void* key) const;


protected:
	// local typedefs for CTypedPtrMap class template
	typedef void* BASE_KEY;
	typedef void* BASE_ARG_KEY;
	typedef void* BASE_VALUE;
	typedef void* BASE_ARG_VALUE;
};

inline  int CMapPtrToPtr1::GetCount() const
	{ return m_nCount; }
inline  BOOL CMapPtrToPtr1::IsEmpty() const
	{ return m_nCount == 0; }
inline  void CMapPtrToPtr1::SetAt(void* key, void* newValue)
	{ (*this)[key] = newValue; }
inline  POSITION2 CMapPtrToPtr1::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION2; }
inline  UINT CMapPtrToPtr1::GetHashTableSize() const
	{ return m_nHashTableSize; }

