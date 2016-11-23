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

   class GradientPointException : public GradientException
   {
   public:
      GradientPointException(char *what);
   };

   class GradientPoint
   {
   public:
      double x, y;

      GradientPoint(double x, double y);
      GradientPoint(LONG x, LONG y, LONG cx, LONG cy);
      GradientPoint(POINT point, SIZE rect);
      GradientPoint(GradientPoint &point);
      GradientPoint();

      friend bool operator<(const GradientPoint &l, const GradientPoint &r);
      friend bool operator>(const GradientPoint &l, const GradientPoint &r);
      
   private:
      void initialize(double x, double y);
      void initialize(LONG x, LONG y, LONG cx, LONG cy);
   };

   class Gradient
   {
   protected:
      std::map<GradientPoint, Color> modes;
      GradientPoint min, max;
      SIZE size;

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
      
      Color formula(LONG x, LONG y);
      std::map<POINT, Color> render(void);

   protected:
      void initialize(void);
      void getMinMax(void);
   };
}
