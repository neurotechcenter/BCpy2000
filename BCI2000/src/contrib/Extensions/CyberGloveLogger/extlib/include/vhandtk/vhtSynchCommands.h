/********************************************************************
 FILE: $Id: vhtSynchCommands.h,v 1.3 2002/08/28 23:16:24 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Sep 1998
<header>

 DESCRIPTION:
  -
 HISTORY:
  -
</header>
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
#ifndef __VHTSC_H
#define __VHTSC_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif


typedef unsigned char vhtSynchCommand;
#define VHTSC_BASE 100
#define VHTSC_CONNECT_PARENT        VHTSC_BASE + 1
#define VHTSC_DISCONNECT_PARENT     VHTSC_BASE + 2
#define VHTSC_PING                  VHTSC_BASE + 3
#define VHTSC_ADD_NODE              VHTSC_BASE + 4
#define VHTSC_ADD_NODE_READ_ONLY    VHTSC_BASE + 5
#define VHTSC_REMOVE_NODE           VHTSC_BASE + 6
#define VHTSC_REMOVE_ALL_NODES      VHTSC_BASE + 7
#define VHTSC_DISCONNECT_CHILD      VHTSC_BASE + 8
#define VHTSC_UPDATE_BOUNDINGSPHERE VHTSC_BASE + 9
#define VHTSC_UPDATE_LM             VHTSC_BASE + 10
#define VHTSC_UPDATE_TRANSFORM      VHTSC_BASE + 11
#define VHTSC_UPDATE_NODE           VHTSC_BASE + 12

//#include "vhtTransformGroup.h"
#include "vhtBoundingSphere.h"
class vhtNode;

VH_CORE_DLL_EXPORT int vhtscWriteConnectParent(        char *cmdBuf, int cmdBufSize);
VH_CORE_DLL_EXPORT int vhtscWriteDisconnectParent(     char *cmdBuf, int cmdBufSize);
VH_CORE_DLL_EXPORT int vhtscWritePing(                 char *cmdBuf, int cmdBufSize, int id, int pingPort);
VH_CORE_DLL_EXPORT int vhtscWritePingResponse(         char *cmdBuf, int cmdBufSize, int id);
VH_CORE_DLL_EXPORT int vhtscWriteAddNode(              char *cmdBuf, int cmdBufSize, vhtNode *node, bool readOnly=false);
VH_CORE_DLL_EXPORT int vhtscWriteRemoveNode(           char *cmdBuf, int cmdBufSize, vhtNode *node);
VH_CORE_DLL_EXPORT int vhtscWriteRemoveAllNodes(       char *cmdBuf, int cmdBufSize);
VH_CORE_DLL_EXPORT int vhtscWriteDisconnectChild(      char *cmdBuf, int cmdBufSize);
VH_CORE_DLL_EXPORT int vhtscWriteUpdateNode(           char *cmdBuf, int cmdBufSize, const vhtNode *node, int nodeId=-1);
/*
int vhtscWriteUpdateBoundingSphere( char *cmdBuf, int cmdBufSize, const vhtBoundingSphere *boundingSphere, int nodeId=-1);
int vhtscWriteUpdateLM(             char *cmdBuf, int cmdBufSize, const vhtNode *transformGroup, int nodeId=-1);
int vhtscWriteUpdateTransform(      char *cmdBuf, int cmdBufSize, const vhtNode *transformGroup, int nodeId=-1);
*/

#endif



