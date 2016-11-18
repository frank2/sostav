#include "sostav/win32/edit.hpp"

using namespace Sostav;
using namespace Sostav::Win32;

Edit::Edit
(HWND parent)
   : SubclassedWindow(parent, L"EDIT")
{
}

Edit::Edit
(Window *parent)
   : SubclassedWindow(parent, L"EDIT")
{
}

Edit::~Edit
(void)
{
   SubclassedWindow::~SubclassedWindow();
}
