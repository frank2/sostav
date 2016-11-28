#include "sostav/drawing/gradient.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

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
