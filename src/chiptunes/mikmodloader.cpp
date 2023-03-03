#include "sostav/chiptunes/mikmodloader.hpp"

using namespace Sostav;
using namespace Sostav::Chiptunes;

std::set<MikModLoader> MikModLoader::Loaders;

MikModLoaderException::MikModLoaderException
(const WCHAR *what)
   : Exception(what)
{
}

MikModLoader::MikModLoader
(MLOADER *loader)
{
   this->setLoader(loader);
}

MikModLoader::MikModLoader
(const MikModLoader &loader)
{
   this->setLoader(loader.getLoader());
}

MikModLoader::MikModLoader
(void)
{
   this->loader = NULL;
}

MikModLoader
MikModLoader::ImpulseTracker
(void)
{
   return MikModLoader(&load_it);
}

MikModLoader
MikModLoader::Module
(void)
{
   return MikModLoader(&load_mod);
}

MikModLoader
MikModLoader::ScreamTracker3
(void)
{
   return MikModLoader(&load_s3m);
}

MikModLoader
MikModLoader::FastTracker2
(void)
{
   return MikModLoader(&load_xm);
}

void
MikModLoader::Load
(MikModLoader loader)
{
   if (MikModLoader::Loaders.find(loader) == MikModLoader::Loaders.end())
      MikModLoader::Loaders.insert(loader);
}

void
MikModLoader::LoadDefault
(void)
{
   MikModLoader::Load(MikModLoader::ImpulseTracker());
   MikModLoader::Load(MikModLoader::Module());
   MikModLoader::Load(MikModLoader::ScreamTracker3());
   MikModLoader::Load(MikModLoader::FastTracker2());
}

void
MikModLoader::Unload
(MikModLoader loader)
{
   if (MikModLoader::Loaders.find(loader) != MikModLoader::Loaders.end())
      MikModLoader::Loaders.erase(loader);
}

void
MikModLoader::Register
(void)
{
   std::set<MikModLoader>::iterator iter;

   for (iter=MikModLoader::Loaders.begin(); iter!=MikModLoader::Loaders.end(); ++iter)
      MikMod_RegisterLoader(iter->getLoader());
}

bool
MikModLoader::operator<
(const MikModLoader &b) const
{
   MLOADER *other = b.getLoader();

   return this->loader < other;
}

bool
MikModLoader::operator==
(const MikModLoader &b) const
{
   return !(*this < b) && !(b < *this);
}

bool
MikModLoader::operator!=
(const MikModLoader &b) const
{
   return !(*this == b);
}

bool
MikModLoader::operator>
(const MikModLoader &b) const
{
   return b < *this;
}

bool
MikModLoader::operator<=
(const MikModLoader &b) const
{
   return *this < b || *this == b;
}

bool
MikModLoader::operator>=
(const MikModLoader &b) const
{
   return *this > b || *this == b;
}

void
MikModLoader::setLoader
(MLOADER *loader)
{
   this->loader = loader;
}

MLOADER *
MikModLoader::getLoader
(void) const
{
   return this->loader;
}
