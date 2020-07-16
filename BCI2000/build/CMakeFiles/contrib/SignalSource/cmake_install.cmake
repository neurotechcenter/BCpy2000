# Install script for directory: C:/BCI2000.python3/src/contrib/SignalSource

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/BCI2000")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/actiCHamp/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/AmpServerPro/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/BioRadio/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Biosemi/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/B-Alert/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/DASSource/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Emotiv/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Enobio/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Micromed/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/ModularEEG/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Neuralynx/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/NeuroscanAccessSDK/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/NeuroSky/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/NIDAQ-MX/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/TMSi/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/TuckerDavis/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/vAmp/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/EEGOSource/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/LiveAmp/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/RDAClient/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Neuroscan/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/FieldTripBufferSource/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/FilePlayback/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Blackrock/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/Ripple/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/DSISerial/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/LSLSource/cmake_install.cmake")
  include("C:/BCI2000.python3/build/CMakeFiles/contrib/SignalSource/OpenBCISource/cmake_install.cmake")

endif()

