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

#ifndef XCSOAR_UTILS_PROFILE_HPP
#define XCSOAR_UTILS_PROFILE_HPP

#include <tchar.h>

#ifndef WIN32
/* for GConf */
#define PROFILE_KEY_PREFIX "/apps/XCSoar/"
#endif

#ifdef PROFILE_KEY_PREFIX
#define CONF(key) _T(PROFILE_KEY_PREFIX key)
#else
#define CONF(key) _T(key)
#endif

extern const TCHAR *szProfileDisplayType[];
extern const TCHAR *szProfileColour[];
extern const TCHAR *szProfileBrush[];
extern const TCHAR *szProfileAirspaceMode[];
extern const TCHAR szProfileSpeedUnitsValue[];
extern const TCHAR szProfileDistanceUnitsValue[];
extern const TCHAR szProfileAltitudeUnitsValue[];
extern const TCHAR szProfileTemperatureUnitsValue[];
extern const TCHAR szProfileLiftUnitsValue[];
extern const TCHAR szProfileTaskSpeedUnitsValue[];
extern const TCHAR szProfileDisplayUpValue[];
extern const TCHAR szProfileDisplayText[];
extern const TCHAR szProfileSafetyAltitudeArrival[];
extern const TCHAR szProfileSafetyAltitudeTerrain[];
extern const TCHAR szProfileSafteySpeed[];
extern const TCHAR szProfilePolarID[];
extern const TCHAR szProfileWayPointFile[];
extern const TCHAR szProfileAdditionalWayPointFile[];
extern const TCHAR szProfileAirspaceFile[];
extern const TCHAR szProfileAdditionalAirspaceFile[];
extern const TCHAR szProfileAirfieldFile[];
extern const TCHAR szProfileTopologyFile[];
extern const TCHAR szProfilePolarFile[];
extern const TCHAR szProfileTerrainFile[];
extern const TCHAR szProfileLanguageFile[];
extern const TCHAR szProfileStatusFile[];
extern const TCHAR szProfileInputFile[];
extern const TCHAR szProfileAltMode[];
extern const TCHAR szProfileClipAlt[];
extern const TCHAR szProfileAltMargin[];
extern const TCHAR szProfileSnailTrail[];
extern const TCHAR szProfileDrawTopology[];
extern const TCHAR szProfileDrawTerrain[];
extern const TCHAR szProfileFinalGlideTerrain[];
extern const TCHAR szProfileAutoWind[];
extern const TCHAR *szProfileAirspacePriority[];
extern const TCHAR szProfileAirspaceWarning[];
extern const TCHAR szProfileAirspaceBlackOutline[];
extern const TCHAR szProfileWarningTime[];
extern const TCHAR szProfileAcknowledgementTime[];
extern const TCHAR szProfileCircleZoom[];
extern const TCHAR szProfileHomeWaypoint[];
extern const TCHAR szProfileAlternate1[];         // VENTA3
extern const TCHAR szProfileAlternate2[];
extern const TCHAR szProfileTeamcodeRefWaypoint[];
extern const TCHAR szProfilePilotName[];
extern const TCHAR szProfileAircraftType[];
extern const TCHAR szProfileAircraftRego[];
extern const TCHAR szProfileLoggerID[];
extern const TCHAR szProfileLoggerShort[];
extern const TCHAR szProfileSoundVolume[];
extern const TCHAR szProfileSoundDeadband[];
extern const TCHAR szProfileSoundAudioVario[];
extern const TCHAR szProfileSoundTask[];
extern const TCHAR szProfileSoundModes[];
extern const TCHAR szProfileNettoSpeed[];
extern const TCHAR szProfileCDICruise[];
extern const TCHAR szProfileCDICircling[];
extern const TCHAR szProfileAutoBlank[];
extern const TCHAR szProfileAutoBacklight[]; // VENTA4
extern const TCHAR szProfileAutoSoundVolume[]; // VENTA4
extern const TCHAR szProfileGestures[];
extern const TCHAR szProfileAverEffTime[]; // VENTA6
extern const TCHAR szProfileVarioGauge[];
extern const TCHAR szProfileDebounceTimeout[];
extern const TCHAR szProfileAppIndFinalGlide[];
extern const TCHAR szProfileAppIndLandable[];
extern const TCHAR szProfileAppInverseInfoBox[];
extern const TCHAR szProfileAppInfoBoxColors[];
extern const TCHAR szProfileAppGaugeVarioSpeedToFly[];
extern const TCHAR szProfileAppGaugeVarioAvgText[];
extern const TCHAR szProfileAppGaugeVarioMc[];
extern const TCHAR szProfileAppGaugeVarioBugs[];
extern const TCHAR szProfileAppGaugeVarioBallast[];
extern const TCHAR szProfileAppGaugeVarioGross[];
extern const TCHAR szProfileAppCompassAppearance[];
extern const TCHAR szProfileAppStatusMessageAlignment[];
extern const TCHAR szProfileAppTextInputStyle[];
extern const TCHAR szProfileAppDialogStyle[];
extern const TCHAR szProfileAppInfoBoxBorder[];
extern const TCHAR szProfileAppInfoBoxModel[]; // VENTA-ADDON MODEL CHANGE
extern const TCHAR szProfileAppAveNeedle[];
extern const TCHAR szProfileAutoAdvance[];
extern const TCHAR szProfileUTCOffset[];
extern const TCHAR szProfileBlockSTF[];
extern const TCHAR szProfileAutoZoom[];
extern const TCHAR szProfileMenuTimeout[];
extern const TCHAR szProfileLockSettingsInFlight[];
extern const TCHAR szProfileTerrainContrast[];
extern const TCHAR szProfileTerrainBrightness[];
extern const TCHAR szProfileTerrainRamp[];
extern const TCHAR szProfileEnableFLARMMap[];
extern const TCHAR szProfileEnableFLARMGauge[];
extern const TCHAR szProfileEnableTAGauge[];
extern const TCHAR szProfileSnailTrail[];
extern const TCHAR szProfileTrailDrift[];
extern const TCHAR szProfileDetourCostMarker[];
extern const TCHAR szProfileGliderScreenPosition[];
extern const TCHAR szProfileSetSystemTimeFromGPS[];

extern const TCHAR szProfileVoiceClimbRate[];
extern const TCHAR szProfileVoiceTerrain[];
extern const TCHAR szProfileVoiceWaypointDistance[];
extern const TCHAR szProfileVoiceTaskAltitudeDifference[];
extern const TCHAR szProfileVoiceMacCready[];
extern const TCHAR szProfileVoiceNewWaypoint[];
extern const TCHAR szProfileVoiceInSector[];
extern const TCHAR szProfileVoiceAirspace[];

extern const TCHAR szProfileFinishMinHeight[];
extern const TCHAR szProfileStartMaxHeight[];
extern const TCHAR szProfileStartMaxHeightMargin[];
extern const TCHAR szProfileStartMaxSpeed[];
extern const TCHAR szProfileStartMaxSpeedMargin[];
extern const TCHAR szProfileStartHeightRef[];

extern const TCHAR szProfileEnableNavBaroAltitude[];

extern const TCHAR szProfileLoggerTimeStepCruise[];
extern const TCHAR szProfileLoggerTimeStepCircling[];

extern const TCHAR szProfileSafetyMacCready[];
extern const TCHAR szProfileAbortSafetyUseCurrent[];
extern const TCHAR szProfileAutoMcMode[];
extern const TCHAR szProfileEnableExternalTriggerCruise[];
extern const TCHAR szProfileOLCRules[];
extern const TCHAR szProfileHandicap[];
extern const TCHAR szProfileSnailWidthScale[];
extern const TCHAR szProfileLatLonUnits[];
extern const TCHAR szProfileUserLevel[];
extern const TCHAR szProfileRiskGamma[];
extern const TCHAR szProfileWindArrowStyle[];
extern const TCHAR szProfileDisableAutoLogger[];
extern const TCHAR szProfileMapFile[];
extern const TCHAR szProfileBallastSecsToEmpty[];
extern const TCHAR szProfileAccelerometerZero[];
extern const TCHAR szProfileUseCustomFonts[];
extern const TCHAR szProfileFontInfoWindowFont[];
extern const TCHAR szProfileFontTitleWindowFont[];
extern const TCHAR szProfileFontMapWindowFont[];
extern const TCHAR szProfileFontTitleSmallWindowFont[];
extern const TCHAR szProfileFontMapWindowBoldFont[];
extern const TCHAR szProfileFontCDIWindowFont[];
extern const TCHAR szProfileFontMapLabelFont[];
extern const TCHAR szProfileFontStatisticsFont[];
extern const TCHAR szProfileFontBugsBallastFont[];
extern const TCHAR szProfileFontAirspacePressFont[];
extern const TCHAR szProfileFontAirspaceColourDlgFont[];
extern const TCHAR szProfileFontTeamCodeFont[];

extern const TCHAR szProfileInfoBoxGeometry[];

extern const TCHAR szProfileFlarmSideData[];
extern const TCHAR szProfileFlarmAutoZoom[];
extern const TCHAR szProfileFlarmNorthUp[];

extern const TCHAR szProfileIgnoreNMEAChecksum[];
extern const TCHAR szProfileDisplayOrientation[];

#endif
