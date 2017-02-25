#pragma once

#include <windows.h>

#include <exception>
#include <string>

#include "locale.hpp"

namespace Sostav
{
   class Exception : public std::exception
   {
   protected:
      std::string whatVal;

   public:
      DWORD error;
      
      Exception(const WCHAR *what);
      
      virtual const char *what(void) const noexcept;
   };
}
