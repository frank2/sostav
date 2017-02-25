#include "sostav/win32/button.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

ButtonException::ButtonException
(const WCHAR *what)
   : Exception(what)
{
}

Button::Button
(Window *parent)
   : SubclassedWindow(parent, L"BUTTON")
{
}

Button::Button
(Button &window)
   : SubclassedWindow(window)
{
}

Button::Button
(void)
   : SubclassedWindow()
{
   this->setClassName(L"BUTTON");
}

LRESULT
Button::onKeyUp
(DWORD keyValue, DWORD keyFlags)
{
   switch (keyValue)
   {
   case VK_SPACE:
   case VK_RETURN:
   {
      this->sendMessage(BM_CLICK, NULL, NULL);

      return (LRESULT)0;
   }
   }

   return SubclassedWindow::onKeyUp(keyValue, keyFlags);
}

LRESULT
Button::onLButtonDown
(WORD virtualKeys, WORD x, WORD y)
{
   /* for some reason subclassed buttons don't send WM_COMMAND. fix this in the default behavior. */
   if (this->hasStyle(BS_PUSHBUTTON) && this->parent != NULL)
      this->parent->sendMessage(WM_COMMAND, MAKEWPARAM(this->menu, 0), (LPARAM)this->hwnd);

   return SubclassedWindow::onLButtonUp(virtualKeys, x, y);
}
      
