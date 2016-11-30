#include "sostav/math/line.hpp"

using namespace Sostav;
using namespace Sostav::Math;

LineException::LineException
(const char *what)
   : Exception(what)
{
}

Line::Line
(double x1, double y1, double x2, double y2)
{
   this->x1 = x1;
   this->y1 = y1;
   this->x2 = x2;
   this->y2 = y2;
}

Line::Line
(Line &line)
{
   this->x1 = line.x1;
   this->y1 = line.y1;
   this->x2 = line.x2;
   this->y2 = line.y2;
}

Line::Line
(void)
{
   this->x1 = this->y1 = this->x2 = this->y2 = 0.0;
}

double
Line::slope
(void)
{
   double xVal, yVal;

   yVal = this->y2 - this->y1;
   xVal = this->x2 - this->x1;

   if (xVal == 0.0)
      return NAN;

   return yVal/xVal;
}

double
Line::length
(void)
{
   double slope;

   slope = this->slope();

   if (slope == 0.0)
      return fabs(this->x2 - this->x1);
   else if (isnan(slope))
      return fabs(this->y2 - this->y1);
   else
      return sqrt(pow(this->x2 - this->x1, 2.0) + pow(this->y2 - this->y1, 2.0));
}
