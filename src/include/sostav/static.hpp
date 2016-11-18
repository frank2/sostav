#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/window.hpp"

namespace Sostav
{
   typedef Exception StaticException;

   class Static
   {
   public:
      Static(HWND parent, std::wstring caption=std::wstring());
      Static(Window *parent, std::wstring caption=std::wstring());
      Static();
      ~Static();

   protected:
      virtual void onPaint(void);
   };
}
