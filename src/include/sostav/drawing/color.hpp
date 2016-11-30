#pragma once

#include <windows.h>
#include <math.h>

namespace Sostav
{
   namespace Drawing
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
      
         Color(BYTE a, BYTE r, BYTE g, BYTE b);
         Color(BYTE r, BYTE g, BYTE b);
         Color(DWORD hexValue);
         Color(const Color &color);
         Color();
         
         const static BYTE Alpha_Transparent = 0;
         const static BYTE Alpha_Opaque = 0xFF;

         static Color Transparent(void);
         static Color FromColorRef(COLORREF color);

         COLORREF colorRef(void) const;
         bool isOpaque(void) const;
         bool isTransparent(void) const;
         bool isTranslucent(void) const;
         Color blend(Color color, double percentage);
      };
   }
}

#define SVRGB(s) RGB((s).r, (s).g, (s).b)
#define SVCOLORREF(s) ((s).hexValue & 0xFFFFFF)
