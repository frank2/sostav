#pragma once

#include <windows.h>

#include <string>

#include "sostav/exception.hpp"
#include "sostav/windows/subclassedwindow.hpp"

/* these dependencies allow us to have cue banners */
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

namespace Sostav
{
   namespace Win32
   {
      class EditException : public Exception
      {
      public:
         EditException(const WCHAR *what);
      };

      class Edit : public Windows::SubclassedWindow
      {
      protected:
         std::wstring cueText;
         bool showCueOnFocus;
         DWORD limit;
         
      public:
         Edit(Windows::Window *parent);
         Edit(Edit &window);
         Edit();

         void setCueText(std::wstring cueText);
         std::wstring getCueText(void);

         void setShowCueOnFocus(bool show);
         bool getShowCueOnFocus(void) const;

         void setLimit(DWORD limit);
         DWORD getLimit(void);

         virtual void postCreate(void);
      };
   }
}
