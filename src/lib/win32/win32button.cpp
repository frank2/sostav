#include "sostav/win32/button.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

ButtonException::ButtonException
(const char *what)
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

Button::~Button
(void)
{
   SubclassedWindow::~SubclassedWindow();
}
