MFC demo project readme
=======================

The MFC demo project illustrates how to build a BCI2000 application module
under Microsoft Visual C++ using the MFC GUI framework.

When creating your own Application Module on top of this demo, you will
need to modify code of two classes.

TaskFilter:
 A BCI2000 filter that contains the actual functionality of the 
 application module -- registering parameters and states, and trial
 sequencing.

MFCdemoDlg:
 An MFC dialog class that provides a window and a device context for
 graphics output.

Building the demo requires Visual C++ 7.1 (Visual Studio .NET 2003) 
or newer.

Note that run-time type information (RTTI) compiler switch is disabled for 
new projects but must be enabled for the BCI2000 framework. 
Otherwise, exceptions will be thrown when the framework tries to access 
class name information.
