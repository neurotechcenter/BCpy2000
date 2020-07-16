bci2000_add_signal_processing_module(
  SignalSharingDemoSignalProcessing
  SignalSharingDemoFilter.cpp
  PipeDefinition.cpp
  SUPPORT ../SignalSharingDemo.bat=>/batch
  DEPENDS Operator
  DEPENDS SignalGenerator
  DEPENDS DummyApplication
)


