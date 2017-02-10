#pragma once

#include <windows.h>

#include <mikmod.h>

#include <sostav/exception.hpp>
#include <sostav/chiptunes/mikmoddriver.hpp>
#include <sostav/chiptunes/mikmodloader.hpp>

#pragma comment(lib, "libmikmod")

namespace Sostav
{
   namespace Chiptunes
   {
      class MikModModuleException : public Exception
      {
      public:
         MikModModuleException(const WCHAR *what);
      };

      class MikModModule
      {
      protected:
         size_t bufferSize;
         LPBYTE bufferData;
         HANDLE playThread;
         HANDLE stopEvent;

      public:
         MikModModule(LPWSTR resourceName, std::wstring resourceType);
         MikModModule(std::wstring filename);
         MikModModule(LPBYTE bufferData, size_t bufferSize);
         MikModModule(MikModModule &module);
         MikModModule();
         ~MikModModule();
         
         static DWORD WINAPI PlayThread(LPVOID mikModModule);

         void setBuffer(LPBYTE bufferData, size_t bufferSize);
         LPBYTE getBuffer(void) const;
         size_t getBufferSize(void) const;
         
         void loadResource(LPWSTR resourceName, std::wstring resourceType);
         void loadFilename(std::wstring filename);

         bool isPlaying(void) const;
         bool shouldStop(void) const;
         bool isPaused(void) const;

         void play(void);
         void stop(void);
         void pause(void);
      };
   }
}
