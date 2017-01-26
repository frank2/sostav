#include "sostav/math/angle.hpp"

using namespace Sostav;
using namespace Sostav::Math;

AngleException::AngleException
(const char *what)
   : Exception(what)
{
}

Angle::Angle
(double radians)
{
   this->radians = radians;
}

Angle::Angle
(const Angle &angle)
{
   this->radians = angle.getRadians();
}

Angle::Angle
(void)
{
   this->radians = NAN;
}

double
Angle::getRadians
(void) const
{
   return this->radians;
}

void
Angle::setRadians
(double radians)
{
   this->radians = radians;
}

double
Angle::getDegrees
(void) const
{
   return atan(this->radians) * (180 / M_PI);
}

void
Angle::setDegrees
(double degrees)
{
   this->radians = tan(degrees / (180 / M_PI));
}

bool
Angle::isNan
(void) const
{
   return isnan(this->radians);
}
