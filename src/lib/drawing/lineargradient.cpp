#include "sostav/gradient.hpp"

using namespace Sostav;

GradientException::GradientException
(char *what)
   : Exception(what)
{
}

Gradient::Gradient
(Color start, Color end)
{
   this->initialize();
   this->addColor(0, start);
   this->addColor(1, end);
}

Gradient::Gradient
(std::map<LONG, Color> modes)
{
   this->initialize();
   this->setModes(modes);
}

Gradient::Gradient
(Gradient &gradient)
{
   this->initialize();
   this->setModes(gradient.getModes());
}

Gradient::Gradient
(void)
{
   this->initialize();
}

void
Gradient::setModes
(std::map<LONG, Color> modes)
{
   this->modes = modes;
   this->getMinMax();
}

std::map<LONG, Color>
Gradient::getModes
(void)
{
   return this->modes;
}

size_t
Gradient::getModeCount
(void)
{
   return this->modes.size();
}

void
Gradient::addColor
(LONG key, Color color)
{
   this->modes[key] = color;

   if (key < this->min)
      this->min = key;

   if (key > this->max)
      this->max = key;
}

void
Gradient::removeColor
(LONG key)
{
   this->modes.erase(key);
}

void
Gradient::moveColor
(LONG key, LONG newKey)
{
   if (this->modes.find(key) == this->modes.end())
      throw GradientException("could not find key in modes");
   
   this->addColor(newKey, this->modes[key]);
   this->removeColor(key);
}

Color
Gradient::step
(LONG stepIndex, LONG stepMax)
{
   double stepRatio, rangeIndex, rangeRatio;
   LONG rangeUpper, rangeLower;
   Color colorUpper, colorLower;
   std::map<LONG, Color>::iterator modeIter;

   if (this->getModeCount() == 0)
      return Color::Transparent();

   if (this->getModeCount() == 1)
      return this->modes.begin()->second;
   
   /* get the ratio of stepIndex to stepMax (stepIndex / stepMax) */
   stepRatio = (double)(stepIndex / stepMax);
   
   /* multiply this against (max - min) to get (offset + min) to the gradient */
   rangeIndex = (this->max - this->min) * stepRatio + this->min;
   
   /* find the upper bound in the keys as well as its previous entry */
   modeIter = this->modes.upper_bound(rangeIndex);
   
   rangeUpper = modeIter->first;
   colorUpper = modeIter->second;
   
   --modeIter;
   
   rangeLower = modeIter->first;
   colorLower = modeIter->second;
   
   /* find the ratio of our offset to the discovered range */
   rangeRatio = (double)((rangeIndex - rangeLower) / (rangeUpper - rangeLower));
   
   /* use this ratio to blend the colors */
   return colorLower.blend(colorUpper, rangeRatio);
}

std::list<Color>
Gradient::render
(LONG stepMax)
{
   std::list<Color> colors;
   
   for (LONG start=0; start<=stepMax; ++start)
      colors.push_back(this->step(start, stepMax));

   return colors;
}
