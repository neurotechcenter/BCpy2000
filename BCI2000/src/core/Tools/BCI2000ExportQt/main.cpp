#include "QtMain.h"
#include "ProcessUtils.h"
#include "MainWindow.h"

int main( int argc, char** argv )
{
  ProcessUtils::InitializeDesktopIntegration( "org.bci2000.Tools" );
  QtApplication app( argc, argv );
  MainWindow w;
  w.show();
  return app.Run();
}
