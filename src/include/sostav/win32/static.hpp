#pragma once

#include <windows.h>

#include "sostav/exception.hpp"
#include "sostav/windows/subclassedwindow.hpp"

namespace Sostav
{
   namespace Win32
   {
      class StaticException : public Exception
      {
      public:
         StaticException(const char *what);
      };
      
      class Static : public Windows::SubclassedWindow
      {
      public:
         Static(Window *parent);
         Static(Static &window);
         Static();
         ~Static();
      };
   }
}

         
