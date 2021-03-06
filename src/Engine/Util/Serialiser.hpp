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
#ifndef SERIALISER_HPP
#define SERIALISER_HPP

#include "Task/Visitors/TaskPointVisitor.hpp"
#include "Task/Visitors/ObservationZoneVisitor.hpp"
#include "Task/Tasks/OrderedTask.hpp"

class DataNode;
class Waypoint;

/**
 * Class to serialise and de-serialise tasks to/from a #DataNode structure
 */
class Serialiser:
  public TaskPointConstVisitor,
  public ObservationZoneConstVisitor
{
public:
  /** 
   * Constructor
   * 
   * @param the_node Node this serialiser will edit
   * 
   * @return Initialised object
   */
  Serialiser(DataNode& the_node): m_node(the_node) {};

  /** 
   * Serialise a task (create a DataNode structure to reflect the task)
   * 
   * @param data OrderedTask to serialise
   */
  void serialise(const OrderedTask& data);

  /** 
   * De-serialise a task (create a task to reflect the DataNode structure)
   * 
   * @param data OrderedTask to serialise
   */
  void deserialise(OrderedTask& data);

  void Visit(const StartPoint& data);
  void Visit(const ASTPoint& data);
  void Visit(const AATPoint& data);
  void Visit(const FinishPoint& data);
  void Visit(const UnorderedTaskPoint& data);
  void Visit(const FAISectorZone& data);
  void Visit(const KeyholeZone& data);
  void Visit(const BGAFixedCourseZone& data);
  void Visit(const BGAEnhancedOptionZone& data);
  void Visit(const SectorZone& data);
  void Visit(const LineSectorZone& data);
  void Visit(const CylinderZone& data);

protected:
  /** 
   * Serialise OrderedTaskBehaviour
   * 
   * @param data Item to serialise
   */
  void serialise(const OrderedTaskBehaviour& data);

  /** 
   * Deserialise OrderedTaskBehaviour
   * 
   * @param data Item to deserialise
   */
  void deserialise(OrderedTaskBehaviour& data);

  /** 
   * Serialise a Waypoint
   * 
   * @param data Item to serialise
   */
  void serialise(const Waypoint& data);

  /** 
   * Deserialise a Waypoint; client responsible for deletion
   * 
   * @return Newly constructed Waypoint or NULL on failure
   */
  Waypoint* deserialise_waypoint();

  /** 
   * Serialise a GeoPoint
   * 
   * @param data Item to serialise
   */
  void serialise(const GeoPoint& data);

  /** 
   * Deserialise a GeoPoint
   * 
   * @param data Item to deserialise
   */
  void deserialise(GeoPoint& data);

  /** 
   * Serialise an ObservationZonePoint
   * 
   * @param data Item to serialise
   */
  void serialise(const ObservationZonePoint& data);

  /** 
   * Deserialise an ObservationZonePoint; client responsible for deletion
   * 
   * @param wp Waypoint base of point
   * @param is_turnpoint Whether the point is a turnpoint
   *
   * @return Newly constructed ObservationZonePoint or NULL on failure
   */
  ObservationZonePoint* deserialise_oz(const Waypoint& wp, const bool is_turnpoint);

  /** 
   * Serialise an OrderedTaskPoint
   * 
   * @param data Item to serialise
   * @param name Type of point
   */
  DataNode* serialise(const OrderedTaskPoint& data, const TCHAR* name);

  /** 
   * Deserialise a point, appending it to the task
   * 
   * @param data OrderedTask to append to
   */
  void deserialise_point(OrderedTask& data);

private:
  DataNode &m_node;

  OrderedTask::Factory_t task_factory_type() const;
  const TCHAR* task_factory_type(OrderedTask::Factory_t the_type) const;
};

#endif
