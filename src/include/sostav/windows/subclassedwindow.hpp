#pragma once

#include <windows.h>
#include <commctrl.h>

#include "sostav/windows/window.hpp"

#pragma comment(lib, "comctl32")

namespace Sostav
{
   namespace Windows
   {
      class SubclassedWindowException : public Exception
      {
      public:
         SubclassedWindowException(const char *what);
      };

      class SubclassedWindow : public Window
      {
      protected:
         UINT_PTR subclassID;
         bool oldMethod;
      
      public:
         SubclassedWindow(Window *parent, std::wstring className);
         SubclassedWindow(SubclassedWindow &window);
         SubclassedWindow();
         ~SubclassedWindow();

         static LRESULT CALLBACK WndProcSubclass(HWND hwnd
                                                 ,UINT msg
                                                 ,WPARAM wParam
                                                 ,LPARAM lParam
                                                 ,UINT_PTR uIdSubclass
                                                 ,DWORD_PTR dwRefData);

         virtual void setHWND(HWND window);

         void setSubclassID(UINT_PTR id);
         UINT_PTR getSubclassID(void) const;

         void setSubclassMethod(bool method);
         bool getSubclassMethod(void) const;

         virtual void create(void);

      protected:
         virtual void subclassWindow(HWND window);
      };
   }
}
