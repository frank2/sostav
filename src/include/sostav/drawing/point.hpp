#pragma once

#include <windows.h>
#include <math.h>

#include "sostav/exception.hpp"
#include "sostav/math/point.hpp"

namespace Sostav
{
   namespace Drawing
   {
      class RelativePoint;

      class AbsolutePoint
      {
      protected:
         long x, y;

      public:
         AbsolutePoint(long x, long y);
         AbsolutePoint(POINT point);
         AbsolutePoint(const AbsolutePoint &abs);
         AbsolutePoint();

         bool operator<(const AbsolutePoint &b) const;
         bool operator==(const AbsolutePoint &b) const;
         bool operator!=(const AbsolutePoint &b) const;
         bool operator>(const AbsolutePoint &b) const;
         bool operator<=(const AbsolutePoint &b) const;
         bool operator>=(const AbsolutePoint &b) const;

         static AbsolutePoint FromRelative(double x, double y, long width, long height);
         static AbsolutePoint FromRelative(double x, double y, SIZE size);
         static AbsolutePoint FromRelative(RelativePoint point, long width, long height);
         static AbsolutePoint FromRelative(RelativePoint point, SIZE size);
         static long FromRelativeX(double x, long width);
         static long FromRelativeY(double y, long height);

         void set(long x, long y);
         void set(POINT point);
         void setRelative(double x, double y, long width, long height);
         void setRelative(double x, double y, SIZE size);
         void setRelative(RelativePoint relative, long width, long height);
         void setRelative(RelativePoint relative, SIZE size);
         void setX(long x);
         void setRelativeX(double x, long width);
         void setY(long y);
         void setRelativeY(double y, long height);
         
         long getX(void) const;
         double getRelativeX(long width) const;
         long getY(void) const;
         double getRelativeY(long height) const;
         POINT getPoint(void) const;

         RelativePoint relative(long width, long height) const;
         RelativePoint relative(SIZE size) const;
      };

      class RelativePoint : public Math::Point
      {
      public:
         RelativePoint(double x, double y);
         RelativePoint(const RelativePoint &rel);
         RelativePoint();

         static RelativePoint FromAbsolute(long x, long y, long width, long height);
         static RelativePoint FromAbsolute(POINT point, long width, long height);
         static RelativePoint FromAbsolute(POINT point, SIZE size);
         static RelativePoint FromAbsolute(AbsolutePoint point, long width, long height);
         static RelativePoint FromAbsolute(AbsolutePoint point, SIZE size);
         static double FromAbsoluteX(long x, long width);
         static double FromAbsoluteY(long y, long height);

         void setAbsolute(long x, long y, long width, long height);
         void setAbsolute(long x, long y, SIZE size);
         void setAbsolute(POINT point, long width, long height);
         void setAbsolute(POINT point, SIZE size);
         void setAbsolute(AbsolutePoint point, long width, long height);
         void setAbsolute(AbsolutePoint point, SIZE size);
         void setAbsoluteX(long x, long width);
         void setAbsoluteY(long y, long height);
         
         long getAbsoluteX(long width) const;
         long getAbsoluteY(long height) const;

         AbsolutePoint absolute(long width, long height) const;
         AbsolutePoint absolute(SIZE size) const;
      };

      class MixedPoint
      {
      protected:
         AbsolutePoint absolute;
         RelativePoint relative;
         SIZE size;

      public:
         MixedPoint(AbsolutePoint absolute, RelativePoint relative, SIZE size);
         MixedPoint(AbsolutePoint absolute, SIZE size);
         MixedPoint(RelativePoint relative, SIZE size);
         MixedPoint(AbsolutePoint absolute);
         MixedPoint(RelativePoint relative);
         MixedPoint(MixedPoint &mixed);
         MixedPoint();

         void setAbsolute(AbsolutePoint absolute);
         void setAbsolute(long x, long y);
         void setAbsoluteX(long x);
         void setAbsoluteY(long y);
         
         AbsolutePoint getAbsolute(void);
         long getAbsoluteX(void);
         long getAbsoluteY(void);

         void setRelative(RelativePoint relative);
         void setRelative(double x, double y);
         void setRelativeX(double x);
         void setRelativeY(double y);

         RelativePoint getRelative(void);
         double getRelativeX(void);
         double getRelativeY(void);

         void setSize(SIZE size);
         void setSize(long width, long height);
         SIZE getSize(void);
      };
   }
}
