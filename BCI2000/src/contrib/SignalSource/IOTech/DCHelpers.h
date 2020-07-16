#ifndef DCHELPERS_H
#define DCHELPERS_H

#include <atlconv.h>
#include <atlbase.h>
#include <atlstr.h>

#import "DaqCOM2.tlb" rename("LoadString", "DaqCOM2LoadString")
using namespace DAQCOMLib;

#define WM_KICKIDLE     0x036A  // (params unused) causes idles to kick in
#define dcTRY                  try{
#define dcCATCH             }catch(_com_error &e){dump_daqcom_error(e);}

bool bContains(unsigned int* list, unsigned int n, unsigned int findme);
void dump_daqcom_error(_com_error &e);
bool DoEvents();

#endif // DCHELPERS_CPP_INCLUDED
