#include "sostav/windows/paintedwindow.hpp"

using namespace Sostav;
using namespace Sostav::Windows;

PaintedWindowException::PaintedWindowException
(const WCHAR *what)
   : Exception(what)
{
}

PaintedWindow::PaintedWindow
(Window *window, std::wstring className)
   : Window(window, className)
{
   this->paintContext = NULL;
   memset(&this->paintStruct, 0, sizeof(PAINTSTRUCT));
}

PaintedWindow::PaintedWindow
(PaintedWindow &window)
   : Window(window)
{
   this->paintContext = NULL;
   memset(&this->paintStruct, 0, sizeof(PAINTSTRUCT));
}

PaintedWindow::PaintedWindow
(void)
   : Window()
{
   this->paintContext = NULL;
   memset(&this->paintStruct, 0, sizeof(PAINTSTRUCT));
}

void
PaintedWindow::paint
(HDC context)
{
   if (this->hasHWND())
      this->defWndProc(this->hwnd, WM_PAINT, NULL, NULL);
}

void
PaintedWindow::beginPaint
(void)
{
   if (!this->hasHWND())
      throw PaintedWindowException(L"can't begin paint on window with no hwnd");
   
   this->paintContext = BeginPaint(this->hwnd, &this->paintStruct);
}

void
PaintedWindow::endPaint
(void)
{
   if (!this->hasHWND())
      throw PaintedWindowException(L"can't end paint on window with no hwnd");

   EndPaint(this->hwnd, &this->paintStruct);

   memset(&this->paintStruct, 0, sizeof(PAINTSTRUCT));
   this->paintContext = NULL;
}

LRESULT
PaintedWindow::onPaint
(void)
{
   this->beginPaint();
   this->paint(this->paintContext);
   this->endPaint();

   return (LRESULT)0;
}
