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
#include "PolygonInterior.hpp"

// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

//    a Point is defined by its coordinates {int x, y;}
//===================================================================

// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2 on the line
//            <0 for P2 right of the line
//    See: the January 2001 Algorithm "Area of 2D and 3D Triangles and Polygons"
inline static int
isLeft( const GeoPoint &P0, const GeoPoint &P1, const GeoPoint &P2 )
{
    return ( (P1.Longitude - P0.Longitude) * (P2.Latitude - P0.Latitude)
             - (P2.Longitude - P0.Longitude) * (P1.Latitude - P0.Latitude) ).sign();
}
//===================================================================

// PolygonInterior(): winding number interior test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  true if P is inside V

bool
PolygonInterior( const GeoPoint &P, const std::vector<SearchPoint>& V)
{
  int n = V.size()-1;
  if (n<2) {
    return false;
  }

  int    wn = 0;    // the winding number counter

  // loop through all edges of the polygon
  for (int i=0; i<n; i++) {   // edge from V[i] to V[i+1]
    if (V[i].get_location().Latitude <= P.Latitude) {         // start y <= P.Latitude
      if (V[i+1].get_location().Latitude > P.Latitude)      // an upward crossing
        if (isLeft( V[i].get_location(), V[i+1].get_location(), P)>0)  // P left of edge
          ++wn;            // have a valid up intersect
    }
    else {                       // start y > P.Latitude (no test needed)
      if (V[i+1].get_location().Latitude <= P.Latitude)     // a downward crossing
        if (isLeft( V[i].get_location(), V[i+1].get_location(), P)<0)  // P right of edge
          --wn;            // have a valid down intersect
    }
  }
  return wn != 0;
}


