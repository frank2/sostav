#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/subclassedwindow.hpp"

namespace Sostav
{
   namespace Win32
   {
      class StaticException : public Exception
      {
      public:
         StaticException(const WCHAR *what);
      };
      
      class Static : public Windows::SubclassedWindow
      {
      public:
         Static(Window *parent, std::wstring caption=std::wstring());
         Static(Static &window);
         Static();

      protected:
         virtual void onPaint(void);
      };
   }
}

         
