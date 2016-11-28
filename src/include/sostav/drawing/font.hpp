#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"

namespace Sostav
{
   namespace Drawing
   {
      typedef Exception FontException;

      class Font
      {
      protected:
         LOGFONT fontData;

      public:
         Font(std::wstring face, LONG points);
         Font(std::wstring face, LONG height, LONG width);
         Font(Font &font);
         Font();
         ~Font();
         
         void setLogFont(LOGFONT logFont);
         LOGFONT getLogFont(void);

         void setHeight(LONG height);
         LONG getHeight(void);

         void setWidth(LONG width);
         LONG getWidth(void);

         void setPointSize(LONG points);

         void setFace(std::wstring face);
         std::wstring getFace(void);

         HFONT getHandle(void);

      protected:
         virtual void initialize(void);
      };
   }
}
