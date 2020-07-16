#include "ProcessUtils.h"
#include "QtMain.h"
#include "MainWindow.h"

int main( int argc, char* argv[] )
{
  ProcessUtils::InitializeDesktopIntegration( "org.bci2000.OnlineModules" );
  QtApplication app( argc, argv );
  MainWindow w;
  w.show();
  return app.Run();
}

