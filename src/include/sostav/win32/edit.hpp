#pragma once

#include <windows.h>

#include "sostav/exception.hpp"
#include "sostav/windows/subclassedwindow.hpp"

namespace Sostav
{
   namespace Win32
   {
      class EditException : public Exception
      {
      public:
         EditException(const char *what);
      };

      class Edit : public Windows::SubclassedWindow
      {
      public:
         Edit(Windows::Window *parent);
         Edit(Edit &window);
         Edit();
         ~Edit();
      }
   }
}
