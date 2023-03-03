#pragma once

#include <windows.h>

#include <algorithm>
#include <list>
#include <set>

#include "sostav/exception.hpp"
#include "sostav/windows/subclassedwindow.hpp"

namespace Sostav
{
   namespace Win32
   {
      class ButtonException : public Exception
      {
      public:
         ButtonException(const WCHAR *what);
      };

      class Button : public Windows::SubclassedWindow
      {
      public:
         Button(Windows::Window *parent);
         Button(Button &window);
         Button();

         bool checked(void);

      protected:
         virtual LRESULT onKeyUp(DWORD keyValue, DWORD keyFlags);
         virtual LRESULT onLButtonUp(WORD virtualKeys, WORD x, WORD y);
      };
   }
}
