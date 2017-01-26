#pragma once

#define _USE_MATH_DEFINES

#include <cmath>

#include "sostav/exception.hpp"

namespace Sostav
{
   namespace Math
   {
      class AngleException : public Exception
      {
      public:
         AngleException(const char *what);
      };

      class Angle
      {
      protected:
         double radians;

      public:
         Angle(double radians);
         Angle(const Angle &angle);
         Angle();

         double getRadians(void) const;
         void setRadians(double radians);

         double getDegrees(void) const;
         void setDegrees(double degrees);

         bool isNan(void) const;
      };
   }
}
