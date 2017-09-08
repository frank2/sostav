#include "sostav/win32/notifyicon.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

NotifyIconException::NotifyIconException
(const WCHAR *what)
   : Exception(what)
{
}

NotifyIcon::NotifyIcon
(Window *parent, Icon icon)
   : Window(parent, L"SvNotifyIcon")
{
   ZeroMemory(&this->iconData, sizeof(NOTIFYICONDATA));
   this->iconData.cbSize = sizeof(NOTIFYICONDATA);

   this->setIcon(icon);
}

NotifyIcon::NotifyIcon
(NotifyIcon &window)
   : Window(window)
{
   ZeroMemory(&this->iconData, sizeof(NOTIFYICONDATA));
   this->iconData.cbSize = sizeof(NOTIFYICONDATA);
   
   this->setNotifyIconData(window.getNotifyIconData());
}

NotifyIcon::NotifyIcon
(void)
   : Window()
{
   ZeroMemory(&this->iconData, sizeof(NOTIFYICONDATA));
   this->iconData.cbSize = sizeof(NOTIFYICONDATA);
   
   this->setClassName(L"SvNotifyIcon");
}

LRESULT
NotifyIcon::windowProc
(UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch(msg)
   {
   case NotifyIcon::Callback:
      this->onCallback((UINT)lParam);
      return (LRESULT)0;

   default:
      return Window::windowProc(msg, wParam, lParam);
   }
}

void
NotifyIcon::addToTray
(void)
{
   this->iconData.hWnd = this->hwnd;

   if (!Shell_NotifyIcon(NIM_ADD, &this->iconData))
      throw NotifyIconException(L"Shell_NotifyIcon failed");
}

void
NotifyIcon::removeFromTray
(void)
{
   if (!Shell_NotifyIcon(NIM_DELETE, &this->iconData))
      throw NotifyIconException(L"Shell_NotifyIcon failed");
}

void
NotifyIcon::setNotifyIconData
(NOTIFYICONDATA data)
{
   this->iconData = data;
}

NOTIFYICONDATA
NotifyIcon::getNotifyIconData
(void) const
{
   return this->iconData;
}

void
NotifyIcon::setID
(UINT id)
{
   this->iconData.uID = id;
}

UINT
NotifyIcon::getID
(void) const
{
   return this->iconData.uID;
}

void
NotifyIcon::setIcon
(Icon icon)
{
   Window::setIcon(icon);

   this->iconData.hIcon = this->icon.getHandle();

   if (!icon.hasHandle())
      this->iconData.uFlags &= ~NIF_ICON;
   else
      this->iconData.uFlags |= NIF_ICON;
}

void
NotifyIcon::setToolTip
(std::wstring toolTip)
{
   if (toolTip.length() > 63)
      throw NotifyIconException(L"tool tip too long");

   if (toolTip.empty())
   {
      ZeroMemory(&this->iconData.szTip, sizeof(this->iconData.szTip));
      this->iconData.uFlags &= ~NIF_TIP;
   }
   else
   {
      wcscpy_s(this->iconData.szTip, 64, toolTip.c_str());
      this->iconData.uFlags |= NIF_TIP;
   }
}

std::wstring
NotifyIcon::getToolTip
(void) const
{
   return std::wstring(this->iconData.szTip);
}

void
NotifyIcon::setState
(DWORD state)
{
   this->iconData.dwState = state;

   if (state == 0)
      this->iconData.uFlags &= ~NIF_STATE;
   else
      this->iconData.uFlags |= NIF_STATE;
}

DWORD
NotifyIcon::getState
(void) const
{
   return this->iconData.dwState;
}

void
NotifyIcon::setStateMask
(DWORD stateMask)
{
   this->iconData.dwStateMask = stateMask;

   if (stateMask == 0)
      this->iconData.uFlags &= ~NIF_STATE;
   else
      this->iconData.uFlags |= NIF_STATE;
}

DWORD
NotifyIcon::getStateMask
(void) const
{
   return this->iconData.dwStateMask;
}

void
NotifyIcon::setInfo
(std::wstring info)
{
   if (info.length() > 255)
      throw NotifyIconException(L"info data too long");

   if (info.empty())
   {
      ZeroMemory(&this->iconData.szInfo, sizeof(this->iconData.szInfo));
      this->iconData.uFlags &= ~NIF_INFO;
   }
   else
   {
      wcscpy_s(this->iconData.szInfo, 256, info.c_str());
      this->iconData.uFlags |= NIF_INFO;
   }
}

std::wstring
NotifyIcon::getInfo
(void) const
{
   return std::wstring(this->iconData.szInfo);
}

void
NotifyIcon::setTimeout
(UINT timeout)
{
   this->iconData.uTimeout = timeout;
}

UINT
NotifyIcon::getTimeout
(void) const
{
   return this->iconData.uTimeout;
}

void
NotifyIcon::setInfoTitle
(std::wstring infoTitle)
{
   if (infoTitle.length() > 63)
      throw NotifyIconException(L"info title too long");

   if (infoTitle.empty())
   {
      ZeroMemory(&this->iconData.szInfoTitle, sizeof(this->iconData.szInfoTitle));
      this->iconData.uFlags &= ~NIF_INFO;
   }
   else
   {
      wcscpy_s(this->iconData.szInfoTitle, 64, infoTitle.c_str());
      this->iconData.uFlags |= NIF_INFO;
   }
}

std::wstring
NotifyIcon::getInfoTitle
(void) const
{
   return std::wstring(this->iconData.szInfoTitle);
}

void
NotifyIcon::setInfoFlags
(DWORD flags)
{
   this->iconData.dwInfoFlags = flags;
}

DWORD
NotifyIcon::getInfoFlags
(void) const
{
   return this->iconData.dwInfoFlags;
}

void
NotifyIcon::addInfoFlag
(DWORD flag)
{
   this->iconData.dwInfoFlags |= flag;
}

void
NotifyIcon::removeInfoFlag
(DWORD flag)
{
   this->iconData.dwInfoFlags &= ~flag;
}

void
NotifyIcon::setGUID
(GUID guid)
{
   this->iconData.guidItem = guid;
}

GUID
NotifyIcon::getGUID
(void) const
{
   return this->iconData.guidItem;
}

void
NotifyIcon::setBalloonIcon
(Icon icon)
{
   this->iconData.hBalloonIcon = icon.getHandle();

   if (this->iconData.hBalloonIcon == NULL)
      this->iconData.dwInfoFlags &= ~NIIF_USER;
   else
      this->iconData.dwInfoFlags |= NIIF_USER;
}

HICON
NotifyIcon::getBalloonIcon
(void) const
{
   return this->iconData.hBalloonIcon;
}

void
NotifyIcon::update
(void)
{
   /* UpdateWindow will have no effect on our hwnd, so don't worry about Window::update */
   if (!Shell_NotifyIcon(NIM_MODIFY, &this->iconData))
      throw NotifyIconException(L"Shell_NotifyIcon failed");

   this->clear();
}

void
NotifyIcon::clear
(void)
{
   this->iconData.uFlags = NIF_MESSAGE | NIF_ICON | (this->iconData.uFlags & NIF_TIP);
   ZeroMemory(this->iconData.szInfo, sizeof(this->iconData.szInfo));
   this->iconData.uTimeout = 0;
   ZeroMemory(this->iconData.szInfoTitle, sizeof(this->iconData.szInfoTitle));
   this->iconData.dwInfoFlags = NIIF_NONE;
   this->iconData.hBalloonIcon = NULL;
}

void
NotifyIcon::preCreate
(void)
{
   this->iconData.uVersion = NOTIFYICON_VERSION_4;
   this->iconData.uCallbackMessage = NotifyIcon::Callback;
   this->iconData.uFlags |= NIF_MESSAGE;

   if (this->iconData.hIcon == NULL && this->parent != NULL)
      this->iconData.hIcon = this->parent->getIcon().getHandle();
}

void
NotifyIcon::postCreate
(void)
{
   Window::postCreate();
   this->addToTray();
}

void
NotifyIcon::onCallback
(UINT message)
{
   return;
}

LRESULT
NotifyIcon::onDestroy
(void)
{
   Window::onDestroy();
   this->removeFromTray();
   return (LRESULT)0;
}
