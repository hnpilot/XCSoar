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

#include "SectorZone.hpp"
#include "Navigation/Geometry/GeoVector.hpp"

GeoPoint 
SectorZone::get_boundary_parametric(fixed t) const
{ 
  const Angle half = StartRadial.HalfAngle(EndRadial);
  const Angle angle = (half+Angle::radians(t*fixed_two_pi)).as_bearing();
  if (angleInSector(angle)) {
    return GeoVector(Radius, angle).end_point(get_location());
  } else {
    const fixed sweep = ((Angle::radians(fixed_two_pi)-
                         (EndRadial-StartRadial).as_bearing())*fixed_half).value_native();

    const fixed d_start = (StartRadial-angle).as_bearing().value_native()/sweep;
    const fixed d_end = (angle-EndRadial).as_bearing().value_native()/sweep;

    if (d_start< d_end) {
      return GeoVector(Radius*(fixed_one-d_start), 
                       StartRadial).end_point(get_location());
    } else {
      return GeoVector(Radius*(fixed_one-d_end), 
                       EndRadial).end_point(get_location());
    }
  }
}

fixed 
SectorZone::score_adjustment() const
{
  return fixed_zero;
}

void 
SectorZone::updateSector() 
{
  SectorStart = GeoVector(Radius, StartRadial).end_point(get_location());
  SectorEnd = GeoVector(Radius, EndRadial).end_point(get_location());
}


bool 
SectorZone::isInSector(const AIRCRAFT_STATE &ref) const
{
  GeoVector f(get_location(), ref.Location);

  return (f.Distance<=Radius) && angleInSector(f.Bearing);
}

void 
SectorZone::setStartRadial(const Angle x) 
{
  StartRadial = x;
  updateSector();
}

void 
SectorZone::setEndRadial(const Angle x) 
{
  EndRadial = x;
  updateSector();
}  

bool 
SectorZone::angleInSector(const Angle b) const
{
  if (StartRadial<EndRadial) {
    return ((b<=EndRadial) && (b>=StartRadial));
  } else {
    return ((b<=EndRadial) || (b>=StartRadial));
  }
}

bool
SectorZone::equals(const ObservationZonePoint* other) const
{
  const SectorZone *z = (const SectorZone *)other;

  return CylinderZone::equals(other) &&
    StartRadial == z->getStartRadial() &&
    EndRadial == z->getEndRadial();
}
