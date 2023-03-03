#pragma once

#include <windows.h>

#include <exception>
#include <stdarg.h>
#include <stdlib.h>
#include <string>

namespace Sostav
{
   class LocaleException : public std::exception
   {
   protected:
      const char *whatVal;
      
   public:
      LocaleException(const char *what);
      const char *what(void);
   };
         
   class Locale
   {
   protected:
      static Locale Instance;

      UINT codePage;
      LANGID language;

      Locale();

   public:
      static Locale *GetInstance(void);
      
      static void SetCodePage(UINT codePage);
      static UINT GetCodePage(void);
      
      static void SetLanguage(LANGID language);
      static LANGID GetLanguage(void);

      static std::wstring LoadString(UINT stringID, ...);
      static std::wstring LoadStringVA(UINT stringID, va_list args);

      static std::string WideToMultiByte(UINT codepage, std::wstring wideString);
      static std::string WideToMultiByte(std::wstring wideString);

      static std::wstring MultiByteToWide(UINT codepage, std::string multiByteString);
      static std::wstring MultiByteToWide(std::string wideString);

      void setCodePage(UINT codePage);
      UINT getCodePage(void) const;

      void setLanguage(LANGID language);
      LANGID getLanguage(void) const;

      std::wstring loadString(UINT stringID, va_list args);

      std::string wideToMultiByte(UINT codepage, std::wstring wideString);
      std::string wideToMultiByte(std::wstring wideString);

      std::wstring multiByteToWide(UINT codepage, std::string multiByteString);
      std::wstring multiByteToWide(std::string multiByteString);
   };
}
