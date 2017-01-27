#pragma once

#include <windows.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include "sostav/exception.hpp"
#include "sostav/drawing/color.hpp"
#include "sostav/drawing/font.hpp"
#include "sostav/drawing/point.hpp"

namespace Sostav
{
   namespace Windows
   {
      class WindowException : public Exception
      {
      public:
         WindowException(const char *what);
      };

      typedef LRESULT (CALLBACK *WindowProcedure)(HWND, UINT, WPARAM, LPARAM);

      class Window
      {
      protected:
         Window *parent;
         HWND hwnd;
         WindowProcedure defWndProc;

         Drawing::AbsolutePoint point;
         SIZE size;
      
         HDC paintContext;
         PAINTSTRUCT paintStruct;
   
         HICON icon;
         HCURSOR cursor;
         HMENU menu;
      
         DWORD style, exStyle, classStyle;

         Drawing::Font typeface;
         Drawing::Color bgColor, fgColor;
         HBRUSH bgBrush, fgBrush;

         std::wstring className, menuName, windowText;
         std::list<Window *> children;

      public:
         static std::map<HWND, Window *> WindowPool;
         static Window *LastWindow;
      
         Window(Window *parent, std::wstring className=L"SvWindow");
         Window(Window &window);
         Window();
         ~Window();

         static void MessageLoop(void);
         static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
         static void MapWindow(HWND hwnd, Window *window);
         static void UnmapWindow(HWND hwnd);
         static Window *FindWindow(HWND hwnd);

         virtual LRESULT CALLBACK windowProc(UINT msg
                                             ,WPARAM wParam
                                             ,LPARAM lParam);

         bool hasChild(Window *child) const;
         void addChild(Window *child);
         void removeChild(Window *child);

         bool hasHWND(void) const;
         
         void setParent(Window *newParent);
         Window *getParent(void) const;

         virtual void setHWND(HWND window);
         HWND getHWND(void) const;

         void setDefWndProc(WindowProcedure wndProc);
         WindowProcedure getDefWndProc(void) const;

         void setPosition(long x, long y);
         void setPosition(POINT position);
         void setPosition(Drawing::AbsolutePoint point);
         Drawing::AbsolutePoint getPosition(void) const;

         void setRelativePosition(double x, double y);
         void setRelativePosition(Math::Point point);
         Drawing::RelativePoint getRelativePosition(void) const;

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

         void setIcon(HICON icon);
         HICON getIcon(void) const;

         void setCursor(HCURSOR cursor);
         HCURSOR getCursor(void) const;

         void setMenu(HMENU menu);
         HMENU getMenu(void) const;

         void setStyle(DWORD style);
         DWORD getStyle(void);
         void addStyle(DWORD style);
         void removeStyle(DWORD style);

         void setExStyle(DWORD exStyle);
         DWORD getExStyle(void);
         void addExStyle(DWORD exStyle);
         void removeExStyle(DWORD exStyle);

         void setClassStyle(DWORD classStyle);
         DWORD getClassStyle(void);
         void addClassStyle(DWORD classStyle);
         void removeClassStyle(DWORD classStyle);

         void setClassName(std::wstring className);
         std::wstring getClassName(void) const;
         
         void setMenuName(std::wstring menuName);
         std::wstring getMenuName(void) const;

         void setWindowText(std::wstring windowText);
         std::wstring getWindowText(void);

         void setTypeface(Drawing::Font typeface);
         Drawing::Font getTypeface(void) const;

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

         virtual void preCreate(void);
         virtual void create(void);
         virtual void postCreate(void);

         virtual void registerClass(void);
         virtual void destroy(void);
         virtual void invalidate(void);
         virtual void update(void);
         virtual void show(void);
         virtual void hide(void);
         virtual void paint(HDC context);

      protected:
         virtual void beginPaint(void);
         virtual void endPaint(void);

         virtual void initialize(Window *window, std::wstring className);

         virtual HBRUSH onCtlColorEdit(HDC context, HWND control);
         virtual HBRUSH onCtlColorStatic(HDC context, HWND control);
         virtual void onDestroy(void);
         virtual LRESULT onEraseBkgnd(HDC context);
         virtual void onPaint(void);
      };
   }
}
