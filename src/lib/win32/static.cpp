#include "sostav/win32/static.hpp"

using namespace Sostav;
using namespace Sostav::Win32;

Static::Static
(HWND parent, std::wstring caption)
   : SubclassedWindow(parent, L"STATIC")
{
   this->setWindowText(caption);
}

Static::Static
(Window *parent, std::wstring caption)
   : SubclassedWindow(parent, L"STATIC")
{
   this->setWindowText(caption);
}
