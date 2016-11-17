#pragma once

#include <windows.h>

namespace Sostav
{
   class Color
   {
   public:
      union
      {
         struct
         {
            BYTE b,g,r,a;
         };
         DWORD hexValue;
      };

      static BYTE Alpha_Transparent = 0;
      static BYTE Alpha_Opaque = 0xFF;
      
      Color(BYTE a, BYTE r, BYTE g, BYTE b);
      Color(BYTE r, BYTE g, BYTE b);
      Color(DWORD hexValue);
      Color(Color color);
      Color();

      static Color FromColorRef(COLORREF color);
      static Color Transparent(void);

      COLORREF colorRef(void);
      bool isOpaque(void);
      bool isTransparent(void);
      bool isTranslucent(void);
   };
}

#define SVRGB(s) RGB((s).r, (s).g, (s).b)
#define SVCOLORREF(s) ((s).hexValue & 0xFFFFFF)
