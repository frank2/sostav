#include "sostav/subclassedwindow.hpp"

using namespace Sostav;

SubclassedWindow::SubclassedWindow
(HWND parent, std::wstring className)
   : Window::Window(parent, className)
{
}

SubclassedWindow::SubclassedWindow
(Window *parent, std::wstring className)
   : Window::Window(parent, className)
{
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

   return window->subclassProc(msg, wParam, lParam);
}

LRESULT CALLBACK
SubclassedWindow::subclassProc
(UINT msg, WPARAM wParam, LPARAM lParam)
{
   /* handle some window messages */
   switch(msg)
   {
   default:
      return DefSubclassProc(this->hwnd, msg, wParam, lParam);
   }
}

void
SubclassedWindow::setHWND
(HWND window)
{
   Window::setHWND(window);

   if (!SetWindowSubclass(window
                          ,SubclassedWindow::WndProcSubclass
                          ,this->subclassID
                          ,(DWORD_PTR)this))
      throw SubclassedWindowException("SetWindowSubclass failed");
}

void
SubclassedWindow::setSubclassID
(UINT_PTR id)
{
   if (this->hwnd != NULL)
      throw SubclassedWindowException("cannot set subclass ID after subclassed window created");

   this->subclassID = id;
}

UINT_PTR
SubclassedWindow::getSubclassID
(void)
{
   return this->subclassID;
}

void
SubclassedWindow::create
(void)
{
   Window::create();

   if (!SetWindowSubclass(this->hwnd
                          ,SubclassedWindow::WndProcSubclass
                          ,this->subclassID
                          ,(DWORD_PTR)this))
      throw SubclassedWindowException("SetWindowSubclass failed");
}

void
SubclassedWindow::initialize
(HWND parent, std::wstring className)
{
   Window::initialize(parent, className);

   this->subclassID = 0;
}
