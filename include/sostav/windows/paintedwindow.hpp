#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/window.hpp"

namespace Sostav
{
   namespace Windows
   {
      class PaintedWindowException : public Exception
      {
      public:
         PaintedWindowException(const WCHAR *what);
      };

      class PaintedWindow : public Window
      {
      protected:
         HDC paintContext;
         PAINTSTRUCT paintStruct;

      public:
         PaintedWindow(Window *window, std::wstring className=L"SvPaintedWindow");
         PaintedWindow(PaintedWindow &window);
         PaintedWindow();

         virtual void paint(HDC context);

      protected:
         virtual void beginPaint(void);
         virtual void endPaint(void);

         virtual LRESULT onPaint(void);
      };
   }
}
