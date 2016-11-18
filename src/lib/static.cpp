#include "sostav/static.hpp"

using namespace Sostav;

Static::Static
(HWND parent, std::wstring caption)
   : Window(parent, L"SvStatic")
{
   this->setWindowText(caption);
}

Static::Static
(Window *parent, std::wstring caption)
   : Window(parent, L"SvStatic")
{
   this->setWindowText(caption);
}

Static::Static
(void)
   : Window()
{
}

Static::~Static
(void)
{
   Window::~Window();
}

void
Static::onPaint
(void)
{
   HFONT fontHandle;
   HGDIOBJ oldFont;
   
   this->beginPaint();

   fontHandle = this->typeface.getHandle();

   oldFont = SelectObject(this->paintContext, fontHandle);

   if (this->bgColor.isTranslucent())
      throw StaticException("static controls can't be translucent");
   else if (this->bgColor.isTransparent)
      SetBkMode(this->paintContext, TRANSPARENT);
   else
   {
      RECT rect = {0,0,this->size.cx, this->size.cy};
      HBRUSH brush;
      
      SetBkColor(this->paintContext, this->bgColor.colorRef());
      brush = this->getBGBrush();

      FillRect(this->paintContext, &rect, brush);
   }

   if (!this->fgColor.isOpaque())
      throw StaticException("foreground color must be opaque");

   SetTextColor(this->paintContext, this->fgColor.colorRef());

   TextOut(this->paintContext
           ,0
           ,0
           ,this->windowText.c_str()
           ,this->windowText.size());

   fontHandle = (HBRUSH)SelectObject(this->paintContext, oldFont);

   DeleteObject(fontHandle);
   
   this->endPaint();
}
