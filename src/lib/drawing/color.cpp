#include "sostav/drawing/color.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

Color::Color
(BYTE a, BYTE r, BYTE g, BYTE b)
{
   this->a = a;
   this->r = r;
   this->g = g;
   this->b = b;
}

Color::Color
(BYTE r, BYTE g, BYTE b)
{
   this->a = Color::Alpha_Opaque;
   this->r = r;
   this->g = g;
   this->b = b;
}

Color::Color
(DWORD hexValue)
{
   this->hexValue = hexValue;
}

Color::Color
(const Color &color)
{
   this->hexValue = color.hexValue;
}

Color::Color
(void)
{
   this->hexValue = 0;
}

Color
Color::FromColorRef
(COLORREF color)
{
   return Color((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

Color
Color::Transparent
(void)
{
   return Color();
}

COLORREF
Color::colorRef
(void) const
{
   return RGB(this->r, this->g, this->b);
}

bool
Color::isOpaque
(void) const
{
   return this->a == Color::Alpha_Opaque;
}

bool
Color::isTransparent
(void) const
{
   return this->a == Color::Alpha_Transparent;
}

bool
Color::isTranslucent
(void) const
{
   return !this->isOpaque() && !this->isTransparent();
}

Color
Color::blend
(Color color, double percentage)
{
   LONG aD, rD, gD, bD;
   Color newColor;

   aD = color.a - this->a;
   rD = color.r - this->r;
   gD = color.g - this->g;
   bD = color.b - this->b;

   newColor.a = (BYTE)(this->a + floor(aD * percentage));
   newColor.r = (BYTE)(this->r + floor(rD * percentage));
   newColor.g = (BYTE)(this->g + floor(gD * percentage));
   newColor.b = (BYTE)(this->b + floor(bD * percentage));

   return newColor;
}
