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
   };
}

#define SVRGB(s) RGB((s).r, (s).g, (s).b)
#define SVCOLORREF(s) ((s).hexValue & 0xFFFFFF)
