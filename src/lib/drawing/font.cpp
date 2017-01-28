#include "sostav/drawing/font.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;

Font::Font
(std::wstring face, LONG points)
{
   memset(&this->fontData, 0, sizeof(LOGFONT));

   this->setFace(face);
   this->setPointSize(points);
}

Font::Font
(std::wstring face, LONG height, LONG width)
{
   memset(&this->fontData, 0, sizeof(LOGFONT));

   this->setFace(face);
   this->setHeight(height);
   this->setWidth(width);
}

Font::Font
(const Font &font)
{
   this->setLogFont(font.getLogFont());
}

Font::Font
(void)
{
   NONCLIENTMETRICS metrics;

   metrics.cbSize = sizeof(NONCLIENTMETRICS);

   if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS
                             ,sizeof(NONCLIENTMETRICS)
                             ,(LPVOID)&metrics
                             ,NULL))
      throw FontException("couldn't query system metrics");

   memset(&this->fontData, 0, sizeof(LOGFONT));
          
   this->setLogFont(metrics.lfMessageFont);
}

Font::~Font
(void)
{
}

void
Font::setLogFont
(LOGFONT logFont)
{
   memcpy_s(&this->fontData, sizeof(LOGFONT), &logFont, sizeof(LOGFONT));
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
}

void
Font::setFace
(std::wstring face)
{
   if (face.size() >= LF_FACESIZE)
      throw FontException("font face too large for font structure");

   memset(&this->fontData.lfFaceName, 0, sizeof(this->fontData.lfFaceName));
   wcscpy_s(this->fontData.lfFaceName, LF_FACESIZE, face.c_str());
}

std::wstring
Font::getFace
(void) const
{
   return std::wstring(this->fontData.lfFaceName);
}

HFONT
Font::getHandle
(void) const
{
   HFONT handle;

   handle = CreateFontIndirect(&this->fontData);

   if (handle == NULL)
      throw FontException("CreateFontIndirect failed");

   return handle;
}
