#pragma once

#include <windows.h>

#include "sostav/windows/window.hpp"
#include "sostav/drawing/color.hpp"

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
         HDC screenDC;
         Drawing::Color transparency;
         DWORD updateFlag;
         
      public:
         LayeredWindow(Window *parent, std::wstring className=L"SvLayeredWindow");
         LayeredWindow(LayeredWindow &window);
         LayeredWindow();
         ~LayeredWindow();

         void setTransparency(Drawing::Color color);
         Drawing::Color getTransparency(void) const;

         void setAlpha(BYTE alpha);
         BYTE getAlpha(void) const;

         void setUpdateFlag(DWORD flag);
         DWORD getUpdateFlag(void) const;

         virtual void show(void);
         virtual void update(void);
         virtual void preCreate(void);
         virtual void layeredUpdate(HDC context);
         
      protected:
         virtual void beginPaint(void);
         virtual void endPaint(void);
      };
   }
}
