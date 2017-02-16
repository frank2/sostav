#include "sostav/drawing/image.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

ImageException::ImageException
(const WCHAR *what)
   : Exception(what)
{
}

Image::Image
(LPCWSTR name, std::wstring type)
{
   this->initializeCOM();
   this->loadResource(name, type);
}

Image::Image
(std::wstring filename)
{
   this->initializeCOM();
   this->loadFile(filename);
}

Image::Image
(const Image &image)
{
   this->copyHandle(image.getBitmapHandle());
}

Image::Image
(void)
{
   this->initializeCOM();
   ZeroMemory(&this->imageHeader, sizeof(BITMAP));
   this->imageHandle = NULL;
}

Image::~Image
(void)
{
   if (this->hasImage())
      this->destroy();
}

Image &
Image::operator=
(const Image &b)
{
   this->copyImage(b);
   return *this;
}

void
Image::setBitmapHandle
(HBITMAP handle)
{
   if (this->hasImage())
      this->destroy();
   
   this->imageHandle = handle;

   if (handle == NULL)
   {
      ZeroMemory(&this->imageHeader, sizeof(BITMAP));
      return;
   }

   if (GetObject(this->imageHandle, sizeof(BITMAP), &this->imageHeader) == 0)
      throw ImageException(L"GetObject failed");
}

HBITMAP
Image::getBitmapHandle
(void) const
{
   return this->imageHandle;
}

BITMAP
Image::getBitmapHeader
(void) const
{
   return this->imageHeader;
}

Color *
Image::getPixelBuffer
(void) const
{
   if (this->imageHeader.bmBits == NULL)
      return NULL;

   if (this->imageHeader.bmBitsPixel != 32)
      throw ImageException(L"only 32-bit bitmaps are supported");

   return (Color *)this->imageHeader.bmBits;
}

size_t
Image::getPixelBufferSize
(void) const
{
   /* for when I actually *do* support non-32bit bitmaps */
   return (this->imageHeader.bmBitsPixel / 8) * this->imageHeader.bmWidth * this->imageHeader.bmHeight;
}

void
Image::setImageSize
(LONG cx, LONG cy)
{
   if (this->hasImage())
      this->destroy();

   this->imageHeader.bmWidth = cx;
   this->imageHeader.bmHeight = cy;
}

void
Image::setImageSize
(SIZE size)
{
   this->setImageSize(size.cx, size.cy);
}

SIZE
Image::getImageSize
(void) const
{
   SIZE size;

   size.cx = this->imageHeader.bmWidth;
   size.cy = this->imageHeader.bmHeight;

   return size;
}

bool
Image::hasImage
(void) const
{
   return this->imageHandle != NULL;
}

void
Image::createDIBSection
(DWORD cx, DWORD cy)
{
   BITMAPINFO bitmapInfo;
   HDC screen;

   this->setImageSize(cx, cy); /* implicitly destroys old image */
   
   ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

   bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitmapInfo.bmiHeader.biWidth = cx;
   bitmapInfo.bmiHeader.biHeight = -cy;
   bitmapInfo.bmiHeader.biPlanes = 1;
   bitmapInfo.bmiHeader.biBitCount = 32;
   bitmapInfo.bmiHeader.biCompression = BI_RGB;

   screen = GetDC(NULL);
   this->imageHandle = CreateDIBSection(screen
                                        ,&bitmapInfo
                                        ,DIB_RGB_COLORS
                                        ,&this->imageHeader.bmBits
                                        ,NULL
                                        ,0);
   ReleaseDC(NULL, screen);

   if (this->imageHandle == NULL)
      throw ImageException(L"CreateDIBSection failed");

   this->imageHeader.bmType = 0;
   this->imageHeader.bmBitsPixel = 32;
   this->imageHeader.bmPlanes = 1;
   this->imageHeader.bmWidthBytes = 4 * this->imageHeader.bmWidth;
}

void
Image::createDIBSection
(SIZE size)
{
   this->createDIBSection(size.cx, size.cy);
}

void
Image::loadFromBuffer
(size_t bufferSize, LPBYTE bufferData)
{
   IID formatID;
   LPDWORD magic;
   HGLOBAL globalHandle;
   LPVOID globalData;
   IStream *dataStream;
   IWICBitmapDecoder *wicDecoder;
   UINT frameCount;
   IWICBitmapFrameDecode *wicFrameDecoder;
   IWICBitmapSource *wicBitmap;
   UINT stride, image;

   /* greets to https://code.logos.com/blog/2008/09/displaying_a_splash_screen_with_c_part_i.html */

   magic = (LPDWORD)bufferData;

   if (*magic == 0x474e5089) /* PNG */
      formatID = CLSID_WICPngDecoder;
   else if (*((LPWORD)magic) == 0x4D42) /* BMP */
      formatID = CLSID_WICBmpDecoder;
   else if (*magic == 0xE0FFD8FF) /* JPG */
      formatID = CLSID_WICJpegDecoder;
   else if (*magic == 0x38464947) /* GIF */
      formatID = CLSID_WICGifDecoder;
   else
      throw ImageException(L"image buffer is an unsupported format");

   globalHandle = GlobalAlloc(GMEM_MOVEABLE, bufferSize);

   if (globalHandle == NULL)
      throw ImageException(L"GlobalAlloc failed");

   globalData = GlobalLock(globalHandle);

   if (globalData == NULL)
   {
      GlobalFree(globalHandle);
      throw ImageException(L"GlobalLock failed");
   }

   CopyMemory((LPVOID)globalData, bufferData, bufferSize);
   GlobalUnlock(globalHandle);

   if (FAILED(CreateStreamOnHGlobal(globalHandle, TRUE, &dataStream)))
   {
      GlobalFree(globalHandle);
      throw ImageException(L"CreateStreamOnHGlobal failed");
   }

   if (FAILED(CoCreateInstance(formatID
                               ,NULL
                               ,CLSCTX_INPROC_SERVER
                               ,__uuidof(wicDecoder)
                               ,(LPVOID *)&wicDecoder)))
   {
      GlobalFree(globalHandle);
      dataStream->Release();
      throw ImageException(L"CoCreateInstance failed");
   }

   if (FAILED(wicDecoder->Initialize(dataStream, WICDecodeMetadataCacheOnLoad)))
   {
      GlobalFree(globalHandle);
      dataStream->Release();
      throw ImageException(L"Initialize failed");
   }

   frameCount = 0;

   if (FAILED(wicDecoder->GetFrameCount(&frameCount)))
   {
      GlobalFree(globalHandle);
      wicDecoder->Release();
      dataStream->Release();
      throw ImageException(L"GetFrameCount failed");
   }

   if (frameCount != 1)
   {
      GlobalFree(globalHandle);
      wicDecoder->Release();
      dataStream->Release();
      throw ImageException(L"image has an unsupported amount of frames");
   }

   if (FAILED(wicDecoder->GetFrame(0, &wicFrameDecoder)))
   {
      GlobalFree(globalHandle);
      wicDecoder->Release();
      dataStream->Release();
      throw ImageException(L"GetFrame failed");
   }

   
   if (FAILED(WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA
                                     ,wicFrameDecoder
                                     ,&wicBitmap)))
   {
      GlobalFree(globalHandle);
      wicDecoder->Release();
      dataStream->Release();
      throw ImageException(L"WICConvertBitmapSource failed");
   }
   
   wicFrameDecoder->Release();
   wicDecoder->Release();

   if (FAILED(wicBitmap->GetSize((UINT *)&this->imageHeader.bmWidth
                                 ,(UINT *)&this->imageHeader.bmHeight)))
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      dataStream->Release();
      throw ImageException(L"GetSize failed");
   }

   if (this->imageHeader.bmWidth == 0 || this->imageHeader.bmHeight == 0)
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      dataStream->Release();
      throw ImageException(L"image has a null width or height");
   }

   this->createDIBSection(this->imageHeader.bmWidth, this->imageHeader.bmHeight);

   stride = this->imageHeader.bmWidth * 4; /* 32-bit bitmap scanline */
   image = stride * this->imageHeader.bmHeight;

   if (FAILED(wicBitmap->CopyPixels(NULL, stride, image, (LPBYTE)this->imageHeader.bmBits)))
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      dataStream->Release();
      throw ImageException(L"CopyPixels failed");
   }

   wicBitmap->Release();
   GlobalFree(globalHandle);
   dataStream->Release();
}

void
Image::loadResource
(LPCWSTR name, std::wstring type)
{
   HRSRC resourceHandle;
   size_t resourceSize;
   HGLOBAL loadedHandle;
   LPBYTE loadedData;

   resourceHandle = FindResource(NULL, name, type.c_str());

   if (resourceHandle == NULL)
      throw ImageException(L"FindResource failed");

   resourceSize = SizeofResource(NULL, resourceHandle);
   loadedHandle = LoadResource(NULL, resourceHandle);

   if (loadedHandle == NULL)
      throw ImageException(L"LoadResource failed");

   loadedData = (LPBYTE)LockResource(loadedHandle);

   if (loadedData == NULL)
      throw ImageException(L"LockResource failed");

   this->loadFromBuffer(resourceSize, loadedData);
}

void
Image::loadFile
(std::wstring filename)
{
   HANDLE fileHandle;
   size_t fileSize;
   LPBYTE fileData;
   DWORD bytesRead;

   fileHandle = CreateFile(filename.c_str()
                           ,GENERIC_READ
                           ,NULL
                           ,NULL
                           ,OPEN_EXISTING
                           ,FILE_ATTRIBUTE_NORMAL
                           ,NULL);

   if (fileHandle == NULL)
      throw ImageException(L"CreateFile failed");

   fileSize = GetFileSize(fileHandle, NULL);
   fileData = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, fileSize);

   if (!ReadFile(fileHandle, (LPVOID)fileData, fileSize, &bytesRead, NULL))
   {
      CloseHandle(fileHandle);
      HeapFree(GetProcessHeap(), NULL, fileData);
      throw ImageException(L"ReadFile failed");
   }

   CloseHandle(fileHandle);

   this->loadFromBuffer(fileSize, fileData);

   HeapFree(GetProcessHeap(), NULL, fileData);
}

void
Image::setPixel
(DWORD x, DWORD y, Color color)
{
   Color *dibBuffer;

   if (!this->hasImage())
      throw ImageException(L"can't set pixel on null image");
   
   if (x >= this->imageHeader.bmWidth || y >= this->imageHeader.bmHeight)
      throw ImageException(L"pixel out of bounds");

   dibBuffer = (Color *)this->imageHeader.bmBits;
   
   dibBuffer[y * this->imageHeader.bmWidth + x] = color;
}

void
Image::setPixel
(POINT point, Color color)
{
   this->setPixel(point.x, point.y, color);
}

void
Image::setPixel
(AbsolutePoint point, Color color)
{
   this->setPixel(point.getX(), point.getY(), color);
}

Color
Image::getPixel
(DWORD x, DWORD y) const
{
   Color *dibBuffer;
   
   if (!this->hasImage())
      throw ImageException(L"can't get pixel from null image");
   
   if (x >= this->imageHeader.bmWidth || y >= this->imageHeader.bmHeight)
      throw ImageException(L"pixel out of bounds");

   dibBuffer = (Color *)this->imageHeader.bmBits;
   
   return dibBuffer[y * this->imageHeader.bmWidth + x];
}

Color
Image::getPixel
(POINT point) const
{
   return this->getPixel(point.x, point.y);
}

Color
Image::getPixel
(AbsolutePoint point) const
{
   return this->getPixel(point.getX(), point.getY());
}

Image
Image::crop
(DWORD x, DWORD y, DWORD cx, DWORD cy) const
{
   Image croppedImage;

   if (!this->hasImage())
      throw ImageException(L"can't crop a null image");

   if (x+cx >= this->imageHeader.bmWidth || y+cy >= this->imageHeader.bmHeight)
      throw ImageException(L"crop selection is too large");

   croppedImage.createDIBSection(cx, cy);

   for (DWORD ty=0; ty<cy; ++ty)
      for (DWORD tx=0; tx<cx; ++tx)
         croppedImage.setPixel(tx, ty, this->getPixel(tx+x, ty+y));

   return croppedImage;
}

Image
Image::crop
(POINT point, SIZE size) const
{
   return this->crop(point.x, point.y, size.cx, size.cy);
}

Image
Image::crop
(RECT rect) const
{
   return this->crop(rect.left
                     ,rect.top
                     ,rect.right - rect.left
                     ,rect.bottom - rect.top);
}

Image
Image::crop
(AbsolutePoint point, SIZE size) const
{
   return this->crop(point.getX(), point.getY(), size.cx, size.cy);
}

void
Image::copyImage
(Image image)
{
   Color *pixelBufferSrc, *pixelBufferDest;
   size_t pixelBufferSize;
   SIZE imageSize;

   if (!image.hasImage())
      throw ImageException(L"can't copy null image");

   if (this->hasImage())
      this->destroy();

   imageSize = image.getImageSize();

   this->createDIBSection(imageSize.cx, imageSize.cy);
   pixelBufferDest = this->getPixelBuffer();
   pixelBufferSrc = image.getPixelBuffer();
   pixelBufferSize = image.getPixelBufferSize();

   CopyMemory((LPVOID)pixelBufferDest
              ,(LPVOID)pixelBufferSrc
              ,pixelBufferSize);
}

void
Image::copyHandle
(HBITMAP image)
{
   BITMAP bitmapHeader;

   if (image == NULL)
      throw ImageException(L"can't copy a null image handle");

   if (GetObject(image, sizeof(BITMAP), &bitmapHeader) == 0)
      throw ImageException(L"GetObject failed");

   if (bitmapHeader.bmBitsPixel != 32)
      throw ImageException(L"non-32-bit image objects not supported");

   this->createDIBSection(bitmapHeader.bmWidth, bitmapHeader.bmHeight);
   
   CopyMemory(this->imageHeader.bmBits
              ,bitmapHeader.bmBits
              ,bitmapHeader.bmWidth * bitmapHeader.bmHeight * 4);
}

void
Image::destroy
(void)
{
   if (!this->hasImage())
      throw ImageException(L"no image to destroy");

   DeleteObject(this->imageHandle);
   ZeroMemory(&this->imageHeader, sizeof(BITMAP));
   this->imageHandle = NULL;
}

Image
Image::renderTransparency
(Color bgColor) const
{
   Image renderedImage;
   SIZE imageSize;

   if (!this->hasImage())
      throw ImageException(L"can't render a null image");

   imageSize = this->getImageSize();
   renderedImage.createDIBSection(imageSize.cx, imageSize.cy);

   for (LONG cy=0; cy<imageSize.cy; ++cy)
   {
      for (LONG cx=0; cx<imageSize.cx; ++cx)
      {
         Color currentColor = this->getPixel(cx, cy);
         Color blendedColor;
         double result = (255 - currentColor.a) / 255.0;

         blendedColor = currentColor.blend(bgColor, result);
         blendedColor.a = 0xFF;
         renderedImage.setPixel(cx, cy, blendedColor);
      }
   }

   return renderedImage;
}

void
Image::initializeCOM
(void)
{
   HRESULT result = CoInitializeEx(NULL, NULL);

   if (result != S_OK && result != S_FALSE)
      throw ImageException(L"CoInitializeEx failed");
}
