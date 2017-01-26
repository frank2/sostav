#include "sostav/math/line.hpp"

using namespace Sostav;
using namespace Sostav::Math;

LineException::LineException
(const char *what)
   : Exception(what)
{
}

Line::Line
(Point p1, Point p2)
{
   this->setP1(p1);
   this->setP2(p2);
}

Line::Line
(double x1, double y1, double x2, double y2)
{
   this->setP1(x1, y1);
   this->setP2(x2, y2);
}

Line::Line
(double slope, double yIntercept)
{
   this->slope.setRadians(slope);
   this->yIntercept = yIntercept;
   this->xIntercept = NAN;
   this->length = NAN;
   this->solve();
}

Line::Line
(Angle slope, double yIntercept)
{
   this->slope = slope;
   this->yIntercept = yIntercept;
   this->xIntercept = NAN;
   this->length = NAN;
   this->solve();
}

Line::Line
(const Line &line)
{
   this->p1 = line.getP1();
   this->p2 = line.getP2();

   if (!this->p1.isNan() && !this->p2.isNan())
   {
      this->setP1(p1);
      this->setP2(p2);
   }
   else
   {
      this->slope = line.getSlope();
      this->yIntercept = line.getYIntercept();
      this->xIntercept = line.getXIntercept();
   }
}

Line::Line
(void)
{
   this->yIntercept = this->xIntercept = this->length = NAN;
}

Angle
Line::getSlope
(void) const
{
   return this->slope;
}

void
Line::setSlope
(double slope)
{
   /* setting the slope invalidates the X intercept and shifts the points */
   this->slope.setRadians(slope);

   if (!isnan(this->yIntercept))
      this->setXIntercept(NAN);
   else if (!isnan(this->xIntercept))
      this->setYIntercept(NAN);
   else
      this->solve();
}

void
Line::setSlope
(Angle angle)
{
   this->setSlope(angle.getRadians());
}

double
Line::getXIntercept
(void) const
{
   return this->xIntercept;
}

void
Line::setXIntercept
(double xIntercept)
{
   this->xIntercept = xIntercept;
   this->p1.setX(NAN);
   this->p2.setX(NAN);
   this->solve();
}

double
Line::getYIntercept
(void) const
{
   return this->yIntercept;
}

void
Line::setYIntercept
(double yIntercept)
{
   this->yIntercept = yIntercept;
   this->p1.setY(NAN);
   this->p2.setY(NAN);
   this->solve();
}

double
Line::getLength
(void) const
{
   return this->length;
}

void
Line::setP1
(double x, double y)
{
   this->p1.setX(x);
   this->p1.setY(y);
   this->solve();
}

void
Line::setP1
(Point p)
{
   this->setP1(p.getX(), p.getY());
}

Point
Line::getP1
(void) const
{
   return this->p1;
}

void
Line::setP2
(double x, double y)
{
   this->p2.setX(x);
   this->p2.setY(y);
   this->solve();
}

void
Line::setP2
(Point p)
{
   this->setP2(p.getX(), p.getY());
}

Point
Line::getP2
(void) const
{
   return this->p2;
}

bool
Line::isVertical
(void) const
{
   return this->getSlope().isNan();
}

bool
Line::isHorizontal
(void) const
{
   return this->getSlope().getRadians() == 0.0;
}

double
Line::fX
(double x) const
{
   if (isnan(x))
      return NAN;
   
   if (this->isVertical())
   {
      if (x == this->getXIntercept())
         return INFINITY;
      else
         return NAN;
   }

   return this->getSlope().getRadians() * x + this->getYIntercept();
}

double
Line::fY
(double y) const
{
   if (isnan(y))
      return NAN;
   
   if (this->isHorizontal())
   {
      if (y == this->getYIntercept())
         return INFINITY;
      else
         return NAN;
   }
   else if (this->isVertical())
      return this->getXIntercept();
      
   return y/this->getSlope().getRadians() + this->getXIntercept();
}

Point
Line::intersection
(const Line line) const
{
   double m = this->getSlope().getRadians();
   double n = line.getSlope().getRadians();
   double b, c, d, e;
   double ix, iy;

   /* lines are parallel and do not intersect */
   if (m == n)
      return Point();

   b = this->getYIntercept();
   c = line.getYIntercept();
   d = this->getXIntercept();
   e = line.getXIntercept();

   if (e == d)
      return Point(d, 0);
   else if (b == c)
      return Point(0, b);

   ix = (c - b)/(m - n);
   iy = (m - n)/(e - d);

   return Point(ix, iy);
}
      
void
Line::solve
(void)
{
   /* four possible states:

    * have two points
    * have slope and an intercept
    * have point and a slope
    * have two intercepts */

   if (!this->p1.isNan() && !this->p2.isNan())
   {
      this->slope = this->p1.slope(this->p2);
      this->length = this->p1.length(this->p2);

      /* y = mx + b
         -b = mx - y
         b = y - mx */

      if (this->isVertical())
         this->yIntercept = NAN;
      else
         this->yIntercept = this->p1.getY() - this->slope.getRadians() * this->p1.getX();

      /* x = y/m + d
         -d = y/m - x
         d = x - y/m */

      if (this->isHorizontal())
         this->xIntercept = NAN;
      else if (this->isVertical())
         this->xIntercept = this->p1.getX();
      else
         this->xIntercept = this->p1.getX() - this->p1.getY() / this->slope.getRadians();
   }
   else if (!this->slope.isNan() && (!isnan(this->xIntercept) || !isnan(this->yIntercept)))
   {
      if (!isnan(this->yIntercept))
      {
         /* m = -b/d
            d * m = -b
            d = -b/m */
         
         if (this->slope.getRadians() == 0.0)
            this->xIntercept = NAN;
         else
            this->xIntercept = -this->yIntercept/this->slope.getRadians();

         this->alignYIntercept();
      }
      else
      {
         /* m = -b/d
            d * m = -b
            b = -d * m */

         this->yIntercept = -this->xIntercept * this->slope.getRadians();

         this->alignXIntercept();
      }
   }
   else if (!this->slope.isNan() && (!this->p1.isNan() || !this->p2.isNan()))
   {
      Point validPoint = (!this->p1.isNan()) ? this->p1 : this->p2;

      /* y2 - y1 / x2 - x1 = m
         y2 - y1 / 0 - x = m
         y2 - y1 = m * -x
         y2 = y1 + m * -x */

      this->yIntercept = this->slope.getRadians() * -this->p1.getX();

      if (this->slope.getRadians() == 0.0)
         this->xIntercept = NAN;
      else
         this->xIntercept = -this->yIntercept/this->slope.getRadians();
   }
   else if (!isnan(this->xIntercept) && !isnan(this->yIntercept))
   {
      if (this->xIntercept == 0.0)
         this->slope.setRadians(NAN);
      else
         this->slope.setRadians(-this->yIntercept/this->xIntercept);
   }
}

void
Line::alignXIntercept
(void)
{
   double p1X, p2X, p1Y, p2Y;
   
   p1Y = this->p1.getY();
   p2Y = this->p2.getY();
   p1X = this->fY(p1Y);
   p2X = this->fY(p2Y);

   this->p1.setX(p1X);
   this->p2.setX(p2X);
   this->length = this->p1.length(this->p2);
}

void
Line::alignYIntercept
(void)
{
   double p1X, p2X, p1Y, p2Y;
   
   p1X = this->p1.getX();
   p2X = this->p2.getX();
   p1Y = this->fX(p1X);
   p2Y = this->fX(p2X);

   this->p1.setY(p1Y);
   this->p2.setY(p2Y);
   this->length = this->p1.length(this->p2);
}
