#pragma once

#include <exception>

namespace Sostav
{
   class Exception : public std::exception
   {
   public:
      const char *whatVal;
      
      Exception(const char *what);
      virtual const char *what(void) const noexcept;
   };
}
