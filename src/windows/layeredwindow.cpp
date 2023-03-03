#include "sostav/windows/layeredwindow.hpp"

using namespace Sostav;
using namespace Sostav::Windows;

LayeredWindowException::LayeredWindowException
(const WCHAR *what)
   : Exception(what)
{
}

LayeredWindow::LayeredWindow
(Window *parent, std::wstring className)
   : PaintedWindow(parent, className)
{
   this->transparency = Drawing::Color::Transparent();
   this->screenDC = NULL;
   this->updateFlag = ULW_ALPHA;
}

LayeredWindow::LayeredWindow
(LayeredWindow &window)
   : PaintedWindow(window)
{
   this->setTransparency(window.getTransparency());
   this->screenDC = NULL;
   this->setUpdateFlag(window.getUpdateFlag());
}

LayeredWindow::LayeredWindow
(void)
   : PaintedWindow()
{
   this->transparency = Drawing::Color::Transparent();
   this->screenDC = NULL;
   this->updateFlag = ULW_ALPHA;
}

void
LayeredWindow::setTransparency
(Drawing::Color color)
{
   this->transparency.r = color.r;
   this->transparency.g = color.g;
   this->transparency.b = color.b;

   if (this->hasHWND())
   {
      this->setUpdateFlag(ULW_COLORKEY);
      this->layeredUpdate(NULL);
   }
}

Drawing::Color
LayeredWindow::getTransparency
(void) const
{
   return this->transparency;
}

void
LayeredWindow::setAlpha
(BYTE alpha)
{
   this->transparency.a = alpha;

   if (this->hasHWND())
   {
      this->setUpdateFlag(ULW_ALPHA);
      this->layeredUpdate(NULL);
   }
}

BYTE
LayeredWindow::getAlpha
(void) const
{
   return this->transparency.a;
}

void
LayeredWindow::setUpdateFlag
(DWORD flag)
{
   this->updateFlag = flag;
}

DWORD
LayeredWindow::getUpdateFlag
(void) const
{
   return this->updateFlag;
}

void
LayeredWindow::show
(void)
{
   Window::show();

   this->update();
}

void
LayeredWindow::update
(void)
{
   if (!this->hasHWND())
      return;

   this->beginPaint();
   this->paint(this->paintContext);
   this->endPaint();
}

void
LayeredWindow::preCreate
(void)
{
   PaintedWindow::preCreate();
   
   this->addExStyle(WS_EX_LAYERED);
}

void
LayeredWindow::layeredUpdate
(HDC context)
{
   POINT ptZero = { 0 };
   BLENDFUNCTION blender;
   POINT position;
   
   if (!this->hasHWND())
      throw LayeredWindowException(L"can't update window without an hwnd");

   blender.BlendOp = AC_SRC_OVER;
   blender.BlendFlags = 0;
   blender.SourceConstantAlpha = this->transparency.a;
   blender.AlphaFormat = AC_SRC_ALPHA;

   position = this->point.getPoint();
   
   if (!UpdateLayeredWindow(this->hwnd
                            ,(context == NULL) ? NULL : this->screenDC
                            ,&position
                            ,&this->size
                            ,context
                            ,(context == NULL) ? NULL : &ptZero
                            ,this->transparency.colorRef()
                            ,&blender
                            ,this->updateFlag))
      throw LayeredWindowException(L"UpdateLayeredWindow failed");
}

void
LayeredWindow::beginPaint
(void)
{
   this->screenDC = GetDC(NULL);
   this->paintContext = CreateCompatibleDC(this->screenDC);
}

void
LayeredWindow::endPaint
(void)
{
   if (!this->hasHWND())
      throw LayeredWindowException(L"can't end paint with no hwnd present");
   
   this->layeredUpdate(this->paintContext);
   DeleteDC(this->paintContext);
   ReleaseDC(NULL, this->screenDC);
   
   this->screenDC = NULL;
   this->paintContext = NULL;
}
