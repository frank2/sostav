#include "sostav/drawing/font.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

FontException::FontException
(const char *what)
   : Exception(what)
{
}

Font::Font
(std::wstring face, LONG points)
{
   memset(&this->fontData, 0, sizeof(LOGFONT));

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
   memset(&this->fontData, 0, sizeof(LOGFONT));
   this->fontHandle = NULL;
}

Font::~Font
(void)
{
   if (this->fontHandle != NULL)
      DeleteObject(this->fontHandle);
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
      throw FontException("font face too large for font structure");

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

HFONT
Font::getHandle
(void)
{
   if (this->fontHandle == NULL)
      this->fontHandle = CreateFontIndirect(&this->fontData);

   if (this->fontHandle == NULL)
      throw FontException("CreateFontIndirect failed");

   return this->fontHandle;
}
