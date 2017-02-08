#include "sostav/win32/syslink.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

SysLinkException::SysLinkException
(const WCHAR *what)
   : Exception(what)
{
}

SysLink::SysLink
(Window *parent)
   : SubclassedWindow(parent, L"SYSLINK")
{
}

SysLink::SysLink
(SysLink &window)
   : SubclassedWindow(window)
{
}

SysLink::SysLink
(void)
   : SubclassedWindow()
{
   this->setClassName(L"SYSLINK");
}
