#include "QtMain.h"
#include "ProcessUtils.h"
#include "BCI2000FileInfo.h"

int main( int argc, char** argv )
{
  ProcessUtils::InitializeDesktopIntegration( "org.bci2000.Tools" );
  QtApplication app( argc, argv );
  BCI2000FileInfo w;
  w.show();
  return app.Run();
}
