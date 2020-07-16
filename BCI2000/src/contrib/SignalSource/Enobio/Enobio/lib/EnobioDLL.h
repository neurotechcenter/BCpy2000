#ifndef ___CEnobioCtrl_HEADER__
#define ___CEnobioCtrl_HEADER__
class Enobio;
#ifdef __cplusplus
extern "C" {
#endif

#define FUNCTION __declspec(dllimport)
  FUNCTION Enobio* __cdecl  CEnobioCreate();
  FUNCTION bool __cdecl     CEnobioInitialize(Enobio* enobioClass,int sampleBlockSize);
  FUNCTION bool __cdecl     CEnobioCaptureStart (Enobio* enobioClass) ;
  FUNCTION bool __cdecl     CEnobioWaitForData(Enobio* enobioClass,int** buf,int sampleBlockSize);
  FUNCTION bool __cdecl     CEnobioCaptureStop (Enobio* enobioClass);
  FUNCTION bool __cdecl     CEnobioClose(Enobio* enobioClass);
  FUNCTION int __cdecl      CEnobioGetLastError(Enobio* enobioClass);
  FUNCTION const char * __cdecl    CEnobioStrError(int NumError,Enobio* enobioClass);
  FUNCTION bool __cdecl  CEnobioIsInitialized(Enobio* enobioClass);
  FUNCTION int* __cdecl CEnobioChannelStatus(Enobio*);
  FUNCTION void __cdecl  CEnobioResetBuffer(Enobio* enobioClass);
#ifdef __cplusplus
}
#endif
#endif //___CEnobioCtrl_HEADER__
