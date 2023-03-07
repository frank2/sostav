#include "sostav/windows/imagewindow.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

ImageWindowException::ImageWindowException
(const WCHAR *what)
   : Exception(what)
{
}

ImageWindow::ImageWindow
(Window *parent, std::wstring className, Image image)
   : PaintedWindow(parent, className)
{
   if (image.hasImage())
      this->setImage(image);
}

ImageWindow::ImageWindow
(ImageWindow &window)
   : PaintedWindow(window)
{
   this->setImage(window.getImage());
}

ImageWindow::ImageWindow
(void)
   : PaintedWindow()
{
   this->setClassName(L"SvImageWindow");
}

Image
ImageWindow::getImage
(void) const
{
   return this->image;
}

void
ImageWindow::setImage
(Image image)
{
   this->image.copyImage(image);
   this->setSize(this->image.getImageSize());

   if (this->hasHWND())
      this->invalidate();
}

void
ImageWindow::paint
(HDC context)
{
   this->drawImage(context);
}

/* for some reason the optimizer mangles this code and causes the call to BitBlt to fail
   in a really weird way-- GetLastError() is useless and I don't feel like digging into
   NtGdiBitBlt to figure out where it's failing */
#pragma optimize("", off)
void
ImageWindow::drawImage
(HDC context)
{
   HDC imageContext;
   SIZE imageSize;
   Image renderedImage;

   /* nothing to paint-- this is okay */
   if (!this->image.hasImage())
      return;

   imageContext = CreateCompatibleDC(context);

   renderedImage = this->image.renderTransparency(this->getBGColor());
   SelectObject(imageContext, renderedImage.getBitmapHandle());
   imageSize = this->image.getImageSize();

   if (!BitBlt(context, 0, 0, imageSize.cx, imageSize.cy, imageContext, 0, 0, SRCCOPY))
      ImageWindowException(L"BitBlt failed");

   ReleaseDC(NULL, imageContext);
}
#pragma optimize("", on)

LayeredImageWindow::LayeredImageWindow
(Window *parent, std::wstring className, Image image)
   : LayeredWindow(parent, className)
{
   this->setImage(image);
}

LayeredImageWindow::LayeredImageWindow
(LayeredImageWindow &window)
   : LayeredWindow(window)
{
   this->setImage(window.getImage());
}

LayeredImageWindow::LayeredImageWindow
(void)
   : LayeredWindow()
{
   this->setClassName(L"SvLayeredImageWindow");
}

Image
LayeredImageWindow::getImage
(void) const
{
   return this->image;
}

void
LayeredImageWindow::setImage
(Image image)
{
   this->image.copyImage(image);
   this->setSize(this->image.getImageSize());

   if (this->hasHWND())
      this->invalidate();
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
   if (this->image.hasImage())
      SelectObject(context, this->image.getBitmapHandle());
}
