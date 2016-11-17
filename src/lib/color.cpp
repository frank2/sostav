#include "sostav/color.hpp"

using namespace Sostav;

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
(Color color)
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
(void)
{
   return RGB(this->r, this->g, this->b);
}

bool
Color::isOpaque
(void)
{
   return this->a == Color::Alpha_Opaque;
}

bool
Color::isTransparent
(void)
{
   return this->a == Color::Alpha_Transparent;
}

bool
Color::isTranslucent
(void)
{
   return !this->isOpaque() && !this->isTransparent();
}
