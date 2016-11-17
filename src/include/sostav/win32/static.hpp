#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/subclassedwindow.hpp"

namespace Sostav { namespace Win32
{
   typedef Exception StaticException;

   class Static : public SubclassedWindow
   {
   public:
      Static(HWND parent, std::wstring caption=std::wstring());
      Static(Window *parent, std::wstring caption=std::wstring());
   };
}}
