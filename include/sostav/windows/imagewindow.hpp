#pragma once

#include <windows.h>

#include <string>

#include <sostav/exception.hpp>
#include <sostav/drawing/image.hpp>
#include <sostav/windows/layeredwindow.hpp>
#include <sostav/windows/paintedwindow.hpp>

namespace Sostav
{
   namespace Windows
   {
      class ImageWindowException : public Exception
      {
      public:
         ImageWindowException(const WCHAR *what);
      };

      class ImageWindow : public PaintedWindow
      {
      protected:
         Drawing::Image image;
         
      public:
         ImageWindow(Window *parent
                     ,std::wstring className=L"SvImageWindow"
                     ,Drawing::Image image=Drawing::Image());
         ImageWindow(ImageWindow &window);
         ImageWindow();

         virtual void setImage(Drawing::Image image);
         Drawing::Image getImage(void) const;

         virtual void paint(HDC context);
         virtual void drawImage(HDC context);
      };

      class LayeredImageWindow : public LayeredWindow
      {
      protected:
         Drawing::Image image;
         
      public:
         LayeredImageWindow(Window *parent
                            ,std::wstring className=L"SvLayeredImageWindow"
                            ,Drawing::Image image=Drawing::Image());
         LayeredImageWindow(LayeredImageWindow &window);
         LayeredImageWindow();

         virtual void setImage(Drawing::Image image);
         Drawing::Image getImage(void) const;

         virtual void paint(HDC context);
         virtual void drawImage(HDC context);
      };
   }
}
