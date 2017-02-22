#pragma once

#include <windows.h>
#include <windowsx.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>

#include "sostav/exception.hpp"
#include "sostav/drawing/color.hpp"
#include "sostav/drawing/font.hpp"
#include "sostav/drawing/icon.hpp"
#include "sostav/drawing/point.hpp"

namespace Sostav
{
   namespace Windows
   {
      class WindowException : public Exception
      {
      public:
         WindowException(const WCHAR *what);
      };

      class Window
      {
      protected:
         Window *parent;
         HWND hwnd;
         WNDPROC defWndProc;

         bool active;
         bool captured;
         bool enabled;
         bool focused;
         bool hovering;
         bool moving;
         bool visible;

         Drawing::AbsolutePoint point;
         Drawing::AbsolutePoint capturePoint;
         SIZE size;
   
         Drawing::Icon icon;
         HCURSOR cursor;
         HMENU menu;
      
         DWORD style, exStyle, classStyle;

         Drawing::Font typeface;
         Drawing::Color bgColor, fgColor, borderColor;
         HBRUSH bgBrush, fgBrush, borderBrush;
         BYTE borderSize;

         std::wstring className, menuName, windowText;
         std::list<Window *> children;
         std::set<Window *> links;

      public:
         static std::map<HWND, Window *> WindowPool;
         static Window *LastWindow;
      
         Window(Window *parent, std::wstring className=L"SvWindow");
         Window(Window &window);
         Window();
         ~Window();

         static void MessageLoop(void);
         static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
         static void MapWindow(HWND hwnd, Window *window);
         static void UnmapWindow(HWND hwnd);
         static Window *FindWindow(HWND hwnd);

         virtual LRESULT windowProc(UINT msg
                                    ,WPARAM wParam
                                    ,LPARAM lParam);

         bool hasChild(Window *child) const;
         void addChild(Window *child);
         void removeChild(Window *child);
         std::list<Window *> getChildren(void) const;
         
         void setParent(Window *newParent);
         Window *getParent(void) const;

         bool hasLink(Window *link) const;
         void addLink(Window *link);
         void removeLink(Window *link);

         bool isActive(void) const;
         bool isCaptured(void) const;
         bool isEnabled(void) const;
         bool hasFocus(void) const;
         bool isHovering(void) const;
         bool isMoving(void) const;
         bool isVisible(void);

         virtual void setHWND(HWND window);
         HWND getHWND(void) const;
         bool hasHWND(void) const;

         LRESULT sendMessage(UINT msg, WPARAM wParam, LPARAM lParam);

         void setDefWndProc(WNDPROC wndProc);
         WNDPROC getDefWndProc(void) const;

         void setPosition(long x, long y);
         void setPosition(POINT position);
         void setPosition(Drawing::AbsolutePoint point);
         Drawing::AbsolutePoint getPosition(void) const;

         void setRelativePosition(double x, double y);
         void setRelativePosition(Math::Point point);
         Drawing::RelativePoint getRelativePosition(void) const;

         void move(long x, long y);

         void setTopWindow(void);
         void setTopmostWindow(void);

         void insertAfter(HWND hwnd);
         void insertAfter(Window *window);
         void insertBefore(HWND hwnd);
         void insertBefore(Window *window);

         void center(void);
         void centerX(void);
         void centerY(void);

         void setSize(long cx, long cy);
         void setSize(SIZE size);
         SIZE getSize(void) const;
         SIZE getParentSize(void) const;
         
         void setRect(long left, long top, long right, long bottom);
         void setRect(RECT rect);
         RECT getRect(void) const;

         virtual void setIcon(Drawing::Icon icon);
         Drawing::Icon getIcon(void) const;

         void setCursor(HCURSOR cursor);
         HCURSOR getCursor(void) const;

         void setMenu(HMENU menu);
         HMENU getMenu(void) const;

         void setStyle(DWORD style);
         DWORD getStyle(void);
         void addStyle(DWORD style);
         void removeStyle(DWORD style);
         bool hasStyle(DWORD style);

         void setExStyle(DWORD exStyle);
         DWORD getExStyle(void);
         void addExStyle(DWORD exStyle);
         void removeExStyle(DWORD exStyle);
         bool hasExStyle(DWORD exStyle);

         void setClassStyle(DWORD classStyle);
         DWORD getClassStyle(void);
         void addClassStyle(DWORD classStyle);
         void removeClassStyle(DWORD classStyle);
         bool hasClassStyle(DWORD classStyle);

         void setClassName(std::wstring className);
         std::wstring getClassName(void) const;
         
         void setMenuName(std::wstring menuName);
         std::wstring getMenuName(void) const;

         void setWindowText(std::wstring windowText);
         std::wstring getWindowText(void);

         void setTypeface(Drawing::Font typeface);
         Drawing::Font getTypeface(void) const;

         void setDefaultColors(void);

         void setBGColor(BYTE a, BYTE r, BYTE g, BYTE b);
         void setBGColor(DWORD hexValue);
         void setBGColor(Drawing::Color color);
         Drawing::Color getBGColor(void) const;
         HBRUSH getBGBrush(void);

         void setFGColor(BYTE a, BYTE r, BYTE g, BYTE b);
         void setFGColor(DWORD hexValue);
         void setFGColor(Drawing::Color color);
         Drawing::Color getFGColor(void) const;
         HBRUSH getFGBrush(void);

         void setBorderColor(BYTE a, BYTE r, BYTE g, BYTE b);
         void setBorderColor(DWORD hexValue);
         void setBorderColor(Drawing::Color color);
         Drawing::Color getBorderColor(void) const;
         HBRUSH getBorderBrush(void);
         
         void setBorderSize(BYTE size);
         BYTE getBorderSize(void) const;

         virtual void create(void);

         virtual void registerClass(void);
         virtual void destroy(void);
         virtual void invalidate(void);
         virtual void update(void);
         virtual void show(void);
         virtual void focus(void);
         virtual void hide(void);
         virtual void enable(void);
         virtual void disable(void);

      protected:
         virtual void preCreate(void);
         virtual void postCreate(void);

         virtual LRESULT onActivate(BYTE activeState, HWND activeWindow);
         virtual HBRUSH onCtlColorEdit(HDC context, HWND control);
         virtual HBRUSH onCtlColorStatic(HDC context, HWND control);
         virtual HBRUSH onCtlColorBtn(HDC context, HWND control);
         virtual LRESULT onCreate(LPCREATESTRUCT creationData);
         virtual LRESULT onDestroy(void);
         virtual LRESULT onEnable(BOOL enabled);
         virtual LRESULT onEraseBkgnd(HDC context);
         virtual LRESULT onKeyDown(DWORD keyValue, DWORD keyFlags);
         virtual LRESULT onKeyUp(DWORD keyValue, DWORD keyFlags);
         virtual LRESULT onKillFocus(HWND gainedFocus);
         virtual LRESULT onLButtonDown(WORD virtualKeys, WORD x, WORD y);
         virtual LRESULT onLButtonUp(WORD virtualKeys, WORD x, WORD y);
         virtual LRESULT onMouseLeave(void);
         virtual LRESULT onMouseMove(WORD virtualKeys, WORD x, WORD y);
         virtual LRESULT onMove(WORD x, WORD y);
         virtual LRESULT onNCCalcSize(BOOL switchValue, LPARAM pointer);
         virtual LRESULT onNCPaint(HRGN paintRegion);
         virtual LRESULT onPaint(void);
         virtual LRESULT onRButtonDown(WORD virtualKeys, WORD x, WORD y);
         virtual LRESULT onRButtonUp(WORD virtualKeys, WORD x, WORD y);
         virtual LRESULT onSetFocus(HWND lostFocus);
         virtual LRESULT onShowWindow(BOOL shown, WORD status);
         virtual LRESULT onWindowPosChanging(LPWINDOWPOS windowPos);
         virtual LRESULT onWindowPosChanged(LPWINDOWPOS windowPos);
      };
   }
}
