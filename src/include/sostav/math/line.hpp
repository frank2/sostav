#pragma once

#include <math.h>

#include "sostav/exception.hpp"

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
      public:
         double x1, y1, x2, y2;

         Line(double x1, double y1, double x2, double y2);
         Line(Line &line);
         Line();

         double slope(void);
         double length(void);
      };
   }
}
