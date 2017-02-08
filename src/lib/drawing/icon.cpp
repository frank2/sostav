#include "sostav/drawing/icon.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

IconException::IconException
(const WCHAR *what)
   : Exception(what)
{
}

Icon::Icon
(LPWSTR iconFile)
{
   ZeroMemory(&this->iconInfo, sizeof(ICONINFO));
              
   if (IS_INTRESOURCE(iconFile))
   {
      switch((UINT)iconFile)
      {
      case 32512: // IDI_APPLICATION
      case 32516: // IDI_ASTERISK
      case 32513: // IDI_ERROR
      case 32515: // IDI_EXCLAMATION
      case 32514: // IDI_QUESTION
      case 32518: // IDI_SHIELD
      case 32517: // IDI_WINLOGO
         this->loadOEM((UINT)iconFile);

      default:
         this->loadResource(iconFile);
      }
   }
   else if (FindResource(NULL, iconFile, RT_GROUP_ICON) != NULL)
      this->loadResource(iconFile);
   else
      this->loadFile(iconFile);
}

Icon::Icon
(const Icon &icon)
{
   ZeroMemory(&this->iconInfo, sizeof(ICONINFO));

   if (icon.hasHandle())
      this->setHandle(icon.getHandle());
   else
      this->setIconInfo(icon.getIconInfo());
}

Icon::Icon
(void)
{
   ZeroMemory(&this->iconInfo, sizeof(ICONINFO));
   this->iconHandle = NULL;
}

Icon::~Icon
(void)
{
   if (this->hasHandle())
      DestroyIcon(this->iconHandle);
   else if (this->hasInfo())
   {
      if (this->iconInfo.hbmMask != NULL)
         DeleteObject(this->iconInfo.hbmMask);

      if (this->iconInfo.hbmColor != NULL)
         DeleteObject(this->iconInfo.hbmColor);
   }
}

Icon
Icon::Application
(void)
{
   return Icon(IDI_APPLICATION);
}

Icon
Icon::Asterisk
(void)
{
   return Icon(IDI_ASTERISK);
}

Icon
Icon::Error
(void)
{
   return Icon(IDI_ERROR);
}

Icon
Icon::Exclamation
(void)
{
   return Icon(IDI_EXCLAMATION);
}

Icon
Icon::Question
(void)
{
   return Icon(IDI_QUESTION);
}

Icon
Icon::Shield
(void)
{
   return Icon(IDI_SHIELD);
}

Icon
Icon::WinLogo
(void)
{
   return Icon(IDI_WINLOGO);
}

Icon
Icon::MainIcon
(void)
{
   return Icon(L"MAINICON");
}

bool
Icon::hasHandle
(void) const
{
   return (this->iconHandle != NULL);
}

bool
Icon::hasInfo
(void) const
{
   return (this->iconInfo.hbmColor != NULL);
}

void
Icon::setHandle
(HICON iconHandle)
{
   if (iconHandle != NULL)
      this->iconHandle = CopyIcon(iconHandle);
   else
      this->iconHandle = NULL;

   if (iconHandle != NULL && !GetIconInfo(this->iconHandle, &this->iconInfo))
      throw IconException(L"GetIconInfo failed");
}

HICON
Icon::getHandle
(void) const
{
   return this->iconHandle;
}

void
Icon::setIconInfo
(ICONINFO iconInfo)
{
   this->iconInfo = iconInfo;

   if (this->hasHandle())
   {
      DestroyIcon(this->iconHandle);
      this->iconHandle = NULL;
   }
}

ICONINFO
Icon::getIconInfo
(void) const
{
   return this->iconInfo;
}

void
Icon::loadOEM
(UINT resourceID)
{
   this->loadIcon(NULL, MAKEINTRESOURCE(resourceID), LR_DEFAULTCOLOR);
}

void
Icon::loadResource
(LPWSTR resourceID)
{
   this->loadIcon(GetModuleHandle(NULL), resourceID, LR_DEFAULTCOLOR);
}

void
Icon::loadFile
(std::wstring filename)
{
   this->loadIcon(NULL, (LPWSTR)filename.c_str(), LR_DEFAULTCOLOR | LR_LOADFROMFILE);
}

void
Icon::loadIcon
(HINSTANCE instance, LPWSTR iconFile, UINT loadFlags)
{
   HANDLE result;

   result = LoadImage(instance
                      ,iconFile
                      ,IMAGE_ICON
                      ,GetSystemMetrics(SM_CXICON)
                      ,GetSystemMetrics(SM_CYICON)
                      ,loadFlags);

   if (result == NULL)
      throw IconException(L"LoadImage failed");

   this->setHandle((HICON)result);

   /* we've copied the icon now, so be diligent and destroy the result of LoadImage */
   DestroyIcon((HICON)result);
}

SmallIcon::SmallIcon
(LPWSTR iconFile)
   : Icon(iconFile)
{
}

SmallIcon::SmallIcon
(SmallIcon &icon)
   : Icon(icon)
{
}

SmallIcon::SmallIcon
(void)
   : Icon()
{
}

void
SmallIcon::loadIcon
(HINSTANCE instance, LPWSTR iconFile, UINT loadFlags)
{
   HANDLE result;

   result = LoadImage(instance
                      ,iconFile
                      ,IMAGE_ICON
                      ,GetSystemMetrics(SM_CXSMICON)
                      ,GetSystemMetrics(SM_CYSMICON)
                      ,loadFlags);

   if (result == NULL)
      throw IconException(L"LoadImage failed");

   this->setHandle((HICON)result);

   /* we've copied the icon now, so be diligent and destroy the result of LoadImage */
   DestroyIcon((HICON)result);
}
