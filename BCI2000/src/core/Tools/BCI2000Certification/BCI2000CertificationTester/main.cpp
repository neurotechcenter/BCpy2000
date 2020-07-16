#include "QtMain.h"
#include "ProcessUtils.h"
#include "CertLauncherGui.h"

int main(int argc, char* argv[])
{
  ProcessUtils::InitializeDesktopIntegration("org.bci2000.Tools");
  QtApplication app(argc, argv);
  CertLauncherGui gui;
  gui.show();
  return app.Run();
}
