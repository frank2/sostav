#pragma once

#include <windows.h>
#include <math.h>

#include "sostav/exception.hpp"
#include "sostav/math/angle.hpp"

namespace Sostav
{
   namespace Math
   {
      class PointException : public Exception
      {
         PointException(const char *what);
      };

      class Point
      {
      protected:
         double x, y;

      public:
         Point(double x, double y);
         Point(const Point &p);
         Point();

         bool operator<(const Point &b) const;
         bool operator>(const Point &b) const;
         bool operator==(const Point &b) const;
         bool operator!=(const Point &b) const;
         bool operator<=(const Point &b) const;
         bool operator>=(const Point &b) const;

         double getX(void) const;
         void setX(double x);

         double getY(void) const;
         void setY(double y);

         void set(double x, double y);

         Angle slope(double x2, double y2) const;
         Angle slope(Point p2) const;

         double length(double x2, double y2) const;
         double length(Point p2) const;

         bool isNan(void) const;
      };
   }
}
