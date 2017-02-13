#include "sostav/windows/imagedialogwindow.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

ImageDialogWindowException::ImageDialogWindowException
(const WCHAR *what)
   : Exception(what)
{
}

ImageDialogWindowFrame::ImageDialogWindowFrame
(void)
   : LayeredImageWindow()
{
   this->windowPane = NULL;
   this->paneCrop = {0,0,0,0};
   this->setClassName(L"SvImageDialogWindowFrame");
}

void
ImageDialogWindowFrame::setWindowPane
(ImageDialogWindowPane *windowPane)
{
   if (this->windowPane != NULL)
      this->removeLink(this->windowPane);
   
   this->windowPane = windowPane;
   this->addLink(windowPane);

   if (this->image.hasImage())
      this->cropFrame();
}

ImageDialogWindowPane *
ImageDialogWindowFrame::getWindowPane
(void) const
{
   return this->windowPane;
}

void
ImageDialogWindowFrame::setPaneCrop
(DWORD x, DWORD y, DWORD cx, DWORD cy)
{
   this->paneCrop.left = x;
   this->paneCrop.top = y;
   this->paneCrop.right = x + cx;
   this->paneCrop.bottom = y + cy;

   if (this->windowPane != NULL && this->image.hasImage())
      this->cropFrame();
}

void
ImageDialogWindowFrame::setPaneCrop
(POINT point, SIZE size)
{
   this->setPaneCrop(point.x, point.y, size.cx, size.cy);
}

void
ImageDialogWindowFrame::setPaneCrop
(RECT rect)
{
   this->setPaneCrop(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

RECT
ImageDialogWindowFrame::getPaneCrop
(void) const
{
   return this->paneCrop;
}

void
ImageDialogWindowFrame::cropFrame
(void)
{
   AbsolutePoint framePosition, panePosition;

   /* temporarily remove the link so we can adjust the crop frame without
      moving everything */
   if (this->hasLink(this->windowPane))
      this->removeLink(this->windowPane);
   
   if (this->windowPane == NULL)
      throw ImageDialogWindowException(L"no window pane present");

   if (!this->image.hasImage())
      throw ImageDialogWindowException(L"no image to crop");

   framePosition = this->getPosition();
   panePosition.setX(framePosition.getX() + this->paneCrop.left);
   panePosition.setY(framePosition.getY() + this->paneCrop.top);
   
   this->windowPane->setImage(this->image.crop(this->paneCrop));
   this->windowPane->setPosition(panePosition);

   this->addLink(this->windowPane);
}

void
ImageDialogWindowFrame::setImage
(Image image)
{
   LayeredImageWindow::setImage(image);

   if (this->windowPane != NULL && this->image.hasImage())
      this->cropFrame();
}

void
ImageDialogWindowFrame::preCreate
(void)
{
   LayeredImageWindow::preCreate();

   if (!this->image.hasImage())
      throw ImageDialogWindowException(L"window has no image");

   this->windowPane->addStyle(WS_VISIBLE | WS_POPUP);
   this->cropFrame();
}

ImageDialogWindowPane::ImageDialogWindowPane
(void)
   : ImageWindow()
{
   this->setClassName(L"SvImageDialogWindowPane");
}

void
ImageDialogWindowPane::postCreate
(void)
{
   ImageWindow::postCreate();
   
   this->setTopWindow();
}
