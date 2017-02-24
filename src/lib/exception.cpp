#include "sostav/exception.hpp"

using namespace Sostav;

Exception::Exception
(const WCHAR *what)
   : std::exception()
{
   int bufferSize, result;
   char *bufferData;
   std::string converted;

   error = GetLastError();

   converted = Locale::WideToMultiByte(what);
   bufferSize = converted.length()+1;
   bufferData = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);
   strcpy_s(bufferData, bufferSize, converted.c_str());

   this->whatVal = bufferData;
}

Exception::~Exception
(void)
{
   HeapFree(GetProcessHeap(), NULL, this->whatVal);
   this->whatVal = NULL;
}

const char *
Exception::what
(void)
   const noexcept
{
   return this->whatVal;
}
