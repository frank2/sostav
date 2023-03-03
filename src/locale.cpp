#include "sostav/locale.hpp"

using namespace Sostav;

Locale Locale::Instance;

LocaleException::LocaleException
(const char *what)
   : std::exception()
{
   this->whatVal = what;
}

const char *
LocaleException::what
(void)
{
   return this->whatVal;
}

Locale::Locale
(void)
{
   /* sorry broskis, I's amerikanski */
   this->codePage = CP_UTF8;
   this->language = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
}

Locale *
Locale::GetInstance
(void)
{
   return &Locale::Instance;
}

void
Locale::SetCodePage
(UINT codePage)
{
   Locale::GetInstance()->setCodePage(codePage);
}

UINT
Locale::GetCodePage
(void)
{
   return Locale::GetInstance()->getCodePage();
}

void
Locale::SetLanguage
(LANGID language)
{
   Locale::GetInstance()->setLanguage(language);
}

LANGID
Locale::GetLanguage
(void)
{
   return Locale::GetInstance()->getLanguage();
}

std::wstring
Locale::LoadString
(UINT stringID, ...)
{
   va_list args;
   std::wstring result;

   va_start(args, stringID);

   result = Locale::LoadStringVA(stringID, args);

   va_end(args);

   return result;
}

std::wstring
Locale::LoadStringVA
(UINT stringID, va_list args)
{
   return Locale::GetInstance()->loadString(stringID, args);
}

std::string
Locale::WideToMultiByte
(UINT codepage, std::wstring wideString)
{
   return Locale::GetInstance()->wideToMultiByte(codepage, wideString);
}

std::string
Locale::WideToMultiByte
(std::wstring wideString)
{
   return Locale::GetInstance()->wideToMultiByte(wideString);
}

std::wstring
Locale::MultiByteToWide
(UINT codePage, std::string multiByteString)
{
   return Locale::GetInstance()->multiByteToWide(codePage, multiByteString);
}

std::wstring
Locale::MultiByteToWide
(std::string multiByteString)
{
   return Locale::GetInstance()->multiByteToWide(multiByteString);
}

void
Locale::setCodePage
(UINT codePage)
{
   this->codePage = codePage;
}

UINT
Locale::getCodePage
(void) const
{
   return this->codePage;
}

void
Locale::setLanguage
(LANGID language)
{
   this->language = language;
}

LANGID
Locale::getLanguage
(void) const
{
   return this->language;
}

std::wstring
Locale::loadString
(UINT stringID, va_list args)
{
   /* to better understand this code, see https://support.microsoft.com/en-us/kb/200893 */
   UINT resourceBlock = stringID / 16 + 1;
   int resourceIndex = stringID % 16;
   HRSRC resourceHandle;
   LPWCH tableColumn;

   resourceHandle = FindResourceEx(GetModuleHandle(NULL), RT_STRING, MAKEINTRESOURCE(resourceBlock), this->language);

   if (resourceHandle == NULL)
   {
      /* try everywhere this time */
      resourceHandle = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceBlock), RT_STRING);

      if (resourceHandle == NULL)
         throw LocaleException("failed to find string ID");
   }

   tableColumn = (LPWCH)LoadResource(GetModuleHandle(NULL), resourceHandle);

   if (tableColumn == NULL)
      throw LocaleException("couldn't load string column");

   /* iterate over the string entries in this block */
   for (int i=0; i<16; ++i)
   {
      int stringLength;
      std::wstring preFormatted;
      int postFormatSize;
      LPWCH postFormatBuffer;
      std::wstring result;
 
      stringLength = *tableColumn;
      ++tableColumn;

      if (i != resourceIndex)
      {
         tableColumn += stringLength;
         continue;
      }
      else if (i == resourceIndex && stringLength == 0)
      {
         break;
      }

      preFormatted = std::wstring(tableColumn, 0, stringLength);
      postFormatSize = _vscwprintf(preFormatted.c_str(), args);

      if (postFormatSize == -1)
         throw LocaleException("_vscwprintf failed");

      postFormatBuffer = (LPWCH)HeapAlloc(GetProcessHeap()
                                          ,HEAP_ZERO_MEMORY
                                          ,(postFormatSize+1)*sizeof(WCHAR));

      if (_vsnwprintf_s(postFormatBuffer
                        ,postFormatSize+1
                        ,postFormatSize+1
                        ,preFormatted.c_str()
                        ,args) == -1)
         throw LocaleException("_vsnwprintf_s failed");

      result = std::wstring(postFormatBuffer);

      HeapFree(GetProcessHeap(), NULL, postFormatBuffer);

      return result;
   }

   throw LocaleException("failed to find string in table");
}

std::string
Locale::wideToMultiByte
(UINT codepage, std::wstring wideString)
{
   int bufferSize, result;
   char *bufferData;
   std::string newString;

   bufferSize = WideCharToMultiByte(codepage
                                    ,NULL
                                    ,wideString.c_str()
                                    ,wideString.length()
                                    ,NULL
                                    ,NULL
                                    ,NULL
                                    ,NULL)+1;

   if (bufferSize == 0)
      throw LocaleException("WideCharToMultiByte failed to calculate size");

   bufferData = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);

   result = WideCharToMultiByte(codepage
                                ,NULL
                                ,wideString.c_str()
                                ,wideString.length()
                                ,bufferData
                                ,bufferSize
                                ,NULL
                                ,NULL);

   if (result == 0)
      throw LocaleException("WideCharToMultiByte failed");

   newString = std::string(bufferData);

   HeapFree(GetProcessHeap(), NULL, bufferData);

   return newString;
}

std::string
Locale::wideToMultiByte
(std::wstring wideString)
{
   return this->wideToMultiByte(this->codePage, wideString);
}

std::wstring
Locale::multiByteToWide
(UINT codepage, std::string multiByteString)
{
   int bufferSize, result;
   LPWCH bufferData;
   std::wstring newString;

   bufferSize = MultiByteToWideChar(codepage
                                    ,NULL
                                    ,multiByteString.c_str()
                                    ,multiByteString.length()
                                    ,NULL
                                    ,NULL)+1;

   if (bufferSize == 0)
      throw LocaleException("MultiByteToWideChar failed to calculate size");

   bufferData = (LPWCH)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize * sizeof(WCHAR));

   result = MultiByteToWideChar(codepage
                                ,NULL
                                ,multiByteString.c_str()
                                ,multiByteString.length()
                                ,bufferData
                                ,bufferSize);

   if (result == 0)
      throw LocaleException("MultiByteToWideChar failed");

   newString = std::wstring(bufferData);

   HeapFree(GetProcessHeap(), NULL, bufferData);

   return newString;
}

std::wstring
Locale::multiByteToWide
(std::string multiByteString)
{
   return this->multiByteToWide(this->codePage, multiByteString);
}
