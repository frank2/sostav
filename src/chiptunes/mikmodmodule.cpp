#include "sostav/chiptunes/mikmodmodule.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace Sostav;
using namespace Sostav::Chiptunes;

int MikModModule::Instances = 0;
bool MikModModule::Initialized = false;

MikModModuleException::MikModModuleException
(const WCHAR *what)
   : Exception(what)
{
}

MikModModule::MikModModule
(LPWSTR resourceName, std::wstring resourceType)
{
   MikModModule::Instances++;

   this->bufferData = NULL;
   this->bufferSize = 0;
   this->playThread = NULL;
   this->stopEvent = NULL;
   this->module = NULL;
   this->wrap = false;
   this->loop = true;
   
   this->setBufferFromResource(resourceName, resourceType);
}

MikModModule::MikModModule
(std::wstring filename)
{
   MikModModule::Instances++;

   this->bufferData = NULL;
   this->bufferSize = 0;
   this->playThread = NULL;
   this->stopEvent = NULL;
   this->module = NULL;
   this->wrap = false;
   this->loop = true;

   this->setBufferFromFilename(filename);
}

MikModModule::MikModModule
(LPBYTE bufferData, size_t bufferSize)
{
   MikModModule::Instances++;

   this->bufferData = NULL;
   this->bufferSize = 0;
   this->playThread = NULL;
   this->stopEvent = NULL;
   this->module = NULL;
   this->wrap = false;
   this->loop = true;

   this->setBuffer(bufferData, bufferSize);
}

MikModModule::MikModModule
(const MikModModule &module)
{
   MikModModule::Instances++;

   this->bufferData = NULL;
   this->bufferSize = 0;
   this->playThread = NULL;
   this->stopEvent = NULL;
   this->wrap = false;
   this->loop = true;
   
   this->setBuffer(module.getBuffer(), module.getBufferSize());
}

MikModModule::MikModModule
(void)
{
   MikModModule::Instances++;

   this->bufferData = NULL;
   this->bufferSize = 0;
   this->playThread = NULL;
   this->stopEvent = NULL;
   this->module = NULL;
   this->wrap = false;
   this->loop = true;
}

MikModModule::~MikModModule
(void)
{
   if (this->isPlaying())
      this->stop();

   MikModModule::Instances--;

   if (MikModModule::Instances == 0 && MikModModule::Initialized)
      this->shutdown();
}

DWORD WINAPI
MikModModule::PlayThread
(LPVOID mikModModule)
{
   MikModModule *sostavModule = (MikModModule *)mikModModule;
   HANDLE mutex;
   DWORD mutexResult;

   mutex = CreateMutexW(NULL, FALSE, L"Sostav::Chiptunes::MikModModule::PlayThread");
   mutexResult = WaitForSingleObject(mutex, 0);

   if (mutexResult != WAIT_OBJECT_0 && mutexResult != WAIT_ABANDONED)
      return 0;

   if (!sostavModule->isLoaded())
      sostavModule->load();

   Player_Start(sostavModule->getModule());
   Player_SetPosition(0);
   
   while (Player_Active())
   {
      if (sostavModule->shouldStop())
         break;

      MikMod_Update();
   }

   Player_Stop();
   sostavModule->unload();

   ReleaseMutex(mutex);

   return 0;
}

void
MikModModule::setBufferFromResource
(LPWSTR resourceName, std::wstring resourceType)
{
   HRSRC resourceHandle;
   size_t resourceSize;
   HGLOBAL loadedHandle;
   LPBYTE loadedData;

   resourceHandle = FindResourceW(NULL, resourceName, resourceType.c_str());

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
MikModModule::setBufferFromFilename
(std::wstring filename)
{
   HANDLE fileHandle;
   size_t fileSize;
   LPBYTE fileData;
   DWORD bytesRead;

   fileHandle = CreateFileW(filename.c_str()
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

void
MikModModule::setBuffer
(LPBYTE bufferData, size_t bufferSize)
{
   if (this->bufferData != nullptr)
   {
      this->bufferData = nullptr;
   }

   if (this->module != nullptr)
   {
      this->module = nullptr;
   }

   this->bufferData = std::shared_ptr<BYTE>(new BYTE[bufferSize]);
   CopyMemory(this->bufferData.get(), bufferData, bufferSize);

   this->bufferSize = bufferSize;
}

LPBYTE
MikModModule::getBuffer
(void) const
{
   return this->bufferData.get();
}

size_t
MikModModule::getBufferSize
(void) const
{
   return this->bufferSize;
}

MODULE *
MikModModule::getModule
(void) const
{
   return this->module.get();
}

void
MikModModule::setWrapping
(bool wrap)
{
   this->wrap = wrap;
}

bool
MikModModule::getWrapping
(void) const
{
   return this->wrap;
}

void
MikModModule::setLooping
(bool loop)
{
   this->loop = loop;
}

bool
MikModModule::getLooping
(void) const
{
   return this->loop;
}

bool
MikModModule::isLoaded
(void) const
{
   return this->module != nullptr;
}

void
MikModModule::load
(void)
{
   if (!MikModModule::Initialized)
      this->init();

   MODULE *module = Player_LoadMem((const char *)this->bufferData.get()
                                   ,this->bufferSize
                                   ,64
                                   ,0);
                    

   if (module == nullptr)
      throw MikModModuleException(L"Player_LoadMem failed");

   this->module = std::shared_ptr<MODULE>(module, MikModDeleter());
   this->module->wrap = static_cast<int>(this->wrap);
   this->module->loop = static_cast<int>(this->loop);
}

void
MikModModule::unload
(void)
{
   this->module = nullptr;
}

std::wstring
MikModModule::getSongName
(void) const
{
   std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;

   if (this->isLoaded())
      return converter.from_bytes(this->module->songname);
   else if (this->bufferData != nullptr)
   {
      char *title = Player_LoadTitleMem((const char *)this->bufferData.get(), this->bufferSize);

      return converter.from_bytes(title);
   }
   else
      throw MikModModuleException(L"no means to acquire title from module");
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

   Sleep(100);

   if (!this->isPlaying())
      throw MikModModuleException(L"play thread failed, is another song already playing?");
}

void
MikModModule::blockingPlay
(void)
{
   this->play();

   while (this->isPlaying())
      Sleep(1000);
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

void
MikModModule::init
(void)
{
   MikMod_InitThreads();

   if (MikModDriver::Drivers.empty())
      MikModDriver::LoadDefault();
   
   MikModDriver::Register();

   if (MikModLoader::Loaders.empty())
      MikModLoader::LoadDefault();

   MikModLoader::Register();

   md_mode |= DMODE_SOFT_MUSIC | DMODE_NOISEREDUCTION | DMODE_HQMIXER;

   if (MikMod_Init(""))
      throw MikModModuleException(L"MikMod_Init failed");

   MikModModule::Initialized = true;
}

void
MikModModule::shutdown
(void)
{
   MikMod_Exit();

   MikModModule::Initialized = false;
}
