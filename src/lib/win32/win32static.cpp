#include "sostav/win32/static.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

StaticException::StaticException
(const WCHAR *what)
   : Exception(what)
{
}

Static::Static
(Window *parent, std::wstring caption)
   : SubclassedWindow(parent, L"STATIC")
{
   this->setWindowText(caption);
}

Static::Static
(Static &window)
   : SubclassedWindow(window)
{
   this->setWindowText(window.getWindowText());
}

Static::Static
(void)
   : SubclassedWindow()
{
   this->setClassName(L"STATIC");
}

void
Static::onPaint
(void)
{
   this->defWndProc(this->hwnd, WM_PAINT, NULL, NULL);
}
