#ifndef __NRVTIME_H__
#define __NRVTIME_H__
/////////////////////////////////////////////////////////////////////////////
//
// Module: nrvtime.h
//
// Purpose: The main time functions declarations. Includes CNDateTimeSpan and
//          CNDateTime.     
//
//
// Copyright 1998 by Taugagreining hf. - All rights reserved
//
// THIS CODE IS CONFIDENTIAL
//
// Created        by GAR & IGA
// Modified       by 
//////////////////////////////////////////////////////////////////////////////
#include <atlstr.h>
#include <nrvdate.h>



class CNDateTimeSpan
{
   friend class CNDateTime;

public:
   CNDateTimeSpan(double dSpan = 0.0);
   CNDateTimeSpan(long lDays, int nHours, int nMins, double dSecs);

// Attributes
public:
   double GetTotalDays() const;    // span in days (about -3.65e6 to 3.65e6)
   double GetTotalHours() const;   // span in hours (about -8.77e7 to 8.77e6)
   double GetTotalMinutes() const; // span in minutes (about -5.26e9 to 5.26e9)
   double GetTotalSeconds() const; // span in seconds (about -3.16e11 to 3.16e11)

   double GetFraction() const;

   CString Format(LPCTSTR pFormat) const;
   CString Format(UINT nFormatID) const;

// Operations
public:
   void SetDateTimeSpan(long lDays, int nHours, int nMins, double dSecs);
   operator double() const;
   CNDateTimeSpan& operator =(double dSpan);
   CString ConvertToText(bool bShort = true, bool bFraction = true, const LPCTSTR lpzDayAbbr = NULL) const;

   // DateTimeSpan math
  	bool operator==(const CNDateTimeSpan& dateSpan) const;
   bool operator==(double dSpan) const;
   bool operator!=(const CNDateTimeSpan& dateSpan) const;
   bool operator!=(double dSpan) const;
   bool operator<(const CNDateTimeSpan& dateSpan) const;
   bool operator<(double dSpan) const;
   bool operator>(const CNDateTimeSpan& dateSpan) const;
   bool operator>(double dSpan) const;
   bool operator<=(const CNDateTimeSpan& dateSpan) const;
   bool operator<=(double dSpan) const;
   bool operator>=(const CNDateTimeSpan& dateSpan) const;
   bool operator>=(double dSpan) const;
	
	CNDateTimeSpan operator+(const CNDateTimeSpan& dateSpan) const;
	CNDateTimeSpan operator-(const CNDateTimeSpan& dateSpan) const;
	CNDateTimeSpan& operator+=(const CNDateTimeSpan& dateSpan);
	CNDateTimeSpan& operator-=(const CNDateTimeSpan& dateSpan);
	CNDateTimeSpan operator-() const;

private:
   double m_dSpan;
};

class CNDateTime
{
public:
   CNDateTime();
   CNDateTime(const NRVDATE& nrvDate);
   CNDateTime(double dDate, double dFraction);

public:
   static CNDateTime GetCurrentTime();
   static CNDateTime GetCurrentTimeWithoutFraction();

   int GetYear() const;
   int GetMonth() const;       // month of year (1 = Jan)
   int GetDay() const;         // day of month (0-31)
   int GetHour() const;        // hour in day (0-23)
   int GetMinute() const;      // minute in hour (0-59)
   int GetSecond() const;      // second in minute (0-59)
   int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
   int GetDayOfYear() const;   // days since start of year, Jan 1 = 1

   double GetSecFraction() const;
   int SetSecFraction(double dFraction);
	int SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, double dFrac);
   int SetDate(int nYear, int nMonth, int nDay);
	int SetTime(int nHour, int nMin, int nSec, double dFrac);  

   CString Format(LPCTSTR pFormat) const;
   CString Format(UINT nFormatID) const;

	CNDateTime& operator=(const NRVDATE& dtSrc);
   CNDateTime& operator=(DATE dt);

   operator NRVDATE() const;
   operator DATE() const;

	bool operator==(const CNDateTime& date) const;
	bool operator!=(const CNDateTime& date) const;
	bool operator<(const CNDateTime& date) const;
	bool operator>(const CNDateTime& date) const;
	bool operator<=(const CNDateTime& date) const;
	bool operator>=(const CNDateTime& date) const;

	// DateTime math
	CNDateTime operator+(const CNDateTimeSpan& dateSpan) const;
	CNDateTime operator-(const CNDateTimeSpan& dateSpan) const;
	CNDateTime& operator+=(const CNDateTimeSpan& dateSpan);
	CNDateTime& operator-=(const CNDateTimeSpan& dateSpan);
	CNDateTimeSpan operator-(const CNDateTime& date) const;

public:
   NRVDATE m_nrvDate;

private:
   static void _MakeValid(NRVDATE& nrvDate);
};


//////////////////////////////////////////////////////////////////////////////
// inline function implementations
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// CNDateTimeSpan inlines
//////////////////////////////////////////////////////////////////////////////
inline CNDateTimeSpan::CNDateTimeSpan(double dSpan)
   : m_dSpan(dSpan)
{
}

inline CNDateTimeSpan::operator double() const
{
   return m_dSpan;
}

inline CNDateTimeSpan& CNDateTimeSpan::operator=(double dSpan)
{
   m_dSpan = dSpan;
   return *this;
}

//////////////////////////////////////////////////////////////////////////////
// CNDateTime inlines
//////////////////////////////////////////////////////////////////////////////
inline CNDateTime::CNDateTime()
{
   m_nrvDate.dDate = 0.0;
   m_nrvDate.dFraction = 0.0;
}

inline CNDateTime::CNDateTime(const NRVDATE& nrvDate)
   : m_nrvDate(nrvDate)
{
   _MakeValid(m_nrvDate);
}

inline CNDateTime::CNDateTime(double dDate, double dFraction)
{
   m_nrvDate.dDate = dDate;
   m_nrvDate.dFraction = dFraction;
   _MakeValid(m_nrvDate);
}

inline CNDateTime& CNDateTime::operator+=(const CNDateTimeSpan& dateSpan)
{
   m_nrvDate.dFraction += dateSpan.m_dSpan;
   _MakeValid(m_nrvDate);
   return *this;
}

inline CNDateTime CNDateTime::operator+(const CNDateTimeSpan& dateSpan) const
{
   CNDateTime cndtResult (*this);
   cndtResult += dateSpan;
   return cndtResult;
}

inline CNDateTime& CNDateTime::operator-=(const CNDateTimeSpan& dateSpan)
{
   m_nrvDate.dFraction -= dateSpan.m_dSpan;
   _MakeValid(m_nrvDate);
   return *this;
}

inline CNDateTime CNDateTime::operator-(const CNDateTimeSpan& dateSpan) const
{
   CNDateTime cndtResult (*this);
   cndtResult -= dateSpan;
   return cndtResult;
}

inline bool CNDateTime::operator<(const CNDateTime& date) const
{
   if (m_nrvDate.dDate < date.m_nrvDate.dDate)
      return true;

   return m_nrvDate.dDate == date.m_nrvDate.dDate && m_nrvDate.dFraction < date.m_nrvDate.dFraction;
}

inline bool CNDateTime::operator>(const CNDateTime& date) const
{
   return (date < *this);
}

inline bool CNDateTime::operator<=(const CNDateTime& date) const
{
   return !(date < *this);
}

inline bool CNDateTime::operator>=(const CNDateTime& date) const
{
   return !(*this < date);
}

inline CNDateTime::operator NRVDATE() const
{
   return m_nrvDate;
}

inline CNDateTime::operator DATE() const
{
   return m_nrvDate.dDate;
}


#endif //__NRVTIME_H__

