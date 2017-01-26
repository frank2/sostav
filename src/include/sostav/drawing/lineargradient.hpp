#pragma once

#include <windows.h>

#include "sostav/exception.hpp"
#include "sostav/drawing/color.hpp"
#include "sostav/drawing/gradient.hpp"
#include "sostav/drawing/point.hpp"

namespace Sostav
{
   namespace Drawing
   {
      class LinearGradientException : public Exception
      {
      public:
         LinearGradientException(const char *what);
      };

      class LinearGradient : public Gradient
      {
      public:
         LinearGradient(std::map<double, Color> modes, Color base);
         LinearGradient(LinearGradient &gradient);
         LinearGradient();

         void setLinearModes(std::map<double, Color> modes);
         std::map<double, Color> getLinearModes(void) const;

         void addLinearColor(double key, Color color);
         void removeLinearColor(double key);
         void moveLinearColor(double key, double newKey);

         virtual Color formula(double x, double y);
