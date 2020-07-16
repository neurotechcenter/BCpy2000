/////////////////////////////////////////////////////////////////////////////
// Module:  NrvErrorMacros.h 
// Purpose: Error macros for creating COM errors with description and a
//          source
//
// Copyright 2000 by Taugagreining hf. - All rights reserved
// THIS CODE IS CONFIDENTIAL
// Created:  16.10.2000 HEI
/////////////////////////////////////////////////////////////////////////////

#ifndef __NRVERRORMACROS_H_INCLUDED__
#define __NRVERRORMACROS_H_INCLUDED__

class _com_error;
class _bstr_t;

// Helper functions to reduce code bloat from the macros and make debugging
// easier
namespace NrvErrorFuncs
{
   _com_error CreateComError(HRESULT hr, const wchar_t* pszError,
      const CLSID& clsid, const char* pszTimeStamp,
      const char* pszFile, int iLine);

   void ErrorBox(const _com_error& e, const char* pszTimeStamp,
      const char* pszFile, int iLine);
   _bstr_t GetErrorMessage(const _com_error& e, const char* pszTimeStamp,
      const char* pszFile, int iLine);

   HRESULT ReReturnError(const _com_error& e, const char* pszTimeStamp,
      const char* pszFile, int iLine);
   HRESULT ReturnError(HRESULT hr, const wchar_t* pszError,
      const CLSID& clsid, const char* pszTimeStamp, const char* pszFile,
      int iLine);
   void ThrowErrorInfo(HRESULT hr, const char* pszTimeStamp,
      const char* pszFile, int iLine);

   IErrorInfo* _GetErrorInfo(const wchar_t* pszError, const CLSID& clsid,
      const char* pszTimeStamp, const char* pszFile, int iLine);

   bool ContinueOnError(HRESULT hr);

}


/////////////////////////////////////////////////////////////////////////////
// See the relevant function for a description of these
/////////////////////////////////////////////////////////////////////////////
#define ERRORBOX(x)        \
   NrvErrorFuncs::ErrorBox(x, __TIMESTAMP__, __FILE__, __LINE__)

#define GETERRORMESSAGE(x) \
   NrvErrorFuncs::GetErrorMessage(x, __TIMESTAMP__, __FILE__, __LINE__)

#define RERETURNERROR(x)   \
   return NrvErrorFuncs::ReReturnError(x, __TIMESTAMP__, __FILE__, __LINE__)

#define THROWERROR(result, wcError, clsid)                              \
   throw NrvErrorFuncs::CreateComError(result, wcError, clsid,          \
      __TIMESTAMP__, __FILE__, __LINE__)

#define RETURNERROR(result, wcError, clsid)                             \
   return NrvErrorFuncs::ReturnError(result, wcError, clsid,            \
      __TIMESTAMP__, __FILE__, __LINE__)

#define THROWERRORINFO(hr) \
   NrvErrorFuncs::ThrowErrorInfo(hr, __TIMESTAMP__, __FILE__, __LINE__)

#endif
