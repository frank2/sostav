#include "sostav/win32/button.hpp"

#include <windows.h>

using namespace Sostav;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

ButtonException::ButtonException
(const WCHAR *what)
   : Exception(what)
{
}

Button::Button
(Window *parent)
   : SubclassedWindow(parent, L"BUTTON")
{
}

Button::Button
(Button &window)
   : SubclassedWindow(window)
{
}

Button::Button
(void)
   : SubclassedWindow()
{
   this->setClassName(L"BUTTON");
}

bool
Button::checked
(void)
{
   /* TODO fix this to store the state when BM_SETCHECK is sent */
   if (!this->hasHWND())
      return false;

   return this->sendMessage(BM_GETCHECK, NULL, NULL) == BST_CHECKED;
}

LRESULT
Button::onKeyUp
(DWORD keyValue, DWORD keyFlags)
{
   switch (keyValue)
   {
   case VK_SPACE:
   case VK_RETURN:
   {
      this->sendMessage(BM_CLICK, NULL, NULL);

      return (LRESULT)0;
   }
   }

   return SubclassedWindow::onKeyUp(keyValue, keyFlags);
}

LRESULT
Button::onLButtonUp
(WORD virtualKeys, WORD x, WORD y)
{
   /* subclassing a button means a lot of its features don't work correctly. reimplement them here. */
   if (this->hasStyle(BS_AUTOCHECKBOX))
   {
      LRESULT checked = this->sendMessage(BM_GETCHECK, NULL, NULL);

      if (checked == BST_CHECKED)
         this->sendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED, NULL);
      else
         this->sendMessage(BM_SETCHECK, (WPARAM)BST_UNCHECKED, NULL);
   }
   else if (this->hasStyle(BS_AUTORADIOBUTTON))
   {
      std::list<Window *> children = this->parent->getChildren();
      std::list<Window *>::reverse_iterator groupSearch, searchRoot;
      bool found = false;

      groupSearch = std::find(children.rbegin(), children.rend(), this);

      if (groupSearch == children.rend())
         throw WindowException(L"couldn't find self in parent child list");

      searchRoot = groupSearch;

      do
      {
         Window *windowCheck = *groupSearch;

         if (windowCheck->hasStyle(BS_AUTORADIOBUTTON | WS_GROUP))
         {
            found = true;
            break;
         }

         groupSearch++;

         if (groupSearch == children.rend())
            groupSearch = children.rbegin();
      } while (searchRoot != groupSearch);

      if (found)
      {
         std::list<Window *>::iterator groupStart, groupIter;

         groupStart = std::find(children.begin(), children.end(), *groupSearch);
         groupIter = groupStart;

         do
         {
            Window *windowCheck = *groupIter;

            if (groupIter != groupStart && windowCheck->hasStyle(BS_AUTORADIOBUTTON | WS_GROUP))
               break;

            if (windowCheck->hasStyle(BS_AUTORADIOBUTTON))
               windowCheck->sendMessage(BM_SETCHECK, (WPARAM)BST_UNCHECKED, NULL);

            groupIter++;

            if (groupIter == children.end())
               groupIter = children.begin();
         } while (groupIter != groupStart);
      }

      this->sendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED, NULL);
   }
   else if (this->hasStyle(BS_PUSHBUTTON) && this->parent != NULL)
      this->parent->sendMessage(WM_COMMAND, MAKEWPARAM(this->menu, 0), (LPARAM)this->hwnd);

   return SubclassedWindow::onLButtonUp(virtualKeys, x, y);
}
