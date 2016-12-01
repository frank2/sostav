#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/layeredwindow.hpp"
#include "sostav/windows/window.hpp"

namespace Sostav
{
   namespace Windows
   {
      class ImageWindowException : public Exception
      {
      public:
         ImageWindowException(const char *what);
      };

      class ImageWindow : public Window
      {
      protected:
         HBITMAP image;
         
      public:
         ImageWindow(HWND parent, std::wstring className=L"SvImageWindow", HBITMAP image=NULL);
         ImageWindow(Window *window, std::wstring className=L"SvImageWindow", HBITMAP image=NULL);
         ImageWindow();

         HBITMAP getImage(void) const;
         void setImage(HBITMAP image);

         virtual void paint(HDC context);
         virtual void drawImage(HDC context);
      };

      class LayeredImageWindow : public LayeredWindow
      {
      protected:
         HBITMAP image;
         
      public:
         LayeredImageWindow(HWND parent, std::wstring className=L"SvLayeredImageWindow", HBITMAP image=NULL);
         LayeredImageWindow(Window *window, std::wstring className=L"SvLayeredImageWindow", HBITMAP image=NULL);
         LayeredImageWindow();

         HBITMAP getImage(void) const;
         void setImage(HBITMAP image);

         virtual void paint(HDC context);
         virtual void drawImage(HDC context);
      };
   }
}
