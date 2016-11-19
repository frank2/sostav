#pragma once

#include <windows.h>

#include <algorithm>
#include <list>
#include <map>

#include "sostav/color.hpp"
#include "sostav/exception.hpp"

namespace Sostav
{
   class GradientException : public Exception
   {
   public:
      GradientException(char *what);
   };

   class Gradient
   {
   protected:
      std::map<LONG, Color> modes;
      LONG min, max;

   public:
      Gradient(Color start, Color end);
      Gradient(std::map<LONG, Color> modes);
      Gradient(Gradient &gradient);
      Gradient();

      void setModes(std::map<LONG, Color> modes);
      std::map<LONG, Color> getModes(void);

      size_t getModeCount(void);

      void addColor(LONG key, Color color);
      void removeColor(LONG key);
      void moveColor(LONG key, LONG newKey);
      
      Color step(LONG stepIndex, LONG stepMax);
      std::list<Color> render(LONG stepMax);

   protected:
      void initialize(void);
      void getMinMax(void);
   };
}
