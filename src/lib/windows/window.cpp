#include "sostav/windows/window.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

std::map<HWND, Window *> Window::WindowPool;
Window *Window::LastWindow;

WindowException::WindowException
(const WCHAR *what)
   : Exception(what)
{
}

Window::Window
(Window *parent, std::wstring className)
{
   this->parent = parent;
   this->hwnd = NULL;
   this->setDefWndProc(DefWindowProc);

   this->active = false;
   this->enabled = true;
   this->visible = false;
   this->moving = false;
   this->captured = false;
   this->hovering = false;
   
   this->style = WS_VISIBLE;
   this->exStyle = 0;
   this->classStyle = 0;

   if (this->parent != NULL)
   {
      this->addStyle(WS_CHILD);
      this->parent->addChild(this);
   }
   
   this->size.cx = this->size.cy = 0;

   this->cursor = LoadCursor(NULL, IDC_ARROW);
   this->menu = NULL;

   this->setDefaultColors();
   this->bgBrush = NULL;
   this->fgBrush = NULL;
   this->borderBrush = NULL;
   this->borderSize = 0;

   if (className.empty())
      throw WindowException(L"class name cannot be empty");
   
   this->className.assign(className);
}

Window::Window
(Window &window)
{
   this->parent = window.getParent();
   this->hwnd = NULL; /* intentionally do not copy the hwnd */
   this->setDefWndProc(window.getDefWndProc());

   this->active = window.isActive();
   this->enabled = window.isEnabled();
   this->visible = window.isVisible();
   this->moving = window.isMoving();
   this->captured = window.isCaptured();
   this->hovering = window.isHovering();

   this->style = window.getStyle();
   this->exStyle = window.getExStyle();
   this->classStyle = window.getClassStyle();

   if (this->parent != NULL)
   {
      this->addStyle(WS_CHILD);
      this->parent->addChild(this);
   }

   this->setPosition(window.getPosition());
   this->setSize(window.getSize());

   this->setIcon(window.getIcon());
   this->cursor = window.getCursor();
   this->menu = window.getMenu();

   this->typeface = window.getTypeface();
   this->bgColor = window.getBGColor();
   this->fgColor = window.getFGColor();
   this->borderColor = window.getBorderColor();
   this->bgBrush = NULL;
   this->fgBrush = NULL;
   this->borderBrush = NULL;
   this->borderSize = window.getBorderSize();

   this->className.assign(window.getClassName());
   this->menuName.assign(window.getMenuName());
   this->windowText.assign(window.getWindowText());
}

Window::Window
(void)
{
   this->parent = NULL;
   this->hwnd = NULL;
   this->setDefWndProc(DefWindowProc);

   this->active = false;
   this->enabled = true;
   this->visible = false;
   this->moving = false;
   this->captured = false;
   this->hovering = false;
   
   this->style = 0;
   this->exStyle = 0;
   this->classStyle = 0;
   this->size.cx = this->size.cy = 0;

   this->setDefaultColors();
   this->cursor = LoadCursor(NULL, IDC_ARROW);
   this->menu = NULL;
   this->bgBrush = NULL;
   this->fgBrush = NULL;
   this->borderBrush = NULL;
   this->borderSize = 0;
}

Window::~Window
()
{
   std::list<Window *>::iterator childIter, beginIter, endIter;
   
   if (this->parent != NULL)
      this->parent->removeChild(this);

   if (this->hasHWND())
      this->destroy();

   beginIter = this->children.begin();
   endIter = this->children.end();

   for (childIter=this->children.begin(); childIter!=this->children.end(); ++childIter)
   {
      Window *child = *childIter;

      if (child->hasHWND())
         child->destroy();
   }

   this->children.clear();
}

void
Window::MessageLoop
(void)
{
   MSG msg;

   while (GetMessage(&msg, NULL, 0, 0))
   {
      try
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
      catch (Exception &e)
      {
         std::wcerr << L"Sostav exception:" << e.what() << std::endl;
      }
   }
}

LRESULT CALLBACK
Window::WndProc
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   Window *window = Window::FindWindow(hwnd);

   if (window == NULL)
   {
      /* window is being created, map the hwnd to the last window to call Window::create */
      Window::MapWindow(hwnd, Window::LastWindow);
      window = Window::LastWindow;
      window->setHWND(hwnd);
   }

   if (window == NULL)
      throw WindowException(L"couldn't find window by hwnd for callback");

   return window->windowProc(msg, wParam, lParam);
}

void
Window::MapWindow
(HWND hwnd, Window *window)
{
   Window::WindowPool[hwnd] = window;
}

void
Window::UnmapWindow
(HWND hwnd)
{
   Window::WindowPool.erase(hwnd);
}

Window *
Window::FindWindow
(HWND hwnd)
{
   std::map<HWND, Window *>::iterator iter;

   if (hwnd == NULL)
      return NULL;

   iter = Window::WindowPool.find(hwnd);

   if (iter == Window::WindowPool.end())
      return NULL;

   return iter->second;
}

LRESULT
Window::windowProc
(UINT msg, WPARAM wParam, LPARAM lParam)
{
   /* handle some window messages */
   switch(msg)
   {
   case WM_ACTIVATE:
      return this->onActivate((BYTE)wParam, (HWND)lParam);

   case WM_CHAR:
      return this->onChar((DWORD)wParam, (DWORD)lParam);

   case WM_COMMAND:
      return this->onCommand(HIWORD(wParam), LOWORD(wParam), (HWND)lParam);
      
   case WM_CTLCOLOREDIT:
      return (LRESULT)this->onCtlColorEdit((HDC)wParam, (HWND)lParam);
   
   case WM_CTLCOLORSTATIC:
      return (LRESULT)this->onCtlColorStatic((HDC)wParam, (HWND)lParam);

   case WM_CTLCOLORBTN:
      return (LRESULT)this->onCtlColorBtn((HDC)wParam, (HWND)lParam);

   case WM_CREATE:
      return this->onCreate((LPCREATESTRUCT)lParam);
      
   case WM_DESTROY:
      return this->onDestroy();

   case WM_ENABLE:
      return this->onEnable((BOOL)wParam);

   case WM_ERASEBKGND:
      return this->onEraseBkgnd((HDC)wParam);

   case WM_KEYDOWN:
      return this->onKeyDown((DWORD)wParam, (DWORD)lParam);

   case WM_KEYUP:
      return this->onKeyUp((DWORD)wParam, (DWORD)lParam);

   case WM_KILLFOCUS:
      return this->onKillFocus((HWND)wParam);

   case WM_LBUTTONDOWN:
      return this->onLButtonDown((WORD)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

   case WM_LBUTTONUP:
      return this->onLButtonUp((WORD)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

   case WM_MOUSELEAVE:
      return this->onMouseLeave();

   case WM_MOUSEMOVE:
      return this->onMouseMove((WORD)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

   case WM_MOVE:
      return this->onMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

   case WM_NCCALCSIZE:
      return this->onNCCalcSize((BOOL)wParam, lParam);
      
   case WM_NCPAINT:
      return this->onNCPaint((HRGN)wParam);

   case WM_NOTIFY:
      return this->onNotify((UINT)wParam, (NMHDR *)lParam);

   case WM_PAINT:
      return this->onPaint();

   case WM_RBUTTONDOWN:
      return this->onRButtonDown((WORD)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

   case WM_RBUTTONUP:
      return this->onRButtonUp((WORD)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

   case WM_SETFOCUS:
      return this->onSetFocus((HWND)wParam);

   case WM_SHOWWINDOW:
      return this->onShowWindow((BOOL)wParam, (WORD)lParam);

   case WM_WINDOWPOSCHANGING:
      return this->onWindowPosChanging((LPWINDOWPOS)lParam);

   case WM_WINDOWPOSCHANGED:
      return this->onWindowPosChanged((LPWINDOWPOS)lParam);
      
   default:
      return this->defWndProc(this->hwnd, msg, wParam, lParam);
   }
}

bool
Window::hasChild
(Window *child) const
{
   return (std::find(this->children.begin(), this->children.end(), child) != this->children.end());
}

void
Window::addChild
(Window *child)
{
   if (!this->hasChild(child))
      this->children.push_back(child);
}

void
Window::removeChild
(Window *child)
{
   if (!this->hasChild(child))
      throw WindowException(L"couldn't find child window");

   this->children.erase(std::find(this->children.begin(), this->children.end(), child));
}

std::list<Window *>
Window::getChildren
(void) const
{
   return this->children;
}

void
Window::setParent
(Window *newParent)
{
   if (this->parent != NULL)
      this->parent->removeChild(this);

   if (newParent != NULL)
      newParent->addChild(this);

   this->parent = newParent;
}

Window *
Window::getParent
(void) const
{
   return this->parent;
}

bool
Window::hasLink
(Window *link) const
{
   return (this->links.find(link) != this->links.end());
}

void
Window::addLink
(Window *link)
{
   if (!this->hasLink(link))
      this->links.insert(link);

   if (!link->hasLink(this))
      link->addLink(this);
}

void
Window::removeLink
(Window *link)
{
   if (this->hasLink(link))
      this->links.erase(link);

   if (link->hasLink(this))
      link->removeLink(this);
}

bool
Window::isActive
(void) const
{
   return this->active;
}

bool
Window::isEnabled
(void) const
{
   return this->enabled;
}

bool
Window::isVisible
(void)
{
   if (this->hasHWND())
      this->visible = this->hasStyle(WS_VISIBLE);
   
   return this->visible;
}

bool
Window::isMoving
(void) const
{
   return this->moving;
}

bool
Window::isCaptured
(void) const
{
   return this->captured;
}

bool
Window::isHovering
(void) const
{
   return this->hovering;
}

void
Window::setHWND
(HWND hwnd)
{
   if (this->hwnd != NULL)
      Window::UnmapWindow(this->hwnd);
   
   this->hwnd = hwnd;

   if (hwnd == NULL)
      return;
   
   Window::MapWindow(hwnd, this);

   /* don't use hasStyle here because that only checks the variable, getStyle retrieves it from the hwnd */
   if (this->parent != NULL && (this->getStyle() & WS_CHILD) == WS_CHILD)
      SetParent(hwnd, this->parent->getHWND());
}

HWND
Window::getHWND
(void) const
{
   return this->hwnd;
}

bool
Window::hasHWND
(void) const
{
   return this->hwnd != NULL;
}

LRESULT
Window::sendMessage
(UINT msg, WPARAM wParam, LPARAM lParam)
{
   if (!this->hasHWND())
      throw WindowException(L"can't send message to an uncreated window");

   return SendMessage(this->hwnd, msg, wParam, lParam);
}

void
Window::setDefWndProc
(WNDPROC wndProc)
{
   this->defWndProc = wndProc;
}

WNDPROC
Window::getDefWndProc
(void) const
{
   return this->defWndProc;
}

void
Window::setPosition
(long x, long y)
{
   this->moving = true;
   
   if (!this->links.empty())
   {
      /* also move links, but only if they don't have hwnds-- otherwise it's
         handled by onWindowPosChanged */
      std::set<Window *>::iterator iter;

      for (iter=this->links.begin(); iter!=this->links.end(); ++iter)
      {
         Window *link = *iter;

         if (!link->hasHWND() && !link->isMoving())
            link->move(x - this->point.getX(), y - this->point.getY());
      }
   }

   this->moving = false;

   if (this->hasHWND() && !SetWindowPos(this->hwnd
                                        ,NULL
                                        ,x
                                        ,y
                                        ,0
                                        ,0
                                        ,SWP_NOACTIVATE | SWP_NOOWNERZORDER
                                        | SWP_NOZORDER | SWP_NOSIZE))
      throw WindowException(L"SetWindowPos failed");

   this->point.setX(x);
   this->point.setY(y);

   /* redundantly diffuse the moving switch so that we're sure we're not moving */
   this->moving = false;
}

void
Window::setPosition
(POINT position)
{
   this->setPosition(position.x, position.y);
}

void
Window::setPosition
(AbsolutePoint point)
{
   this->setPosition(point.getX(), point.getY());
}

AbsolutePoint
Window::getPosition
(void) const
{
   return this->point;
}

void
Window::setRelativePosition
(double x, double y)
{
   SIZE parentSize = this->getParentSize();

   this->point.setRelative(x, y, parentSize.cx, parentSize.cy);

   this->setPosition(this->point.getX(), this->point.getY());
}

void
Window::setRelativePosition
(Math::Point point)
{
   this->setRelativePosition(point.getX(), point.getY());
}

RelativePoint
Window::getRelativePosition
(void) const
{
   return this->point.relative(this->size);
}

void
Window::move
(long x, long y)
{
   this->setPosition(x + this->point.getX(), y + this->point.getY());
}

void
Window::setTopWindow
(void)
{
   if (!this->hasHWND())
      throw WindowException(L"no window handle to set");

   this->moving = true;

   if (!SetWindowPos(this->hwnd
                     ,HWND_TOP
                     ,0
                     ,0
                     ,0
                     ,0
                     ,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE))
      throw WindowException(L"SetWindowPos failed");

   this->moving = false;
}

void
Window::setTopmostWindow
(void)
{
   if (!this->hasHWND())
      throw WindowException(L"no window handle to set");

   this->moving = true;

   if (!SetWindowPos(this->hwnd
                     ,HWND_TOPMOST
                     ,0
                     ,0
                     ,0
                     ,0
                     ,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE))
      throw WindowException(L"SetWindowPos failed");

   this->moving = false;
}

void
Window::insertAfter
(HWND hwnd)
{
   if (!this->hasHWND())
      throw WindowException(L"can't change z-order of uncreated window");

   this->moving = true;

   if (!SetWindowPos(this->hwnd
                     ,hwnd
                     ,0
                     ,0
                     ,0
                     ,0
                     ,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE))
      throw WindowException(L"SetWindowPos failed");

   this->moving = false;
}

void
Window::insertAfter
(Window *window)
{
   if (!window->hasHWND())
      throw WindowException(L"supplied window has no hwnd");

   this->insertAfter(window->getHWND());
}

void
Window::insertBefore
(HWND hwnd)
{
   if (!this->hasHWND())
      throw WindowException(L"can't change z-order of uncreated window");

   this->moving = true;

   if (!SetWindowPos(hwnd
                     ,this->hwnd
                     ,0
                     ,0
                     ,0
                     ,0
                     ,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE))
      throw WindowException(L"SetWindowPos failed");

   this->moving = false;
}

void
Window::insertBefore
(Window *window)
{
   if (!window->hasHWND())
      throw WindowException(L"supplied window has no hwnd");

   this->insertBefore(window->getHWND());
}

void
Window::center
(void)
{
   this->centerX();
   this->centerY();
}

void
Window::centerX
(void)
{
   SIZE parentSize = this->getParentSize();
   
   this->setPosition(parentSize.cx / 2 - this->size.cx / 2
                     ,this->point.getY());
}

void
Window::centerY
(void)
{
   SIZE parentSize = this->getParentSize();

   this->setPosition(this->point.getX()
                     ,parentSize.cy / 2 - this->size.cy / 2);
}

void
Window::setSize
(long cx, long cy)
{
   if (this->hasHWND() && !SetWindowPos(this->hwnd
                                        ,NULL
                                        ,0
                                        ,0
                                        ,cx
                                        ,cy
                                        ,SWP_NOACTIVATE | SWP_NOOWNERZORDER
                                        | SWP_NOZORDER | SWP_NOMOVE))
      throw WindowException(L"SetWindowPos failed");

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
(void) const
{
   return this->size;
}

SIZE
Window::getParentSize
(void) const
{
   /* this is a const, so check the style var directly */
   if (this->parent != NULL && (this->style & WS_CHILD) == WS_CHILD)
      return this->parent->getSize();
   else
   {
      HWND desktop = GetDesktopWindow();
      RECT rect;
      SIZE size;

      if (!GetWindowRect(desktop, &rect))
         throw WindowException(L"GetWindowRect failed");
      
      size.cx = rect.right - rect.left;
      size.cy = rect.bottom - rect.top;

      return size;
   }
}

void
Window::setRect
(long left, long top, long right, long bottom)
{
   if (this->hasHWND() && !SetWindowPos(this->hwnd
                                        ,NULL
                                        ,left
                                        ,top
                                        ,right - left
                                        ,bottom - top
                                        ,SWP_NOACTIVATE | SWP_NOOWNERZORDER
                                        | SWP_NOZORDER))
      throw WindowException(L"SetWindowPos failed");

   this->point.setX(left);
   this->point.setY(top);
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
(void) const
{
   RECT rect;

   rect.left = this->point.getX();
   rect.top = this->point.getY();
   rect.right = rect.left + this->size.cx;
   rect.bottom = rect.top + this->size.cy;

   return rect;
}

void
Window::setIcon
(Drawing::Icon icon)
{
   this->icon.setHandle(icon.getHandle());
   
   if (this->hasHWND() && SetClassLongPtr(this->hwnd
                                          ,GCLP_HICON
                                          ,(LONG_PTR)this->icon.getHandle()) == 0)
      throw WindowException(L"SetClassLong failed");
}

Drawing::Icon
Window::getIcon
(void) const
{
   return this->icon;
}

void
Window::setCursor
(HCURSOR cursor)
{
   if (this->hasHWND() && SetClassLongPtr(this->hwnd
                                          ,GCLP_HCURSOR
                                          ,(LONG_PTR)cursor) == 0)
      throw WindowException(L"SetClassLong failed");

   this->cursor = cursor;
}

HCURSOR
Window::getCursor
(void) const
{
   return this->cursor;
}

void
Window::setMenu
(HMENU menu)
{
   if (this->hasHWND() && !SetMenu(this->hwnd, menu))
      throw WindowException(L"SetMenu failed");

   this->menu = menu;
}

HMENU
Window::getMenu
(void) const
{
   return this->menu;
}

void
Window::setStyle
(DWORD style)
{
   if (this->hasHWND() && SetWindowLongPtr(this->hwnd
                                           ,GWL_STYLE
                                           ,style) == 0)
      throw WindowException(L"SetWindowLongPtr failed");

   this->style = style;
}

DWORD
Window::getStyle
(void)
{
   if (this->hasHWND())
   {
      DWORD wndStyle;

      wndStyle = (DWORD)GetWindowLongPtr(this->hwnd
                                         ,GWL_STYLE);

      if (wndStyle == 0)
         throw WindowException(L"GetWindowLongPtr failed");

      this->style = wndStyle;
   }
       
   return this->style;
}

void
Window::addStyle
(DWORD style)
{
   this->setStyle(this->getStyle() | style);
}

void
Window::removeStyle
(DWORD style)
{
   this->setStyle(this->getStyle() & ~style);
}

bool
Window::hasStyle
(DWORD style)
{
   return (this->getStyle() & style) == style;
}

void
Window::setExStyle
(DWORD exStyle)
{
   if (this->hasHWND() && SetWindowLongPtr(this->hwnd
                                           ,GWL_EXSTYLE
                                           ,exStyle) == 0)
      throw WindowException(L"SetWindowLongPtr failed");

   this->exStyle = exStyle;
}

DWORD
Window::getExStyle
(void)
{
   if (this->hasHWND())
   {
      DWORD wndExStyle;

      wndExStyle = (DWORD)GetWindowLongPtr(this->hwnd
                                           ,GWL_EXSTYLE);

      if (wndExStyle == 0)
         throw WindowException(L"GetWindowLongPtr failed");

      this->exStyle = wndExStyle;
   }

   return this->exStyle;
}

void
Window::addExStyle
(DWORD exStyle)
{
   this->setExStyle(this->getExStyle() | exStyle);
}

void
Window::removeExStyle
(DWORD exStyle)
{
   this->setExStyle(this->getExStyle() & ~exStyle);
}

bool
Window::hasExStyle
(DWORD style)
{
   return (this->getExStyle() & style) == style;
}

void
Window::setClassStyle
(DWORD classStyle)
{
   if (this->hasHWND() && SetClassLongPtr(this->hwnd
                                          ,GCL_STYLE
                                          ,(LONG_PTR)classStyle) == 0)
      throw WindowException(L"SetClassLong failed");

   this->classStyle = classStyle;
}

DWORD
Window::getClassStyle
(void)
{
   if (this->hasHWND())
   {
      DWORD wndClassStyle;

      wndClassStyle = (DWORD)GetClassLongPtr(this->hwnd
                                         ,GCL_STYLE);

      if (wndClassStyle == 0)
         throw WindowException(L"GetClassLongPtr failed");

      this->classStyle = wndClassStyle;
   }

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

bool
Window::hasClassStyle
(DWORD style)
{
   return (this->getClassStyle() & style) == style;
}

void
Window::setClassName
(std::wstring className)
{
   if (this->hasHWND())
      throw WindowException(L"class already registered, window has been created");

   this->className.assign(className);
}

std::wstring
Window::getClassName
(void) const
{
   return this->className;
}

void
Window::setMenuName
(std::wstring menuName)
{
   if (this->hasHWND() && SetClassLongPtr(this->hwnd
                                          ,GCLP_MENUNAME
                                          ,(LONG_PTR)menuName.c_str()) == 0)
      throw WindowException(L"SetClassLong failed");

   this->menuName.assign(menuName);
}

std::wstring
Window::getMenuName
(void) const
{
   return this->menuName;
}

void
Window::setWindowText
(std::wstring windowText)
{
   if (this->hasHWND() && !SetWindowText(this->hwnd, windowText.c_str()))
      throw WindowException(L"SetWindowText failed");

   this->windowText.assign(windowText);
}

std::wstring
Window::getWindowText
(void)
{
   if (this->hasHWND())
   {
      WCHAR *windowText;
      size_t textLength;

      textLength = GetWindowTextLength(this->hwnd);

      if (textLength == 0)
         this->windowText.clear();
      else
      {
         windowText = (WCHAR *)malloc(sizeof(WCHAR) * (textLength+1));
         
         memset(windowText, 0, sizeof(WCHAR) * (textLength+1));

         if (GetWindowText(this->hwnd, windowText, textLength+1) == 0)
            throw WindowException(L"GetWindowText failed");

         this->windowText.assign(windowText);

         free(windowText);
      }
   }
   
   return this->windowText;
}

void
Window::setTypeface
(Font typeface)
{
   this->typeface = typeface;

   if (this->hasHWND())
      SendMessage(this->hwnd, WM_SETFONT, (WPARAM)this->typeface.getHandle(), (LPARAM)TRUE);
}

Font
Window::getTypeface
(void) const
{
   return this->typeface;
}

void
Window::setDefaultColors
(void)
{
   this->setBGColor(Color::FromColorRef(GetSysColor(COLOR_WINDOW)));
   this->setFGColor(Color::FromColorRef(GetSysColor(COLOR_WINDOWTEXT)));
   this->setBorderColor(Color::FromColorRef(GetSysColor(COLOR_ACTIVEBORDER)));
}

void
Window::setBGColor
(BYTE a, BYTE r, BYTE g, BYTE b)
{
   this->bgColor.a = a;
   this->bgColor.r = r;
   this->bgColor.g = g;
   this->bgColor.b = b;

   if (this->bgBrush != NULL)
   {
      DeleteObject(this->bgBrush);
      this->bgBrush = NULL;
   }

   if (this->hasHWND())
   {
      HBRUSH brush = this->getBGBrush();

      if (SetClassLongPtr(this->hwnd
                          ,GCLP_HBRBACKGROUND
                          ,(LONG_PTR)brush) == 0)
         throw WindowException(L"SetClassLongPtr failed");
   }

   this->invalidate();
}

void
Window::setBGColor
(DWORD hexValue)
{
   setBGColor((hexValue >> 24) & 0xFF
              ,(hexValue >> 16) & 0xFF
              ,(hexValue >> 8) & 0xFF
              ,hexValue & 0xFF);
}

void
Window::setBGColor
(Color color)
{
   setBGColor(color.a, color.r, color.g, color.b);
}

Color
Window::getBGColor
(void) const
{
   /* const, so check this->style directly */
   if (this->parent == NULL || (this->style & WS_CHILD) != WS_CHILD)
      return this->bgColor;
   else
   {
      if (this->bgColor.isTransparent())
         return this->parent->getBGColor();
      else if (this->bgColor.isTranslucent())
         return this->bgColor.blend(this->parent->getBGColor()
                                    ,(255 - this->bgColor.a) / 255.0);
      else
         return this->bgColor;
   }
}

HBRUSH
Window::getBGBrush
(void)
{
   if (this->bgBrush == NULL)
      this->bgBrush = CreateSolidBrush(this->getBGColor().colorRef());

   return this->bgBrush;
}

void
Window::setFGColor
(BYTE a, BYTE r, BYTE g, BYTE b)
{
   this->fgColor.a = a;
   this->fgColor.r = r;
   this->fgColor.g = g;
   this->fgColor.b = b;

   if (this->fgBrush != NULL)
   {
      DeleteObject(this->fgBrush);
      this->fgBrush = NULL;
      this->getFGBrush();
   }
   
   this->invalidate();
}

void
Window::setFGColor
(DWORD hexValue)
{
   setFGColor((hexValue >> 24) & 0xFF
              ,(hexValue >> 16) & 0xFF
              ,(hexValue >> 8) & 0xFF
              ,hexValue & 0xFF);
}

void
Window::setFGColor
(Color color)
{
   setFGColor(color.a, color.r, color.g, color.b);
}

Color
Window::getFGColor
(void) const
{
   return this->fgColor;
}

HBRUSH
Window::getFGBrush
(void)
{
   if (this->fgBrush == NULL)
      this->fgBrush = CreateSolidBrush(this->fgColor.colorRef());

   return this->fgBrush;
}

void
Window::setBorderColor
(BYTE a, BYTE r, BYTE g, BYTE b)
{
   this->borderColor.a = a;
   this->borderColor.r = r;
   this->borderColor.g = g;
   this->borderColor.b = b;

   if (this->borderBrush != NULL)
   {
      DeleteObject(this->borderBrush);
      this->borderBrush = NULL;
      this->getBorderBrush();
   }
   
   this->invalidate();
}

void
Window::setBorderColor
(DWORD hexValue)
{
   setBorderColor((hexValue >> 24) & 0xFF
                  ,(hexValue >> 16) & 0xFF
                  ,(hexValue >> 8) & 0xFF
                  ,hexValue & 0xFF);
}

void
Window::setBorderColor
(Color color)
{
   setBorderColor(color.a, color.r, color.g, color.b);
}

Color
Window::getBorderColor
(void) const
{
   return this->borderColor;
}

HBRUSH
Window::getBorderBrush
(void)
{
   if (this->borderBrush == NULL)
      this->borderBrush = CreateSolidBrush(this->borderColor.colorRef());

   return this->borderBrush;
}

void
Window::setBorderSize
(BYTE size)
{
   this->borderSize = size;

   if (this->hasHWND() && !SetWindowPos(this->hwnd
                                        ,0, 0, 0, 0, 0
                                        ,SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE
                                        | SWP_NOACTIVATE | SWP_NOZORDER))
      throw WindowException(L"SetWindowPos failed");
}

BYTE
Window::getBorderSize
(void) const
{
   return this->borderSize;
}

void
Window::preCreate
(void)
{
   return;
}

void
Window::create
(void)
{
   std::list<Window *>::iterator childIter;
   std::set<Window *>::iterator linkIter;

   this->preCreate();
   
   this->registerClass();
   
   Window::LastWindow = this;

   this->hwnd = CreateWindowEx(this->exStyle
                               ,this->className.c_str()
                               ,(this->windowText.empty()) ? NULL : this->windowText.c_str()
                               ,this->style
                               ,this->point.getX(), this->point.getY()
                               ,this->size.cx, this->size.cy
                               ,(this->parent == NULL || !this->hasStyle(WS_CHILD)) ? NULL : this->parent->getHWND()
                               ,this->menu
                               ,GetModuleHandle(NULL)
                               ,NULL);

   if (this->hwnd == NULL)
      throw WindowException(L"CreateWindowEx failed");

   /* be explicitly redundant about this to prevent race conditions */
   Window::MapWindow(this->hwnd, this);

   for (childIter=this->children.begin(); childIter!=this->children.end(); ++childIter)
   {
      Window *child = *childIter;

      if (!child->hasHWND())
         child->create();
   }

   for (linkIter=this->links.begin(); linkIter!=this->links.end(); ++linkIter)
   {
      Window *link = *linkIter;

      if (!link->hasHWND())
         link->create();
   }

   this->postCreate();
}

void
Window::postCreate
(void)
{
   SendMessage(this->hwnd, WM_SETFONT, (WPARAM)this->typeface.getHandle(), (LPARAM)FALSE);
   EnableWindow(this->hwnd, (BOOL)this->enabled);
}

void
Window::registerClass
(void)
{
   WNDCLASSEX classInfo;

   if (this->className.empty())
      throw WindowException(L"class name cannot be empty");

   /* if the class exists already, bail. */
   if (GetClassInfoEx(GetModuleHandle(NULL)
                      ,this->className.c_str()
                      ,&classInfo))
      return;

   memset(&classInfo, 0, sizeof(WNDCLASSEX));
      
   classInfo.cbSize = sizeof(WNDCLASSEX);
   classInfo.style = this->classStyle;
   classInfo.lpfnWndProc = Window::WndProc;
   classInfo.hInstance = GetModuleHandle(NULL);
   classInfo.hIcon = this->icon.getHandle();
   classInfo.hCursor = this->cursor;
   classInfo.hbrBackground = this->getBGBrush();

   if (!this->menuName.empty())
      classInfo.lpszMenuName = this->menuName.c_str();

   classInfo.lpszClassName = this->className.c_str();
   classInfo.hIconSm = this->icon.getHandle();

   if (RegisterClassEx(&classInfo) == 0)
      throw WindowException(L"failed to register window class");
}

void
Window::destroy
(void)
{
   std::list<Window *>::iterator childIter;
   std::set<Window *>::iterator linkIter;
   
   if (this->hasHWND())
   {
      HWND oldHWND;

      /* make a copy of the old hwnd-- the local one gets destroyed when
         DestroyWindow gets called, and we need to unmap the old handle. */
      
      oldHWND = this->hwnd;
      
      DestroyWindow(this->hwnd);

      Window::UnmapWindow(oldHWND);
   }
}

void
Window::invalidate
(void)
{
   RECT rect = {0,0,this->size.cx,this->size.cy};

   if (this->hasHWND())
      InvalidateRect(this->hwnd, &rect, FALSE);
}

void
Window::update
(void)
{
   if (!this->hasHWND())
      return;
   
   if (!UpdateWindow(this->hwnd))
      throw WindowException(L"UpdateWindow failed");
}

void
Window::show
(void)
{
   std::list<Window *>::iterator childIter;
   std::set<Window *>::iterator linkIter;

   if (!this->hasHWND())
      this->create();

   ShowWindow(this->hwnd, SW_SHOW);

   for (childIter=this->children.begin(); childIter!=this->children.end(); ++childIter)
   {
      Window *child = *childIter;

      if (child->hasStyle(WS_VISIBLE) && !child->isVisible())
         child->show();
   }

   for (linkIter=this->links.begin(); linkIter!=this->links.end(); ++linkIter)
   {
      Window *link = *linkIter;

      if (link->hasStyle(WS_VISIBLE) && !link->isVisible())
         link->show();
   }
}

void
Window::focus
(void)
{
   if (this->hasHWND() && SetFocus(this->getHWND()) == NULL)
      throw WindowException(L"SetFocus failed");
}

void
Window::hide
(void)
{
   if (this->hasHWND() && !ShowWindow(this->hwnd, SW_HIDE))
      throw WindowException(L"ShowWindow failed");
}

void
Window::enable
(void)
{
   this->enabled = true;

   if (this->hasHWND() && !EnableWindow(this->hwnd, TRUE))
      throw WindowException(L"EnableWindow failed");
}

void
Window::disable
(void)
{
   this->enabled = false;

   if (this->hasHWND() && !EnableWindow(this->hwnd, FALSE))
      throw WindowException(L"EnableWindow failed");
}

LRESULT
Window::onActivate
(BYTE activeState, HWND activeWindow)
{
   if (activeState == WA_ACTIVE || activeState == WA_CLICKACTIVE)
      this->active = true;
   else
      this->active = false;

   return this->defWndProc(this->hwnd, WM_ACTIVATE, (WPARAM)activeState, (LPARAM)activeWindow);
}

LRESULT
Window::onChar
(DWORD keyValue, DWORD keyFlags)
{
   switch(keyValue)
   {
   case VK_TAB:
      /* return 0 to kill dings */
      return (LRESULT)0;
   }

   return this->defWndProc(this->hwnd, WM_CHAR, (WPARAM)keyValue, (LPARAM)keyFlags);
}

LRESULT
Window::onCommand
(WORD notificationCode, WORD identifier, HWND handle)
{
   return this->defWndProc(this->hwnd, WM_COMMAND, MAKEWPARAM(identifier, notificationCode), (LPARAM)handle);
}

HBRUSH
Window::onCtlColorEdit
(HDC context, HWND control)
{
   Window *window = Window::FindWindow(control);

   /* window is unmanaged when event fires, return default proc */
   if (window == NULL)
      return (HBRUSH)this->defWndProc(this->hwnd
                                      ,WM_CTLCOLOREDIT
                                      ,(WPARAM)context
                                      ,(LPARAM)control);

   SetTextColor(context, window->getFGColor().colorRef());
   SetBkColor(context, window->getBGColor().colorRef());

   if (window->getBGColor().isTransparent())
      return (HBRUSH)GetStockObject(NULL_BRUSH);
   else
      return window->getBGBrush();
}

HBRUSH
Window::onCtlColorStatic
(HDC context, HWND control)
{
   Window *window = Window::FindWindow(control);

   /* window is unmanaged when event fires, return default proc */
   if (window == NULL)
      return (HBRUSH)this->defWndProc(this->hwnd
                                      ,WM_CTLCOLORSTATIC
                                      ,(WPARAM)context
                                      ,(LPARAM)control);

   SetTextColor(context, window->getFGColor().colorRef());
   SetBkColor(context, window->getBGColor().colorRef());
   
   if (window->getBGColor().isTransparent())
      return (HBRUSH)GetStockObject(NULL_BRUSH);
   else
      return window->getBGBrush();
}

HBRUSH
Window::onCtlColorBtn
(HDC context, HWND control)
{
   Window *window = Window::FindWindow(control);

   /* window is unmanaged when event fires, return default proc */
   if (window == NULL)
      return (HBRUSH)this->defWndProc(this->hwnd,
                                      WM_CTLCOLORBTN
                                      ,(WPARAM)context
                                      ,(LPARAM)control);

   SetTextColor(context, window->getFGColor().colorRef());
   SetBkColor(context, window->getBGColor().colorRef());
   
   if (window->getBGColor().isTransparent())
      return (HBRUSH)GetStockObject(NULL_BRUSH);
   else
      return window->getBGBrush();
}

LRESULT
Window::onCreate
(LPCREATESTRUCT creationData)
{
   /* windows created with WS_VISIBLE don't send WM_SHOW, so set this->visible
      if WS_VISIBLE is present */
   this->visible = this->hasStyle(WS_VISIBLE);
   
   return this->defWndProc(this->hwnd, WM_CREATE, NULL, (LPARAM)creationData);
}

LRESULT
Window::onDestroy
(void)
{
   std::list<Window *>::iterator childIter;
   std::set<Window *>::iterator linkIter;
   
   for(childIter=this->children.begin(); childIter!=this->children.end(); ++childIter)
   {
      Window *child = *childIter;

      if (child->hasHWND())
         child->destroy();
   }

   this->defWndProc(this->hwnd, WM_DESTROY, NULL, NULL);

   if (this->bgBrush != NULL)
   {
      DeleteObject(this->bgBrush);
      this->bgBrush = NULL;
   }

   if (this->fgBrush != NULL)
   {
      DeleteObject(this->fgBrush);
      this->fgBrush = NULL;
   }

   if (this->borderBrush != NULL)
   {
      DeleteObject(this->borderBrush);
      this->borderBrush = NULL;
   }

   this->hwnd = NULL;

   for (linkIter=this->links.begin(); linkIter!=this->links.end(); ++linkIter)
   {
      Window *link = *linkIter;

      if (link->hasHWND())
         link->destroy();
   }

   return (LRESULT)0;
}

LRESULT
Window::onEnable
(BOOL enabled)
{
   this->enabled = enabled;

   return this->defWndProc(this->hwnd, WM_ENABLE, (WPARAM)enabled, NULL);
}

LRESULT
Window::onEraseBkgnd
(HDC context)
{
   return this->defWndProc(this->hwnd, WM_ERASEBKGND, (WPARAM)context, (LPARAM)NULL);
}

LRESULT
Window::onKeyDown
(DWORD keyValue, DWORD keyFlags)
{
   switch(keyValue)
   {
   case VK_TAB:
   {
      SHORT shift = GetKeyState(VK_SHIFT);
      bool direction = (shift & 0x8000) > 0; /* backwards if true, forwards otherwise */
      std::list<Window *> children;

      if (!this->hasStyle(WS_TABSTOP))
      {
         /* if this element does not have the tabstop style, look for the first child
            that does */
         children = this->children;

         if (direction)
         {
            for (std::list<Window *>::reverse_iterator childIter=children.rbegin();
                 childIter!=children.rend();
                 childIter++)
            {
               Window *window = *childIter;
               
               if (window->hasStyle(WS_TABSTOP) && window->isEnabled() && window->isVisible())
               {
                  window->focus();
                  break;
               }
            }
         }
         else
         {
            for (std::list<Window *>::iterator childIter=this->children.begin();
                 childIter!=this->children.end();
                 childIter++)
            {
               Window *window = *childIter;

               if (window->hasStyle(WS_TABSTOP) && window->isEnabled() && window->isVisible())
               {
                  window->focus();
                  break;
               }
            }
         }

         return (LRESULT)0;
      }

      if (this->parent == NULL)
         throw WindowException(L"cannot tab on root window");

      children = this->parent->getChildren();

      if (direction)
      {
         std::list<Window *>::reverse_iterator childIter, originIter;
         
         childIter = std::find(children.rbegin(), children.rend(), this);
         
         if (childIter == children.rend())
            throw WindowException(L"could not find window in children");

         originIter = childIter;

         do
         {
            Window *window;

            childIter++;

            if (childIter == children.rend())
               childIter = children.rbegin();

            window = *childIter;

            if (!window->hasStyle(WS_TABSTOP) || !window->isEnabled() || !window->isVisible())
               continue;

            window->focus();
            break;
         } while (childIter != originIter);
      }
      else
      {
         std::list<Window *>::iterator childIter, originIter;

         childIter = std::find(children.begin(), children.end(), this);
         
         if (childIter == children.end())
            throw WindowException(L"could not find window in children");

         originIter = childIter;

         do
         {
            Window *window;

            childIter++;

            if (childIter == children.end())
               childIter = children.begin();

            window = *childIter;

            if (!window->hasStyle(WS_TABSTOP) || !window->isEnabled() || !window->isVisible())
               continue;

            window->focus();
            break;
         } while (childIter != originIter);
      }

      return (LRESULT)0;
   }
   }

   return this->defWndProc(this->hwnd, WM_KEYDOWN, (WPARAM)keyValue, (LPARAM)keyFlags);
}

LRESULT
Window::onKeyUp
(DWORD keyValue, DWORD keyFlags)
{
   return this->defWndProc(this->hwnd, WM_KEYUP, (WPARAM)keyValue, (LPARAM)keyFlags);
}

LRESULT
Window::onKillFocus
(HWND gainedFocus)
{
   this->focused = false;
   
   return this->defWndProc(this->hwnd, WM_KILLFOCUS, (WPARAM)gainedFocus, (LPARAM)NULL);
}

LRESULT
Window::onLButtonDown
(WORD virtualKeys, WORD x, WORD y)
{
   this->captured = true;
   this->capturePoint.setX(x);
   this->capturePoint.setY(y);

   SetCapture(this->hwnd);
   
   return this->defWndProc(this->hwnd, WM_LBUTTONDOWN, (WPARAM)virtualKeys, MAKELPARAM(x, y));
}

LRESULT
Window::onLButtonUp
(WORD virtualKeys, WORD x, WORD y)
{
   this->captured = false;
   this->capturePoint.setX(0);
   this->capturePoint.setY(0);

   ReleaseCapture();
   
   return this->defWndProc(this->hwnd, WM_LBUTTONUP, (WPARAM)virtualKeys, MAKELPARAM(x, y));
}

LRESULT
Window::onMouseLeave
(void)
{
   this->hovering = false;
   
   return this->defWndProc(this->hwnd, WM_MOUSELEAVE, (WPARAM)NULL, (LPARAM)NULL);
}

LRESULT
Window::onMouseMove
(WORD virtualKeys, WORD x, WORD y)
{
   TRACKMOUSEEVENT mouseEvent;

   ZeroMemory(&mouseEvent, sizeof(TRACKMOUSEEVENT));

   mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
   mouseEvent.dwFlags = TME_LEAVE;
   mouseEvent.hwndTrack = this->hwnd;
   mouseEvent.dwHoverTime = HOVER_DEFAULT;
   
   this->hovering = true;

   TrackMouseEvent(&mouseEvent);
   
   return this->defWndProc(this->hwnd, WM_MOUSEMOVE, (WPARAM)virtualKeys, (LPARAM)((y << 16) | x));
}

LRESULT
Window::onMove
(WORD x, WORD y)
{
   this->moving = false;
   
   return this->defWndProc(this->hwnd, WM_MOVE, (WPARAM)NULL, (LPARAM)((y << 16) | x));
}

LRESULT
Window::onNCCalcSize
(BOOL switchValue, LPARAM pointer)
{
   PRECT rect;

   this->defWndProc(this->hwnd, WM_NCCALCSIZE, (WPARAM)switchValue, pointer);

   if (switchValue == TRUE)
   {
      LPNCCALCSIZE_PARAMS params = (LPNCCALCSIZE_PARAMS)pointer;

      rect = &params->rgrc[0];
   }
   else
      rect = (PRECT)pointer;

   InflateRect(rect, -this->borderSize, -this->borderSize);
   
   return 0;
}

LRESULT
Window::onNCPaint
(HRGN paintRegion)
{
   HDC paintDC;
   RECT rect = {0, 0, this->size.cx, this->size.cy};
   HPEN pen;
   
   this->defWndProc(this->hwnd, WM_NCPAINT, (WPARAM)paintRegion, NULL);

   if (this->borderSize == 0)
      return 0;

   paintDC = GetWindowDC(this->hwnd);
   pen = CreatePen(PS_SOLID, this->borderSize * 2, this->borderColor.colorRef());

   SelectObject(paintDC, pen);
   SelectObject(paintDC, GetStockObject(NULL_BRUSH));
   Rectangle(paintDC, rect.left, rect.top, rect.right, rect.bottom);

   ReleaseDC(this->hwnd, paintDC);
   DeleteObject(pen);

   return 0;
}

LRESULT
Window::onNotify
(UINT identifier, NMHDR *notifyHeader)
{
   return this->defWndProc(this->hwnd, WM_NOTIFY, (WPARAM)identifier, (LPARAM)notifyHeader);
}

LRESULT
Window::onPaint
(void)
{
   return this->defWndProc(this->hwnd, WM_PAINT, NULL, NULL);
}

LRESULT
Window::onRButtonDown
(WORD virtualKeys, WORD x, WORD y)
{
   return this->defWndProc(this->hwnd, WM_RBUTTONDOWN, (WPARAM)virtualKeys, (LPARAM)((y << 16) | x));
}

LRESULT
Window::onRButtonUp
(WORD virtualKeys, WORD x, WORD y)
{
   return this->defWndProc(this->hwnd, WM_RBUTTONUP, (WPARAM)virtualKeys, (LPARAM)((y << 16) | x));
}

LRESULT
Window::onSetFocus
(HWND lostFocus)
{
   this->focused = true;
   
   return this->defWndProc(this->hwnd, WM_SETFOCUS, (WPARAM)lostFocus, (LPARAM)NULL);
}

LRESULT
Window::onShowWindow
(BOOL shown, WORD status)
{
   this->visible = shown;
   
   return this->defWndProc(this->hwnd, WM_SHOWWINDOW, (WPARAM)shown, (LPARAM)status);
}

LRESULT
Window::onWindowPosChanging
(LPWINDOWPOS windowPos)
{
   if ((windowPos->flags & SWP_NOMOVE) == 0)
      this->moving = true;
   
   return this->defWndProc(this->hwnd, WM_WINDOWPOSCHANGING, (WPARAM)NULL, (LPARAM)windowPos);
}

LRESULT
Window::onWindowPosChanged
(LPWINDOWPOS windowPos)
{
   if ((windowPos->flags & SWP_NOSIZE) == 0)
   {
      this->size.cx = windowPos->cx;
      this->size.cy = windowPos->cy;
   }

   if ((windowPos->flags & SWP_NOMOVE) == 0)
   {
      std::set<Window *>::iterator iter;
      long deltaX, deltaY;
      
      deltaX = windowPos->x - this->point.getX();
      deltaY = windowPos->y - this->point.getY();

      this->point.setX(windowPos->x);
      this->point.setY(windowPos->y);

      for (iter=this->links.begin(); iter!=this->links.end(); ++iter)
      {
         Window *link = *iter;
         
         if (!link->isMoving())
            link->move(deltaX, deltaY);
      }

      this->moving = false;
   }

   return this->defWndProc(this->hwnd, WM_WINDOWPOSCHANGED, (WPARAM)NULL, (LPARAM)windowPos);
}
