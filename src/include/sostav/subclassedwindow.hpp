#pragma once

#include <windows.h>
#include <commctrl.h>

#include "sostav/window.hpp"

#pragma comment(lib, "comctl32")

namespace Sostav
{
   typedef Exception SubclassedWindowException;

   class SubclassedWindow : public Window
   {
   protected:
      UINT_PTR subclassID;
      
   public:
      SubclassedWindow(HWND parent, std::wstring className);
      SubclassedWindow(Window *parent, std::wstring className);
      ~SubclassedWindow();

      static LRESULT CALLBACK WndProcSubclass(HWND hwnd
                                              ,UINT msg
                                              ,WPARAM wParam
                                              ,LPARAM lParam
                                              ,UINT_PTR uIdSubclass
                                              ,DWORD_PTR dwRefData);

      virtual void setHWND(HWND window);

      void setSubclassID(UINT_PTR id);
      UINT_PTR getSubclassID(void);

      virtual void create(void);

   protected:
      virtual void initialize(HWND parent, std::wstring className);
   };
}
