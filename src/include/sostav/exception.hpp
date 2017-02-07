#pragma once

#include <windows.h>

#include <exception>

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
