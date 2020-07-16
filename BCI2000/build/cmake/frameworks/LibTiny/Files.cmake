###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: List of files belonging to the LibTiny utils library.

SET( TINY_DIR
  ${PROJECT_SRC_DIR}/shared/utils/Lib
)

SET( SRC_LIBTINY

  ${TINY_DIR}/Uncopyable.h
  ${TINY_DIR}/Lockable.cpp
  ${TINY_DIR}/Runnable.h
  ${TINY_DIR}/SharedPointer.h
  ${TINY_DIR}/ObjectPool.h
  ${TINY_DIR}/StaticObject.h
  ${TINY_DIR}/LazyArray.h
  ${TINY_DIR}/Broadcaster.cpp

  ${TINY_DIR}/Atomic.h
  ${TINY_DIR}/Synchronized.h
  ${TINY_DIR}/SynchronizedQueue.h
  ${TINY_DIR}/SpinLock.h
  ${TINY_DIR}/Mutex.cpp
  ${TINY_DIR}/SelfPipe.cpp
  ${TINY_DIR}/SelfPipeQueue.h

  ${TINY_DIR}/Waitable.cpp
  ${TINY_DIR}/WaitableEvent.cpp
  ${TINY_DIR}/Semaphore.cpp
  ${TINY_DIR}/WaitableImplementation.cpp

  ${TINY_DIR}/Thread.cpp
  ${TINY_DIR}/ReusableThread.cpp
  ${TINY_DIR}/ThreadLocal.cpp
  ${TINY_DIR}/ThreadUtils.cpp

  ${TINY_DIR}/ChildProcess.cpp
  ${TINY_DIR}/ProcessUtils.cpp

  ${TINY_DIR}/FilePath.cpp
  ${TINY_DIR}/FileUtils.cpp
  ${TINY_DIR}/DylibImports.cpp
  ${TINY_DIR}/SysError.cpp
  ${TINY_DIR}/SharedMemory.cpp

  ${TINY_DIR}/StringUtils.cpp  
  ${TINY_DIR}/EscapedString.cpp
  ${TINY_DIR}/ClassName.cpp
  ${TINY_DIR}/WildcardMatch.cpp
  ${TINY_DIR}/EnvVariable.cpp

  ${TINY_DIR}/TimeUtils.cpp
  ${TINY_DIR}/Clock.cpp
  ${TINY_DIR}/StopWatch.h

  ${TINY_DIR}/IOObjects.cpp
  ${TINY_DIR}/Streambuf.cpp

  ${TINY_DIR}/sockstream.cpp
  ${TINY_DIR}/MemoryBuffer.cpp
  ${TINY_DIR}/FdObject.cpp
  ${TINY_DIR}/Sockets.cpp
  ${TINY_DIR}/Files.cpp
  ${TINY_DIR}/FileMapping.cpp  
  
  ${TINY_DIR}/NullStream.h

  ${TINY_DIR}/ExceptionCatcher.cpp
  ${TINY_DIR}/Exception.cpp
  ${TINY_DIR}/Debugging.cpp
  
  ${TINY_DIR}/LCRandomGenerator.cpp
  ${TINY_DIR}/Uuid.h
  ${TINY_DIR}/BinaryData.cpp
  ${TINY_DIR}/Numeric.h
  ${TINY_DIR}/Ratio.cpp
)

if(MSVC)
  list(APPEND SRC_LIBTINY
    ${PROJECT_SRC_DIR}/shared/compat/msvc_stdlib.cpp
  )
endif()

if( WIN32 )
  list( APPEND SRC_LIBTINY
    ${TINY_DIR}/PEHeader.cpp
  )
elseif( APPLE )
  list( APPEND SRC_LIBTINY
    ${TINY_DIR}/MachOHeader.cpp
  )
endif()


