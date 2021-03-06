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


#ifndef WAYPOINTFILESEEYOU_HPP
#define WAYPOINTFILESEEYOU_HPP

#include "WayPointFile.hpp"

class WayPointFileSeeYou: 
  public WayPointFile 
{
public:
  WayPointFileSeeYou(const TCHAR* file_name, const int _file_num,
                     const bool _compressed=false): WayPointFile(file_name,
                                                                 _file_num,
                                                                 _compressed) {};

protected:
  /**
   * Parses a SeeYou waypoint file line
   * @see parseLine()
   * @see http://data.naviter.si/docs/cup_format.pdf
   */
  bool parseLine(const TCHAR* line, const unsigned linenum,
                 Waypoints &way_points, const RasterTerrain *terrain);

  bool IsWritable() { return false; }

private:
  static bool parseString(const TCHAR* src, tstring& dest);
  static bool parseAngle(const TCHAR* src, Angle& dest, const bool lat);
  static bool parseAltitude(const TCHAR* src, fixed& dest);
  static bool parseFlags(const TCHAR* src, WaypointFlags& dest);
  static bool parseStyle(const TCHAR* src, WaypointFlags& dest);
};

#endif
