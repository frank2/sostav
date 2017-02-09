#pragma once

#include <windows.h>

#include <mikmod.h>

#include <sostav/exception.hpp>

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
         MLOADER loader;
         HANDLE playThread;
         HANDLE stopEvent;
         MODULE *loadedModule;

      public:
         MikModModule(MLOADER loader);
         MikModModule(MikModModule &module);
         MikModModule();
         ~MikModModule();

         const static WCHAR *PlayMutexName = L"Local\\MikModModule(PlayThread)";
         static int Instances;
         
         static DWORD WINAPI PlayThread(LPVOID mikModModule);

         void setLoader(MLOADER loader);
         MLOADER getLoader(void) const;

         HANDLE getStopEvent(void) const;

         void loadResource(LPWSTR resourceName, std::wstring resourceType);
         void loadFilename(std::wstring filename);
         void loadData(LPBYTE bufferData, size_t bufferSize);

         bool isPlaying(void) const;
         bool shouldStop(void) const;
         bool isPaused(void) const;

         void play(void);
         void stop(void);
         void pause(void);
      };
   }
}
