#include "sostav/exception.hpp"

using namespace Sostav;

Exception::Exception
(const WCHAR *what)
   : std::exception()
{
   this->whatVal = Locale::WideToMultiByte(what);
}

const char *
Exception::what
(void)
   const noexcept
{
   return this->whatVal.c_str();
}
