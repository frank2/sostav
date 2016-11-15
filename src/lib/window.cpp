#include "sostav/window.hpp"

using namespace Sostav;
std::map<HWND, Window *> WINDOW_POOL;

Window::Window
(HWND parent, std::wstring className)
{
   this->parentHWND = parent;
   this->parentWindow = Window::FindWindow(parent);
   this->hwnd = NULL;
   
   if (this->parentWindow != NULL || this->parentHWND != NULL)
   {
      this->addStyle(WS_CHILD);

      if (this->parentWindow != NULL)
         this->parentWindow->addChild(this);
   }
   
   this->position.x = this->position.y = this->size.cx = this->size.cy = 0;

   this->paintContext = NULL;

   this->icon = NULL;
   this->cursor = LoadCursor(NULL, IDC_ARROW);
   this->menu = NULL;

   this->style = WS_VISIBLE;
   this->exStyle = 0;
   this->classStyle = 0;

   this->bgColor.hexValue = 0xFF000000 | GetSysColor(COLOR_WINDOW);

   if (className.empty())
      throw WindowException("class name cannot be empty");
   
   this->className.assign(className);
}

Window::Window
(Window *parent, std::wstring className)
{
   Window((HWND)((parent == NULL) ? NULL : parent->getHWND())
          ,className);

   /* if the parent window doesn't have an hwnd, set the parentWindow after
      the fact */
   if (this->parentWindow == NULL && parent != NULL)
      this->parentWindow = parent;
}

Window::~Window
()
{
   if (this->hwnd != NULL)
      this->destroy();
}

void
Window::MapWindow
(HWND hwnd, Window *window)
{
   WINDOW_POOL[hwnd] = window;
}

void
Window::UnmapWindow
(HWND hwnd)
{
   WINDOW_POOL.erase(hwnd);
}

Window *
Window::FindWindow
(HWND hwnd)
{
   std::map<HWND, Window *>::iterator iter;

   if (hwnd == NULL)
      return NULL;

   iter = WINDOW_POOL.find(hwnd);

   if (iter == WINDOW_POOL.end())
      return NULL;

   return iter->second;
}

LRESULT CALLBACK
Window::windowProc
(UINT msg, WPARAM wParam, LPARAM lParam)
{
   /* handle some window messages */
   switch(msg)
   {
   default:
      return DefWindowProc(this->hwnd, msg, wParam, lParam);
   }
}

void
Window::setParent
(HWND newParent)
{
   if (this->hwnd != NULL)
      SetParent(this->hwnd, newParent);

   this->parentHWND = newParent;
}

void
Window::setParent
(Window *newParent)
{
   if (this->parentWindow != NULL)
      this->parentWindow->removeChild(this);

   if (newParent != NULL)
      newParent->addChild(this);

   this->parentWindow = newParent;

   setParent(newParent->getHWND());
}

Window *
Window::getParentWindow
(void)
{
   return this->parentWindow;
}

HWND
Window::getParentHWND
(void)
{
   return this->parentHWND;
}

void
Window::setHWND
(HWND hwnd)
{
   if (this->hwnd != NULL)
      Window::UnmapWindow(this->hwnd);
   
   this->hwnd = hwnd;
   Window::MapWindow(hwnd, this);

   if (hwnd == NULL)
      return;
   
   /* if the parentWindow is non-null, get that hwnd. otherwise, get the
      parentHWND variable. */

   if (this->parentWindow != NULL)
      SetParent(hwnd, this->parentWindow->getHWND());
   else
      SetParent(hwnd, this->parentHWND);
}

HWND
Window::getHWND
(void)
{
   if (this->hwnd == NULL)
      this->create();
   
   return this->hwnd;
}

void
Window::setPosition
(long x, long y)
{
   if (this->hwnd != NULL && !SetWindowPos(this->hwnd
                                           ,NULL
                                           ,x
                                           ,y
                                           ,0
                                           ,0
                                           ,SWP_NOACTIVATE | SWP_NOOWNERZORDER
                                           | SWP_NOZORDER | SWP_NOSIZE))
      throw WindowException("SetWindowPos failed");

   this->position.x = x;
   this->position.y = y;
}

void
Window::setPosition
(POINT position)
{
   this->setPosition(position.x, position.y);
}

POINT
Window::getPosition
(void)
{
   return this->position;
}

void
Window::setSize
(long cx, long cy)
{
   if (this->hwnd != NULL && !SetWindowPos(this->hwnd
                                           ,NULL
                                           ,0
                                           ,0
                                           ,cx
                                           ,cy
                                           ,SWP_NOACTIVATE | SWP_NOOWNERZORDER
                                           | SWP_NOZORDER | SWP_NOMOVE))
      throw WindowException("SetWindowPos failed");

   this->size.cx = cx;
   this->size.cy = cy;
}

void
Window::setSize
(SIZE size)
{
   this->setSize(size.cx, size.cy);
}

SIZE
Window::getSize
(void)
{
   return this->size;
}

void
Window::setRect
(long left, long top, long right, long bottom)
{
   if (this->hwnd != NULL && !SetWindowPos(this->hwnd
                                           ,NULL
                                           ,left
                                           ,top
                                           ,right - left
                                           ,bottom - top
                                           ,SWP_NOACTIVATE | SWP_NOOWNERZORDER
                                           | SWP_NOZORDER))
      throw WindowException("SetWindowPos failed");

   this->position.x = left;
   this->position.y = top;
   this->size.cx = right - left;
   this->size.cy = bottom - top;
}

void
Window::setRect
(RECT rect)
{
   this->setRect(rect.left, rect.top, rect.right, rect.bottom);
}

RECT
Window::getRect
(void)
{
   RECT rect;

   rect.left = this->position.x;
   rect.top = this->position.y;
   rect.right = rect.left + this->size.cx;
   rect.bottom = rect.top + this->size.cy;

   return rect;
}

void
Window::setIcon
(HICON icon)
{
   if (this->hwnd != NULL && SetClassLong(this->hwnd
                                          ,GCL_HICON
                                          ,(LONG)icon) == 0)
      throw WindowException("SetClassLong failed");

   this->icon = icon;
}

HICON
Window::getIcon
(void)
{
   return this->icon;
}

void
Window::setCursor
(HCURSOR cursor)
{
   if (this->hwnd != NULL && SetClassLong(this->hwnd
                                          ,GCL_HCURSOR
                                          ,(LONG)cursor) == 0)
      throw WindowException("SetClassLong failed");

   this->cursor = cursor;
}

HCURSOR
Window::getCursor
(void)
{
   return this->cursor;
}

void
Window::setMenu
(HMENU menu)
{
   if (this->hwnd != NULL && !SetMenu(this->hwnd, menu))
      throw WindowException("SetMenu failed");

   this->menu = menu;
}

HMENU
Window::getMenu
(void)
{
   return this->menu;
}

void
Window::setStyle
(DWORD style)
{
   if (this->hwnd != NULL && SetWindowLongPtr(this->hwnd
                                              ,GWL_STYLE
                                              ,style) == 0)
      throw WindowException("SetWindowLongPtr failed");

   this->style = style;
}

DWORD
Window::getStyle
(void)
{
   return this->style;
}

void
Window::addStyle
(DWORD style)
{
   this->setStyle(this->style | style);
}

void
Window::removeStyle
(DWORD style)
{
   this->setStyle(this->style & ~style);
}

void
Window::setExStyle
(DWORD exStyle)
{
   if (this->hwnd != NULL && SetWindowLongPtr(this->hwnd
                                              ,GWL_EXSTYLE
                                              ,exStyle) == 0)
      throw WindowException("SetWindowLongPtr failed");

   this->exStyle = exStyle;
}

DWORD
Window::getExStyle
(void)
{
   return this->exStyle;
}

void
Window::addExStyle
(DWORD exStyle)
{
   this->setExStyle(this->exStyle | exStyle);
}

void
Window::removeExStyle
(DWORD exStyle)
{
   this->setExStyle(this->exStyle & ~exStyle);
}

void
Window::setClassStyle
(DWORD classStyle)
{
   if (this->hwnd != NULL && SetClassLong(this->hwnd
                                          ,GCL_STYLE
                                          ,classStyle) == 0)
      throw WindowException("SetClassLong failed");

   this->classStyle = classStyle;
}

DWORD
Window::getClassStyle
(void)
{
   return this->classStyle;
}

void
Window::addClassStyle
(DWORD classStyle)
{
   this->setClassStyle(this->classStyle | classStyle);
}

void
Window::removeClassStyle
(DWORD classStyle)
{
   this->setClassStyle(this->classStyle & ~classStyle);
}

void
Window::setClassName
(std::wstring className)
{
   if (this->hwnd != NULL)
      throw WindowException("class already registered, window has been created");

   this->className.assign(className);
}

std::wstring
Window::getClassName
(void)
{
   return this->className;
}

void
Window::setMenuName
(std::wstring menuName)
{
   if (this->hwnd != NULL && SetClassLong(this->hwnd
                                          ,GCL_MENUNAME
                                          ,(LONG)menuName.c_str()) == 0)
      throw WindowException("SetClassLong failed");

   this->menuName.assign(menuName);
}

std::wstring
Window::getMenuName
(void)
{
   return this->menuName;
}

void
Window::setWindowText
(std::wstring windowText)
{
   if (this->hwnd != NULL && !SetWindowText(this->hwnd, windowText.c_str()))
      throw WindowException("SetWindowText failed");

   this->windowText.assign(windowText);
}

std::wstring
Window::getWindowText
(void)
{
   return this->windowText;
}

void
Window::setBGColor
(BYTE a, BYTE r, BYTE g, BYTE b)
{
   this->bgColor.a = a;
   this->bgColor.r = r;
   this->bgColor.g = g;
   this->bgColor.b = b;

   this->invalidate();
}

void
Window::setBGColor
(DWORD hexValue)
{
   setBGColor((hexValue >> 24) & 0xFF
              ,(hexValue >> 16) & 0xFF
              ,(hexValue >> 8) & 0xFF
              ,hexValue && 0xFF);
}

void
Window::setBGColor
(Color color)
{
   setBGColor(color.a, color.r, color.g, color.b);
}

Color
Window::getBGColor
(void)
{
   return this->bgColor;
}

bool
Window::hasChild
(Window *child)
{
   return (this->children.find(child) != this->children.end());
}

void
Window::addChild
(Window *child)
{
   this->children.insert(child);
}

void
Window::removeChild
(Window *child)
{
   if (!this->hasChild(child))
      throw WindowException("couldn't find child window");

   this->children.erase(child);
}

void
Window::create
(void)
{
   WNDCLASSEX classInfo;

   if (this->className.empty())
      throw WindowException("class name cannot be empty");

   /* if there's no such class, attempt to register it. */
   if (!GetClassInfoEx(GetModuleHandle(NULL)
                       ,this->className.c_str()
                       ,&classInfo))
   {
      memset(&classInfo, 0, sizeof(WNDCLASSEX));
      
      classInfo.cbSize = sizeof(WNDCLASSEX);
      classInfo.style = this->classStyle;
      classInfo.lpfnWndProc = WndProc;
      classInfo.hInstance = GetModuleHandle(NULL);
      classInfo.hIcon = this->icon;
      classInfo.hCursor = this->cursor;
      classInfo.hbrBackground = CreateSolidBrush(SVRGB(this->bgColor));

      if (!this->menuName.empty())
         classInfo.lpszMenuName = this->menuName.c_str();

      classInfo.lpszClassName = this->className.c_str();
      classInfo.hIconSm = this->icon;

      if (RegisterClassEx(&classInfo) == 0)
         throw WindowException("failed to register window class");
   }

   this->hwnd = CreateWindowEx(this->exStyle
                               ,this->className.c_str()
                               ,(this->windowText.empty()) ? NULL : this->windowText.c_str()
                               ,this->style
                               ,this->position.x, this->position.y
                               ,this->size.cx, this->size.cy
                               ,(this->parentWindow != NULL) ? this->parentWindow->getHWND() :
                                (this->parentHWND != NULL) ? this->parentHWND : NULL
                               ,this->menu
                               ,GetModuleHandle(NULL)
                               ,NULL);

   if (this->hwnd == NULL)
      throw WindowException("failed to create window");

   Window::MapWindow(this->hwnd, this);
}

void
Window::destroy
(void)
{
   if (this->hwnd != NULL)
   {
      Window::UnmapWindow(this->hwnd);
      DestroyWindow(this->hwnd);
   }

   if (this->parentWindow != NULL)
      this->parentWindow->removeChild(this);
   
   this->hwnd = NULL;
}

void
Window::invalidate
(void)
{
   RECT rect = {0,0,this->size.cx,this->size.cy};

   if (this->hwnd != NULL)
      InvalidateRect(this->hwnd, &rect, FALSE);
}

LRESULT CALLBACK
WndProc
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   Window *window = Window::FindWindow(hwnd);

   if (window == NULL)
      throw WindowException("couldn't find window by hwnd for callback");

   return window->windowProc(msg, wParam, lParam);
}
   
