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
