#pragma once

#include <windows.h>

#include <algorithm>
#include <cstdio>
#include <map>

#include "sostav/exception.hpp"
#include "sostav/drawing/color.hpp"
#include "sostav/drawing/point.hpp"
#include "sostav/math/line.hpp"

#pragma comment(lib, "msimg32")

namespace Sostav
{
   namespace Drawing
   {
      class GradientException : public Exception
      {
      public:
         GradientException(const char *what);
      };
         
      class Gradient
      {
      protected:
         std::map<RelativePoint, Color> modes;
         Color base;

      public:
         Gradient(std::map<RelativePoint, Color> modes, Color base);
         Gradient(Gradient &gradient);
         Gradient();

         void setModes(std::map<RelativePoint, Color> modes);
         std::map<RelativePoint, Color> getModes(void) const;

         void setBase(Color base);
         Color getBase(void) const;

         void addColor(RelativePoint key, Color color);
         void removeColor(RelativePoint key);
         void moveColor(RelativePoint key, RelativePoint newKey);
         
         virtual Color formula(double x, double y) const;
         virtual Color formula(RelativePoint point) const;
         virtual Color formula(long x, long y, long width, long height) const;
         virtual Color formula(long x, long y, SIZE size) const;
         virtual Color formula(AbsolutePoint point, long width, long height) const;
         virtual Color formula(AbsolutePoint point, SIZE size) const;

         virtual HBITMAP render(long width, long height) const;
         virtual HBITMAP render(SIZE size) const;
      };
   }
}
