/* (C) 2000-2010, BCI2000 Project
/* http://www.bci2000.org
/*/
#include "PCHIncludes.h"
#pragma hdrstop

// Not using borland, we'll use QT instead
#include "CoreModule_Qt.h"

//---------------------------------------------------------------------------
#ifdef _WIN32

int WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
        bool success = CoreModuleQT().Run( __argc, __argv );
        return success ? 0 : -1;
}

#else // _WIN32

int main( int argc, char *argv[] )
{
  bool success = CoreModuleQT().Run( argc, argv );
  return success ? 0 : -1;
}

#endif // _WIN32
//---------------------------------------------------------------------------

