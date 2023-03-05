#pragma once

#include <windows.h>

#include <mikmod.h>

#include <codecvt>
#include <locale>
#include <memory>
#include <string>

#include <sostav/exception.hpp>
#include <sostav/chiptunes/mikmoddriver.hpp>
#include <sostav/chiptunes/mikmodloader.hpp>

namespace Sostav
{
   namespace Chiptunes
   {
      struct MikModDeleter
      {
         void operator()(MODULE *module) {
            Player_Free(module);
         }
      };
      
      class MikModModuleException : public Exception
      {
      public:
         MikModModuleException(const WCHAR *what);
      };

      class MikModModule
      {
      protected:
         size_t bufferSize;
         std::shared_ptr<BYTE> bufferData;
         HANDLE playThread;
         HANDLE stopEvent;
         std::shared_ptr<MODULE> module;

         static int Instances;
         static bool Initialized;

      public:
         MikModModule(LPWSTR resourceName, std::wstring resourceType);
         MikModModule(std::wstring filename);
         MikModModule(LPBYTE bufferData, size_t bufferSize);
         MikModModule(const MikModModule &module);
         MikModModule();
         ~MikModModule();
         
         static DWORD WINAPI PlayThread(LPVOID mikModModule);

         void setBufferFromResource(LPWSTR resourceName, std::wstring resourceType);
         void setBufferFromFilename(std::wstring filename);
         void setBuffer(LPBYTE bufferData, size_t bufferSize);
         LPBYTE getBuffer(void) const;
         size_t getBufferSize(void) const;

         MODULE *getModule(void) const;

         bool isLoaded(void) const;
         void load(void);

         std::wstring getSongName(void) const;

         bool isPlaying(void) const;
         bool shouldStop(void) const;
         bool isPaused(void) const;

         void play(void);
         void blockingPlay(void);
         void stop(void);
         void pause(void);

      protected:
         void init(void);
         void shutdown(void);
      };
   }
}
