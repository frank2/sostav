#pragma once

#include <windows.h>
#include <wincodec.h>

#include <string>

#include <sostav/exception.hpp>
#include <sostav/drawing/color.hpp>
#include <sostav/drawing/point.hpp>

#pragma comment(lib, "windowscodecs")

namespace Sostav
{
   namespace Drawing
   {
      class ImageException : public Exception
      {
      public:
         ImageException(const WCHAR *what);
      };

      class Image
      {
      protected:
         BITMAP imageHeader;
         HBITMAP imageHandle;
         
      public:
         Image(LPCWSTR name, std::wstring type);
         Image(std::wstring filename);
         Image(HBITMAP image);
         Image(const Image &image);
         Image();
         ~Image();

         Image &operator=(const Image &b);

         void setBitmapHandle(HBITMAP handle);
         HBITMAP getBitmapHandle(void) const;

         BITMAP getBitmapHeader(void) const;

         Color *getPixelBuffer(void) const;
         size_t getPixelBufferSize(void) const;

         void setImageSize(LONG cx, LONG cy);
         void setImageSize(SIZE size);
         SIZE getImageSize(void) const;

         bool hasImage(void) const;

         void createDIBSection(DWORD cx, DWORD cy);
         void createDIBSection(SIZE size);

         void loadFromBuffer(size_t bufferSize, LPBYTE bufferData);
         void loadResource(LPCWSTR name, std::wstring type);
         void loadFile(std::wstring filename);

         void setPixel(DWORD x, DWORD y, Color color);
         void setPixel(POINT point, Color color);
         void setPixel(AbsolutePoint point, Color color);
         
         Color getPixel(DWORD x, DWORD y) const;
         Color getPixel(POINT point) const;
         Color getPixel(AbsolutePoint point) const;

         Image crop(DWORD x, DWORD y, DWORD cx, DWORD cy) const;
         Image crop(POINT point, SIZE size) const;
         Image crop(RECT rect) const;
         Image crop(AbsolutePoint point, SIZE size) const;

         void copyImage(Image image);
         void copyHandle(HBITMAP image);
         void destroy();

         Image renderTransparency(Color bgColor) const;
         
      protected:
         void initializeCOM(void);
      };
   }
}
