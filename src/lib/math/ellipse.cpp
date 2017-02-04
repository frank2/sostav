#include "sostav/math/ellipse.hpp"

using namespace Sostav;
using namespace Sostav::Math;

EllipseException::EllipseException
(const WCHAR *what)
   : Exception(what)
{
}

Ellipse::Ellipse
(double x, double y, double xRadius, double yRadius)
{
   this->center.setX(x);
   this->center.setY(y);
   this->xRadius = xRadius;
   this->yRadius = yRadius;
}

Ellipse::Ellipse
(Point center, double xRadius, double yRadius)
{
   this->center = center;
   this->xRadius = xRadius;
   this->yRadius = yRadius;
}

Ellipse::Ellipse
(const Ellipse &ellipse)
{
   this->center = ellipse.getCenter();
   this->xRadius = ellipse.getXRadius();
   this->yRadius = ellipse.getYRadius();
}

Ellipse::Ellipse
(void)
{
   this->xRadius = this->yRadius = NAN;
}

void
Ellipse::setCenter
(double x, double y)
{
   this->center.setX(x);
   this->center.setY(y);
}

void
Ellipse::setCenter
(Point center)
{
   this->setCenter(center.getX(), center.getY());
}

Point
Ellipse::getCenter
(void) const
{
   return this->center;
}

void
Ellipse::setXRadius
(double xRadius)
{
   this->xRadius = xRadius;
}

double
Ellipse::getXRadius
(void) const
{
   return this->xRadius;
}

void
Ellipse::setYRadius
(double yRadius)
{
   this->yRadius = yRadius;
}

double
Ellipse::getYRadius
(void) const
{
   return this->yRadius;
}

bool
Ellipse::isXMajor
(void) const
{
   return this->xRadius > this->yRadius;
}

bool
Ellipse::isYMajor
(void) const
{
   return this->yRadius > this->xRadius;
}

std::pair<double, double>
Ellipse::fX
(double x)
{
   throw EllipseException(L"function not implemented yet");
   /*
     ((x - h)/M)^2 + ((y - k)/m)^2 = 1
     ((y - k)/m)^2 = 1 - ((x - h)/M)^2
     (y - k)/m = sqrt(1 - ((x - h)/M)^2)
     y - k = sqrt(1 - ((x - h)/M)^2) * m
     y = sqrt(1 - ((x - h)/M)^2) * m + k
     y = sqrt(1 - ((x - h)/m)^2) * M + k
   */
}

std::pair<double, double>
Ellipse::fY
(double y)
{
   throw EllipseException(L"function not implemented yet");
   
   /*
     ((x - h)/M)^2 + ((y - k)/m)^2 = 1
     ((x - h)/M)^2 = 1 - ((y - k)/m)^2
     (x - h)/M = sqrt(1 - ((y - k)/m)^2)
     x - h = sqrt(1 - ((y - k)/m)^2) * M
     x = sqrt(1 - ((y - k)/m)^2) * M + h
     x = sqrt(1 - ((y - k)/M)^2) * m + h
   */
}

std::pair<Point, Point>
Ellipse::lineIntersection
(Line line)
{
   throw EllipseException(L"function not implemented yet");

   /* ((x - h)/a)^2 + ((y - k)/b)^2 = 1
      ((x - h)/a)^2 + ((sx + i - k)/b)^2 = 1
      (x - h)^2/a^2 + (sx + i - k)^2/b^2 = 1
      (b^2 * (x - h)^2)/a^2 + (sx + i - k)^2 = b^2
      (b^2 * (x - h)^2) + (a^2 * (sx + i - k)^2) = a^2b^2
      
      (x - h) * (x - h)
      x^2 - hx
          - hx + h^2
      x^2 - 2hx + h^2

      (sx + i - k) * (sx + i - k)
      s^2x^2 + isx - ksx
             + isx + i^2 - ik
                   - ksx - ik + k^2
      s^2x^2 + 2isx + i^2 - 2ksx - 2ik + k^2

      (b^2 * (x^2 - 2hx + h^2)) + (a^2 * (s^2x^2 + 2isx + i^2 - 2ksx - 2ik + k^2)) = a^2b^2
      b^2x^2 - 2b^2hx + b^2h^2 + a^2s^2x^2 + 2a^2isx + a^2i^2 - 2a^2ksx - 2a^2ik + a^2k^2 = a^2b^2
   */
}
