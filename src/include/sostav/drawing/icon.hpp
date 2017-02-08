#pragma once

#include <windows.h>

#include <string>

#include <sostav/exception.hpp>

namespace Sostav
{
   namespace Drawing
   {
      class IconException : public Exception
      {
      public:
         IconException(const WCHAR *what);
      };

      class Icon
      {
      protected:
         HICON iconHandle;
         ICONINFO iconInfo;

      public:
         Icon(LPWSTR iconFile);
         Icon(const Icon &icon);
         Icon();
         ~Icon();

         static Icon Application(void);
         static Icon Asterisk(void);
         static Icon Error(void);
         static Icon Exclamation(void);
         static Icon Question(void);
         static Icon Shield(void);
         static Icon WinLogo(void);
         static Icon MainIcon(void);

         bool hasHandle(void) const;
         bool hasInfo(void) const;

         void setHandle(HICON iconHandle);
         HICON getHandle(void) const;

         void setIconInfo(ICONINFO iconInfo);
         ICONINFO getIconInfo(void) const;

         void loadOEM(UINT resourceID);
         void loadResource(LPWSTR resourceID);
         void loadFile(std::wstring filename);

      protected:
         virtual void loadIcon(HINSTANCE instance, LPWSTR iconFile, UINT loadFlags);
      };

      class SmallIcon : public Icon
      {
      public:
         SmallIcon(LPWSTR iconFile);
         SmallIcon(SmallIcon &icon);
         SmallIcon();

      protected:
         virtual void loadIcon(HINSTANCE instance, LPWSTR iconFile, UINT loadFlags);
      };
   }
}
         
         
