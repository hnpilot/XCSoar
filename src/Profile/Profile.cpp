/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009

	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>
	Paolo Ventafridda <coolwind@email.it>
	Tobias Lohner <tobias@lohner-net.de>
	Mirek Jezek <mjezek@ipplc.cz>
	Max Kellermann <max@duempel.org>
	Tobias Bieniek <tobias.bieniek@gmx.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Profile/Profile.hpp"
#include "LogFile.hpp"
#include "Asset.hpp"
#include "LocalPath.hpp"
#include "StringUtil.hpp"

#ifndef USE_PROFILE_MAP
#include "Profile/Registry.hpp"
#else
#include "Profile/ProfileMap.hpp"
#endif

#define XCSPROFILE "xcsoar-registry.prf"

TCHAR startProfileFile[MAX_PATH];
TCHAR defaultProfileFile[MAX_PATH];
TCHAR failsafeProfileFile[MAX_PATH];

void
Profile::Load()
{
  if (!use_files())
    return;

  LogStartUp(_T("Loading profiles"));
  // load registry backup if it exists
  LoadFile(failsafeProfileFile);
  LoadFile(startProfileFile);
}

void
Profile::LoadFile(const TCHAR *szFile)
{
  if (!use_files())
    return;

  if (string_is_empty(szFile))
    return;

  LogStartUp(_T("Loading profile from %s"), szFile);
#ifndef USE_PROFILE_MAP
  Registry::Import(szFile);
#else
  ProfileMap::Import(szFile);
#endif
}

void
Profile::Save()
{
  if (!use_files())
    return;

  LogStartUp(_T("Saving profiles"));
  SaveFile(defaultProfileFile);
}

void
Profile::SaveFile(const TCHAR *szFile)
{
  if (!use_files())
    return;

  if (string_is_empty(szFile))
    return;

  LogStartUp(_T("Saving profile to %s"), szFile);
#ifndef USE_PROFILE_MAP
  Registry::Export(szFile);
#else
  ProfileMap::Export(szFile);
#endif
}


void
Profile::SetFiles(const TCHAR* override)
{
  if (!use_files())
    return;

  // Set the default profile file
  if (is_altair())
    LocalPath(defaultProfileFile, _T("config/")_T(XCSPROFILE));
  else
    LocalPath(defaultProfileFile, _T(XCSPROFILE));

  // Set the failsafe profile file
  LocalPath(failsafeProfileFile, _T(XCSPROFILE));

  // Set the profile file to load at startup
  // -> to the default file
  _tcscpy(startProfileFile, defaultProfileFile);

  // -> to the given filename (if exists)
  if (!string_is_empty(override))
    _tcsncpy(startProfileFile, override, MAX_PATH - 1);
}

bool
Profile::Get(const TCHAR *key, int &value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Get(key, value);
#else
  return ProfileMap::Get(key, value);
#endif
}

bool
Profile::Get(const TCHAR *key, short &value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Get(key, value);
#else
  return ProfileMap::Get(key, value);
#endif
}

bool
Profile::Get(const TCHAR *key, bool &value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Get(key, value);
#else
  return ProfileMap::Get(key, value);
#endif
}

bool
Profile::Get(const TCHAR *key, unsigned &value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Get(key, value);
#else
  return ProfileMap::Get(key, value);
#endif
}

bool
Profile::Get(const TCHAR *key, double &value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Get(key, value);
#else
  return ProfileMap::Get(key, value);
#endif
}

#ifdef FIXED_MATH

bool
Profile::Get(const TCHAR *key, fixed &value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Get(key, value);
#else
  return ProfileMap::Get(key, value);
#endif
}

#endif

bool
Profile::Get(const TCHAR *key, TCHAR *value, size_t dwSize)
{
#ifndef USE_PROFILE_MAP
  if (Registry::Get(key, value, dwSize))
#else
  if (ProfileMap::Get(key, value, dwSize))
#endif
    return true;
  else {
    value[0] = _T('\0');
    return false;
  }
}

bool
Profile::Set(const TCHAR *key, int value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, value);
#else
  return ProfileMap::Set(key, value);
#endif
}

bool
Profile::Set(const TCHAR *key, short value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, value);
#else
  return ProfileMap::Set(key, value);
#endif
}

bool
Profile::Set(const TCHAR *key, bool value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, value);
#else
  return ProfileMap::Set(key, value);
#endif
}

bool
Profile::Set(const TCHAR *key, unsigned value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, value);
#else
  return ProfileMap::Set(key, value);
#endif
}

bool
Profile::Set(const TCHAR *key, double value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, (DWORD)value);
#else
  return ProfileMap::Set(key, (DWORD)value);
#endif
}

bool
Profile::Set(const TCHAR *key, long value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, value);
#else
  return ProfileMap::Set(key, value);
#endif
}

bool
Profile::Set(const TCHAR *key, const TCHAR *value)
{
#ifndef USE_PROFILE_MAP
  return Registry::Set(key, value);
#else
  return ProfileMap::Set(key, value);
#endif
}

void
Profile::SetStringIfAbsent(const TCHAR *key, const TCHAR *value)
{
  TCHAR temp[MAX_PATH];
  if (!Get(key, temp, MAX_PATH))
    Set(key, value);
}

bool
Profile::GetPath(const TCHAR *key, TCHAR *value)
{
  if (!Get(key, value, MAX_PATH) || string_is_empty(value))
    return false;

  ExpandLocalPath(value);
  return true;
}