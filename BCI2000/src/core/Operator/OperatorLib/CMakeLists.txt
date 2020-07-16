###########################################################################
## $Id: CMakeLists.txt 5775 2018-06-04 13:03:53Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the OperatorLib

# Set the executable name
SET( NAME OperatorLib )

SET( SRC_PROJECT
  include/BCI_OperatorLib.h
  src/BCI_OperatorLib.cpp
  src/CallbackBase.cpp
  src/StateMachine.cpp
  src/CoreClient.cpp
  src/EventLink.cpp
  src/TelnetServer.cpp
  src/ScriptInterpreter.cpp
  src/CommandInterpreter.cpp

  src/ObjectTypes/ObjectType.cpp
  src/ObjectTypes/ConditionType.cpp
  src/ObjectTypes/ImpliedType.cpp
  src/ObjectTypes/SystemTypes.cpp
  src/ObjectTypes/SystemStates.cpp
  src/ObjectTypes/ParameterTypes.cpp
  src/ObjectTypes/StateTypes.cpp
  src/ObjectTypes/EventTypes.cpp
  src/ObjectTypes/VariableTypes.cpp
  src/ObjectTypes/SignalTypes.cpp
  src/ObjectTypes/VisPropertyTypes.cpp
  src/ObjectTypes/MessageTypes.cpp
  src/ObjectTypes/ScriptType.cpp
  src/ObjectTypes/ScriptEvents.cpp
  src/ObjectTypes/InterpreterExpression.cpp
  src/ObjectTypes/Watches.cpp
  src/ObjectTypes/WatchTypes.cpp
  src/ObjectTypes/ExpressionType.cpp
  src/ObjectTypes/FileSystemTypes.cpp
  src/ObjectTypes/LineType.cpp
  src/ObjectTypes/ProcessType.cpp

  src/ScriptParser/Script.cpp
  src/ScriptParser/ParserToken.cpp
  src/ScriptParser/ParserNodes.cpp
  src/ScriptParser/ScriptParser.cpp

  ${BCI2000_SRC_DIR}/shared/bcistream/BCIStream_opdll.cpp
)

IF( WIN32 )
  SET( SRC_PROJECT
    ${SRC_PROJECT}
    src/Main.cpp
  )
ENDIF( WIN32 )

UTILS_INCLUDE( frameworks/Core )
utils_include( frameworks/LibTiny )
utils_set_appicon( puzzlepiece )
BCI2000_ADD_TARGET( INFO "Operator library" DLL ${NAME} ${SRC_PROJECT} )

# Add Pre-processor defines
IF( WIN32 )
  SET_PROPERTY( TARGET ${NAME} APPEND PROPERTY COMPILE_FLAGS "-DLIBOPERATOR_LIBRARY" )
ENDIF( WIN32 )
