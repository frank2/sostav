#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/subclassedwindow.hpp"

namespace Sostav { namespace Win32
{
   typedef Exception EditException;

   class Edit : public SubclassedWindow
   {
   public:
      Edit(HWND parent);
      Edit(Window *parent);
      ~Edit();
   };
}}
      
