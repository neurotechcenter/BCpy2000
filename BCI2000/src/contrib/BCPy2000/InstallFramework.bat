@echo off & setlocal

echo .
echo This script invokes the setup.py script in .\framework to make a binary
echo installer, copies the installer to the current directory, and runs it
echo The advantage of doing it in two stages like that, instead of just
echo giving the command-line argument "install" and letting python do all the
echo work, is that the Windows installer .exe creates and registers an
echo UNinstaller for you that can be accessed from the Add and Remove Programs
echo control panel.
echo .
echo python.exe doesn't seem to get put on the path by default, so we'll
echo have to run the setup.py script using the START command which is the
echo same as double-clicking.  So it won't work if you have changed the
echo default file association of .py files - for example, such that they
echo open in an editor instead of running.
echo .
echo Note that the BCPy2000 package alone is not enough. Ensure you have all
echo the other dependencies: VisionEgg, pygame, PyOpenGL, NumPy, ..., ... 
echo Check the documentation.
echo .

erase framework\dist\*.exe
start "Building BCPy2000 Installer" /D framework /WAIT BCPy2000_setup.py installer
for /F %%G in ('dir /B framework\dist\*.exe') do (move framework\dist\%%G .\  & start %%G)
