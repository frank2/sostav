#pragma once

#include <windows.h>
#include <wincodec.h>

#include <string>

#include <sostav/exception.hpp>
#include <sostav/drawing/color.hpp>

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
         SIZE imageSize;
         size_t dibSize;
         Color *dibBuffer;
         HBITMAP imageHandle;
         
      public:
         Image(LPCWSTR name, std::wstring type);
         Image(std::wstring filename);
         Image(const Image &image);
         Image();
         ~Image();

         SIZE getImageSize(void) const;
         size_t getDIBSize(void) const;
         Color *getDIBBuffer(void) const;
         
         void setImageHandle(HBITMAP image);
         HBITMAP getImageHandle(void) const;

         bool hasImage(void) const;

         void loadFromBuffer(size_t bufferSize, LPBYTE bufferData);
         void loadResource(LPCWSTR name, std::wstring type);
         void loadFile(std::wstring filename);

         HBITMAP renderTransparency(Color bgColor);
         
      protected:
         void initializeCOM(void);
      };
   }
}
