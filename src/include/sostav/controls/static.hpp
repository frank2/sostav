#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/window.hpp"

namespace Sostav
{
   namespace Controls
   {
      class StaticException : public Exception
      {
      public:
         StaticException(const char *what);
      };

      class Static : public Windows::Window
      {
      public:
         Static(Windows::Window *parent, std::wstring caption=std::wstring());
         Static(Static &window);
         Static();
         ~Static();

         virtual void paint(HDC context);
      };
   }
}
