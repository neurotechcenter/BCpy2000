
#pragma once

#include <crtdbg.h>
#include <windows.h>

const long WM_SOCKET_NOTIFY  = WM_USER + 1666;
const long WM_SOCKET_DEAD = WM_USER + 1667;
const long WM_DESTROY_WINDOW = WM_USER + 1668;

class CSocketwnd
{
public:
   CSocketwnd();
   ~CSocketwnd();

   void Create();
   void OnDestroyWindow();
   void DestroyWindow();
   HWND GetHwnd();
   static long __stdcall WindowProc(HWND hwnd, unsigned int msg, WPARAM wp, LPARAM lp);

   virtual void OnSocketNotify(WPARAM wp, LPARAM lp);
   virtual void OnSocketDead(WPARAM wp, LPARAM lp);

   struct WndClassRegister;
   static WndClassRegister m_register;

   HWND m_hWnd;
};
