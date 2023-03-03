#pragma once

#include <windows.h>

#include <list>
#include <string>

#include "sostav/exception.hpp"

namespace Sostav
{
   namespace Drawing
   {
      class FontException : public Exception
      {
      public:
         FontException(const WCHAR *what);
      };

      class Font
      {
      protected:
         HFONT fontHandle;
         LOGFONTW fontData;

      public:
         enum SystemFont
         {
            SF_CAPTION = 0
            ,SF_SMCAPTION
            ,SF_MENU
            ,SF_STATUS
            ,SF_MESSAGE
         };
         
         Font(std::wstring face, LONG points);
         Font(std::wstring face, LONG height, LONG width);
         Font(const Font &font);
         Font();
         ~Font();

         static Font CaptionFont(void);
         static Font SmallCaptionFont(void);
         static Font MenuFont(void);
         static Font StatusFont(void);
         static Font MessageFont(void);
         static Font DefaultFont(Font::SystemFont fontName);
         
         void setLogFont(LOGFONTW logFont);
         LOGFONTW getLogFont(void) const;

         void setHeight(LONG height);
         LONG getHeight(void) const;

         void setWidth(LONG width);
         LONG getWidth(void) const;

         void setWeight(LONG weight);
         LONG getWeight(void) const;

         void toggleItalic(void);
         BOOL getItalic(void) const;
         
         void toggleUnderline(void);
         BOOL getUnderline(void) const;
         
         void toggleStrikeOut(void);
         BOOL getStrikeOut(void) const;

         void setPointSize(LONG points);

         void setFace(std::wstring face);
         std::wstring getFace(void) const;

         void setHandle(HFONT font);
         HFONT getHandle(void);
      };

      class FontResources
      {
      protected:
         static FontResources instance;
         std::list<HANDLE> resources;
         std::list<std::pair<std::wstring,DWORD> > files;

         FontResources();
         ~FontResources();

      public:
         static FontResources *GetInstance(void);

         int loadFile(std::wstring filename, DWORD flags);
         int loadMemory(LPVOID buffer, size_t size);
         int loadResource(LPWSTR resourceName, std::wstring resourceType);
      };
   }
}
