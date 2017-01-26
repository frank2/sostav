#pragma once

#include <cmath>
#include <utility>

#include "sostav/exception.hpp"
#include "sostav/math/point.hpp"

namespace Sostav
{
   namespace Math
   {
      class EllipseException : public Exception
      {
      public:
         EllipseException(const char *what);
      };

      class Ellipse
      {
      protected:
         Point center;
         double xRadius, yRadius;

      public:
         Ellipse(double x, double y, double xRadius, double yRadius);
         Ellipse(Point center, double xRadius, double yRadius);
         Ellipse(const Ellipse &ellipse);
         Ellipse();

         void setCenter(double x, double y);
         void setCenter(Point center);
         Point getCenter(void) const;

         void setXRadius(double xRadius);
         double getXRadius(void) const;

         void setYRadius(double yRadius);
         double getYRadius(void) const;
         
         bool isXMajor(void) const;
         bool isYMajor(void) const;

         std::pair<double, double> fX(double x);
         std::pair<double, double> fY(double y);

         std::pair<Point, Point> lineIntersection(Line line);
      };
   }
}
