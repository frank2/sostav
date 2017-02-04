#include "sostav/exception.hpp"

using namespace Sostav;

Exception::Exception
(const WCHAR *what)
   : std::exception()
{
   int bufferSize, result;

   error = GetLastError();

   bufferSize = WideCharToMultiByte(CP_UTF8
                                    ,NULL
                                    ,what
                                    ,wcslen(what)
                                    ,NULL
                                    ,NULL
                                    ,NULL
                                    ,NULL)+1;

   if (bufferSize == 0)
      throw std::exception();

   this->whatVal = (char *)HeapAlloc(GetProcessHeap()
                                     ,HEAP_ZERO_MEMORY
                                     ,bufferSize);

   result = WideCharToMultiByte(CP_UTF8
                                ,NULL
                                ,what
                                ,wcslen(what)
                                ,this->whatVal
                                ,bufferSize
                                ,NULL
                                ,NULL);

   if (result == 0)
      throw std::exception();
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
