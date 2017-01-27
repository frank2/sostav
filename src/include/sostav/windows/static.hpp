#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/window.hpp"

namespace Sostav
{
   namespace Windows
   {
      typedef Exception StaticException;

      class Static : public Window
      {
      protected:
         
      public:
         Static(Window *parent, std::wstring caption=std::wstring());
         Static();
         ~Static();

         virtual void paint(HDC context);
      };
   }
}
