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
(void)
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
(void)
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
(double x, double y, long width, long height)
{
   Color result = this->base;
   Line diagonal(0, height, width, 0);
   double diagonalLength = diagonal.length();
   std::map<RelativePoint, Color>::iterator iter;

   for (iter=this->modes.begin(); iter!=this->modes.end(); ++iter)
   {
      RelativePoint point = iter->first;
      Color color = iter->second;
      Line modeLine(x, y, point.getX(), point.getY());
      double lineLength = modeLine.length();
      double ratio;

      if (lineLength >= diagonalLength)
         ratio = 0.0;
      else
         ratio = (diagonalLength - lineLength) / diagonalLength;

      result.blend(color, ratio);
   }

   return result;
}

Color
Gradient::formula
(double x, double y, SIZE size)
{
   return this->formula(x, y, size.cx, size.cy);
}

Color
Gradient::formula
(RelativePoint point, long width, long height)
{
   return this->formula(point.getX(), point.getY(), width, height);
}

Color
Gradient::formula
(RelativePoint point, SIZE size)
{
   return this->formula(point.getX(), point.getY(), size.cx, size.cy);
}

Color
Gradient::formula
(long x, long y, long width, long height)
{
   return this->formula(RelativePoint::FromAbsolulteX(x, width)
                        ,RelativePoint::FromAbsoluteY(y, height)
                        ,width
                        ,height);
}

Color
Gradient::formula
(long x, long y, SIZE size)
{
   return this->formula(RelativePoint::FromAbsoluteX(x, size.cx)
                        ,RelativePoint::FromAbsoluteY(y, size.cy)
                        ,size.cx
                        ,size.cy);
}

Color
Gradient::formula
(AbsolutePoint point, long width, long height)
{
   return this->formula(RelativePoint::FromAbsoluteX(point.getX(), width)
                        ,RelativePoint::FromAbsoluteY(point.getY(), height)
                        ,width
                        ,height);
}

Color
Gradient::formula
(AbsolutePoint point, SIZE size)
{
   return this->formula(RelativePoint::FromAbsoluteX(point.getX(), size.cx)
                        ,RelativePoint::FromAbsoluteY(point.getY(), size.cy)
                        ,size.cx
                        ,size.cy);
}

void
Gradient::paint
(HDC context)
{
}
