#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/paintedwindow.hpp"

namespace Sostav
{
   namespace Controls
   {
      class StaticException : public Exception
      {
      public:
         StaticException(const WCHAR *what);
      };

      class Static : public Windows::PaintedWindow
      {
      public:
         Static(Windows::Window *parent, std::wstring caption=std::wstring());
         Static(Static &window);
         Static();

         virtual void paint(HDC context);
      };
   }
}
