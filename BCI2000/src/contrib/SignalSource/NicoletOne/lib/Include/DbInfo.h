// DbInfo.h: interface for the CDbInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBINFO_H__1E5EA021_CE33_4DE3_97B2_9068C05586EE__INCLUDED_)
#define AFX_DBINFO_H__1E5EA021_CE33_4DE3_97B2_9068C05586EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TextInfoBase.h"
#include <float.h>

static const GUID TESTINFOGUID = 
{ 0x35f356d9, 0xf1c, 0x4dfe, { 0x82, 0x86, 0xd3, 0xdb, 0x33, 0x46, 0xfd, 0x75 } };

class CTestInfo : public CTextInfoBase
{
public:
	CTestInfo()
   {
	_SetGuid(TESTINFOGUID);

   m_dHoursSinceLastMeal = -10.0;
   m_dPortNumber = 0.0;

   _AddItem(4, &m_bstrTechnicianID);
	_AddItem(6, &m_bstrPhysicianID);
	_AddItem(8, &m_bstrSecretaryID);
	_AddItem(10, &m_bstrSiteID);
	_AddItem(11, &m_bstrSiteDesc);
	_AddItem(12, &m_bstrNotes);
	_AddItem(16, &m_dHoursSinceLastMeal);
	_AddItem(17, &m_bstrMachineName);
   _AddItem(18, &m_bstrOperationRoomID);
   _AddItem(19, &m_bstrDiagnosis);
   _AddItem(20, &m_bstrSurgeon);
   _AddItem(21, &m_bstrAnesthesiologist);
   _AddItem(22, &m_bstrNeurophysiologist);
   _AddItem(23, &m_bstrOperationRoom);
   _AddItem(24, &m_bstrServerID);
   _AddItem(25, &m_bstrMonitorID);
   _AddItem(26, &m_dPortNumber);
   }

public:
   _bstr_t m_bstrTechnicianID;
	_bstr_t m_bstrPhysicianID;
	_bstr_t m_bstrSecretaryID;
	_bstr_t m_bstrSiteID;
	_bstr_t m_bstrSiteDesc;
	_bstr_t m_bstrNotes;
	double m_dHoursSinceLastMeal;
	_bstr_t m_bstrMachineName;
   _bstr_t m_bstrOperationRoomID;
   _bstr_t m_bstrOperationRoom;
   _bstr_t m_bstrServerID;
   _bstr_t m_bstrMonitorID;
   double m_dPortNumber;
   _bstr_t m_bstrDiagnosis;
   _bstr_t m_bstrSurgeon;
   _bstr_t m_bstrAnesthesiologist;
   _bstr_t m_bstrNeurophysiologist;
};

// {02948284-17EC-4538-A7FA-8E18BD65E167}
static const GUID STUDYINFOGUID = 
{ 0x2948284, 0x17ec, 0x4538, { 0xa7, 0xfa, 0x8e, 0x18, 0xbd, 0x65, 0xe1, 0x67 } };

class CStudyInfo : public CTextInfoBase
{
public:
   CStudyInfo()
   {
   _SetGuid(STUDYINFOGUID);

   m_dStudyCreationTime = -DBL_MAX;

   _AddItem(1, &m_bstrStudyID);
	_AddItem(2, &m_bstrStudyTypeID);
   _AddItem(3, &m_bstrStudyTypeDesc);
	_AddItem(13, &m_bstrStudyNo);
	_AddItem(20, &m_bstrMedication);           //INStudy
	_AddItem(21, &m_bstrPatientState);         //INStudy
	_AddItem(22, &m_bstrRequestedBy);          //INStudy
   _AddItem(23, &m_dStudyCreationTime);
   _AddItem(24, &m_bstrNotes);
   }

public:
   _bstr_t m_bstrStudyID;
	_bstr_t m_bstrStudyNo;
	_bstr_t m_bstrStudyTypeID;
   _bstr_t m_bstrStudyTypeDesc;
	_bstr_t m_bstrRequestedBy;          //INStudy
   double m_dStudyCreationTime;
	_bstr_t m_bstrMedication;           //INStudy
	_bstr_t m_bstrPatientState;         //INStudy
	_bstr_t m_bstrNotes;
};

// {D0B3FD0B-49D9-4bf0-8929-296DE5A55910}
static const GUID PATIENTINFOGUID = 
{ 0xd0b3fd0b, 0x49d9, 0x4bf0, { 0x89, 0x29, 0x29, 0x6d, 0xe5, 0xa5, 0x59, 0x10 } };


class CPatientInfo : public CTextInfoBase 
{
public:
	CPatientInfo()
   {
   _SetGuid(PATIENTINFOGUID);

   m_dDOB = -DBL_MAX;
   m_dDOD = -DBL_MAX;
   m_dHeight = -10.0;
   m_dWeight = -10.0;

   _AddItem(1, &m_bstrPatientID);
   _AddItem(2, &m_bstrFirstName);
   _AddItem(3, &m_bstrMiddleNames);
   _AddItem(4, &m_bstrLastName);
   _AddItem(5, &m_bstrAlternativeID);
   _AddItem(6, &m_bstrMothersMaidenName);
   _AddItem(7, &m_dDOB);
   _AddItem(8, &m_dDOD);
   _AddItem(9, &m_bstrStreetAddress);
   _AddItem(10, &m_bstrSexID);
   _AddItem(11, &m_bstrTelephone);
   _AddItem(12, &m_bstrNotes);
   _AddItem(13, &m_bstrDominance);
   _AddItem(14, &m_bstrSiteID);

   _AddItem(15, &m_bstrSuffix);
   _AddItem(16, &m_bstrPrefix);
   _AddItem(17, &m_bstrDegree);
   _AddItem(18, &m_bstrApartment);
   _AddItem(19, &m_bstrCity);
   _AddItem(20, &m_bstrState);
   _AddItem(21, &m_bstrCountry);
   _AddItem(22, &m_bstrLanguage);
   _AddItem(23, &m_dHeight);
   _AddItem(24, &m_dWeight);
   _AddItem(25, &m_bstrRace);
   _AddItem(26, &m_bstrReligion);
   _AddItem(27, &m_bstrMaritialStatus);
   }

public:
   _bstr_t m_bstrPatientID;
   _bstr_t m_bstrFirstName;
   _bstr_t m_bstrMiddleNames;
   _bstr_t m_bstrLastName;
   _bstr_t m_bstrAlternativeID;
   _bstr_t m_bstrMothersMaidenName;
   double m_dDOB;
   double m_dDOD;
   _bstr_t m_bstrStreetAddress;
   _bstr_t m_bstrSexID;
   _bstr_t m_bstrTelephone;
   _bstr_t m_bstrNotes;
   _bstr_t m_bstrDominance;
   _bstr_t m_bstrSiteID;

   _bstr_t m_bstrSuffix;
   _bstr_t m_bstrPrefix;
   _bstr_t m_bstrDegree;
   _bstr_t m_bstrApartment;
   _bstr_t m_bstrCity;
   _bstr_t m_bstrState;
   _bstr_t m_bstrCountry;
   _bstr_t m_bstrLanguage;
   double m_dHeight;
   double m_dWeight;
   _bstr_t m_bstrRace;
   _bstr_t m_bstrReligion;
   _bstr_t m_bstrMaritialStatus;
};

// {7842FEF5-A686-459d-8196-769FC0AD99B3}
static const GUID DOCUMENTINFOGUID = 
{ 0x7842fef5, 0xa686, 0x459d, { 0x81, 0x96, 0x76, 0x9f, 0xc0, 0xad, 0x99, 0xb3 } };

class CDocInfo
{
public:
   CDocInfo() {}
   bool IsValid() const {return m_bstrDocumentID.length() != 0;}
   _bstr_t GetFileName() const
   {
   if (m_bstrFileName.length() > 0)
      return m_bstrFileName;

   return m_bstrDocumentID;
   }

   // INDocument*
   _bstr_t m_bstrTitle;
   _bstr_t m_bstrDocumentID;
   double m_dCreationTime;
   double m_dLastModifiedTime;
   _bstr_t m_bstrFileName;

   // INDocumentType*
   _bstr_t m_bstrDesc;
   _bstr_t m_bstrAbbr;
   _bstr_t m_bstrDocTypeID;
};

class CDocumentsInfo : public CArrayTextInfoBase<CDocInfo>
{
public:
   CDocumentsInfo()
   {
   _SetGuid(DOCUMENTINFOGUID);
   }
public:
   long FindDoc(_bstr_t bstrID) const
   {
   if (bstrID.length() == 0)
      return -1;

   for (ULONG liDoc = 0; liDoc < GetElementCount(); ++liDoc)
   {
      if ((*this)[liDoc].m_bstrDocumentID.length() > 0)
      {
         if (wcsicmp(bstrID, (*this)[liDoc].m_bstrDocumentID) == 0)
            return liDoc;
      }
   }

   return -1;
   }
   ULONG AddDoc(_bstr_t bstrID)
   {
   _ASSERTE(FindDoc(bstrID) < 0);

   ULONG liDoc;
   for (liDoc = 0; liDoc < GetElementCount(); ++liDoc)
   {
      if ((*this)[liDoc].m_bstrDocumentID.length() == 0)
      {
         (*this)[liDoc] = CDocInfo();
         (*this)[liDoc].m_bstrDocumentID = bstrID;

         return liDoc;
      }
   }

   liDoc = AddElement();
   (*this)[liDoc].m_bstrDocumentID = bstrID;
   return liDoc;
   }

protected:
   virtual void _AddItems(ULONG liDoc)
   {
   CDocInfo& doc = (*this)[liDoc];

   _AddItem(MAKELONG(1, liDoc), &doc.m_bstrTitle);
   _AddItem(MAKELONG(2, liDoc), &doc.m_bstrDocumentID);
   _AddItem(MAKELONG(3, liDoc), &doc.m_dCreationTime);
   _AddItem(MAKELONG(4, liDoc), &doc.m_dLastModifiedTime);
   _AddItem(MAKELONG(8, liDoc), &doc.m_bstrFileName);

   _AddItem(MAKELONG(5, liDoc), &doc.m_bstrDesc);
   _AddItem(MAKELONG(6, liDoc), &doc.m_bstrAbbr);
   _AddItem(MAKELONG(7, liDoc), &doc.m_bstrDocTypeID);
   }
};

// {BCDAEE87-2496-4df4-B07C-8B4E31E3C495}
static const GUID USERSINFOGUID = 
{ 0xbcdaee87, 0x2496, 0x4df4, { 0xb0, 0x7c, 0x8b, 0x4e, 0x31, 0xe3, 0xc4, 0x95 } };

class CUsrInfo
{
public:
   _bstr_t m_bstrID;
   _bstr_t m_bstrName;
   _bstr_t m_bstrAbbreviation;
   _bstr_t m_bstrJobTitle;
   _bstr_t m_bstrFullName;
};

class CUsersInfo : public CArrayTextInfoBase<CUsrInfo>
{
public:
   CUsersInfo()
   {
   _SetGuid(USERSINFOGUID);
   }

public:
   long FindUser(_bstr_t bstrID) const
   {
   if (bstrID.length() == 0)
      return -1;

   for (ULONG iUser = 0; iUser < GetElementCount(); ++iUser)
   {
      if ((*this)[iUser].m_bstrID.length() > 0)
      {
         if (wcsicmp(bstrID, (*this)[iUser].m_bstrID) == 0)
            return iUser;
      }
   }

   return -1;

   }
   ULONG AddUser(_bstr_t bstrID)
   {
   _ASSERTE(FindUser(bstrID) < 0);

   ULONG iUser;
   for (iUser = 0; iUser < GetElementCount(); ++iUser)
   {
      if ((*this)[iUser].m_bstrID.length() == 0)
      {
         (*this)[iUser] = CUsrInfo();
         (*this)[iUser].m_bstrID = bstrID;

         return iUser;
      }
   }

   iUser = AddElement();
   (*this)[iUser].m_bstrID = bstrID;
   return iUser;
   }
   long FindOrAdd(_bstr_t bstrID)
   {
   long liUser = FindUser(bstrID);
   if (liUser < 0)
      liUser = AddUser(bstrID);

   return liUser;
   }

protected:
   virtual void _AddItems(ULONG liUser)
   {
   CUsrInfo& usr = (*this)[liUser];

   _AddItem(MAKELONG(1, liUser), &usr.m_bstrID);
   _AddItem(MAKELONG(2, liUser), &usr.m_bstrName);
   _AddItem(MAKELONG(3, liUser), &usr.m_bstrAbbreviation);
   _AddItem(MAKELONG(4, liUser), &usr.m_bstrJobTitle);
   _AddItem(MAKELONG(5, liUser), &usr.m_bstrFullName);
   }
};

#endif // !defined(AFX_DBINFO_H__1E5EA021_CE33_4DE3_97B2_9068C05586EE__INCLUDED_)
