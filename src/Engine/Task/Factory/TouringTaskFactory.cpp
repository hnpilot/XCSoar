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

#include "TouringTaskFactory.hpp"
#include "Task/Tasks/OrderedTask.hpp"
#include "Task/OrderedTaskBehaviour.hpp"

TouringTaskFactory::TouringTaskFactory(OrderedTask& _task,
                               const TaskBehaviour &tb):
  AbstractTaskFactory(_task, tb)
{
  m_start_types.push_back(START_CYLINDER);
  m_intermediate_types.push_back(FAI_SECTOR);
  m_finish_types.push_back(FINISH_CYLINDER);
}

bool 
TouringTaskFactory::validate() const
{

  if (!m_task.has_start() || !m_task.has_finish()) {
    return false;
  }
  // unknown task...
  return true;
}

void 
TouringTaskFactory::update_ordered_task_behaviour(OrderedTaskBehaviour& to)
{
  to.task_scored = false;
  to.aat_min_time = fixed_zero;
  to.fai_finish = false;  
  to.homogeneous_tps = true;
  to.min_points = 2;
  to.max_points = 10;

  to.start_max_speed = fixed_zero;
  to.start_max_height = 0;
  to.start_max_height_ref = 0;
  to.finish_min_height = 0;
  to.start_requires_arm = false;
}
