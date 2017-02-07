#pragma once

#include <windows.h>
#include <windowsx.h>

#include <string>

#include <sostav/exception.hpp>
#include <sostav/windows/window.hpp>

namespace Sostav
{
   namespace Win32
   {
      class NotifyIconException : public Exception
      {
      public:
         NotifyIconException(const WCHAR *what);
      };

      class NotifyIcon : public Windows::Window
      {
      protected:
         NOTIFYICONDATA iconData;

      public:
         const static UINT Callback = WM_APP+1000;
         
         NotifyIcon(Window *parent, HICON icon);
         NotifyIcon(NotifyIcon &window);
         NotifyIcon();

         virtual LRESULT windowProc(UINT msg, WPARAM wParam, LPARAM lParam);

         void setNotifyIconData(NOTIFYICONDATA data);
         NOTIFYICONDATA getNotifyIconData(void) const;

         void setID(UINT id);
         UINT getID(void) const;

         void setIcon(HICON icon);
         HICON getIcon(void) const;

         void setToolTip(std::wstring toolTip);
         std::wstring getToolTip(void) const;

         void setState(DWORD state);
         DWORD getState(void) const;

         void setStateMask(DWORD stateMask);
         DWORD getStateMask(void) const;

         void setInfo(std::wstring info);
         std::wstring getInfo(void) const;

         void setTimeout(UINT timeout);
         UINT getTimeout(void) const;

         void setInfoTitle(std::wstring infoTitle);
         std::wstring getInfoTitle(void) const;

         void setInfoFlags(DWORD flags);
         DWORD getInfoFlags(void) const;
         void addInfoFlag(DWORD flag);
         void removeInfoFlag(DWORD flag);

         void setGUID(GUID guid);
         GUID getGUID(void) const;

         void setBalloonIcon(HICON icon);
         HICON getBalloonIcon(void) const;

         virtual void update(void);

      protected:
         virtual void preCreate(void);
         virtual void postCreate(void);
         virtual void onCallback(UINT event, UINT iconID, DWORD x, DWORD y);
         virtual LRESULT onDestroy(void);
      };
   }
}
