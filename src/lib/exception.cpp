#include "sostav/exception.hpp"

using namespace Sostav;

Exception::Exception
(const char *what)
   : std::exception()
{
   this->whatVal = what;
}

const char *
Exception::what
(void)
   const noexcept
{
   return this->whatVal;
}
