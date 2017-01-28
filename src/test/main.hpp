#pragma once

#include <windows.h>

#include <algorithm>

#include <sostav/sostav.hpp>

#pragma comment(lib, "sostav")

class WindowTest : public Sostav::Windows::Window
{
protected:
   Sostav::Win32::Static staticText;
   Sostav::Win32::Button button;
   Sostav::Win32::Edit edit;

public:
   WindowTest();
   ~WindowTest();
};
