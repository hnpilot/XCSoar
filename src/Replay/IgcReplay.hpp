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

#ifndef IGC_REPLAY_HPP
#define IGC_REPLAY_HPP

#include "Math/fixed.hpp"
#include "Replay/CatmullRomInterpolator.hpp"
#include "IO/FileLineReader.hpp"

#include <tchar.h>
#include <windef.h> /* for MAX_PATH */
#include <stdio.h>

struct GeoPoint;
class Angle;

class IgcReplay
{
public:
  IgcReplay();

  bool Update();
  void Stop();
  void Start();
  const TCHAR* GetFilename();
  void SetFilename(const TCHAR *name);
  fixed TimeScale;

protected:
  virtual bool update_time(const fixed mintime);
  virtual void reset_time();
  virtual void on_reset() = 0;
  virtual void on_stop() = 0;
  virtual void on_bad_file() = 0;
  virtual void on_advance(const GeoPoint &loc,
                          const fixed speed, const Angle bearing,
                          const fixed alt, const fixed baroalt, const fixed t) = 0;
  virtual bool ScanBuffer(const TCHAR *buffer, fixed *Time, fixed *Latitude,
                          fixed *Longitude, fixed *Altitude);

  bool Enabled;
  fixed t_simulation;

  bool ReadPoint(fixed *Time, fixed *Latitude, fixed *Longitude,
                 fixed *Altitude);

private:
  CatmullRomInterpolator cli;

  TCHAR FileName[MAX_PATH];
  FileLineReader *reader;

  bool OpenFile();
  void CloseFile();
};

#endif
