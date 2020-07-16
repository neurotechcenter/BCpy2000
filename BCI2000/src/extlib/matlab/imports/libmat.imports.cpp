// Import declarations for libmat library
// generated Tue Mar 01 12:24:19 2016 by DylibTool
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( libmat, "libmat", functionImports, notFoundMsg, notFoundURL );

extern "C" {
// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
//
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void matClose() { Dylib::libmat.UninitializedFunctionExport( "matClose" ); }
void matCreateMATFile() { Dylib::libmat.UninitializedFunctionExport( "matCreateMATFile" ); }
void matCreateVariable() { Dylib::libmat.UninitializedFunctionExport( "matCreateVariable" ); }
void matDeleteVariable() { Dylib::libmat.UninitializedFunctionExport( "matDeleteVariable" ); }
void matGetDir() { Dylib::libmat.UninitializedFunctionExport( "matGetDir" ); }
void matGetErrno() { Dylib::libmat.UninitializedFunctionExport( "matGetErrno" ); }
void matGetFileType() { Dylib::libmat.UninitializedFunctionExport( "matGetFileType" ); }
void matGetFp() { Dylib::libmat.UninitializedFunctionExport( "matGetFp" ); }
void matGetNextVariable() { Dylib::libmat.UninitializedFunctionExport( "matGetNextVariable" ); }
void matGetNextVariableInfo() { Dylib::libmat.UninitializedFunctionExport( "matGetNextVariableInfo" ); }
void matGetVariable() { Dylib::libmat.UninitializedFunctionExport( "matGetVariable" ); }
void matGetVariableInfo() { Dylib::libmat.UninitializedFunctionExport( "matGetVariableInfo" ); }
void matGetVariableSubset() { Dylib::libmat.UninitializedFunctionExport( "matGetVariableSubset" ); }
void matGrowVariable() { Dylib::libmat.UninitializedFunctionExport( "matGrowVariable" ); }
void matOpen() { Dylib::libmat.UninitializedFunctionExport( "matOpen" ); }
void matPutNextVariable() { Dylib::libmat.UninitializedFunctionExport( "matPutNextVariable" ); }
void matPutVariable() { Dylib::libmat.UninitializedFunctionExport( "matPutVariable" ); }
void matPutVariableAsGlobal() { Dylib::libmat.UninitializedFunctionExport( "matPutVariableAsGlobal" ); }
void matPutVariableSubset() { Dylib::libmat.UninitializedFunctionExport( "matPutVariableSubset" ); }
}

namespace {
enum { flags = Dylib::Import::functionStub | Dylib::Import::optional, };
const Dylib::Import functionImports_[] =
{
  { "matClose", (void**)&matClose, flags },
  { "matCreateMATFile", (void**)&matCreateMATFile, flags },
  { "matCreateVariable", (void**)&matCreateVariable, flags },
  { "matDeleteVariable", (void**)&matDeleteVariable, flags },
  { "matGetDir", (void**)&matGetDir, flags },
  { "matGetErrno", (void**)&matGetErrno, flags },
  { "matGetFileType", (void**)&matGetFileType, flags },
  { "matGetFp", (void**)&matGetFp, flags },
  { "matGetNextVariable", (void**)&matGetNextVariable, flags },
  { "matGetNextVariableInfo", (void**)&matGetNextVariableInfo, flags },
  { "matGetVariable", (void**)&matGetVariable, flags },
  { "matGetVariableInfo", (void**)&matGetVariableInfo, flags },
  { "matGetVariableSubset", (void**)&matGetVariableSubset, flags },
  { "matGrowVariable", (void**)&matGrowVariable, flags },
  { "matOpen", (void**)&matOpen, flags },
  { "matPutNextVariable", (void**)&matPutNextVariable, flags },
  { "matPutVariable", (void**)&matPutVariable, flags },
  { "matPutVariableAsGlobal", (void**)&matPutVariableAsGlobal, flags },
  { "matPutVariableSubset", (void**)&matPutVariableSubset, flags },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

