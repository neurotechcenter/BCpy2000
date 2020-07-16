SET(BCI2000_PRIVATE_EXTLIB  "${BCI2000_SRC_DIR}/private/extlib" )

INCLUDE_EXTENSION( AudioInputFilter "${BCI2000_SRC_DIR}/private/Extensions/AudioInputFilter" )
INCLUDE_EXTENSION( GaugeExtension "${BCI2000_SRC_DIR}/private/Extensions/GaugeExtension" )
INCLUDE_EXTENSION( gEstimFilter "${BCI2000_SRC_DIR}/private/Extensions/gEstimFilter" )

# Revised cmake spaghetti as of 2011-01-14
# 
# CMakeLists.txt includes cmake/SetupBCI2000Macros.cmake
# 
#     cmake/SetupBCI2000Macros.cmake includes cmake/SignalSourceMacro.cmake (among others)
# 
#         cmake/SignalSourceMacro.cmake includes cmake/SignalSourceModules.cmake   (NEW LOCATION)
# 
#             cmake/SignalSourceModules.cmake sets up BCI2000_SIGSRC* variables, leaving them poised
#             to add AlignmentFilter, DataIOFilter, TransmissionFilter, JoystickLogger, KeyLogger
# 
#         cmake/SignalSourceMacro.cmake defines macro BCI2000_ADD_SIGNAL_SOURCE_MODULE
# 
# CMakeLists.txt includes ../src/private/PrivateConfig.cmake (if it's there)
# 
#     ../src/private/PrivateConfig.cmake extends BCI2000_SIGSRC* variables (if environment variables dictate)
# 
# [time passes]
# 
# ../src/contrib/SignalSource/XYZTech/CMakeLists.txt calls the BCI2000_ADD_SIGNAL_SOURCE_MODULE macro
# 
#     cmake/SignalSourceMacro.cmake includes cmake/DefaultSigSrcFramework.cmake
# 
#          cmake/DefaultSigSrcFramework.cmake includes cmake/DefaultBCI2000Framework.cmake
# 
#              cmake/DefaultBCI2000Framework.cmake initializes a set of SRC_BCI2000_SHARED_* and HDR_BCI2000_SHARED_* variables
#
#          cmake/DefaultSigSrcFramework.cmake extends the set of SRC_BCI2000_SHARED_* and HDR_BCI2000_SHARED_* variables, as
#          well as the content of some pre-existing ones, the effect of all of which is to add:
#          RandomGenerator, OSThread, all kinds of FileWriter stuff
# 
#          cmake/DefaultSigSrcFramework.cmake collates SRC_BCI2000_SHARED_* into SRC_BCI2000_FRAMEWORK
#          cmake/DefaultSigSrcFramework.cmake collates HDR_BCI2000_SHARED_* into HDR_BCI2000_FRAMEWORK
#          and *_BCI2000_SHARED_* are never heard from again
# 
#     (OLD LOCATION of cmake/SignalSourceModules.cmake include was here)
# 
#     cmake/SignalSourceMacro.cmake appends BCI2000_SIGSRCSOURCES to SRC_BCI2000_FRAMEWORK
#     cmake/SignalSourceMacro.cmake appends BCI2000_SIGSRCHEADERS to HDR_BCI2000_FRAMEWORK
#     and BCI2000_SIGSRC*   are never heard from again
# 
