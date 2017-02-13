#pragma once

#include <windows.h>

#include <sostav/exception.hpp>
#include <sostav/drawing/image.hpp>
#include <sostav/windows/imagewindow.hpp>

namespace Sostav
{
   namespace Windows
   {
      class ImageDialogWindowException : public Exception
      {
      public:
         ImageDialogWindowException(const WCHAR *what);
      };

      class ImageDialogWindowPane;

      class ImageDialogWindowFrame : public LayeredImageWindow
      {
      protected:
         ImageDialogWindowPane *windowPane;
         RECT paneCrop;

      public:
         ImageDialogWindowFrame();

         void setWindowPane(ImageDialogWindowPane *windowPane);
         ImageDialogWindowPane *getWindowPane(void) const;

         void setPaneCrop(DWORD x, DWORD y, DWORD cx, DWORD cy);
         void setPaneCrop(POINT point, SIZE size);
         void setPaneCrop(RECT rect);
         RECT getPaneCrop(void) const;

         void cropFrame(void);
         virtual void setImage(Drawing::Image image);

      protected:
         virtual void preCreate(void);
      };

      class ImageDialogWindowPane : public ImageWindow
      {
      public:
         ImageDialogWindowPane();

      protected:
         virtual void postCreate(void);
      };
   }
}
