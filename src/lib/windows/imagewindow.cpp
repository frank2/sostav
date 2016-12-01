#include "sostav/windows/imagewindow.hpp"

using namespace Sostav;
using namespace Sostav::Windows;

ImageWindowException::ImageWindowException
(const char *what)
   : Exception(what)
{
}

ImageWindow::ImageWindow
(HWND parent, std::wstring className, HBITMAP image)
   : Window(parent, className)
{
   this->image = image;
}

ImageWindow::ImageWindow
(Window *window, std::wstring className, HBITMAP image)
   : Window(window, className)
{
   this->image = image;
}

ImageWindow::ImageWindow
(void)
   : Window()
{
   this->image = NULL;
}

HBITMAP
ImageWindow::getImage
(void) const
{
   return this->image;
}

void
ImageWindow::setImage
(HBITMAP image)
{
   this->image = image;

   if (this->hasHWND())
      this->update();
}

void
ImageWindow::paint
(HDC context)
{
   this->drawImage(context);
}

void
ImageWindow::drawImage
(HDC context)
{
   HDC imageContext;
   HGDIOBJ oldBitmap;
   BITMAP imageBitmap;

   /* nothing to paint-- this is okay */
   if (this->image == NULL)
      return;

   imageContext = CreateCompatibleDC(context);
   
   oldBitmap = SelectObject(imageContext, this->image);
   GetObject(this->image, sizeof(BITMAP), &imageBitmap);
   BitBlt(context, 0, 0, imageBitmap.bmWidth, imageBitmap.bmHeight, imageContext, 0, 0, SRCCOPY);

   SelectObject(imageContext, oldBitmap);
   DeleteDC(imageContext);
}

LayeredImageWindow::LayeredImageWindow
(HWND parent, std::wstring className, HBITMAP image)
   : LayeredWindow(parent, className)
{
   this->image = image;
}

LayeredImageWindow::LayeredImageWindow
(Window *window, std::wstring className, HBITMAP image)
   : LayeredWindow(window, className)
{
   this->image = image;
}

LayeredImageWindow::LayeredImageWindow
(void)
   : LayeredWindow()
{
   this->image = NULL;
}

HBITMAP
LayeredImageWindow::getImage
(void) const
{
   return this->image;
}

void
LayeredImageWindow::setImage
(HBITMAP image)
{
   this->image = image;
}

void
LayeredImageWindow::paint
(HDC context)
{
   this->drawImage(context);
}

void
LayeredImageWindow::drawImage
(HDC context)
{
   SelectObject(context, this->image);
}
