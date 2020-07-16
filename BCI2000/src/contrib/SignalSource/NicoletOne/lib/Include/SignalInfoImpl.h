//////////////////////////////////////////////////////////////////////
// Module:  SignalInfoImpl.h
// Purpose: Template functions that implement the Find and Union
//          operations on SensorInfo and SignalInfo.
//
// Copyright 2002 by Taugagreining hf. - All rights reserved
// THIS CODE IS CONFIDENTIAL
// Created: 01.08.2002  PER
//////////////////////////////////////////////////////////////////////

#ifndef __SIGNALINFOIMPL_H_INCLUDED__
#define __SIGNALINFOIMPL_H_INCLUDED__

#include <SignalLookupCache.h>
#include <PacketDef.h>

namespace SignalInfoImpl
{
   ///////////////////////////////////////////////////////////////////
   // FindSignal
   //
   // Searches for a signal with a given name. A cache is used for
   // fast lookup of signals that have been used before. Since this
   // cache is not updated in AddElement, SetElement or SetPacket,
   // all results must be double-checked.
   ///////////////////////////////////////////////////////////////////
   template <typename Info, typename Func>
   LONG FindSignal(const Info* pInfo, CSignalLookupCache* pCache, const WCHAR* pszSignal, Func GetName)
   {
      _ASSERTE(pszSignal != NULL);
      _ASSERTE(wcslen(pszSignal) < LABELSIZE);

      ULONG lcElements = pInfo->GetElementCount();

      // Check in the cache
      ULONG liSignal = pCache->Lookup(pszSignal);
      if (liSignal < lcElements)
      {
         if (wcsnicmp(GetName(pInfo->GetElement(liSignal)), pszSignal, LABELSIZE) == 0)
            return liSignal;
      }

      for (liSignal = 0; liSignal < lcElements; ++liSignal)
      {
         _ASSERTE(wcslen(GetName(pInfo->GetElement(liSignal))) < LABELSIZE);
         if (wcsnicmp(GetName(pInfo->GetElement(liSignal)), pszSignal, LABELSIZE) == 0)
         {
            pCache->Set(pszSignal, liSignal);
            return liSignal;
         }
      }

      return -1;
   }

   ///////////////////////////////////////////////////////////////////
   // Union
   //
   // Merges pPacket into pInfo such that if a signal is found in
   // both, the one in pPacket overwrites the one in pInfo.
   ///////////////////////////////////////////////////////////////////
   template <typename Info, typename Func>
   bool Union(Info* pInfo, CSignalLookupCache* pCache, PACKET pPacket, Func GetName)
   {
      if (!pInfo->IsValid())
         return pInfo->SetPacketPermanent(pPacket);

      Info cNewInfo;
      if (!cNewInfo.SetPacket(pPacket))
         return false;

      for (ULONG liNewSignal = 0; liNewSignal < cNewInfo.GetElementCount(); ++liNewSignal)
      {
         long liSensor = FindSignal(pInfo, pCache, GetName(cNewInfo[liNewSignal]), GetName);
         if (liSensor > -1)
            pInfo->SetElement(liSensor, cNewInfo[liNewSignal]);
         else
            pInfo->AddElement(cNewInfo[liNewSignal]);
      }

      return true;
   }
}

#endif
