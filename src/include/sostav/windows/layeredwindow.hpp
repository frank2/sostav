#pragma once

#include <windows.h>

#include "sostav/windows/window.hpp"

namespace Sostav
{
   namespace Windows
   {
      class LayeredWindowException : public Exception
      {
      public:
         LayeredWindowException(const char *what);
      };

      class LayeredWindow : public Window
      {
      protected:
         Drawing::Color transparency;
         
      public:
         LayeredWindow(HWND parent, std::wstring className=L"SvLayeredWindow");
         LayeredWindow(Window *parent, std::wstring className=L"SvLayeredWindow");
         LayeredWindow();

         void setTransparency(Drawing::Color color);
         Drawing::Color getTransparency(void) const;

         void setAlpha(BYTE alpha);
         BYTE getAlpha(void) const;

         virtual void preCreate(void);
         virtual void updateLayered(HDC context);

      protected:
         virtual void initialize(HWND parent, std::wstring className);
      };
   }
}
