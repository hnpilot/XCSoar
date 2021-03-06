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

#include "MapWindowTimer.hpp"

/**
 * Constructor of the MapWindowTimer class
 *
 * Resets all internal fields to the default values
 */
MapWindowTimer::MapWindowTimer():
  timestats_av(0),
  timestats_dirty(false),
  tottime(0)
{
}

/**
 * "Starts" the timer by saving the start time in timestamp_newdata
 */
void
MapWindowTimer::StartTimer()
{
  // Saves the current tick count (time) as start time
  timestamp_newdata.update();
  timestats_dirty = false;
}

/**
 * Returns true if last call of StartTimer() is less
 * then 700ms ago
 * @return True if last call of StartTimer() is less
 * then 700ms ago, False otherwise
 */
bool
MapWindowTimer::RenderTimeAvailable()
{
  if (!timestamp_newdata.check(700))
    // it's been less than 700 ms since last data
    // was posted
    return true;

  return false;
}

/**
 * Interrupts the timer (in case map was panned while drawing)
 */
void
MapWindowTimer::InterruptTimer()
{
  timestats_dirty = true;
  // cause to expire
  timestamp_newdata.update_offset(-700);
}

/**
 * "Stops" the timer by calculating the total drawing time.
 */
void
MapWindowTimer::StopTimer()
{
  if (!timestats_dirty) {
    // Calculates the drawing time with low pass filter
    tottime = (2 * tottime + timestamp_newdata.elapsed()) / 3;
    timestats_av = tottime;
  }

  timestats_dirty = false;
}
