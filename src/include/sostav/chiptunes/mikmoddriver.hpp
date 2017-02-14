#pragma once

#include <windows.h>

#include <set>

#include <mikmod.h>

#include <sostav/exception.hpp>

#pragma comment(lib, "libmikmod")

/* also include the audio driver stuff */
#pragma comment(lib, "dsound")

namespace Sostav
{
   namespace Chiptunes
   {
      class MikModDriverException : public Exception
      {
      public:
         MikModDriverException(const WCHAR *what);
      };

      class MikModDriver
      {
      protected:
         MDRIVER *driver;

      public:
         static std::set<MikModDriver> Drivers;
         
         MikModDriver(MDRIVER *driver);
         MikModDriver(const MikModDriver &driver);
         MikModDriver();

         static MikModDriver Raw(void);
         static MikModDriver StdOut(void);
         static MikModDriver WAV(void);
         static MikModDriver AIFF(void);
         static MikModDriver DirectSound(void);
         static MikModDriver XAudio2(void);
         static MikModDriver MultiMediaAPI(void);
         
         static void Load(MikModDriver driver);
         static void LoadDefault(void);
         static void Unload(MikModDriver driver);
         static void Register(void);

         bool operator<(const MikModDriver &b) const;
         bool operator==(const MikModDriver &b) const;
         bool operator!=(const MikModDriver &b) const;
         bool operator>(const MikModDriver &b) const;
         bool operator<=(const MikModDriver &b) const;
         bool operator>=(const MikModDriver &b) const;

         void setDriver(MDRIVER *driver);
         MDRIVER *getDriver(void) const;

         void commandLine(char *cmd);
      };
   }
}
