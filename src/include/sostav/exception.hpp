#pragma once

#include <windows.h>

#include <exception>
#include <string.h>
#include <string>

#include "locale.hpp"

namespace Sostav
{
   class Exception : public std::exception
   {
   protected:
      char *whatVal;

   public:
      DWORD error;
      
      Exception(const WCHAR *what);
      ~Exception();
      
      virtual const char *what(void) const noexcept;
   };
}
