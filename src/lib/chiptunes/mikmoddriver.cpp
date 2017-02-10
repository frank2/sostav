#include "sostav/chiptunes/mikmoddriver.hpp"

using namespace Sostav;
using namespace Sostav::Chiptunes;

std::set<MikModDriver> MikModDriver::Drivers;

MikModDriverException::MikModDriverException
(const WCHAR *what)
   : Exception(what)
{
}

MikModDriver::MikModDriver
(MDRIVER *driver)
{
   this->setDriver(driver);
}

MikModDriver::MikModDriver
(const MikModDriver &driver)
{
   this->setDriver(driver.getDriver());
}

MikModDriver::MikModDriver
(void)
{
   this->driver = NULL;
}

MikModDriver
MikModDriver::Raw
(void)
{
   return MikModDriver(&drv_raw);
}

MikModDriver
MikModDriver::StdOut
(void)
{
   return MikModDriver(&drv_stdout);
}

MikModDriver
MikModDriver::WAV
(void)
{
   return MikModDriver(&drv_wav);
}

MikModDriver
MikModDriver::AIFF
(void)
{
   return MikModDriver(&drv_aiff);
}

MikModDriver
MikModDriver::DirectSound
(void)
{
   return MikModDriver(&drv_ds);
}

MikModDriver
MikModDriver::XAudio2
(void)
{
   return MikModDriver(&drv_xaudio2);
}

MikModDriver
MikModDriver::MultiMediaAPI
(void)
{
   return MikModDriver(&drv_win);
}

void
MikModDriver::Load
(MikModDriver driver)
{
   if (MikModDriver::Drivers.find(driver) == MikModDriver::Drivers.end())
      MikModDriver::Drivers.insert(driver);
}

void
MikModDriver::Unload
(MikModDriver driver)
{
   if (MikModDriver::Drivers.find(driver) != MikModDriver::Drivers.end())
      MikModDriver::Drivers.erase(driver);
}

void
MikModDriver::Register
(void)
{
   std::set<MikModDriver>::iterator iter;

   for (iter=MikModDriver::Drivers.begin(); iter!=MikModDriver::Drivers.end(); ++iter)
      MikMod_RegisterDriver(iter->getDriver());
}

bool
MikModDriver::operator<
(const MikModDriver &b) const
{
   MDRIVER *other = b.getDriver();

   return strcmp(this->driver->Name, other->Name) < 0;
}

bool
MikModDriver::operator==
(const MikModDriver &b) const
{
   return !(*this < b) && !(b < *this);
}

bool
MikModDriver::operator!=
(const MikModDriver &b) const
{
   return !(*this == b);
}

bool
MikModDriver::operator>
(const MikModDriver &b) const
{
   return b < *this;
}

bool
MikModDriver::operator<=
(const MikModDriver &b) const
{
   return *this < b || *this == b;
}

bool
MikModDriver::operator>=
(const MikModDriver &b) const
{
   return *this > b || *this == b;
}

void
MikModDriver::setDriver
(MDRIVER *driver)
{
   this->driver = driver;
}

MDRIVER *
MikModDriver::getDriver
(void) const
{
   return this->driver;
}
