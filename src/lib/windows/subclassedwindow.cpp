#include "sostav/windows/subclassedwindow.hpp"

using namespace Sostav;
using namespace Sostav::Windows;

SubclassedWindowException::SubclassedWindowException
(const char *what)
   : Exception(what)
{
}

SubclassedWindow::SubclassedWindow
(Window *parent, std::wstring className)
   : Window(parent, className)
{
   this->defWndProc = DefSubclassProc;
   this->subclassID = 0;
   this->oldMethod = false;
}

SubclassedWindow::SubclassedWindow
(SubclassedWindow &window)
   : Window(window)
{
   this->setDefWndProc(window.getDefWndProc());
   this->setSubclassID(window.getSubclassID());
   this->setSubclassMethod(window.getSubclassMethod());
}

SubclassedWindow::SubclassedWindow
(void)
   : Window()
{
   this->defWndProc = DefSubclassProc;
   this->subclassID = 0;
   this->oldMethod = false;
}

SubclassedWindow::~SubclassedWindow
(void)
{
   Window::~Window();
}

LRESULT CALLBACK
SubclassedWindow::WndProcSubclass
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
   SubclassedWindow *window = (SubclassedWindow *)Window::FindWindow(hwnd);

   if (window == NULL)
   {
      SubclassedWindow *refWindow = (SubclassedWindow *)dwRefData;

      if (refWindow == NULL)
         throw SubclassedWindowException("no window pointer passed to subclass proc");
      
      /* window was just subclassed, map the hwnd to the ref data */
      Window::MapWindow(hwnd, (Window *)refWindow);
      window = refWindow;
      window->setHWND(hwnd);
   }
   
   if (window == NULL)
      throw SubclassedWindowException("no window pointer passed to subclass proc");

   return window->windowProc(msg, wParam, lParam);
}

void
SubclassedWindow::setHWND
(HWND window)
{
   Window::setHWND(window);
   this->subclassWindow(window);
}

void
SubclassedWindow::setSubclassID
(UINT_PTR id)
{
   if (this->hasHWND())
      throw SubclassedWindowException("cannot set subclass ID after subclassed window created");

   this->subclassID = id;
}

UINT_PTR
SubclassedWindow::getSubclassID
(void) const
{
   return this->subclassID;
}

void
SubclassedWindow::setSubclassMethod
(bool method)
{
   this->oldMethod = method;
}

bool
SubclassedWindow::getSubclassMethod
(void) const
{
   return this->oldMethod;
}

void
SubclassedWindow::postCreate
(void)
{
   Window::postCreate();
   
   this->subclassWindow(this->hwnd);

   /* set this after subclassing to prevent a race condition */
   Window::MapWindow(this->hwnd, this);
   
   /* use SetWindowPos to retrigger WM_NCCALCSIZE */
   SetWindowPos(this->hwnd
                ,0, 0, 0, 0, 0
                ,SWP_FRAMECHANGED| SWP_NOMOVE | SWP_NOSIZE
                | SWP_NOACTIVATE | SWP_NOZORDER);
}

void
SubclassedWindow::subclassWindow
(HWND window)
{
   if (this->oldMethod)
   {
      if (SetWindowLongPtr(window
                           ,GWLP_WNDPROC
                           ,(UINT_PTR)Window::WndProc) == NULL)
         throw SubclassedWindowException("SetWindowLong failed");
      this->defWndProc = DefWindowProc;
   }
   else if (!this->oldMethod && !SetWindowSubclass(window
                                                   ,SubclassedWindow::WndProcSubclass
                                                   ,this->subclassID
                                                   ,(DWORD_PTR)this))
      throw SubclassedWindowException("SetWindowSubclass failed");
}
