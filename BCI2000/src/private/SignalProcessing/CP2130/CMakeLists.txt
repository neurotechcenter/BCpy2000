###########################################################################
## $Id$
## Authors: belsten@neurotechcenter.org 2018-09-13 
## Description: Build information for the CP2130 filter module

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	find_library(INCLUDE_LIB
		HINTS ${CMAKE_CURRENT_SOURCE_DIR}/extlib/dylib/x64/
		NAMES SLAB_USB_SPI
	)
	SET( FILES_PROJECT
     		${FILES_PROJECT}
      		/extlib/dylib/x64/SLAB_USB_SPI.dll
	)
ELSE()
	find_library(INCLUDE_LIB
		HINTS ${CMAKE_CURRENT_SOURCE_DIR}/extlib/dylib/x86/
		NAMES SLAB_USB_SPI
	)
	SET( FILES_PROJECT
     		${FILES_PROJECT}
      		/extlib/dylib/x86/SLAB_USB_SPI.dll
	)
ENDIF()

bci2000_add_signal_processing_module(
	CP2130 NOBUNDLE
	${FILES_PROJECT}
	PipeDefinition.cpp
	CP2130Filter.cpp
	eeprom.cpp
	si8902.cpp
)

TARGET_LINK_LIBRARIES(CP2130 ${INCLUDE_LIB})