/* -*- coding: utf-8 -*- */

#pragma once

#include <windows.h>

#include <sostav/sostav.hpp>

#include "resource.hpp"

#pragma comment(lib, "sostav")

class ChiptuneButton : public Sostav::Win32::Button
{
protected:
   Sostav::Chiptunes::MikModModule module;
   
public:
   ChiptuneButton(Sostav::Windows::Window *parent, Sostav::Chiptunes::MikModModule module);
   ChiptuneButton();

   void setModule(Sostav::Chiptunes::MikModModule module);

protected:
   virtual LRESULT onLButtonUp(WORD virtualKeys, WORD x, WORD y);
};

class ChiptuneSelector : public Sostav::Win32::Button
{
protected:
   ChiptuneButton impulseTracker, moduleButton, screamTracker3, fastTracker2, noMusic;
   Sostav::Win32::Static songTitle;
   Sostav::Chiptunes::MikModModule *module;
   
public:
   ChiptuneSelector(Sostav::Windows::Window *parent);
   ChiptuneSelector();

   void setModule(Sostav::Chiptunes::MikModModule *module);
   void play(void);
   void stop(void);

protected:
   virtual void preCreate(void);
};

class MainWindowPane : public Sostav::Windows::ImageDialogWindowPane
{
protected:
   Sostav::Windows::ImageWindow sostavBanner, jpgTest, gifTest, bmpTest;
   ChiptuneSelector chiptunes;

public:
   MainWindowPane();

protected:
   virtual void preCreate(void);
};

class MainWindow : public Sostav::Windows::ImageDialogWindowFrame
{
protected:
   Sostav::Win32::NotifyIcon trayIcon;

public:
   MainWindow(MainWindowPane *windowPane);

protected:
   virtual void preCreate(void);
   virtual void postCreate(void);
   virtual LRESULT onDestroy(void);
};
