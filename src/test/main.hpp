#pragma once

#include <windows.h>

#include <algorithm>

#include <sostav/sostav.hpp>

#include "resource.hpp"

#pragma comment(lib, "sostav")

class WindowTest : public Sostav::Windows::Window
{
protected:
   Sostav::Windows::ImageWindow sostavBanner;
   Sostav::Win32::Static staticText;
   Sostav::Win32::Edit edit;
   
   /*
   Sostav::Win32::Button button;
   */

public:
   WindowTest();
   ~WindowTest();
};
