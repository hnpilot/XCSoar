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

#include "InfoBoxes/Content/Task.hpp"

#include "InfoBoxes/InfoBoxWindow.hpp"
#include "Interface.hpp"
#include "Components.hpp"
#include "TaskClientUI.hpp"

#include <tchar.h>

void
InfoBoxContentBearing::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("Bearing"));

  if (XCSoarInterface::Calculated().task_stats.current_leg.
      solution_remaining.Vector.Distance <= fixed(10)) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.0f")_T(DEG)_T("T"),
            (double)XCSoarInterface::Calculated().task_stats.current_leg.
            solution_remaining.Vector.Bearing.value_degrees());
  infobox.SetValue(tmp);
}

void
InfoBoxContentBearingDiff::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("Brng D"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid ||
      XCSoarInterface::Calculated().task_stats.current_leg.solution_remaining.
      Vector.Distance <= fixed(10)) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  double Value =
      (XCSoarInterface::Calculated().task_stats.current_leg.
       solution_remaining.Vector.Bearing - XCSoarInterface::Basic().
       TrackBearing).as_delta().value_degrees();

#ifndef __MINGW32__
  if (Value > 1)
    _stprintf(tmp, _T("%2.0f��"), Value);
  else if (Value < -1)
    _stprintf(tmp, _T("�%2.0f�"), -Value);
  else
    _tcscpy(tmp, _T("��"));
#else
  if (Value > 1)
    _stprintf(tmp, _T("%2.0f°»"), Value);
  else if (Value < -1)
    _stprintf(tmp, _T("«%2.0f°"), -Value);
  else
    _tcscpy(tmp, _T("«»"));
#endif

  infobox.SetValue(tmp);
}

void
InfoBoxContentNextWaypoint::Update(InfoBoxWindow &infobox)
{
  const Waypoint* way_point = task_ui.getActiveWaypoint();

  if (!way_point) {
    // Set Title
    infobox.SetTitle(_T("Next"));

    infobox.SetInvalid();
    return;
  }

  // Set Title
  TCHAR tmp[32];
  if (XCSoarInterface::SettingsMap().DisplayTextType == DISPLAYFIRSTTHREE) {
    _tcsncpy(tmp, way_point->Name.c_str(), 3);
    tmp[3] = '\0';
  } else if (XCSoarInterface::SettingsMap().DisplayTextType == DISPLAYNUMBER) {
    _stprintf(tmp, _T("%d"), way_point->id);
  } else {
    _tcsncpy(tmp, way_point->Name.c_str(), (sizeof(tmp) / sizeof(TCHAR)) - 1);
    tmp[(sizeof(tmp) / sizeof(TCHAR)) - 1] = '\0';
  }
  infobox.SetTitle(tmp);

  if (!XCSoarInterface::Calculated().task_stats.task_valid ||
      XCSoarInterface::Calculated().task_stats.current_leg.solution_remaining.
      Vector.Distance <= fixed(10)) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  double Value =
      (XCSoarInterface::Calculated().task_stats.current_leg.
       solution_remaining.Vector.Bearing - XCSoarInterface::Basic().
       TrackBearing).as_delta().value_degrees();

#ifndef __MINGW32__
  if (Value > 1)
    _stprintf(tmp, _T("%2.0f��"), Value);
  else if (Value < -1)
    _stprintf(tmp, _T("�%2.0f�"), -Value);
  else
    _tcscpy(tmp, _T("��"));
#else
  if (Value > 1)
    _stprintf(tmp, _T("%2.0f°»"), Value);
  else if (Value < -1)
    _stprintf(tmp, _T("«%2.0f°"), -Value);
  else
    _tcscpy(tmp, _T("«»"));
#endif

  infobox.SetValue(tmp);

  // Set Comment
  infobox.SetComment(way_point->Comment.c_str());

  // Set Color
  if (XCSoarInterface::Calculated().task_stats.current_leg.
      solution_remaining.is_final_glide())
    // blue
    infobox.SetColor(2);
  else
    // black
    infobox.SetColor(0);
}

void
InfoBoxContentNextDistance::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("WP Dist"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.1f"),
            (double)Units::ToUserDistance(XCSoarInterface::Calculated().
                task_stats.current_leg.solution_remaining.Vector.Distance));
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::DistanceUnit);
}

void
InfoBoxContentNextAltitudeDiff::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("WP AltD"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.0f"),
            (double)Units::ToUserAltitude(XCSoarInterface::Calculated().
                task_stats.current_leg.solution_remaining.AltitudeDifference));
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::AltitudeUnit);
}

void
InfoBoxContentNextAltitudeRequire::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("WP AltR"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.0f"),
            (double)Units::ToUserAltitude(XCSoarInterface::Calculated().
                task_stats.current_leg.solution_remaining.AltitudeRequired));
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::AltitudeUnit);
}

void
InfoBoxContentFinalDistance::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("Fin Dis"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  double Value = (XCSoarInterface::Calculated().common_stats.task_finished ?
                  Units::ToUserDistance(XCSoarInterface::Calculated().
                    task_stats.current_leg.solution_remaining.Vector.Distance) :
                  Units::ToUserDistance(XCSoarInterface::Calculated().
                    task_stats.total.remaining.get_distance()));
  _stprintf(tmp, _T("%2.0f"), Value);
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::DistanceUnit);
}

void
InfoBoxContentFinalAltitudeDiff::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("Fin AltD"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.0f"),
            (double)Units::ToUserAltitude(XCSoarInterface::Calculated().
                task_stats.total.solution_remaining.AltitudeDifference));
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::AltitudeUnit);
}

void
InfoBoxContentFinalAltitudeRequire::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("Fin AltR"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.0f"),
            (double)Units::ToUserAltitude(XCSoarInterface::Calculated().
                task_stats.total.solution_remaining.AltitudeRequired));
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::AltitudeUnit);
}

void
InfoBoxContentTaskSpeed::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("V Task Av"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  TCHAR tmp[32];
  _stprintf(tmp, _T("%2.0f"),
            (double)Units::ToUserTaskSpeed(XCSoarInterface::Calculated().
                task_stats.total.remaining.get_speed()));
  infobox.SetValue(tmp);

  // Set Unit
  infobox.SetValueUnit(Units::TaskSpeedUnit);
}

void
InfoBoxContentFinalLD::Update(InfoBoxWindow &infobox)
{
  // Set Title
  infobox.SetTitle(_T("Fin LD"));

  if (!XCSoarInterface::Calculated().task_stats.task_valid) {
    infobox.SetInvalid();
    return;
  }

#ifdef OLD_TASK
  if (XCSoarInterface::Calculated().LDFinish != 999) {
    infobox.SetInvalid();
    return;
  }

  // Set Value
  double Value;
  if (XCSoarInterface::Calculated().ValidFinish)
    Value = 0;
  else
    Value = XCSoarInterface::Calculated().LDFinish;

  TCHAR tmp[32];
  _stprintf(tmp, _T("%1.0f"), Value);
  infobox.SetValue(tmp);
#else
  infobox.SetInvalid();
#endif
}