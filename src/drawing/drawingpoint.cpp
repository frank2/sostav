#include "sostav/drawing/point.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Math;

AbsolutePoint::AbsolutePoint
(long x, long y)
{
   this->set(x, y);
}

AbsolutePoint::AbsolutePoint
(POINT point)
{
   this->set(point.x, point.y);
}

AbsolutePoint::AbsolutePoint
(const AbsolutePoint &abs)
{
   this->set(abs.getX(), abs.getY());
}

AbsolutePoint::AbsolutePoint
(void)
{
   this->set(0, 0);
}

bool
AbsolutePoint::operator<
(const AbsolutePoint &b) const
{
   long x, y;

   x = b.getX();

   if (x != this->x)
      return this->x < x;

   y = b.getY();

   return this->y < y;
}

bool
AbsolutePoint::operator==
(const AbsolutePoint &b) const
{
   return !(*this < b) && !(b < *this);
}

bool
AbsolutePoint::operator!=
(const AbsolutePoint &b) const
{
   return !(*this == b);
}

bool
AbsolutePoint::operator>
(const AbsolutePoint &b) const
{
   return b < *this;
}

bool
AbsolutePoint::operator<=
(const AbsolutePoint &b) const
{
   return *this < b || *this == b;
}

bool
AbsolutePoint::operator>=
(const AbsolutePoint &b) const
{
   return *this > b || *this == b;
}

AbsolutePoint
AbsolutePoint::FromRelative
(double x, double y, long width, long height)
{
   AbsolutePoint ret;

   ret.setX(AbsolutePoint::FromRelativeX(x, width));
   ret.setY(AbsolutePoint::FromRelativeY(y, height));

   return ret;
}

AbsolutePoint
AbsolutePoint::FromRelative
(double x, double y, SIZE size)
{
   return AbsolutePoint::FromRelative(x, y, size.cx, size.cy);
}

AbsolutePoint
AbsolutePoint::FromRelative
(RelativePoint point, long width, long height)
{
   return AbsolutePoint::FromRelative(point.getX(), point.getY(), width, height);
}

AbsolutePoint
AbsolutePoint::FromRelative
(RelativePoint point, SIZE size)
{
   return AbsolutePoint::FromRelative(point.getX(), point.getY(), size.cx, size.cy);
}

long
AbsolutePoint::FromRelativeX
(double x, long width)
{
   double result = x * width;
   double floorOf = floor(result);
   double remainder = result - floorOf;

   if (remainder >= 0.5)
      return (long)ceil(result);
   else
      return (long)floorOf;
}

long
AbsolutePoint::FromRelativeY
(double y, long height)
{
   double result = y * height;
   double floorOf = floor(result);
   double remainder = result - floorOf;

   if (remainder >= 0.5)
      return (long)ceil(result);
   else
      return (long)floorOf;
}

void
AbsolutePoint::set
(long x, long y)
{
   this->setX(x);
   this->setY(y);
}

void
AbsolutePoint::set
(POINT point)
{
   this->set(point.x, point.y);
}

void
AbsolutePoint::setRelative
(double x, double y, long width, long height)
{
   this->setRelativeX(x, width);
   this->setRelativeY(y, height);
}

void
AbsolutePoint::setRelative
(double x, double y, SIZE size)
{
   this->setRelative(x, y, size.cx, size.cy);
}

void
AbsolutePoint::setRelative
(RelativePoint relative, long width, long height)
{
   this->setRelative(relative.getX(), relative.getY(), width, height);
}

void
AbsolutePoint::setRelative
(RelativePoint relative, SIZE size)
{
   this->setRelative(relative.getX(), relative.getY(), size.cx, size.cy);
}

void
AbsolutePoint::setX
(long x)
{
   this->x = x;
}

void
AbsolutePoint::setRelativeX
(double x, long width)
{
   this->x = AbsolutePoint::FromRelativeX(x, width);
}

void
AbsolutePoint::setY
(long y)
{
   this->y = y;
}

void
AbsolutePoint::setRelativeY
(double y, long height)
{
   this->y = AbsolutePoint::FromRelativeY(y, height);
}

long
AbsolutePoint::getX
(void) const
{
   return this->x;
}

double
AbsolutePoint::getRelativeX
(long width) const
{
   return RelativePoint::FromAbsoluteX(this->x, width);
}

long
AbsolutePoint::getY
(void) const
{
   return this->y;
}

double
AbsolutePoint::getRelativeY
(long height) const
{
   return RelativePoint::FromAbsoluteY(this->y, height);
}

POINT
AbsolutePoint::getPoint
(void) const
{
   POINT ret;
   
   ret.x = this->x;
   ret.y = this->y;

   return ret;
}

RelativePoint
AbsolutePoint::relative
(long width, long height) const
{
   RelativePoint ret;

   ret.setX(RelativePoint::FromAbsoluteX(this->x, width));
   ret.setY(RelativePoint::FromAbsoluteY(this->y, height));

   return ret;
}

RelativePoint
AbsolutePoint::relative
(SIZE size) const
{
   return this->relative(size.cx, size.cy);
}

RelativePoint::RelativePoint
(double x, double y)
   : Point(x, y)
{
}

RelativePoint::RelativePoint
(const RelativePoint &rel)
   : Point((Point &)rel)
{
}

RelativePoint::RelativePoint
(void)
   : Point()
{
}

RelativePoint
RelativePoint::FromAbsolute
(long x, long y, long width, long height)
{
   RelativePoint ret;

   ret.setX(RelativePoint::FromAbsoluteX(x, width));
   ret.setY(RelativePoint::FromAbsoluteY(y, height));

   return ret;
}

RelativePoint
RelativePoint::FromAbsolute
(POINT point, long width, long height)
{
   return RelativePoint::FromAbsolute(point.x, point.y, width, height);
}

RelativePoint
RelativePoint::FromAbsolute
(POINT point, SIZE size)
{
   return RelativePoint::FromAbsolute(point.x, point.y, size.cx, size.cy);
}

RelativePoint
RelativePoint::FromAbsolute
(AbsolutePoint point, long width, long height)
{
   return RelativePoint::FromAbsolute(point.getX(), point.getY(), width, height);
}

RelativePoint
RelativePoint::FromAbsolute
(AbsolutePoint point, SIZE size)
{
   return RelativePoint::FromAbsolute(point.getX(), point.getY(), size.cx, size.cy);
}

double
RelativePoint::FromAbsoluteX
(long x, long width)
{
   return (double)x / width;
}

double
RelativePoint::FromAbsoluteY
(long y, long height)
{
   return (double)y / height;
}

void
RelativePoint::setAbsolute
(long x, long y, long width, long height)
{
   this->setAbsoluteX(x, width);
   this->setAbsoluteY(y, height);
}

void
RelativePoint::setAbsolute
(long x, long y, SIZE size)
{
   this->setAbsolute(x, y, size.cx, size.cy);
}

void
RelativePoint::setAbsolute
(POINT point, long width, long height)
{
   this->setAbsolute(point.x, point.y, width, height);
}

void
RelativePoint::setAbsolute
(POINT point, SIZE size)
{
   this->setAbsolute(point.x, point.y, size.cx, size.cy);
}

void
RelativePoint::setAbsolute
(AbsolutePoint point, long width, long height)
{
   this->setAbsolute(point.getX(), point.getY(), width, height);
}

void
RelativePoint::setAbsolute
(AbsolutePoint point, SIZE size)
{
   this->setAbsolute(point.getX(), point.getY(), size.cx, size.cy);
}

void
RelativePoint::setAbsoluteX
(long x, long width)
{
   this->x = RelativePoint::FromAbsoluteX(x, width);
}

void
RelativePoint::setAbsoluteY
(long y, long height)
{
   this->y = RelativePoint::FromAbsoluteY(y, height);
}

long
RelativePoint::getAbsoluteX
(long width) const
{
   return AbsolutePoint::FromRelativeX(this->x, width);
}

long
RelativePoint::getAbsoluteY
(long height) const
{
   return AbsolutePoint::FromRelativeY(this->y, height);
}

AbsolutePoint
RelativePoint::absolute
(long width, long height) const
{
   AbsolutePoint ret;

   ret.setRelativeX(this->x, width);
   ret.setRelativeY(this->y, height);

   return ret;
}

AbsolutePoint
RelativePoint::absolute
(SIZE size) const
{
   return this->absolute(size.cx, size.cy);
}

MixedPoint::MixedPoint
(AbsolutePoint absolute, RelativePoint relative, SIZE size)
{
   this->setAbsolute(absolute);
   this->setRelative(relative);
   this->setSize(size);
}

MixedPoint::MixedPoint
(AbsolutePoint absolute, SIZE size)
{
   this->setAbsolute(absolute);
   this->setSize(size);
}

MixedPoint::MixedPoint
(RelativePoint relative, SIZE size)
{
   this->setRelative(relative);
   this->setSize(size);
}

MixedPoint::MixedPoint
(AbsolutePoint absolute)
{
   this->setAbsolute(absolute);
   this->setSize(0, 0);
}

MixedPoint::MixedPoint
(RelativePoint relative)
{
   this->setRelative(relative);
   this->setSize(0, 0);
}

MixedPoint::MixedPoint
(MixedPoint &mixed)
{
   this->setAbsolute(mixed.getAbsolute());
   this->setRelative(mixed.getRelative());
   this->setSize(mixed.getSize());
}

MixedPoint::MixedPoint
(void)
{
   this->setSize(0, 0);
}

void
MixedPoint::setAbsolute
(AbsolutePoint absolute)
{
   this->absolute = absolute;

   if (this->size.cx != 0 && this->size.cy != 0)
      this->relative = this->absolute.relative(this->size);
}

void
MixedPoint::setAbsolute
(long x, long y)
{
   this->absolute.setX(x);
   this->absolute.setY(y);

   if (this->size.cx != 0 && this->size.cy != 0)
      this->relative = this->absolute.relative(this->size);
}

void
MixedPoint::setAbsoluteX
(long x)
{
   this->absolute.setX(x);

   if (this->size.cx != 0)
      this->relative.setAbsoluteX(x, this->size.cx);
}

void
MixedPoint::setAbsoluteY
(long y)
{
   this->absolute.setY(y);

   if (this->size.cy != 0)
      this->relative.setAbsoluteY(y, this->size.cy);
}

AbsolutePoint
MixedPoint::getAbsolute
(void)
{
   return this->absolute;
}

long
MixedPoint::getAbsoluteX
(void)
{
   return this->absolute.getX();
}

long
MixedPoint::getAbsoluteY
(void)
{
   return this->absolute.getY();
}

void
MixedPoint::setRelative
(RelativePoint relative)
{
   this->relative = relative;

   if (this->size.cx != 0 && this->size.cy != 0)
      this->absolute = this->relative.absolute(this->size);
}

void
MixedPoint::setRelative
(double x, double y)
{
   this->relative.setX(x);
   this->relative.setY(y);

   if (this->size.cx != 0 && this->size.cy != 0)
      this->absolute = this->relative.absolute(this->size);
}

void
MixedPoint::setRelativeX
(double x)
{
   this->relative.setX(x);

   if (this->size.cx != 0)
      this->absolute.setRelativeX(x, this->size.cx);
}

void
MixedPoint::setRelativeY
(double y)
{
   this->relative.setY(y);

   if (this->size.cy != 0)
      this->absolute.setRelativeY(y, this->size.cy);
}

RelativePoint
MixedPoint::getRelative
(void)
{
   return this->relative;
}

double
MixedPoint::getRelativeX
(void)
{
   return this->relative.getX();
}

double
MixedPoint::getRelativeY
(void)
{
   return this->relative.getY();
}

void
MixedPoint::setSize
(SIZE size)
{
   this->size = size;
}

void
MixedPoint::setSize
(long width, long height)
{
   this->size.cx = width;
   this->size.cy = height;
}

SIZE
MixedPoint::getSize
(void)
{
   return this->size;
}
