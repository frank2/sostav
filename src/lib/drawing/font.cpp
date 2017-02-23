#include "sostav/drawing/font.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

FontResources FontResources::instance;

FontException::FontException
(const WCHAR *what)
   : Exception(what)
{
}

Font::Font
(std::wstring face, LONG points)
{
   ZeroMemory(&this->fontData, sizeof(LOGFONT));

   this->setFace(face);
   this->setPointSize(points);

   this->fontHandle = NULL;
}

Font::Font
(std::wstring face, LONG height, LONG width)
{
   memset(&this->fontData, 0, sizeof(LOGFONT));

   this->setFace(face);
   this->setHeight(height);
   this->setWidth(width);

   this->fontHandle = NULL;
}

Font::Font
(const Font &font)
{
   this->setLogFont(font.getLogFont());
   this->fontHandle = NULL;
}

Font::Font
(void)
{
   ZeroMemory(&this->fontData, sizeof(LOGFONT));
   this->fontHandle = NULL;
}

Font::~Font
(void)
{
   if (this->fontHandle != NULL)
      DeleteObject(this->fontHandle);
}

Font
Font::CaptionFont
(void)
{
   return Font::DefaultFont(Font::SystemFont::SF_CAPTION);
}

Font
Font::SmallCaptionFont
(void)
{
   return Font::DefaultFont(Font::SystemFont::SF_SMCAPTION);
}

Font
Font::MenuFont
(void)
{
   return Font::DefaultFont(Font::SystemFont::SF_MENU);
}

Font
Font::StatusFont
(void)
{
   return Font::DefaultFont(Font::SystemFont::SF_STATUS);
}

Font
Font::MessageFont
(void)
{
   return Font::DefaultFont(Font::SystemFont::SF_MESSAGE);
}

Font
Font::DefaultFont
(Font::SystemFont fontName)
{
   NONCLIENTMETRICS metrics;
   Font result;

   ZeroMemory(&metrics, sizeof(NONCLIENTMETRICS));

   metrics.cbSize = sizeof(NONCLIENTMETRICS);

   if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS
                             ,sizeof(NONCLIENTMETRICS)
                             ,(LPVOID)&metrics
                             ,NULL))
      throw FontException(L"SystemParametersInfo failed");

   switch(fontName)
   {
   case Font::SystemFont::SF_CAPTION:
      result.setLogFont(metrics.lfCaptionFont);
      break;
      
   case Font::SystemFont::SF_SMCAPTION:
      result.setLogFont(metrics.lfSmCaptionFont);
      break;
      
   case Font::SystemFont::SF_MENU:
      result.setLogFont(metrics.lfMenuFont);
      break;
      
   case Font::SystemFont::SF_STATUS:
      result.setLogFont(metrics.lfStatusFont);
      break;
      
   case Font::SystemFont::SF_MESSAGE:
      result.setLogFont(metrics.lfMessageFont);
      break;

   default:
      throw FontException(L"no such default font found");
   }

   return result;
}

void
Font::setLogFont
(LOGFONT logFont)
{
   memcpy_s(&this->fontData, sizeof(LOGFONT), &logFont, sizeof(LOGFONT));

   if (this->fontHandle != NULL)
   {
      DeleteObject(this->fontHandle);
      this->fontHandle = NULL;
   }
}

LOGFONT
Font::getLogFont
(void) const
{
   return this->fontData;
}

void
Font::setHeight
(LONG height)
{
   this->fontData.lfHeight = height;

   if (this->fontHandle != NULL)
   {
      DeleteObject(this->fontHandle);
      this->fontHandle = NULL;
   }
}

LONG
Font::getHeight
(void) const
{
   return this->fontData.lfHeight;
}

void
Font::setWidth
(LONG width)
{
   this->fontData.lfWidth = width;

   if (this->fontHandle != NULL)
   {
      DeleteObject(this->fontHandle);
      this->fontHandle = NULL;
   }
}

LONG
Font::getWidth
(void) const
{
   return this->fontData.lfWidth;
}

void
Font::setWeight
(LONG weight)
{
   this->fontData.lfWeight = weight;

   if (this->fontHandle != NULL)
   {
      DeleteObject(this->fontHandle);
      this->fontHandle = NULL;
   }
}

LONG
Font::getWeight
(void) const
{
   return this->fontData.lfWeight;
}

void
Font::toggleItalic
(void)
{
   this->fontData.lfItalic ^= TRUE;
}

BOOL
Font::getItalic
(void) const
{
   return this->fontData.lfItalic;
}

void
Font::toggleUnderline
(void)
{
   this->fontData.lfUnderline ^= TRUE;
}

BOOL
Font::getUnderline
(void) const
{
   return this->fontData.lfUnderline;
}

void
Font::toggleStrikeOut
(void)
{
   this->fontData.lfStrikeOut ^= TRUE;
}

BOOL
Font::getStrikeOut
(void) const
{
   return this->fontData.lfStrikeOut;
}

void
Font::setPointSize
(LONG points)
{
   HDC screen;

   screen = GetDC(NULL);

   this->fontData.lfHeight = -MulDiv(points
                                     ,GetDeviceCaps(screen, LOGPIXELSY)
                                     ,72);

   ReleaseDC(NULL, screen);

   if (this->fontHandle != NULL)
   {
      DeleteObject(this->fontHandle);
      this->fontHandle = NULL;
   }
}

void
Font::setFace
(std::wstring face)
{
   if (face.size() >= LF_FACESIZE)
      throw FontException(L"font face too large for font structure");

   memset(&this->fontData.lfFaceName, 0, sizeof(this->fontData.lfFaceName));
   wcscpy_s(this->fontData.lfFaceName, LF_FACESIZE, face.c_str());

   if (this->fontHandle != NULL)
   {
      DeleteObject(this->fontHandle);
      this->fontHandle = NULL;
   }
}

std::wstring
Font::getFace
(void) const
{
   return std::wstring(this->fontData.lfFaceName);
}

void
Font::setHandle
(HFONT font)
{
   LOGFONT logFont;

   if (!GetObject(font, sizeof(LOGFONT), (LPVOID)&logFont))
      throw FontException(L"GetObject failed");

   this->setLogFont(logFont);
   this->fontHandle = font;
}

HFONT
Font::getHandle
(void)
{
   if (this->fontHandle == NULL)
      this->fontHandle = CreateFontIndirect(&this->fontData);

   if (this->fontHandle == NULL)
      throw FontException(L"CreateFontIndirect failed");

   return this->fontHandle;
}

FontResources::FontResources
()
{
}

FontResources::~FontResources
()
{
   for (std::list<HANDLE>::iterator iter=this->resources.begin();
        iter!=this->resources.end();
        ++iter)
      RemoveFontMemResourceEx(*iter);

   for (std::list<std::pair<std::wstring, DWORD > >::iterator iter=this->files.begin();
        iter!=this->files.end();
        ++iter)
      RemoveFontResourceEx(iter->first.c_str(), iter->second, NULL);
}
      
FontResources *
FontResources::GetInstance
(void)
{
   return &FontResources::instance;
}

int
FontResources::loadFile
(std::wstring filename, DWORD flags)
{
   int result = AddFontResourceEx(filename.c_str(), flags, NULL);

   if (result == 0)
      throw FontException(L"AddFontResourceEx failed");

   this->files.push_back(std::pair<std::wstring, DWORD>(filename, flags));
}

int
FontResources::loadMemory
(LPVOID buffer, size_t size)
{
   DWORD fonts;
   HANDLE result = AddFontMemResourceEx(buffer, size, NULL, &fonts);

   if (result == NULL)
      throw FontException(L"AddFontMemResourceEx failed");

   this->resources.push_back(result);

   return fonts;
}

int
FontResources::loadResource
(LPWSTR resourceName, std::wstring resourceType)
{
   HRSRC resourceHandle;
   HGLOBAL resourceLock;
   size_t resourceSize;
   LPVOID resourceData;

   resourceHandle = FindResource(NULL, resourceName, resourceType.c_str());

   if (resourceHandle == NULL)
      throw FontException(L"font resource not found");

   resourceSize = SizeofResource(NULL, resourceHandle);
   resourceLock = LoadResource(NULL, resourceHandle);
   resourceData = LockResource(resourceLock);

   return this->loadMemory(resourceData, resourceSize);
}
