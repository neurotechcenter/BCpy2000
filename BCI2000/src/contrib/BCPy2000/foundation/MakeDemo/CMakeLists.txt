# TODO:
#
# The purpose of this file is to make and package the BCPy2000 demo. It should:
# 
# (1) declare dependencies (don't know how to do that yet) on the following other targets:
# 
# 		BCI2000RemoteLib.dll
# 		BCI2000PythonBindings.py
# 		BCI2000Shell.exe
# 		Operator.exe
# 		OperatorLib.dll
# 		PythonApplication.exe
# 		PythonSignalProcessing.exe
# 		PythonSource.exe
#
# (2) copy all of the above from prog to src/contrib/BCPy2000/demo/prog directory after building
#     (also don't know how to do that yet because some files' names will actually change---e.g.
#     lose their .exe extension, change their .dll extension---from platform to platform).



# NB: should switch to building Python*.exe modules in the main global prog directory by default,
#     and copying them here post-build, rather than vice versa.
# 
# Once this is finished, append ADD_SUBDIRECTORY( MakeDemo ) to ../CMakeLists.txt 
