#pragma once

#include <windows.h>

#include <sostav/sostav.hpp>

#include "resource.hpp"

#pragma comment(lib, "sostav")

class WindowTest : public Sostav::Windows::Window
{
protected:
   bool firstLaunch;
   
   Sostav::Windows::ImageWindow sostavBanner;
   Sostav::Win32::Static enabledStatic, disabledStatic;
   Sostav::Win32::Edit enabledEdit, disabledEdit;
   Sostav::Win32::Button enabledPushButton, disabledPushButton;
   Sostav::Win32::Button enabledRadioButton, disabledRadioButton;
   Sostav::Win32::Button enabledCheckButton, disabledCheckButton;
   
   Sostav::Win32::NotifyIcon trayIcon;

public:
   WindowTest();

protected:
   virtual LRESULT onShowWindow(BOOL show, WORD status);
   virtual LRESULT onDestroy(void);
};
