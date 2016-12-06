#pragma once

#include <windows.h>
#include <math.h>

#include "sostav/exception.hpp"

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

         static const Point InvalidPoint(void);

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

         double slope(double x2, double y2) const;
         double slope(Point p2) const;

         double length(double x2, double y2) const;
         double length(Point p2) const;

         bool valid(void) const;
      };
   }
}
