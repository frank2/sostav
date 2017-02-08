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
   this->enabled = true;
   
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
   this->enabled = true;

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
   this->enabled = true;
   
   this->style = 0;
   this->exStyle = 0;
   this->classStyle = 0;
   this->size.cx = this->size.cy = 0;

   this->setDefaultColors();
   this->cursor = NULL;
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

LRESULT CALLBACK
Window::windowProc
(UINT msg, WPARAM wParam, LPARAM lParam)
{
   /* handle some window messages */
   switch(msg)
   {
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

   case WM_NCCALCSIZE:
      return this->onNCCalcSize((BOOL)wParam, lParam);
      
   case WM_NCPAINT:
      return this->onNCPaint((HRGN)wParam);

   case WM_PAINT:
      this->onPaint();
      return (LRESULT)0;

   case WM_SHOWWINDOW:
      return this->onShowWindow((BOOL)wParam, (WORD)lParam);
      
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

bool
Window::hasHWND
(void) const
{
   return this->hwnd != NULL;
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

   if (this->parent != NULL)
      SetParent(hwnd, this->parent->getHWND());
}

HWND
Window::getHWND
(void) const
{
   return this->hwnd;
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
   if (this->parent != NULL)
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
   if (this->parent == NULL)
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
   std::list<Window *>::iterator iter;

   this->preCreate();
   
   this->registerClass();
   
   Window::LastWindow = this;

   this->hwnd = CreateWindowEx(this->exStyle
                               ,this->className.c_str()
                               ,(this->windowText.empty()) ? NULL : this->windowText.c_str()
                               ,this->style
                               ,this->point.getX(), this->point.getY()
                               ,this->size.cx, this->size.cy
                               ,(this->parent == NULL) ? NULL : this->parent->getHWND()
                               ,this->menu
                               ,GetModuleHandle(NULL)
                               ,NULL);

   if (this->hwnd == NULL)
      throw WindowException(L"CreateWindowEx failed");

   /* be explicitly redundant about this to prevent race conditions */
   Window::MapWindow(this->hwnd, this);

   for (iter=this->children.begin(); iter!=this->children.end(); ++iter)
   {
      Window *child = *iter;

      if (!child->hasHWND())
         child->create();
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
   std::list<Window *>::iterator iter;

   for(iter=this->children.begin(); iter!=this->children.end(); ++iter)
   {
      Window *child = *iter;
      child->destroy();
   }
   
   if (this->hasHWND())
      DestroyWindow(this->hwnd);
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
   std::list<Window *>::iterator iter;

   if (!this->hasHWND())
      this->create();

   ShowWindow(this->hwnd, SW_SHOW);

   for (iter=this->children.begin(); iter!=this->children.end(); ++iter)
   {
      Window *child = *iter;

      child->show();
   }
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

HBRUSH
Window::onCtlColorEdit
(HDC context, HWND control)
{
   Window *window = Window::FindWindow(control);

   if (window == NULL)
      throw WindowException(L"FindWindow failed");

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

   if (window == NULL)
      throw WindowException(L"FindWindow failed");

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

   if (window == NULL)
      throw WindowException(L"FindWindow failed");

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
   return this->defWndProc(this->hwnd, WM_CREATE, NULL, (LPARAM)creationData);
}

LRESULT
Window::onDestroy
(void)
{
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
   
   Window::UnmapWindow(this->hwnd);

   this->hwnd = NULL;

   return 0;
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
   return this->defWndProc(this->hwnd, WM_KEYDOWN, (WPARAM)keyValue, (LPARAM)keyFlags);
}

LRESULT
Window::onKeyUp
(DWORD keyValue, DWORD keyFlags)
{
   return this->defWndProc(this->hwnd, WM_KEYUP, (WPARAM)keyValue, (LPARAM)keyFlags);
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

void
Window::onPaint
(void)
{
   this->defWndProc(this->hwnd, WM_PAINT, NULL, NULL);
}

LRESULT
Window::onShowWindow
(BOOL shown, WORD status)
{
   return this->defWndProc(this->hwnd, WM_SHOWWINDOW, (WPARAM)shown, (LPARAM)status);
}
