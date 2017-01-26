#include "sostav/math/point.hpp"

using namespace Sostav;
using namespace Sostav::Math;

PointException::PointException
(const char *what)
   : Exception(what)
{
}

Point::Point
(double x, double y)
{
   this->setX(x);
   this->setY(y);
}

Point::Point
(const Point &p)
{
   this->setX(p.getX());
   this->setY(p.getY());
}

Point::Point
(void)
{
   this->setX(NAN);
   this->setY(NAN);
}

bool
Point::operator<
(const Point &b) const
{
   double x = b.getX();
   double y;

   if (isnan(x) || isnan(this->x) && !(isnan(x) && isnan(this->x)))
   {
      if (isnan(x))
         return false;
      else if (isnan(this->x))
         return true;
   }
   else if (x != this->x && !(isnan(x) && isnan(this->x)))
      return this->x < x;

   y = b.getY();

   if (isnan(y) || isnan(this->y) && !(isnan(y) && isnan(this->y)))
   {
      if (isnan(y))
         return false;
      else if (isnan(this->y))
         return true;
   }

   return this->y < y;
}

bool
Point::operator>
(const Point &b) const
{
   return b < *this;
}

bool
Point::operator==
(const Point &b) const
{
   return !(*this < b) && !(b < *this);
}

bool
Point::operator!=
(const Point &b) const
{
   return !(*this == b);
}

bool
Point::operator<=
(const Point &b) const
{
   return *this < b || *this == b;
}

bool
Point::operator>=
(const Point &b) const
{
   return *this > b || *this == b;
}

double
Point::getX
(void) const
{
   return this->x;
}

void
Point::setX
(double x)
{
   this->x = x;
}

double
Point::getY
(void) const
{
   return this->y;
}

void
Point::setY
(double y)
{
   this->y = y;
}

void
Point::set
(double x, double y)
{
   this->setX(x);
   this->setY(y);
}

Angle
Point::slope
(double x2, double y2) const
{
   if (x2 - this->x == 0)
      return Angle(); /* angle inits as nan */
   
   return Angle((y2 - this->y)/(x2 - this->x));
}

Angle
Point::slope
(Point p2) const
{
   return this->slope(p2.getX(), p2.getY());
}

double
Point::length
(double x2, double y2) const
{
   Angle slope;

   if (isnan(x2) || isnan(y2))
      return NAN;
   
   slope = this->slope(x2, y2);

   if (slope.getRadians() == 0.0)
      return fabs(x2 - this->x);
   else if (!slope.isNan())
      return fabs(y2 - this->y);
   else
      return sqrt(pow(x2 - this->x, 2.0) + pow(y2 - this->y, 2.0));
}

double
Point::length
(Point p2) const
{
   return this->length(p2.getX(), p2.getY());
}

bool
Point::isNan
(void) const
{
   return isnan(this->x) || isnan(this->y);
}
