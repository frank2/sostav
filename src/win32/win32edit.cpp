#include "sostav/win32/edit.hpp"

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

EditException::EditException
(const WCHAR *what)
   : Exception(what)
{
}

Edit::Edit
(Window *parent)
   : SubclassedWindow(parent, L"EDIT")
{
   this->showCueOnFocus = false;
   this->limit = 0;
}

Edit::Edit
(Edit &window)
   : SubclassedWindow(window)
{
   this->setShowCueOnFocus(window.getShowCueOnFocus());
}

Edit::Edit
(void)
   : SubclassedWindow()
{
   this->setClassName(L"EDIT");
   this->showCueOnFocus = false;
}

void
Edit::setCueText
(std::wstring cueText)
{
   this->cueText.assign(cueText);

   if (this->hasHWND() && (BOOL)this->sendMessage(EM_SETCUEBANNER
                                                  ,(WPARAM)this->showCueOnFocus
                                                  ,(LPARAM)this->cueText.c_str()) == FALSE)
      throw WindowException(L"SendMessage failed");
}

std::wstring
Edit::getCueText
(void)
{
   if (this->hasHWND())
   {
      WCHAR *cueText;
      size_t cueLength = 128;
      int i;
      bool succeeded = false;

      /* EM_GETCUEBANNER provides no means to determine the length...
         so we have to guess. */
      
      for (i=0; i<10; ++i)
      {
         cueText = (WCHAR *)HeapAlloc(GetProcessHeap()
                                      ,HEAP_ZERO_MEMORY
                                      ,cueLength);

         if ((BOOL)SendMessage(this->hwnd
                               ,EM_GETCUEBANNER
                               ,(WPARAM)cueText
                               ,(LPARAM)cueLength) == TRUE)
         {
            succeeded = true;
            break;
         }

         HeapFree(GetProcessHeap(), NULL, cueText);
         cueLength *= 2;
      }

      if (!succeeded)
         throw EditException(L"failed to get cue banner");

      this->cueText.assign(cueText);

      HeapFree(GetProcessHeap(), NULL, cueText);
   }

   return this->cueText;
}
         
void
Edit::setShowCueOnFocus
(bool show)
{
   this->showCueOnFocus = show;

   if (this->hasHWND())
      this->invalidate();
}

bool
Edit::getShowCueOnFocus
(void) const
{
   return this->showCueOnFocus;
}

void
Edit::setLimit
(DWORD limit)
{
   if (this->hasHWND())
      this->sendMessage(EM_SETLIMITTEXT
                        ,(WPARAM)this->limit
                        ,NULL);

   this->limit = limit;
}

DWORD
Edit::getLimit
(void)
{
   if (this->hasHWND())
      this->limit = (DWORD)this->sendMessage(EM_GETLIMITTEXT
                                             ,NULL
                                             ,NULL);

   return this->limit;
}

void
Edit::postCreate
(void)
{
   SubclassedWindow::postCreate();

   if (this->cueText.length() > 0)
      this->sendMessage(EM_SETCUEBANNER
                        ,(WPARAM)this->showCueOnFocus
                        ,(LPARAM)this->cueText.c_str());

   this->sendMessage(EM_SETLIMITTEXT
                     ,(WPARAM)this->limit
                     ,NULL);
}
