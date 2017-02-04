#include "sostav/windows/imagewindow.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

ImageWindowException::ImageWindowException
(const char *what)
   : Exception(what)
{
}

ImageWindow::ImageWindow
(Window *parent, std::wstring className, Image image)
   : PaintedWindow(parent, className)
{
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

ImageWindow::~ImageWindow
(void)
{
   PaintedWindow::~PaintedWindow();
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
   this->image.setImageHandle(image.getImageHandle());
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

void
ImageWindow::drawImage
(HDC context)
{
   HDC imageContext;
   SIZE imageSize;
   HBITMAP renderedImage;

   /* nothing to paint-- this is okay */
   if (!this->image.hasImage())
      return;

   imageContext = CreateCompatibleDC(context);

   renderedImage = this->image.renderTransparency(this->getBGColor());
   SelectObject(imageContext, renderedImage);
   imageSize = this->image.getImageSize();
   BitBlt(context, 0, 0, imageSize.cx, imageSize.cy, imageContext, 0, 0, SRCCOPY);

   ReleaseDC(NULL, imageContext);
   DeleteObject(renderedImage);
}

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

LayeredImageWindow::~LayeredImageWindow
(void)
{
   LayeredWindow::~LayeredWindow();
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
   this->image.setImageHandle(image.getImageHandle());
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
      SelectObject(context, this->image.getImageHandle());
}
