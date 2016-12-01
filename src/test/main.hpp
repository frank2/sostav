#pragma once

#include <windows.h>

#include <algorithm>

#include <sostav/sostav.hpp>

#pragma comment(lib, "sostav")

class GradientTest : public Sostav::Windows::LayeredImageWindow
{
protected:
   Sostav::Drawing::Gradient gradient;

public:
   GradientTest(HWND parent);
   GradientTest(Window *parent);
   GradientTest();

   virtual void preCreate(void);

protected:
   virtual void initialize(HWND parent, std::wstring className);
};
   
