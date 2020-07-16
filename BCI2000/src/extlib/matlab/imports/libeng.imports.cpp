// Import declarations for libeng library
// generated Tue Mar 01 12:24:30 2016 by DylibTool

#if _WIN32
# include <Windows.h>
#endif
#include "FileUtils.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

namespace {
std::string FindMatlabLibdir()
{
#if _WIN32
  const char* exefile = "matlab.exe";
  DWORD count = ::SearchPathA( 0, exefile, 0, 0, 0, 0 );
  std::vector<char> buf( count );
  if( count )
  {
    char* pFile = 0;
    ::SearchPathA( 0, exefile, 0, count, buf.data(), &pFile );
    if( pFile )
      *pFile = 0;
  }
  std::string path = buf.data();
  if( sizeof( void* ) == 8 )
    path += "win64\\";
  else
    path += "win32\\";
  if( !FileUtils::IsDirectory( path ) )
    path = buf.data();
  return path;
}
#else
# error
#endif
}

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "http://www.mathworks.com";
RegisterDylibWithAliases( libeng, (FindMatlabLibdir() + "libeng").c_str(), functionImports, notFoundMsg, notFoundURL );

extern "C" {
// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
//
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void engClose() { Dylib::libeng.UninitializedFunctionExport( "engClose" ); }
void engEvalString() { Dylib::libeng.UninitializedFunctionExport( "engEvalString" ); }
void engGetVariable() { Dylib::libeng.UninitializedFunctionExport( "engGetVariable" ); }
void engGetVisible() { Dylib::libeng.UninitializedFunctionExport( "engGetVisible" ); }
void engOpen() { Dylib::libeng.UninitializedFunctionExport( "engOpen" ); }
void engOpenSingleUse() { Dylib::libeng.UninitializedFunctionExport( "engOpenSingleUse" ); }
void engOutputBuffer() { Dylib::libeng.UninitializedFunctionExport( "engOutputBuffer" ); }
void engPutVariable() { Dylib::libeng.UninitializedFunctionExport( "engPutVariable" ); }
void engSetVisible() { Dylib::libeng.UninitializedFunctionExport( "engSetVisible" ); }
}

namespace {
enum { flags = Dylib::Import::functionStub | Dylib::Import::optional, };
const Dylib::Import functionImports_[] =
{
  { "engClose", (void**)&engClose, flags },
  { "engEvalString", (void**)&engEvalString, flags },
  { "engGetVariable", (void**)&engGetVariable, flags },
  { "engGetVisible", (void**)&engGetVisible, flags },
  { "engOpen", (void**)&engOpen, flags },
  { "engOpenSingleUse", (void**)&engOpenSingleUse, flags },
  { "engOutputBuffer", (void**)&engOutputBuffer, flags },
  { "engPutVariable", (void**)&engPutVariable, flags },
  { "engSetVisible", (void**)&engSetVisible, flags },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

