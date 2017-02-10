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
   this->setImageHandle(image.getImageHandle());
}

Image::Image
(void)
{
   this->initializeCOM();
   this->imageSize = {0,0};
   this->dibSize = 0;
   this->dibBuffer = NULL;
   this->imageHandle = NULL;
}

Image::~Image
(void)
{
   if (this->imageHandle != NULL)
      DeleteObject(this->imageHandle);
}

SIZE
Image::getImageSize
(void) const
{
   return this->imageSize;
}

size_t
Image::getDIBSize
(void) const
{
   return this->dibSize;
}

Color *
Image::getDIBBuffer
(void) const
{
   return this->dibBuffer;
}

void
Image::setImageHandle
(HBITMAP image)
{
   HDC screen;
   BITMAP bitmapHeader;
   BITMAPINFO bitmapInfo;

   if (this->hasImage())
   {
      DeleteObject(this->imageHandle);
      this->dibBuffer = NULL;
      this->dibSize = 0;
      this->imageSize = {0,0};
   }

   GetObject(image, sizeof(BITMAP), (LPVOID)&bitmapHeader);

   this->imageSize = {bitmapHeader.bmWidth, bitmapHeader.bmHeight};

   ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

   bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitmapInfo.bmiHeader.biWidth = this->imageSize.cx;
   bitmapInfo.bmiHeader.biHeight = -this->imageSize.cy;
   bitmapInfo.bmiHeader.biPlanes = 1;
   bitmapInfo.bmiHeader.biBitCount = 32;
   bitmapInfo.bmiHeader.biCompression = BI_RGB;

   screen = GetDC(NULL);
   this->imageHandle = CreateDIBSection(screen
                                        ,&bitmapInfo
                                        ,DIB_RGB_COLORS
                                        ,(LPVOID *)&this->dibBuffer
                                        ,NULL
                                        ,0);
   ReleaseDC(NULL, screen);
   
   CopyMemory((LPVOID)this->dibBuffer
              ,bitmapHeader.bmBits
              ,(bitmapHeader.bmWidth * (bitmapHeader.bmBitsPixel/8)) * bitmapHeader.bmHeight);

   this->dibSize = this->imageSize.cx * this->imageSize.cy;
}

HBITMAP
Image::getImageHandle
(void) const
{
   return this->imageHandle;
}

bool
Image::hasImage
(void) const
{
   return this->imageHandle != NULL;
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
   BITMAPINFO bitmapInfo;
   HDC screen;
   UINT stride, image;

   /* greets to https://code.logos.com/blog/2008/09/displaying_a_splash_screen_with_c_part_i.html */

   magic = (LPDWORD)bufferData;

   if (*magic == 0x474e5089) /* PNG */
      formatID = CLSID_WICPngDecoder;
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

   if (FAILED(wicBitmap->GetSize((UINT *)&this->imageSize.cx, (UINT *)&this->imageSize.cy)))
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      dataStream->Release();
      throw ImageException(L"GetSize failed");
   }

   if (this->imageSize.cx == 0 || this->imageSize.cy == 0)
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      dataStream->Release();
      throw ImageException(L"image has a null width or height");
   }

   ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

   bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitmapInfo.bmiHeader.biWidth = this->imageSize.cx;
   bitmapInfo.bmiHeader.biHeight = -this->imageSize.cy;
   bitmapInfo.bmiHeader.biPlanes = 1;
   bitmapInfo.bmiHeader.biBitCount = 32;
   bitmapInfo.bmiHeader.biCompression = BI_RGB;

   screen = GetDC(NULL);
   this->imageHandle = CreateDIBSection(screen
                                        ,&bitmapInfo
                                        ,DIB_RGB_COLORS
                                        ,(LPVOID *)&this->dibBuffer
                                        ,NULL
                                        ,0);
   ReleaseDC(NULL, screen);

   if (this->imageHandle == NULL)
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      dataStream->Release();
      throw ImageException(L"CreateDIBSection failed");
   }

   stride = this->imageSize.cx * 4; /* 32-bit bitmap scanline */
   image = stride * this->imageSize.cy;

   this->dibSize = this->imageSize.cx * this->imageSize.cy;

   if (FAILED(wicBitmap->CopyPixels(NULL, stride, image, (LPBYTE)this->dibBuffer)))
   {
      GlobalFree(globalHandle);
      wicBitmap->Release();
      DeleteObject(this->imageHandle);
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

HBITMAP
Image::renderTransparency
(Color bgColor)
{
   HDC screen;
   BITMAPINFO bitmapInfo;
   HBITMAP renderedHandle;
   Color *newDibBuffer;
   size_t dibIndex;

   ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));

   bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitmapInfo.bmiHeader.biWidth = this->imageSize.cx;
   bitmapInfo.bmiHeader.biHeight = -this->imageSize.cy;
   bitmapInfo.bmiHeader.biPlanes = 1;
   bitmapInfo.bmiHeader.biBitCount = 32;
   bitmapInfo.bmiHeader.biCompression = BI_RGB;

   screen = GetDC(NULL);
   renderedHandle = CreateDIBSection(screen
                                     ,&bitmapInfo
                                     ,DIB_RGB_COLORS
                                     ,(LPVOID *)&newDibBuffer
                                     ,NULL
                                     ,0);
   ReleaseDC(NULL, screen);

   for (dibIndex=0; dibIndex<this->dibSize; ++dibIndex)
   {
      Color *dibPointer = &newDibBuffer[dibIndex];
      Color *currentPointer = &this->dibBuffer[dibIndex];
      double result = (255 - currentPointer->a) / 255.0;

      if (result == 1.0)
         *dibPointer = bgColor;
      else if (result == 0.0)
         *dibPointer = *currentPointer;
      else
         *dibPointer = currentPointer->blend(bgColor, result);

      dibPointer->a = 0xFF;
   }

   return renderedHandle;
}

void
Image::initializeCOM
(void)
{
   HRESULT result = CoInitializeEx(NULL, NULL);

   if (result != S_OK && result != S_FALSE)
      throw ImageException(L"CoInitializeEx failed");
}
