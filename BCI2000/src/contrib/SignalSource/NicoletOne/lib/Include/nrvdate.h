#ifndef __NRVDATE_H__
#define __NRVDATE_H__

typedef struct tagNRVDATE
{
   double dDate;
   double dFraction;
}NRVDATE;

typedef struct tagTIMESTAMP
{
   NRVDATE nrvStamp;
   double dDuration;
}TIMESTAMP;

#define DAYSECS 86400.0
#define DBL2DATE(dDouble) (LONG)(dDouble*DAYSECS)/DAYSECS
#define DBL2FRAC(dDouble) dDouble - (LONG)(dDouble*DAYSECS)/DAYSECS
#define SECS2DATE(dSecs) (LONG)dSecs/DAYSECS
#define SECS2FRAC(dSecs) dSecs - (LONG)dSecs

#define NRVDATE2SECS(nrvDate) nrvDate.dDate*DAYSECS + nrvDate.dFraction
#define MAXNRVDATE(nrvDate1, nrvDate2) (nrvDate1.dDate > nrvDate2.dDate) ? nrvDate1 : ( (nrvDate1.dDate < nrvDate2.dDate) ? nrvDate2 : \
      ((nrvDate1.dFraction > nrvDate2.dFraction) ? nrvDate1 : nrvDate2))
#define MINNRVDATE(nrvDate1, nrvDate2) (nrvDate1.dDate < nrvDate2.dDate) ? nrvDate1 : ( (nrvDate1.dDate > nrvDate2.dDate) ? nrvDate2 : \
   ((nrvDate1.dFraction < nrvDate2.dFraction) ? nrvDate1 : nrvDate2))

#endif //__NRVDATE_H__
