#pragma once

#include <windows.h>

#include <sostav/exception.hpp>
#include <sostav/windows/subclassedwindow.hpp>

namespace Sostav
{
   namespace Win32
   {
      class SysLinkException : public Exception
      {
      public:
         SysLinkException(const WCHAR *what);
      };

      class SysLink : public Windows::SubclassedWindow
      {
      public:
         SysLink(Windows::Window *parent);
         SysLink(SysLink &window);
         SysLink();
      };
   }
}
