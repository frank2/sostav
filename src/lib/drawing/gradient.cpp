#include "sostav/drawing/gradient.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Math;

GradientException::GradientException
(const char *what)
   : Exception(what)
{
}

Gradient::Gradient
(std::map<RelativePoint, Color> modes, Color base)
{
   this->setBase(base);
   this->setModes(modes);
}

Gradient::Gradient
(Gradient &gradient)
{
   this->setBase(gradient.getBase());
   this->setModes(gradient.getModes());
}

Gradient::Gradient
(void)
{
   this->setBase(Color::Transparent());
}

void
Gradient::setModes
(std::map<RelativePoint, Color> modes)
{
   this->modes = modes;
}

std::map<RelativePoint, Color>
Gradient::getModes
(void) const
{
   return this->modes;
}

void
Gradient::setBase
(Color base)
{
   this->base = base;
}

Color
Gradient::getBase
(void) const
{
   return this->base;
}

void
Gradient::addColor
(RelativePoint key, Color color)
{
   this->modes[key] = color;
}

void
Gradient::removeColor
(RelativePoint key)
{
   this->modes.erase(key);
}

void
Gradient::moveColor
(RelativePoint key, RelativePoint newKey)
{
   std::map<RelativePoint, Color>::iterator iter;
   Color color;

   iter = this->modes.find(key);

   if (iter == this->modes.end())
      throw GradientException("key not found in modes");

   color = this->modes[key];
   this->modes.erase(key);
   this->modes[newKey] = color;
}

Color
Gradient::formula
(double x, double y) const
{
   Color result = this->base;
   Line diagonal(0, 1.0, 1.0, 0);
   double diagonalLength = diagonal.getLength() / 2.0;
   std::map<RelativePoint, Color>::iterator iter;
   WCHAR debugString[1024];
   memset(debugString, 0, sizeof(debugString));

   for (iter=this->modes.begin(); iter!=this->modes.end(); ++iter)
   {
      RelativePoint point = iter->first;
      Color color = iter->second;
      Color newColor;
      Line modeLine(x, y, point.getX(), point.getY());
      double lineLength = modeLine.getLength();
      double ratio;

      if (lineLength >= diagonalLength)
         ratio = 0.0;
      else
         ratio = (diagonalLength - lineLength) / diagonalLength;

      newColor = result.blend(color, ratio);

      /*
      swprintf_s(debugString
                 ,1024
                 ,L"{%0.4f,%0.4f}->{%0.4f,%0.4f}->%0.4f base:[R:%d,G:%d,B:%d,A:%d] target:[R:%d,G:%d,B:%d,A:%d] ratio:%0.4f new:[R:%d,G:%d,B:%d,A:%d]\n"
                 ,x, y, point.getX(), point.getY(), lineLength
                 ,result.r, result.g, result.b, result.a
                 ,color.r, color.g, color.b, color.a
                 ,ratio
                 ,newColor.r, newColor.g, newColor.b, newColor.a);

      OutputDebugString(debugString);
      */

      result = newColor;
   }

   return result;
}

Color
Gradient::formula
(RelativePoint point) const
{
   return this->formula(point.getX(), point.getY());
}

Color
Gradient::formula
(long x, long y, long width, long height) const
{
   return this->formula(RelativePoint::FromAbsoluteX(x, width)
                        ,RelativePoint::FromAbsoluteY(y, height));
}

Color
Gradient::formula
(long x, long y, SIZE size) const
{
   return this->formula(RelativePoint::FromAbsoluteX(x, size.cx)
                        ,RelativePoint::FromAbsoluteY(y, size.cy));
}

Color
Gradient::formula
(AbsolutePoint point, long width, long height) const
{
   return this->formula(RelativePoint::FromAbsoluteX(point.getX(), width)
                        ,RelativePoint::FromAbsoluteY(point.getY(), height));
}

Color
Gradient::formula
(AbsolutePoint point, SIZE size) const
{
   return this->formula(RelativePoint::FromAbsoluteX(point.getX(), size.cx)
                        ,RelativePoint::FromAbsoluteY(point.getY(), size.cy));
}

HBITMAP
Gradient::render
(long width, long height) const
{
   HDC screen;
   HBITMAP dibSection;
   BITMAPINFO info;
   RGBQUAD *quadData, *quadPointer;

   memset(&info, 0, sizeof(BITMAPINFO));

   info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   info.bmiHeader.biWidth = width;
   info.bmiHeader.biHeight = height;
   info.bmiHeader.biPlanes = 1;
   info.bmiHeader.biBitCount = 32;

   screen = GetDC(NULL);

   dibSection = CreateDIBSection(screen
                                 ,&info
                                 ,DIB_RGB_COLORS
                                 ,(LPVOID *)&quadData
                                 ,NULL, 0);

   if (dibSection == NULL)
      throw GradientException("failed to create DIB section");
   
   quadData = (RGBQUAD *)malloc(width * height * sizeof(RGBQUAD));
   quadPointer = quadData;

   for (long y=0; y<height; ++y)
   {
      for (long x=0; x<width; ++x)
      {
         Color color = this->formula(x, y, width, height);
         
         quadPointer->rgbRed = (BYTE)((DWORD)(color.r * color.a / 255));
         quadPointer->rgbGreen = (BYTE)((DWORD)(color.g * color.a / 255));
         quadPointer->rgbBlue = (BYTE)((DWORD)(color.b * color.a / 255));
         quadPointer->rgbReserved = color.a;
            
         ++quadPointer;
      }
   }

   if (!SetDIBits(screen, dibSection, 0, height, quadData, &info, DIB_RGB_COLORS))
      throw GradientException("SetDIBits failed");

   ReleaseDC(NULL, screen);

   return dibSection;
}

HBITMAP
Gradient::render
(SIZE size) const
{
   return this->render(size.cx, size.cy);
}
