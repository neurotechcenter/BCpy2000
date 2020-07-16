###########################################################################
## $Id: IncludeExtensionMacro.cmake 3201 2011-04-13 16:03:27Z mellinger $
## Authors: jezhill@gmail.com
## Description: Contains a macro for including optional extensions to the BCI2000 framework
##              (for example, loggers which will by incorporated into every SignalSource module)
##              gated by environment variables.

MACRO( INCLUDE_EXTENSION NAME DIR)
  STRING( TOUPPER ${NAME} UPPERNAME )
  SET( SETTINGNAME "EXTENSIONS_${UPPERNAME}" )
  OPTION( ${SETTINGNAME} "Whether to augment the BCI2000 Framework using the contributed ${NAME} extension" OFF )
  SET( SETTINGVAL  ${${SETTINGNAME}} )
  get_filename_component( BCI2000_EXTENSION_DIR "${DIR}" ABSOLUTE )
  IF( ${SETTINGVAL} )
    UTILS_CONFIG_STATUS(
      "Including custom extension ${NAME}\n"
      "---- (to disable, set ${SETTINGNAME} to OFF)"
    )
    INCLUDE( "${BCI2000_EXTENSION_DIR}/IncludeExtension.cmake" )
  ELSE()
    UTILS_CONFIG_STATUS(
      "Skipping custom extension ${NAME}\n"
      "---- (to enable, set ${SETTINGNAME} to ON)"
    )
  ENDIF( ${SETTINGVAL} )
ENDMACRO( INCLUDE_EXTENSION NAME DIR)
