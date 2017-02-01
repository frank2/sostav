#include "sostav/win32/edit.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

EditException::EditException
(const char *what)
   : Exception(what)
{
}

Edit::Edit
(Window *parent)
   : SubclassedWindow(parent, L"EDIT")
{
}

Edit::Edit
(Edit &window)
   : SubclassedWindow(window)
{
}

Edit::Edit
(void)
   : SubclassedWindow()
{
   this->setClassName(L"EDIT");
}

Edit::~Edit
(void)
{
   SubclassedWindow::~SubclassedWindow();
}
