#pragma once

#include <windows.h>
#include <wincodec.h>

#include <string>

#include <sostav/exception.hpp>

#pragma comment(lib, "windowscodecs")

namespace Sostav
{
   namespace Drawing
   {
      class ImageException : public Exception
      {
      public:
         ImageException(const char *what);
      };

      class Image
      {
      protected:
         HBITMAP imageHandle;
         
      public:
         Image(LPCWSTR name, std::wstring type);
         Image(std::wstring filename);
         Image(const Image &image);
         Image();
         ~Image();

         void loadFromBuffer(size_t bufferSize, LPBYTE bufferData);
         void loadResource(LPCWSTR name, std::wstring type);
         void loadFile(std::wstring filename);

         void setImageHandle(HBITMAP image);
         HBITMAP getImageHandle(void) const;

      protected:
         void initializeCOM(void);
      };
   }
}
