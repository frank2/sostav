#pragma once

#include <windows.hpp>

#include "sostav/color.hpp"
#include "sostav/exception.hpp"
#include "sostav/window.hpp"

namespace Sostav
{
   typedef Exception RectangleException;

   class Rectangle
   {
   protected:
      DWORD strokeSize;
      Color strokeColor;
