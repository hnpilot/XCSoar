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

#include "SettingsComputerBlackboard.hpp"

#include <algorithm>

SettingsComputerBlackboard::SettingsComputerBlackboard()
{
  settings_computer.AutoWindMode= D_AUTOWIND_CIRCLING;
  settings_computer.EnableBlockSTF = false;

  settings_computer.TeamCodeRefWaypoint = -1;
  settings_computer.TeamFlarmTracking = false;
  settings_computer.TeamFlarmCNTarget[0] = 0;

  settings_computer.AverEffTime=0;
  settings_computer.SoundVolume = 80;
  settings_computer.SoundDeadband = 5;
  settings_computer.EnableNavBaroAltitude=false;
  settings_computer.EnableExternalTriggerCruise=false;
  settings_computer.EnableCalibration = false;
  settings_computer.LoggerTimeStepCruise=5;
  settings_computer.LoggerTimeStepCircling=1;
  settings_computer.DisableAutoLogger = false;
  settings_computer.LoggerShortName = false;
  settings_computer.EnableBestAlternate=false;
  settings_computer.EnableAlternate1=false;
  settings_computer.EnableAlternate2=false;
  settings_computer.BallastTimerActive = false;
  settings_computer.BallastSecsToEmpty = 120;
  settings_computer.UTCOffset = 0;

  // for user-set teammate code
  settings_computer.TeammateCode[0] = 0;
  settings_computer.TeammateCodeValid = false;
  settings_computer.TeamFlarmIdTarget.clear();

  settings_computer.HomeWaypoint = -1;
  settings_computer.Alternate1 = -1;
  settings_computer.Alternate2 = -1;

  settings_computer.EnableVoiceClimbRate = false;
  settings_computer.EnableVoiceTerrain = false;
  settings_computer.EnableVoiceWaypointDistance = false;
  settings_computer.EnableVoiceTaskAltitudeDifference = false;
  settings_computer.EnableVoiceMacCready = false;
  settings_computer.EnableVoiceNewWaypoint = false;
  settings_computer.EnableVoiceInSector = false;
  settings_computer.EnableVoiceAirspace = false;

  settings_computer.EnableAirspaceWarnings = true;

  settings_computer.AltitudeMode = ALLON;
  settings_computer.ClipAltitude = 1000;
  settings_computer.AltWarningMargin = 100;

  settings_computer.EnableSoundVario = true;
  settings_computer.EnableSoundModes = true;
  settings_computer.EnableSoundTask = true;

  std::fill(settings_computer.DisplayAirspaces,
            settings_computer.DisplayAirspaces + AIRSPACECLASSCOUNT,
            true);

  settings_computer.POLARID = 1;
  settings_computer.SafetySpeed = fixed(70);
  settings_computer.BallastSecsToEmpty  = 120;
  settings_computer.BallastTimerActive = false;
}

