bci2000_add_tools_guiapp(
  SignalSharingDemoClientApp
  SignalSharingDemoClientApp.cpp
  SignalSharingDemoWidget.cpp
  SUPPORT ../SignalSharingDemo.bat=>/batch
  OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/demos
  DEPENDS SignalSharingDemoSignalProcessing
)

