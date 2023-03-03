#pragma once

#include <windows.h>

#include <set>

#include <mikmod.h>

#include <sostav/exception.hpp>

namespace Sostav
{
   namespace Chiptunes
   {
      class MikModLoaderException : public Exception
      {
      public:
         MikModLoaderException(const WCHAR *what);
      };

      class MikModLoader
      {
      protected:
         MLOADER *loader;

      public:
         static std::set<MikModLoader> Loaders;
         
         MikModLoader(MLOADER *loader);
         MikModLoader(const MikModLoader &loader);
         MikModLoader();

         static MikModLoader ImpulseTracker(void);
         static MikModLoader Module(void);
         static MikModLoader ScreamTracker3(void);
         static MikModLoader FastTracker2(void);
         
         static void Load(MikModLoader loader);
         static void LoadDefault(void);
         static void Unload(MikModLoader loader);
         static void Register(void);

         bool operator<(const MikModLoader &b) const;
         bool operator==(const MikModLoader &b) const;
         bool operator!=(const MikModLoader &b) const;
         bool operator>(const MikModLoader &b) const;
         bool operator<=(const MikModLoader &b) const;
         bool operator>=(const MikModLoader &b) const;

         void setLoader(MLOADER *loader);
         MLOADER *getLoader(void) const;
      };
   }
}
