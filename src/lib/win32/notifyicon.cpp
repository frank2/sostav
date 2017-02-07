#include "sostav/win32/notifyicon.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

NotifyIconException::NotifyIconException
(const WCHAR *what)
   : Exception(what)
{
}

NotifyIcon::NotifyIcon
(Window *parent, HICON icon)
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
   
   this->setNotifyIconData(window.getNotifyIconData);
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
      this->onCallback(LOWORD(lParam)
                       ,HIWORD(lParam)
                       ,GET_X_LPARAM(wParam)
                       ,GET_Y_LPARAM(wParam));
      return (LRESULT)0;

   default:
      return Window::windowProc(msg, wParam, lParam);
   }
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
NotifyIcon::setNotifyIcon
(HICON icon)
{
   this->iconData.hIcon = icon;

   if (icon == NULL)
      this->iconData.uFlags &= ~NIF_ICON;
   else
      this->iconData.uFlags |= NIF_ICON;
}

HICON
NotifyIcon::getNotifyIcon
(void) const
{
   return this->iconData.hIcon;
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
(HICON icon)
{
   this->iconData.hBalloonIcon = icon;
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
}

void
NotifyIcon::preCreate
(void)
{
   this->iconData.uVersion = NOTIFYICON_VERSION_4;
   this->iconData.uCallbackMessage = NotifyIcon::Callback;

   if (this->iconData.hIcon == NULL && this->parent != NULL)
      this->iconData.hIcon = this->parent->getIcon();
}

void
NotifyIcon::postCreate
(void)
{
   /* the hwnd is mostly a facilitator for the callback message, so don't worry about
      the parent function */

   this->iconData.hWnd = this->hwnd;

   if (!Shell_NotifyIcon(NIM_ADD, &this->iconData))
      throw NotifyIconException(L"Shell_NotifyIcon failed");
}

void
NotifyIcon::onCallback
(UINT event, UINT iconID, DWORD x, DWORD y)
{
   return;
}

LRESULT
NotifyIcon::onDestroy
(void)
{
   Window::onDestroy();
   
   if (!Shell_NotifyIcon(NIM_DELETE, &this->iconData))
      throw NotifyIconException(L"Shell_NotifyIcon failed");

   return (LRESULT)0;
}
