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

#include "CylinderZone.hpp"
#include "Navigation/Geometry/GeoVector.hpp"
#include <stdlib.h>
#include <algorithm>

fixed 
CylinderZone::score_adjustment() const
{
  return Radius;
}

GEOPOINT 
CylinderZone::get_boundary_parametric(fixed t) const
{ 
  return GeoVector(Radius, Angle::radians(t*fixed_two_pi)).end_point(get_location());
}

bool
CylinderZone::equals(const ObservationZonePoint* other) const
{
  if (ObservationZonePoint::equals(other)) {
    if (const CylinderZone* z = dynamic_cast<const CylinderZone*>(other)) {
      return Radius == z->getRadius();
    }
  }
  return false;
}

GEOPOINT
CylinderZone::randomPointInSector(const fixed mag) const
{
  AIRCRAFT_STATE ac;  
  do {
    Angle dir = Angle::degrees(fixed(rand() % 360));
    fixed dmag = max(min(Radius, fixed(100.0)), Radius*mag);
    fixed dis = fixed((0.1 + (rand() % 90) / 100.0)) * dmag;
    GeoVector vec(dis,dir);
    ac.Location = vec.end_point(get_location());
  } while (!isInSector(ac));
  return ac.get_location();
}