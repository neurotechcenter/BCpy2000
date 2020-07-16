#ifndef __TGCOPY_H_
#define __TGCOPY_H_

#include <wchar.h>

// wszcpy
//
// Like wcsncpy, except always sets the final wchar_t to nul.
inline wchar_t* wszcpy(wchar_t* dest, const wchar_t* src, size_t sz)
{
   dest[sz - 1] = L'\0';
   return wcsncpy(dest, src, sz - 1);
}

// wszncpy
//
// Like wszcpy, except allows src to be NULL
inline wchar_t* wszncpy(wchar_t* dest, const wchar_t* src, size_t sz)
{
   return wszcpy(dest, src ? src : L"", sz);
}

// wsznicmp
//
// Like wcsnicmp, but allows both strings to be NULL
inline int wsznicmp(const wchar_t* s1, const wchar_t* s2, size_t sz)
{
   return wcsnicmp((s1 ? s1 : L""), (s2 ? s2 : L""), sz);
}

#define WSZCPY(dest, src) wszcpy(dest, src, sizeof(dest) / sizeof(dest[0]))
#define WSZNCPY(dest, src) wszncpy(dest, src, sizeof(dest) / sizeof(dest[0]))

#endif
