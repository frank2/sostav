#include "sostav/chiptunes/mikmodmodule.hpp"

using namespace Sostav;
using namespace Sostav::Chiptunes;

MikModModuleException::MikModModuleException
(const WCHAR *what)
   : Exception(what)
{
}

MikModModule::MikModModule
(LPWSTR resourceName, std::wstring resourceType)
{
   this->bufferSize = 0;
   this->bufferData = NULL;
   this->playThread = NULL;
   this->stopEvent = NULL;
   
   this->loadResource(resourceName, resourceType);
}

MikModModule::MikModModule
(std::wstring filename)
{
   this->bufferSize = 0;
   this->bufferData = NULL;
   this->playThread = NULL;
   this->stopEvent = NULL;

   this->loadFilename(filename);
}

MikModModule::MikModModule
(LPBYTE bufferData, size_t bufferSize)
{
   this->bufferSize = 0;
   this->bufferData = NULL;
   this->playThread = NULL;
   this->stopEvent = NULL;

   this->setBuffer(bufferData, bufferSize);
}

MikModModule::MikModModule
(MikModModule &module)
{
   this->playThread = NULL;
   this->stopEvent = NULL;
   this->setBuffer(module.getBuffer(), module.getBufferSize());
}

MikModModule::MikModModule
(void)
{
   this->bufferSize = 0;
   this->bufferData = NULL;
   this->playThread = NULL;
   this->stopEvent = NULL;
}

MikModModule::~MikModModule
(void)
{
   if (this->isPlaying())
      this->stop();

   if (this->bufferData != NULL)
      HeapFree(GetProcessHeap(), NULL, this->bufferData);
}

DWORD WINAPI
MikModModule::PlayThread
(LPVOID mikModModule)
{
   MikModModule *sostovModule = (MikModModule *)mikModModule;
   HANDLE mutex;
   DWORD mutexResult;
   MODULE *module;

   mutex = CreateMutex(NULL, FALSE, L"Sostav::Chiptunes::MikModModule::PlayThread");
   mutexResult = WaitForSingleObject(mutex, 0);

   if (mutexResult != WAIT_OBJECT_0 && mutexResult != WAIT_ABANDONED)
      return 0;

   MikMod_InitThreads();
   MikModDriver::Register();
   MikModLoader::Register();

   md_mode |= DMODE_SOFT_MUSIC | DMODE_NOISEREDUCTION | DMODE_HQMIXER;

   if (MikMod_Init(""))
      throw MikModModuleException(L"MikMod_Init failed");

   module = Player_LoadMem((const char *)sostovModule->getBuffer()
                           ,sostovModule->getBufferSize()
                           ,64
                           ,0);

   if (module == NULL)
      throw MikModModuleException(L"Player_LoadMem failed");

   Player_Start(module);

   while (Player_Active())
   {
      DWORD eventResult;

      if (sostovModule->shouldStop())
         break;

      MikMod_Update();
   }

   Player_Stop();

   Player_Free(module);

   MikMod_Exit();

   return 0;
}

void
MikModModule::setBuffer
(LPBYTE bufferData, size_t bufferSize)
{
   if (this->bufferData != NULL)
   {
      HeapFree(GetProcessHeap(), NULL, this->bufferData);
      this->bufferData = NULL;
   }

   this->bufferSize = bufferSize;
   this->bufferData = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);
   CopyMemory(this->bufferData, bufferData, bufferSize);
}

LPBYTE
MikModModule::getBuffer
(void) const
{
   return this->bufferData;
}

size_t
MikModModule::getBufferSize
(void) const
{
   return this->bufferSize;
}

void
MikModModule::loadResource
(LPWSTR resourceName, std::wstring resourceType)
{
   HRSRC resourceHandle;
   size_t resourceSize;
   HGLOBAL loadedHandle;
   LPBYTE loadedData;

   resourceHandle = FindResource(NULL, resourceName, resourceType.c_str());

   if (resourceHandle == NULL)
      throw MikModModuleException(L"FindResource failed");

   resourceSize = SizeofResource(NULL, resourceHandle);
   loadedHandle = LoadResource(NULL, resourceHandle);

   if (loadedHandle == NULL)
      throw MikModModuleException(L"LoadResource failed");

   loadedData = (LPBYTE)LockResource(loadedHandle);

   if (loadedData == NULL)
      throw MikModModuleException(L"LockResource failed");

   this->setBuffer(loadedData, resourceSize);
}

void
MikModModule::loadFilename
(std::wstring filename)
{
   HANDLE fileHandle;
   size_t fileSize;
   LPBYTE fileData;
   DWORD bytesRead;

   fileHandle = CreateFile(filename.c_str()
                           ,GENERIC_READ
                           ,NULL
                           ,NULL
                           ,OPEN_EXISTING
                           ,FILE_ATTRIBUTE_NORMAL
                           ,NULL);

   if (fileHandle == NULL)
      throw MikModModuleException(L"CreateFile failed");

   fileSize = GetFileSize(fileHandle, NULL);
   fileData = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, fileSize);

   if (!ReadFile(fileHandle, (LPVOID)fileData, fileSize, &bytesRead, NULL))
   {
      CloseHandle(fileHandle);
      HeapFree(GetProcessHeap(), NULL, fileData);
      throw MikModModuleException(L"ReadFile failed");
   }

   CloseHandle(fileHandle);

   this->setBuffer(fileData, fileSize);

   HeapFree(GetProcessHeap(), NULL, fileData);
}

bool
MikModModule::isPlaying
(void) const
{
   return this->playThread != NULL && WaitForSingleObject(this->playThread, 0) != WAIT_OBJECT_0;
}

bool
MikModModule::shouldStop
(void) const
{
   return this->stopEvent == NULL || WaitForSingleObject(this->stopEvent, 1000) == WAIT_OBJECT_0;
}

bool
MikModModule::isPaused
(void) const
{
   return this->isPlaying() && Player_Paused();
}

void
MikModModule::play
(void)
{
   if (this->isPlaying())
      throw MikModModuleException(L"module is already playing");
   
   this->stopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
   this->playThread = CreateThread(NULL
                                   ,8192
                                   ,MikModModule::PlayThread
                                   ,this
                                   ,NULL
                                   ,NULL);

   if (this->playThread == NULL)
      throw MikModModuleException(L"CreateThread failed");
}

void
MikModModule::stop
(void)
{
   if (!this->isPlaying())
      throw MikModModuleException(L"module is not playing");

   SetEvent(this->stopEvent);
   WaitForSingleObject(this->playThread, INFINITE);
   CloseHandle(this->stopEvent);

   this->playThread = NULL;
   this->stopEvent = NULL;
}

void
MikModModule::pause
(void)
{
   if (!this->isPlaying())
      throw MikModModuleException(L"module is not playing");

   Player_TogglePause();
}
