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
      public:
         MREADER callbacks;

      protected:
         MDRIVER driver;
         MLOADER loader;
         size_t bufferSize;
         LPBYTE bufferData;
         long bufferPosition;
         HANDLE playThread;
         HANDLE stopEvent;
         MODULE *loadedModule;

      public:
         MikModModule(MDRIVER driver, MLOADER loader);
         MikModModule(MikModModule &module);
         MikModModule();
         ~MikModModule();

         const static WCHAR *PlayMutexName = L"Local\\MikModModule(PlayThread)";
         static HANDLE PlayMutexHandle;
         
         static BOOL EOF(MREADER *reader);
         static BOOL Read(MREADER *reader, void *buffer, size_t size);
         static int Get(MREADER *reader);
         static int Seek(MREADER *reader, long offset, int whence);
         static long Tell(MREADER *reader);
         static DWORD WINAPI PlayThread(LPVOID mikModModule);

         void loadResource(LPWSTR resourceName, std::wstring resourceType);
         void loadFilename(std::wstring filename);
         void loadData(LPBYTE bufferData, size_t bufferSize);

         bool isPlaying(void) const;
         bool shouldStop(void) const;
         bool isPaused(void) const;

         void play(void);
         void stop(void);
