#include "sostav/drawing/image.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

ImageException::ImageException
(const char *what)
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
   this->imageHandle = NULL;
}

Image::~Image
(void)
{
   if (this->imageHandle != NULL)
      DeleteObject(this->imageHandle);

   this->imageHandle = NULL;
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

   magic = (LPDWORD)bufferData;

   if (*magic == 0x474e5089) /* PNG */
      formatID = CLSID_WICPngDecoder;
   else
      throw ImageException("unsupported image format loaded");

   globalHandle = GlobalAlloc(GMEM_MOVEABLE, bufferSize);

   if (globalHandle == NULL)
      throw ImageException("GlobalAlloc failed");

   globalData = GlobalLock(globalHandle);

   if (globalData == NULL)
   {
      GlobalFree(globalHandle);
      throw ImageException("GlobalLock failed");
   }

   CopyMemory((LPVOID)globalData, bufferData, bufferSize);
   GlobalUnlock(globalHandle);

   if (!SUCCEEDED(CreateStreamOnHGlobal(globalHandle, TRUE, &dataStream)))
   {
      GlobalFree(globalHandle);
      throw ImageException("CreateStreamOnHGlobal failed");
   }

   
