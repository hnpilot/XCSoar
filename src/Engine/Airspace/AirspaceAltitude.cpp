/* Copyright_License {

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
#include "AirspaceAltitude.hpp"
#include "Atmosphere/Pressure.hpp"

#include <stdio.h>

void 
AIRSPACE_ALT::set_flight_level(const AtmosphericPressure &press)
{
  static const fixed fl_feet_to_m(30.48);
  if (Base == abFL)
    Altitude = press.AltitudeToQNHAltitude(FL * fl_feet_to_m);
}

void 
AIRSPACE_ALT::set_ground_level(const fixed alt)
{
  if (Base == abAGL)
    Altitude = AGL+alt;
}

const tstring 
AIRSPACE_ALT::get_as_text(const bool concise) const
{
  TCHAR buffer[64];

  switch (Base) {
  case abAGL:
    if (!positive(AGL)) {
      _tcscpy(buffer, _T("GND"));
    } else {
      _stprintf(buffer, _T("%d AGL"), (int)AGL);
    }
    break;
  case abFL:
    _stprintf(buffer, _T("FL%d"), (int)FL);
    break;
  case abMSL:
    _stprintf(buffer, _T("%d"), (int)Altitude);
    break;
  case abUndef:
  default:
    buffer[0] = _T('\0');
    break;
  };
  if (!concise && Base!=abMSL && positive(Altitude)) {
    TCHAR second[64];
    _stprintf(second, _T(" %d"), (int)Altitude);
    return tstring(buffer) + second;
  } else
    return tstring(buffer);
}
