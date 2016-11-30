#include "sostav/windows/layeredwindow.hpp"

using namespace Sostav;
using namespace Sostav::Windows;

LayeredWindowException::LayeredWindowException
(const char *what)
   : Exception(what)
{
}

LayeredWindow::LayeredWindow
(HWND parent, std::wstring className)
   : Window(parent, className)
{
   this->initialize(parent, className);
}

LayeredWindow::LayeredWindow
(Window *parent, std::wstring className)
{
   this->initialize((parent == NULL) ? NULL : parent->getHWND(), className);
}

LayeredWindow::LayeredWindow
(void)
{
   this->initialize(NULL, L"SvLayeredWindow");
}

void
LayeredWindow::setTransparency
(Drawing::Color color)
{
   if (this->hasHWND() && !SetLayeredWindowAttributes(this->hwnd
                                                      ,color.colorRef()
                                                      ,this->transparency.a
                                                      ,LWA_COLORKEY))
      throw LayeredWindowException("SetLayeredWindowAttributes failed");
   
   this->transparency.r = color.r;
   this->transparency.g = color.g;
   this->transparency.b = color.b;
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
   if (this->hasHWND() && !SetLayeredWindowAttributes(this->hwnd
                                                      ,this->transparency.colorRef()
                                                      ,alpha
                                                      ,LWA_ALPHA))
      throw LayeredWindowException("SetLayeredWindowAttributes failed");

   this->transparency.a = alpha;
}

BYTE
LayeredWindow::getAlpha
(void) const
{
   return this->transparency.a;
}

void
LayeredWindow::preCreate
(void)
{
   this->addExStyle(WS_EX_LAYERED);
}

void
LayeredWindow::updateLayered
(HDC context)
{
   POINT ptZero = { 0 };
   BLENDFUNCTION blender;
   
   if (!this->hasHWND())
      throw LayeredWindowException("can't update window without an hwnd");

   blender.BlendOp = AC_SRC_OVER;
   blender.BlendFlags = 0;
   blender.SourceConstantAlpha = 255;
   blender.AlphaFormat = AC_SRC_ALPHA;
   
   if (!UpdateLayeredWindow(this->hwnd
                            ,NULL
                            ,NULL
                            ,NULL
                            ,context
                            ,&ptZero
                            ,this->transparency.colorRef()
                            ,&blender
                            ,ULW_ALPHA))
      throw LayeredWindowException("UpdateLayeredWindow failed");
}

void
LayeredWindow::initialize
(HWND parent, std::wstring className)
{
   this->addExStyle(WS_EX_LAYERED);
   this->transparency = Drawing::Color::Transparent();
}
