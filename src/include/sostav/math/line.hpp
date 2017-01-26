#pragma once

#define NOMINMAX

#include <algorithm>
#include <cmath>

#include "sostav/exception.hpp"
#include "sostav/math/angle.hpp"
#include "sostav/math/point.hpp"

namespace Sostav
{
   namespace Math
   {
      class LineException : public Exception
      {
         LineException(const char *what);
      };

      class Line
      {
      protected:
         Angle slope;
         double xIntercept, yIntercept, length;
         Point p1, p2;

      public:
         Line(Point p1, Point p2);
         Line(double x1, double y1, double x2, double y2);
         Line(double slope, double yIntercept);
         Line(Angle slope, double yIntercept);
         Line(const Line &line);
         Line();

         Angle getSlope(void) const;
         void setSlope(double slope);
         void setSlope(Angle slope);

         double getXIntercept(void) const;
         void setXIntercept(double xIntercept);

         double getYIntercept(void) const;
         void setYIntercept(double yIntercept);
         
         double getLength(void) const;

         void setP1(double x, double y);
         void setP1(Point p);
         Point getP1(void) const;

         void setP2(double x, double y);
         void setP2(Point p);
         Point getP2(void) const;

         bool isVertical(void) const;
         bool isHorizontal(void) const;

         double fX(double x) const;
         double fY(double y) const;

         Point intersection(const Line line) const;

      protected:
         void solve(void);
         void alignXIntercept(void);
         void alignYIntercept(void);
      };
   }
}
