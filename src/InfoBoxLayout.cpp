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

#include "InfoBoxLayout.hpp"
#include "InfoBoxManager.hpp"
#include "InfoBox.hpp"
#include "LogFile.hpp"
#include "SettingsUser.hpp"
#include "Screen/Animation.hpp"
#include "Screen/Layout.hpp"
#include "Profile.hpp"

#include <stdio.h>

// Layouts:
// 0: default, infoboxes along top and bottom, map in middle
// 1: both infoboxes along bottom
// 2: both infoboxes along top
// 3: infoboxes along both sides
// 4: infoboxes along left side
// 5: infoboxes along right side
// 6: infoboxes GNAV

/*
Screen
640x480 landscape

480/6 = 80 control height

2/3 of width is map = 420
leaving 220 = 110 control width
*/


/*
Button 0 (x,y,sx,sy)
Button 1 (x,y,sx,sy)
...
InfoBox 0 (x,y,sx,sy)
*/

unsigned InfoBoxLayout::InfoBoxGeometry = 0;
int InfoBoxLayout::ControlWidth;
int InfoBoxLayout::ControlHeight;
int InfoBoxLayout::TitleHeight;

bool InfoBoxLayout::gnav = false;

bool geometrychanged = false;

bool InfoBoxLayout::fullscreen = false;

void
InfoBoxLayout::GetInfoBoxPosition(unsigned i, RECT rc, int *x, int *y,
    int *sizex, int *sizey)
{
  TCHAR reggeompx[50];
  TCHAR reggeompy[50];
  TCHAR reggeomsx[50];
  TCHAR reggeomsy[50];

  _stprintf(reggeompx, TEXT("InfoBoxPositionPosX%u"), i);
  _stprintf(reggeompy, TEXT("InfoBoxPositionPosY%u"), i);
  _stprintf(reggeomsx, TEXT("InfoBoxPositionSizeX%u"), i);
  _stprintf(reggeomsy, TEXT("InfoBoxPositionSizeY%u"), i);

  Profile::Get(reggeompx, *x);
  Profile::Get(reggeompy, *y);
  Profile::Get(reggeomsx, *sizex);
  Profile::Get(reggeomsy, *sizey);

  if (*sizey != ControlHeight)
    geometrychanged = true;
  if (*sizex != ControlWidth)
    geometrychanged = true;

  if ((*sizex == 0) || (*sizey == 0) || geometrychanged) {
    // not defined in registry so go with defaults
    // these will be saved back to registry
    switch (InfoBoxGeometry) {
    case 0:
      if (i < numInfoWindows / 2) {
        *x = i * ControlWidth;
        *y = rc.top;
      } else {
        *x = (i - numInfoWindows / 2) * ControlWidth;
        *y = rc.bottom - ControlHeight;
      }
      break;

    case 1:
      if (i < numInfoWindows / 2) {
        *x = i * ControlWidth;
        *y = rc.bottom - ControlHeight * 2;
      } else {
        *x = (i - numInfoWindows / 2) * ControlWidth;
        *y = rc.bottom - ControlHeight;
      }
      break;

    case 2:
      if (i < numInfoWindows / 2) {
        *x = i * ControlWidth;
        *y = rc.top;
        ;
      } else {
        *x = (i - numInfoWindows / 2) * ControlWidth;
        *y = rc.top + ControlHeight;
      }
      break;

    case 3:
      if (i < numInfoWindows / 2) {
        *x = rc.left;
        *y = rc.top + ControlHeight * i;
      } else {
        *x = rc.right - ControlWidth;
        *y = rc.top + ControlHeight * (i - numInfoWindows / 2);
      }
      break;

    case 4:
      if (i < numInfoWindows / 2) {
        *x = rc.left;
        *y = rc.top + ControlHeight * i;
      } else {
        *x = rc.left + ControlWidth;
        *y = rc.top + ControlHeight * (i - numInfoWindows / 2);
      }
      break;

    case 5:
      if (i < numInfoWindows / 2) {
        *x = rc.right - ControlWidth * 2;
        *y = rc.top + ControlHeight * i;
      } else {
        *x = rc.right - ControlWidth;
        *y = rc.top + ControlHeight * (i - numInfoWindows / 2);
      }
      break;

    case 6:
      if (i < 3) {
        *x = rc.right - ControlWidth * 2;
        *y = rc.top + ControlHeight * i;
      } else {
        if (i < 6) {
          *x = rc.right - ControlWidth * 2;
          *y = rc.top + ControlHeight * (i - 3) + ControlHeight * 3;
        } else {
          *x = rc.right - ControlWidth;
          *y = rc.top + ControlHeight * (i - 6) + ControlHeight * 3;
        }
      }
      break;

    case 7:
      *x = rc.right - ControlWidth;
      *y = rc.top + ControlHeight * i;
      break;
    };

    *sizex = ControlWidth;
    *sizey = ControlHeight;

    Profile::Set(reggeompx, *x);
    Profile::Set(reggeompy, *y);
    Profile::Set(reggeomsx, (int &)*sizex);
    Profile::Set(reggeomsy, (int &)*sizey);
  }
}

//
// Paolo Ventafridda, VENTA-ADDON Geometry change in Config menu 11
//
void
InfoBoxLayout::ScreenGeometry(RECT rc)
{
  Profile::Get(szProfileInfoBoxGeometry, InfoBoxGeometry);

#if defined(PNA) || defined(FIVV)
// VENTA-ADDON GEOM
  unsigned Temp = 0;
  Profile::Get(szProfileInfoBoxGeom, Temp);
  if (InfoBoxGeometry != Temp) {
    LogStartUp(_T("Geometry was changed in config, applying"));
    InfoBoxGeometry=Temp;
  }
#endif

  // JMW testing only
  geometrychanged = true;

  if (Layout::landscape) {
    if (InfoBoxGeometry < 4) {
      geometrychanged = true;
      InfoBoxGeometry = 6;
    }
  } else if (Layout::square) {
    if (InfoBoxGeometry < 7)
      geometrychanged = true;

    InfoBoxGeometry = 7;
  } else {
    gnav = false;
    if (InfoBoxGeometry >= 3) {
      InfoBoxGeometry = 0;
      geometrychanged = true;
    }
  }

  Profile::Set(szProfileInfoBoxGeometry, (int &)InfoBoxGeometry);

  // JMW testing
  if (InfoBoxGeometry == 6)
    gnav = true;

  if (gnav) {
    numInfoWindows = 9;
  } else if (Layout::square) {
    numInfoWindows = 5;
  } else {
    numInfoWindows = 8;
  }
}

RECT
InfoBoxLayout::GetInfoBoxSizes(RECT rc)
{
  RECT MapRect;

  switch (InfoBoxGeometry) {
  case 0:
    // portrait
    // calculate control dimensions
    ControlWidth = 2 * (rc.right - rc.left) / numInfoWindows;
    ControlHeight = (unsigned)((rc.bottom - rc.top) / CONTROLHEIGHTRATIO);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top + ControlHeight;
    MapRect.left = rc.left;
    MapRect.bottom = rc.bottom - ControlHeight;
    MapRect.right = rc.right;
    break;

  case 1:
    // not used
    // calculate control dimensions

    ControlWidth = 2 * (rc.right - rc.left) / numInfoWindows;
    ControlHeight = (unsigned)((rc.bottom - rc.top) / CONTROLHEIGHTRATIO);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top;
    MapRect.left = rc.left;
    MapRect.bottom = rc.bottom - ControlHeight * 2;
    MapRect.right = rc.right;
    break;

  case 2:
    // not used
    // calculate control dimensions

    ControlWidth = 2 * (rc.right - rc.left) / numInfoWindows;
    ControlHeight = (unsigned)((rc.bottom - rc.top) / CONTROLHEIGHTRATIO);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top + ControlHeight * 2;
    MapRect.left = rc.left;
    MapRect.bottom = rc.bottom;
    MapRect.right = rc.right;
    break;

  case 3:
    // not used
    // calculate control dimensions

    ControlWidth = (unsigned)((rc.right - rc.left) / CONTROLHEIGHTRATIO * 1.3);
    ControlHeight = (unsigned)(2 * (rc.bottom - rc.top) / numInfoWindows);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top;
    MapRect.left = rc.left + ControlWidth;
    MapRect.bottom = rc.bottom;
    MapRect.right = rc.right - ControlWidth;
    break;

  case 4:
    // calculate control dimensions

    ControlWidth = (unsigned)((rc.right - rc.left) / CONTROLHEIGHTRATIO * 1.3);
    ControlHeight = (unsigned)(2 * (rc.bottom - rc.top) / numInfoWindows);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top;
    MapRect.left = rc.left + ControlWidth * 2;
    MapRect.bottom = rc.bottom;
    MapRect.right = rc.right;
    break;

  case 5:
    // not used
    // calculate control dimensions

    ControlWidth = (unsigned)((rc.right - rc.left) / CONTROLHEIGHTRATIO * 1.3);
    ControlHeight = (unsigned)(2 * (rc.bottom - rc.top) / numInfoWindows);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top;
    MapRect.left = rc.left;
    MapRect.bottom = rc.bottom;
    MapRect.right = rc.right - ControlWidth * 2;
    break;

  case 6:
    // landscape
    // calculate control dimensions

    ControlHeight = (unsigned)((rc.bottom - rc.top) / 6);
    ControlWidth = (unsigned)(ControlHeight * 1.44); // preserve relative shape
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top;
    MapRect.left = rc.left;
    MapRect.bottom = rc.bottom;
    MapRect.right = rc.right - ControlWidth * 2;

    break;

  case 7:
    // square
    // calculate control dimensions

    ControlWidth = (unsigned)((rc.right - rc.left) * 0.2);
    ControlHeight = (unsigned)((rc.bottom - rc.top) / 5);
    TitleHeight = (unsigned)(ControlHeight / TITLEHEIGHTRATIO);

    // calculate small map screen size

    MapRect.top = rc.top;
    MapRect.left = rc.left;
    MapRect.bottom = rc.bottom;
    MapRect.right = rc.right - ControlWidth;

    break;
  };

  return MapRect;
}