// Import declarations for libmex library
// generated Tue Mar 01 12:21:41 2016 by DylibTool
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( libmex, "libmex", functionImports, notFoundMsg, notFoundURL );

extern "C" {
// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
//
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void mexAtExit() { Dylib::libmex.UninitializedFunctionExport( "mexAtExit" ); }
void mexCallMATLAB() { Dylib::libmex.UninitializedFunctionExport( "mexCallMATLAB" ); }
void mexCallMATLABWithObject() { Dylib::libmex.UninitializedFunctionExport( "mexCallMATLABWithObject" ); }
void mexCallMATLABWithTrap() { Dylib::libmex.UninitializedFunctionExport( "mexCallMATLABWithTrap" ); }
void mexCallMATLABWithTrapWithObject() { Dylib::libmex.UninitializedFunctionExport( "mexCallMATLABWithTrapWithObject" ); }
void mexCleanUpAfterError() { Dylib::libmex.UninitializedFunctionExport( "mexCleanUpAfterError" ); }
void mexClearMexFile() { Dylib::libmex.UninitializedFunctionExport( "mexClearMexFile" ); }
void mexCreateSimpleFunctionHandle() { Dylib::libmex.UninitializedFunctionExport( "mexCreateSimpleFunctionHandle" ); }
void mexEnterMexLibrary() { Dylib::libmex.UninitializedFunctionExport( "mexEnterMexLibrary" ); }
void mexErrMsgIdAndTxt() { Dylib::libmex.UninitializedFunctionExport( "mexErrMsgIdAndTxt" ); }
void mexErrMsgTxt() { Dylib::libmex.UninitializedFunctionExport( "mexErrMsgTxt" ); }
void mexEvalString() { Dylib::libmex.UninitializedFunctionExport( "mexEvalString" ); }
void mexEvalStringWithTrap() { Dylib::libmex.UninitializedFunctionExport( "mexEvalStringWithTrap" ); }
void mexExitMexLibrary() { Dylib::libmex.UninitializedFunctionExport( "mexExitMexLibrary" ); }
void mexFunctionName() { Dylib::libmex.UninitializedFunctionExport( "mexFunctionName" ); }
void mexGet() { Dylib::libmex.UninitializedFunctionExport( "mexGet" ); }
void mexGetLocalFunctionTable() { Dylib::libmex.UninitializedFunctionExport( "mexGetLocalFunctionTable" ); }
void mexGetVariable() { Dylib::libmex.UninitializedFunctionExport( "mexGetVariable" ); }
void mexGetVariablePtr() { Dylib::libmex.UninitializedFunctionExport( "mexGetVariablePtr" ); }
void mexGetVariableWithObject() { Dylib::libmex.UninitializedFunctionExport( "mexGetVariableWithObject" ); }
void mexIsGlobal() { Dylib::libmex.UninitializedFunctionExport( "mexIsGlobal" ); }
void mexIsLocked() { Dylib::libmex.UninitializedFunctionExport( "mexIsLocked" ); }
void mexLock() { Dylib::libmex.UninitializedFunctionExport( "mexLock" ); }
void mexMakeArrayPersistent() { Dylib::libmex.UninitializedFunctionExport( "mexMakeArrayPersistent" ); }
void mexMakeMemoryPersistent() { Dylib::libmex.UninitializedFunctionExport( "mexMakeMemoryPersistent" ); }
void mexMexDebug() { Dylib::libmex.UninitializedFunctionExport( "mexMexDebug" ); }
void mexMexInformation() { Dylib::libmex.UninitializedFunctionExport( "mexMexInformation" ); }
void mexPrintAssertion() { Dylib::libmex.UninitializedFunctionExport( "mexPrintAssertion" ); }
void mexPrintf() { Dylib::libmex.UninitializedFunctionExport( "mexPrintf" ); }
void mexPutVariable() { Dylib::libmex.UninitializedFunctionExport( "mexPutVariable" ); }
void mexRunMexFile() { Dylib::libmex.UninitializedFunctionExport( "mexRunMexFile" ); }
void mexSet() { Dylib::libmex.UninitializedFunctionExport( "mexSet" ); }
void mexSetCallbacks() { Dylib::libmex.UninitializedFunctionExport( "mexSetCallbacks" ); }
void mexSetCleanUpAfterError() { Dylib::libmex.UninitializedFunctionExport( "mexSetCleanUpAfterError" ); }
void mexSetCurrentLocalFunctionTable() { Dylib::libmex.UninitializedFunctionExport( "mexSetCurrentLocalFunctionTable" ); }
void mexSetLocalFunctionTableCallbacks() { Dylib::libmex.UninitializedFunctionExport( "mexSetLocalFunctionTableCallbacks" ); }
void mexSetMexEnterMexLibrary() { Dylib::libmex.UninitializedFunctionExport( "mexSetMexEnterMexLibrary" ); }
void mexSetMexExitMexLibrary() { Dylib::libmex.UninitializedFunctionExport( "mexSetMexExitMexLibrary" ); }
void mexSetSimpleFunctionToString() { Dylib::libmex.UninitializedFunctionExport( "mexSetSimpleFunctionToString" ); }
void mexSetTrapFlag() { Dylib::libmex.UninitializedFunctionExport( "mexSetTrapFlag" ); }
void mexSimpleFunctionToString() { Dylib::libmex.UninitializedFunctionExport( "mexSimpleFunctionToString" ); }
void mexUnlock() { Dylib::libmex.UninitializedFunctionExport( "mexUnlock" ); }
void mexWarnMsgIdAndTxt() { Dylib::libmex.UninitializedFunctionExport( "mexWarnMsgIdAndTxt" ); }
void mexWarnMsgTxt() { Dylib::libmex.UninitializedFunctionExport( "mexWarnMsgTxt" ); }
}

namespace {
enum { flags = Dylib::Import::functionStub | Dylib::Import::optional, };
const Dylib::Import functionImports_[] =
{
  { "mexAtExit", (void**)&mexAtExit, flags },
  { "mexCallMATLAB", (void**)&mexCallMATLAB, flags },
  { "mexCallMATLABWithObject", (void**)&mexCallMATLABWithObject, flags },
  { "mexCallMATLABWithTrap", (void**)&mexCallMATLABWithTrap, flags },
  { "mexCallMATLABWithTrapWithObject", (void**)&mexCallMATLABWithTrapWithObject, flags },
  { "mexCleanUpAfterError", (void**)&mexCleanUpAfterError, flags },
  { "mexClearMexFile", (void**)&mexClearMexFile, flags },
  { "mexCreateSimpleFunctionHandle", (void**)&mexCreateSimpleFunctionHandle, flags },
  { "mexEnterMexLibrary", (void**)&mexEnterMexLibrary, flags },
  { "mexErrMsgIdAndTxt", (void**)&mexErrMsgIdAndTxt, flags },
  { "mexErrMsgTxt", (void**)&mexErrMsgTxt, flags },
  { "mexEvalString", (void**)&mexEvalString, flags },
  { "mexEvalStringWithTrap", (void**)&mexEvalStringWithTrap, flags },
  { "mexExitMexLibrary", (void**)&mexExitMexLibrary, flags },
  { "mexFunctionName", (void**)&mexFunctionName, flags },
  { "mexGet", (void**)&mexGet, flags },
  { "mexGetLocalFunctionTable", (void**)&mexGetLocalFunctionTable, flags },
  { "mexGetVariable", (void**)&mexGetVariable, flags },
  { "mexGetVariablePtr", (void**)&mexGetVariablePtr, flags },
  { "mexGetVariableWithObject", (void**)&mexGetVariableWithObject, flags },
  { "mexIsGlobal", (void**)&mexIsGlobal, flags },
  { "mexIsLocked", (void**)&mexIsLocked, flags },
  { "mexLock", (void**)&mexLock, flags },
  { "mexMakeArrayPersistent", (void**)&mexMakeArrayPersistent, flags },
  { "mexMakeMemoryPersistent", (void**)&mexMakeMemoryPersistent, flags },
  { "mexMexDebug", (void**)&mexMexDebug, flags },
  { "mexMexInformation", (void**)&mexMexInformation, flags },
  { "mexPrintAssertion", (void**)&mexPrintAssertion, flags },
  { "mexPrintf", (void**)&mexPrintf, flags },
  { "mexPutVariable", (void**)&mexPutVariable, flags },
  { "mexRunMexFile", (void**)&mexRunMexFile, flags },
  { "mexSet", (void**)&mexSet, flags },
  { "mexSetCallbacks", (void**)&mexSetCallbacks, flags },
  { "mexSetCleanUpAfterError", (void**)&mexSetCleanUpAfterError, flags },
  { "mexSetCurrentLocalFunctionTable", (void**)&mexSetCurrentLocalFunctionTable, flags },
  { "mexSetLocalFunctionTableCallbacks", (void**)&mexSetLocalFunctionTableCallbacks, flags },
  { "mexSetMexEnterMexLibrary", (void**)&mexSetMexEnterMexLibrary, flags },
  { "mexSetMexExitMexLibrary", (void**)&mexSetMexExitMexLibrary, flags },
  { "mexSetSimpleFunctionToString", (void**)&mexSetSimpleFunctionToString, flags },
  { "mexSetTrapFlag", (void**)&mexSetTrapFlag, flags },
  { "mexSimpleFunctionToString", (void**)&mexSimpleFunctionToString, flags },
  { "mexUnlock", (void**)&mexUnlock, flags },
  { "mexWarnMsgIdAndTxt", (void**)&mexWarnMsgIdAndTxt, flags },
  { "mexWarnMsgTxt", (void**)&mexWarnMsgTxt, flags },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

