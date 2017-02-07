#include "sostav/controls/static.hpp"

using namespace Sostav;
using namespace Sostav::Controls;
using namespace Sostav::Windows;

StaticException::StaticException
(const WCHAR *what)
   : Exception(what)
{
}

Static::Static
(Window *parent, std::wstring caption)
   : PaintedWindow(parent, L"SvStatic")
{
   this->setWindowText(caption);
}

Static::Static
(Static &window)
   : PaintedWindow(window)
{
   this->setWindowText(window.getWindowText());
}

Static::Static
(void)
   : PaintedWindow()
{
}

void
Static::paint
(HDC context)
{
   HFONT fontHandle;
   HGDIOBJ oldFont;
   
   fontHandle = this->typeface.getHandle();

   oldFont = SelectObject(context, fontHandle);

   if (this->bgColor.isTranslucent())
      throw StaticException(L"static controls can't be translucent");
   else if (this->bgColor.isTransparent())
      SetBkMode(this->paintContext, TRANSPARENT);
   else
   {
      RECT rect = {0,0,this->size.cx,this->size.cy};
      HBRUSH brush;
      
      SetBkColor(context, this->bgColor.colorRef());
      brush = this->getBGBrush();

      FillRect(context, &rect, brush);
   }

   if (!this->fgColor.isOpaque())
      throw StaticException(L"foreground color must be opaque");

   SetTextColor(context, this->fgColor.colorRef());

   if (!TextOut(context
                ,0
                ,0
                ,this->windowText.c_str()
                ,this->windowText.size()))
      throw StaticException(L"TextOut failed");

   fontHandle = (HFONT)SelectObject(context, oldFont);

   DeleteObject(fontHandle);
}
