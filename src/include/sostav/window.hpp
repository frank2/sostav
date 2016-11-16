#pragma once

#include <windows.h>

#include <iostream>
#include <map>
#include <set>
#include <string>

#include "sostav/exception.hpp"
#include "sostav/types.hpp"

namespace Sostav
{
   typedef Exception WindowException;

   class Window
   {
   protected:
      HWND parentHWND;
      Window *parentWindow;
      HWND hwnd;

      POINT position;
      SIZE size;

      HDC paintContext;
      PAINTSTRUCT paintStruct;
   
      HICON icon;
      HCURSOR cursor;
      HMENU menu;
      
      DWORD style, exStyle, classStyle;

      Color bgColor;

      std::wstring className, menuName, windowText;
      std::set<Window *> children;

   public:
      static std::map<HWND, Window *> WindowPool;
      static Window *LastWindow;
      
      Window(HWND parent, std::wstring className);
      Window(Window *parent, std::wstring className);
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

      void setParent(HWND newParent);
      void setParent(Window *newParent);
      Window *getParentWindow(void);
      HWND getParentHWND(void);

      virtual void setHWND(HWND window);
      HWND getHWND(void);

      void setPosition(long x, long y);
      void setPosition(POINT position);
      POINT getPosition(void);

      void setSize(long cx, long cy);
      void setSize(SIZE size);
      SIZE getSize(void);

      void setRect(long left, long top, long right, long bottom);
      void setRect(RECT rect);
      RECT getRect(void);

      void setIcon(HICON icon);
      HICON getIcon(void);

      void setCursor(HCURSOR cursor);
      HCURSOR getCursor(void);

      void setMenu(HMENU menu);
      HMENU getMenu(void);

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
      std::wstring getClassName(void);

      void setMenuName(std::wstring menuName);
      std::wstring getMenuName(void);

      void setWindowText(std::wstring windowText);
      std::wstring getWindowText(void);

      void setBGColor(BYTE a, BYTE r, BYTE g, BYTE b);
      void setBGColor(DWORD hexValue);
      void setBGColor(Color color);
      Color getBGColor(void);

      bool hasChild(Window *child);
      void addChild(Window *child);
      void removeChild(Window *child);

      virtual void create(void);
      virtual void destroy(void);
      virtual void invalidate(void);
      virtual void update(void);
      virtual void show(void);
      virtual void hide(void);

   protected:
      virtual void initialize(HWND parent, std::wstring className);
   };
}
