#pragma once

#include <windows.h>

#include "sostav/exception.hpp"
#include "sostav/windows/subclassedwindow.hpp"

namespace Sostav
{
   namespace Win32
   {
      class ButtonException : public Exception
      {
      public:
         ButtonException(const char *what);
      };

      class Button : public Windows::SubclassedWindow
      {
      public:
         Button(Windows::Window *parent);
         Button(Button &window);
         Button();
         ~Button();
      };
   }
}
