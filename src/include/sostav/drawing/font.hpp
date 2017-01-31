#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"

namespace Sostav
{
   namespace Drawing
   {
      class FontException : public Exception
      {
      public:
         FontException(const char *what);
      };

      class Font
      {
      protected:
         HFONT fontHandle;
         LOGFONT fontData;

      public:
         Font(std::wstring face, LONG points);
         Font(std::wstring face, LONG height, LONG width);
         Font(const Font &font);
         Font();
         ~Font();
         
         void setLogFont(LOGFONT logFont);
         LOGFONT getLogFont(void) const;

         void setHeight(LONG height);
         LONG getHeight(void) const;

         void setWidth(LONG width);
         LONG getWidth(void) const;

         void setPointSize(LONG points);

         void setFace(std::wstring face);
         std::wstring getFace(void) const;

         HFONT getHandle(void);
      };
   }
}
