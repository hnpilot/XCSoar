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
#include "TaskSolveTravelled.hpp"
#include <math.h>
#include "Util/Tolerances.hpp"

TaskSolveTravelled::TaskSolveTravelled(const std::vector<OrderedTaskPoint*>& tps,
                                       const unsigned activeTaskPoint,
                                       const AIRCRAFT_STATE &_aircraft,
                                       const GlidePolar &gp,
                                       const fixed _xmin, 
                                       const fixed _xmax):
  ZeroFinder(fixed(_xmin), fixed(_xmax), fixed(TOLERANCE_CRUISE_EFFICIENCY)),
  tm(tps,activeTaskPoint,gp),
  aircraft(_aircraft) 
{
  dt = aircraft.Time-tps[0]->get_state_entered().Time;
  if (positive(dt)) {
    inv_dt = fixed_one/dt;
  } else {
    inv_dt = fixed_zero; // error!
  }
}

fixed 
TaskSolveTravelled::time_error() 
{
  res = tm.glide_solution(aircraft);
  fixed d = fabs(res.TimeElapsed-dt);
  if (!res.Solution==GlideResult::RESULT_OK) {
    d += res.TimeVirtual;
  }
  if (positive(dt)) {
    d*= inv_dt;
  }
  return d;
}

fixed 
TaskSolveTravelled::search(const fixed ce) 
{
  return find_min(ce);
}
