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

#include "Screen/Graphics.hpp"
#include "Screen/UnitSymbol.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Layout.hpp"
#include "Screen/Ramp.hpp"
#include "Appearance.hpp"
#include "SettingsUser.hpp"
#include "Units.hpp"
#include "Screen/LabelBlock.hpp"
#include "resource.h"
#include "Asset.hpp"

#define NUMSNAILRAMP 6

const COLORRAMP snail_colors[] = {
  {0,         0xff, 0x3e, 0x00},
  {50,        0xcd, 0x4f, 0x27},
  {100,       0x8f, 0x8f, 0x8f},
  {150,       0x27, 0xcd, 0x4f},
  {201,       0x00, 0xff, 0x3e},
  {501,       0x00, 0xff, 0x3e}
};

// airspace brushes/colours
const Color
ScreenGraphics::GetAirspaceColour(const int i)
{
  return Colours[i];
}

const Brush &
ScreenGraphics::GetAirspaceBrush(const int i)
{
  return hAirspaceBrushes[i];
}

const Color
ScreenGraphics::GetAirspaceColourByClass(const int i,
    const SETTINGS_MAP &settings)
{
  return Colours[settings.iAirspaceColour[i]];
}

const Brush &
ScreenGraphics::GetAirspaceBrushByClass(const int i,
    const SETTINGS_MAP &settings)
{
  return hAirspaceBrushes[settings.iAirspaceBrush[i]];
}

const Color ScreenGraphics::ColorSelected = Color(0xC0, 0xC0, 0xC0);
const Color ScreenGraphics::ColorUnselected = Color::WHITE;
const Color ScreenGraphics::ColorWarning = Color::RED;
const Color ScreenGraphics::ColorOK = Color::BLUE;
const Color ScreenGraphics::ColorBlack = Color::BLACK;
const Color ScreenGraphics::ColorMidGrey = Color::GRAY;

const Color ScreenGraphics::inv_redColor = Color(0xff, 0x70, 0x70);
const Color ScreenGraphics::inv_blueColor = Color(0x90, 0x90, 0xff);
const Color ScreenGraphics::inv_yellowColor = Color::YELLOW;
const Color ScreenGraphics::inv_greenColor = Color::GREEN;
const Color ScreenGraphics::inv_magentaColor = Color::MAGENTA;

const Color ScreenGraphics::TaskColor = Color(0, 120, 0); // was 255
const Color ScreenGraphics::BackgroundColor = Color::WHITE;

const Color ScreenGraphics::Colours[] = {
  Color::RED,
  Color::GREEN,
  Color::BLUE,
  Color::YELLOW,
  Color::MAGENTA,
  Color::CYAN,
  Color(0x7F, 0x00, 0x00),
  Color(0x00, 0x7F, 0x00),
  Color(0x00, 0x00, 0x7F),
  Color(0x7F, 0x7F, 0x00),
  Color(0x7F, 0x00, 0x7F),
  Color(0x00, 0x7F, 0x7F),
  Color::WHITE,
  Color(0xC0, 0xC0, 0xC0),
  Color(0x7F, 0x7F, 0x7F),
  Color::BLACK,
};

// JMW TODO: some of these should be loaded after settings are loaded

#include "LogFile.hpp"  // debug

void
ScreenGraphics::Initialise(HINSTANCE hInstance,
    const SETTINGS_MAP &settings_map)
{
  /// @todo enhancement: support red/green color blind pilots with adjusted colour scheme

  int i;

  LogStartUp(TEXT("Initialise graphics"));

  LoadUnitSymbols();

  infoSelectedBrush.set(MapGfx.ColorSelected);
  infoUnselectedBrush.set(MapGfx.ColorUnselected);

  AlarmBrush.set(Color::RED);
  WarningBrush.set(Color(0xFF, 0xA2, 0x00));
  TrafficBrush.set(Color::GREEN);

  hBackgroundBrush.set(BackgroundColor);

  hFLARMTraffic.load(IDB_FLARMTRAFFIC);
  hTerrainWarning.load(IDB_TERRAINWARNING);
  hGPSStatus1.load(IDB_GPSSTATUS1, false);
  hGPSStatus2.load(IDB_GPSSTATUS2, false);
  hLogger.load(IDB_LOGGER);
  hLoggerOff.load(IDB_LOGGEROFF);
  hBmpTeammatePosition.load(IDB_TEAMMATE_POS);

  hAutoMacCready.load(IDB_AUTOMACCREADY, false);
  hCruise.load(IDB_CRUISE, false);
  hClimb.load(IDB_CLIMB, false);
  hFinalGlide.load(IDB_FINALGLIDE, false);
  hAbort.load(IDB_ABORT, false);

  // airspace brushes and colors
  hAirspaceBitmap[0].load(IDB_AIRSPACE0);
  hAirspaceBitmap[1].load(IDB_AIRSPACE1);
  hAirspaceBitmap[2].load(IDB_AIRSPACE2);
  hAirspaceBitmap[3].load(IDB_AIRSPACE3);
  hAirspaceBitmap[4].load(IDB_AIRSPACE4);
  hAirspaceBitmap[5].load(IDB_AIRSPACE5);
  hAirspaceBitmap[6].load(IDB_AIRSPACE6);
  hAirspaceBitmap[7].load(IDB_AIRSPACE7);
  hAirspaceInterceptBitmap.load(IDB_AIRSPACEI);

  hAboveTerrainBitmap.load(IDB_ABOVETERRAIN);

  for (i = 0; i < NUMAIRSPACEBRUSHES; i++) {
    hAirspaceBrushes[i].set(hAirspaceBitmap[i]);
  }
  hAboveTerrainBrush.set(hAboveTerrainBitmap);

  hbWind.set(Color::GRAY);

  hBmpMapScale.load(IDB_MAPSCALE_A);
  hBrushFlyingModeAbort.set(Color::RED);

  hBmpThermalSource.load(IDB_THERMALSOURCE);
  hBmpTarget.load(IDB_TARGET);

  hbCompass.set(has_colors()
                ? Color(0x40, 0x40, 0xFF)
                : Color::WHITE);

  hbThermalBand.set(Color(0x80, 0x80, 0xFF));
  hbBestCruiseTrack.set(Color::BLUE);
  hbFinalGlideBelow.set(Color::RED);
  hbFinalGlideBelowLandable.set(Color(0xFF, 180, 0x00));
  hbFinalGlideAbove.set(Color::GREEN);

  // all below depend on settings!

  BYTE Red, Green, Blue;

  int iwidth;
  int minwidth;
  minwidth = max(IBLSCALE(2), IBLSCALE(settings_map.SnailWidthScale) / 16);

  for (i = 0; i < NUMSNAILCOLORS; i++) {
    short ih = i * 200 / (NUMSNAILCOLORS - 1);
    ColorRampLookup(ih, Red, Green, Blue, snail_colors, NUMSNAILRAMP, 6);

    if (i < NUMSNAILCOLORS / 2) {
      iwidth = minwidth;
    } else {
      iwidth = max(minwidth, (i - NUMSNAILCOLORS / 2)
          * IBLSCALE(settings_map.SnailWidthScale) / NUMSNAILCOLORS);
    }

    hSnailColours[i] = Color((BYTE)Red, (BYTE)Green, (BYTE)Blue);
    hSnailPens[i].set(iwidth, hSnailColours[i]);
  }

  hpCompassBorder.set(IBLSCALE(3), Color::WHITE);

  if (Appearance.InverseAircraft) {
    hpAircraft.set(IBLSCALE(3), Color::BLACK);
    hpAircraftBorder.set(IBLSCALE(1), Color::WHITE);
  } else {
    hpAircraft.set(IBLSCALE(3), Color::WHITE);
    hpAircraftBorder.set(IBLSCALE(1), Color::BLACK);
  }

  hpWind.set(IBLSCALE(2), Color::BLACK);

  hpWindThick.set(IBLSCALE(4), Color(255, 220, 220));

  hpBearing.set(IBLSCALE(2), Color::BLACK);
  hpBestCruiseTrack.set(IBLSCALE(1), Color::BLUE);
  hpCompass.set(IBLSCALE(1), has_colors()
                ? Color(0xcf, 0xcf, 0xFF) : Color::BLACK);
  hpThermalBand.set(IBLSCALE(2), Color(0x40, 0x40, 0xFF));
  hpThermalBandGlider.set(IBLSCALE(2), Color(0x00, 0x00, 0x30));

  hpFinalGlideBelow.set(IBLSCALE(1), Color(0xFF, 0xA0, 0xA0));
  hpFinalGlideBelowLandable.set(IBLSCALE(1), Color(255, 196, 0));

  hpFinalGlideAbove.set(IBLSCALE(1), Color(0xA0, 0xFF, 0xA0));

  hpSpeedSlow.set(IBLSCALE(1), Color::RED);
  hpSpeedFast.set(IBLSCALE(1), Color::GREEN);

  hpStartFinishThick.set(IBLSCALE(5), TaskColor);

  hpStartFinishThin.set(IBLSCALE(1), Color::RED);

  hpMapScale.set(IBLSCALE(1), Color::BLACK);
  hpTerrainLine.set(Pen::DASH, IBLSCALE(1), Color(0x30, 0x30, 0x30));
  hpTerrainLineBg.set(IBLSCALE(1), Color::WHITE);

  hpVisualGlideLightBlack.set(Pen::DASH, IBLSCALE(1), Color::BLACK);
  hpVisualGlideHeavyBlack.set(Pen::DASH, IBLSCALE(2), Color::BLACK);
  hpVisualGlideLightRed.set(Pen::DASH, IBLSCALE(1), Color::RED);
  hpVisualGlideHeavyRed.set(Pen::DASH, IBLSCALE(2), Color::RED);

  SmallIcon.load(IDB_SMALL);
  TurnPointIcon.load(IDB_TURNPOINT);

  if (Appearance.IndLandable == wpLandableDefault) {
    AirportReachableIcon.load_big(IDB_REACHABLE, IDB_REACHABLE_BIG);
    AirportUnreachableIcon.load_big(IDB_LANDABLE, IDB_LANDABLE_BIG);
    FieldReachableIcon.load_big(IDB_REACHABLE, IDB_REACHABLE_BIG);
    FieldUnreachableIcon.load_big(IDB_LANDABLE, IDB_LANDABLE_BIG);
  } else if (Appearance.IndLandable == wpLandableAltA) {
    AirportReachableIcon.load(IDB_AIRPORT_REACHABLE);
    AirportUnreachableIcon.load(IDB_AIRPORT_UNREACHABLE);
    FieldReachableIcon.load(IDB_OUTFIELD_REACHABLE);
    FieldUnreachableIcon.load(IDB_OUTFIELD_UNREACHABLE);
  } else if (Appearance.IndLandable == wpLandableAltB) {
    AirportReachableIcon.load(IDB_AIRPORT_REACHABLE);
    AirportUnreachableIcon.load(IDB_AIRPORT_UNREACHABLE2);
    FieldReachableIcon.load(IDB_OUTFIELD_REACHABLE);
    FieldUnreachableIcon.load(IDB_OUTFIELD_UNREACHABLE2);
  }

  for (int i = 0; i < AIRSPACECLASSCOUNT; i++) {
    hAirspacePens[i].set(IBLSCALE(2), GetAirspaceColourByClass(i, settings_map));
  }
}

void
ScreenGraphics::Destroy()
{
  int i;

  hCruise.reset();
  hClimb.reset();
  hFinalGlide.reset();
  hAutoMacCready.reset();
  hFLARMTraffic.reset();
  hTerrainWarning.reset();
  hGPSStatus1.reset();
  hGPSStatus2.reset();
  hAbort.reset();
  hLogger.reset();
  hLoggerOff.reset();

  hpAircraft.reset();
  hpAircraftBorder.reset();
  hpWind.reset();
  hpWindThick.reset();
  hpBearing.reset();
  hpBestCruiseTrack.reset();
  hpCompass.reset();
  hpThermalBand.reset();
  hpThermalBandGlider.reset();
  hpFinalGlideAbove.reset();
  hpFinalGlideBelow.reset();
  hpFinalGlideBelowLandable.reset();
  hpMapScale.reset();
  hpTerrainLine.reset();
  hpTerrainLineBg.reset();
  hpSpeedFast.reset();
  hpSpeedSlow.reset();
  hpStartFinishThick.reset();
  hpStartFinishThin.reset();

  hpVisualGlideLightBlack.reset();
  hpVisualGlideLightRed.reset();
  hpVisualGlideHeavyRed.reset();
  hpVisualGlideHeavyBlack.reset();

  hbCompass.reset();
  hbThermalBand.reset();
  hbBestCruiseTrack.reset();
  hbFinalGlideBelow.reset();
  hbFinalGlideBelowLandable.reset();
  hbFinalGlideAbove.reset();
  hbWind.reset();

  hBmpMapScale.reset();
  hBmpCompassBg.reset();
  hBackgroundBrush.reset();
  hBmpClimbeAbort.reset();

  hpCompassBorder.reset();
  hBrushFlyingModeAbort.reset();

  SmallIcon.reset();
  TurnPointIcon.reset();

  AirportReachableIcon.reset();
  AirportUnreachableIcon.reset();
  FieldReachableIcon.reset();
  FieldUnreachableIcon.reset();
  hBmpThermalSource.reset();
  hBmpTarget.reset();
  hBmpTeammatePosition.reset();

  for (i = 0; i < NUMAIRSPACEBRUSHES; i++) {
    hAirspaceBrushes[i].reset();
    hAirspaceBitmap[i].reset();
  }
  hAirspaceInterceptBitmap.reset();

  hAboveTerrainBitmap.reset();
  hAboveTerrainBrush.reset();

  for (i = 0; i < AIRSPACECLASSCOUNT; i++) {
    hAirspacePens[i].reset();
  }

  for (i = 0; i < NUMSNAILCOLORS; i++) {
    hSnailPens[i].reset();
  }

  TrafficBrush.reset();
  WarningBrush.reset();
  AlarmBrush.reset();

  infoSelectedBrush.reset();
  infoUnselectedBrush.reset();

  UnloadUnitSymbols();
}

bool
TextInBoxMoveInView(POINT *offset, RECT *brect, const RECT &MapRect)
{
  bool res = false;

  int LabelMargin = 4;

  offset->x = 0;
  offset->y = 0;

  if (MapRect.top > brect->top) {
    int d = MapRect.top - brect->top;
    brect->top += d;
    brect->bottom += d;
    offset->y += d;
    brect->bottom -= d;
    brect->left -= d;
    offset->x -= d;
    res = true;
  }

  if (MapRect.right < brect->right) {
    int d = MapRect.right - brect->right;

    if (offset->y < LabelMargin) {
      int dy;

      if (d > -LabelMargin) {
        dy = LabelMargin - offset->y;
        if (d > -dy)
          dy = -d;
      } else {
        int x = d + (brect->right - brect->left) + 10;

        dy = x - offset->y;

        if (dy < 0)
          dy = 0;

        if (dy > LabelMargin)
          dy = LabelMargin;
      }

      brect->top += dy;
      brect->bottom += dy;
      offset->y += dy;
    }

    brect->right += d;
    brect->left += d;
    offset->x += d;

    res = true;
  }

  if (MapRect.bottom < brect->bottom) {
    if (offset->x == 0) {
      int d = MapRect.bottom - brect->bottom;
      brect->top += d;
      brect->bottom += d;
      offset->y += d;
    } else {
      if (offset->x < -LabelMargin) {
        int d = -(brect->bottom - brect->top) - 10;
        brect->top += d;
        brect->bottom += d;
        offset->y += d;
      } else {
        int d = -(2 * offset->x + (brect->bottom - brect->top));
        brect->top += d;
        brect->bottom += d;
        offset->y += d;
      }
    }

    res = true;
  }

  if (MapRect.left > brect->left) {
    int d = MapRect.left - brect->left;
    brect->right += d;
    brect->left += d;
    offset->x += d;
    res = true;
  }

  return res;
}

// returns true if really wrote something
bool
TextInBox(Canvas &canvas, const TCHAR* Value, int x, int y,
    TextInBoxMode_t Mode, const RECT MapRect, LabelBlock *label_block)
{
  RECT brect;
  POINT org;

  bool drawn = false;

  if ((x < MapRect.left - WPCIRCLESIZE)
      || (x > MapRect.right + (WPCIRCLESIZE * 3))
      || (y < MapRect.top - WPCIRCLESIZE)
      || (y > MapRect.bottom + WPCIRCLESIZE))
    return drawn; // FIX Not drawn really

  org.x = x;
  org.y = y;

  canvas.white_brush();

  if (Mode.AsFlag.Reachable) {
    if (Appearance.IndLandable == wpLandableDefault)
      // make space for the green circle
      x += 5;
  }

  // landable waypoint label inside white box
  if (!Mode.AsFlag.NoSetFont) {
    // VENTA5 predefined font from calling function
    canvas.select(Mode.AsFlag.Border ? MapWindowBoldFont : MapWindowFont);
  }

  SIZE tsize = canvas.text_size(Value);

  if (Mode.AsFlag.AlignRight) {
    x -= tsize.cx;
  } else if (Mode.AsFlag.AlignCenter) {
    x -= tsize.cx / 2;
    y -= tsize.cy / 2;
  }

  bool notoverlapping = true;

  if (Mode.AsFlag.Border || Mode.AsFlag.WhiteBorder) {
    POINT offset;

    brect.left = x - 2;
    brect.right = brect.left + tsize.cx + 4;
    brect.top = y + ((tsize.cy + 4) >> 3) - 2;
    brect.bottom = brect.top + 3 + tsize.cy - ((tsize.cy + 4) >> 3);

    if (Mode.AsFlag.AlignRight)
      x -= 3;

    if (TextInBoxMoveInView(&offset, &brect, MapRect)) {
      x += offset.x;
      y += offset.y;
    }

    if (label_block)
      notoverlapping = label_block->check(brect);
    else
      notoverlapping = true;

    if (notoverlapping) {
      if (Mode.AsFlag.Border)
        canvas.select(MapGfx.hpMapScale);
      else
        canvas.white_pen();


      canvas.round_rectangle(brect.left, brect.top, brect.right, brect.bottom,
          IBLSCALE(8), IBLSCALE(8));

      if (is_embedded()) {
        canvas.text_opaque(x, y, Value);
      } else {
        canvas.background_transparent();
        canvas.text(x, y, Value);
      }

      drawn = true;
    }
  } else if (Mode.AsFlag.FillBackground) {
    POINT offset;

    brect.left = x - 1;
    brect.right = brect.left + tsize.cx + 1;
    brect.top = y + ((tsize.cy + 4) >> 3);
    brect.bottom = brect.top + tsize.cy - ((tsize.cy + 4) >> 3);

    if (Mode.AsFlag.AlignRight)
      x -= 2;

    if (TextInBoxMoveInView(&offset, &brect, MapRect)) {
      x += offset.x;
      y += offset.y;
    }

    if (label_block)
      notoverlapping = label_block->check(brect);
    else
      notoverlapping = true;

    if (notoverlapping) {
      canvas.set_background_color(Color::WHITE);
      canvas.text_opaque(x, y, &brect, Value);
      drawn = true;
    }
  } else if (Mode.AsFlag.WhiteBold) {
    brect.left = x - 2;
    brect.right = brect.left + tsize.cx + 4;
    brect.top = y + ((tsize.cy + 4) >> 3) - 2;
    brect.bottom = brect.top + 3 + tsize.cy - ((tsize.cy + 4) >> 3);

    if (label_block)
      notoverlapping = label_block->check(brect);
    else
      notoverlapping = true;

    if (notoverlapping) {
      canvas.set_text_color(Color::WHITE);

      if (is_embedded()) {
        canvas.text_opaque(x + 2, y, Value);
        canvas.text_opaque(x + 1, y, Value);
        canvas.text_opaque(x - 1, y, Value);
        canvas.text_opaque(x - 2, y, Value);
        canvas.text_opaque(x, y + 1, Value);
        canvas.text_opaque(x, y - 1, Value);
        canvas.set_text_color(Color::BLACK);

        canvas.text_opaque(x, y, Value);
      } else {
        canvas.background_transparent();
        canvas.text(x + 1, y, Value);
        canvas.text(x + 2, y, Value);
        canvas.text(x - 1, y, Value);
        canvas.text(x - 2, y, Value);
        canvas.text(x, y + 1, Value);
        canvas.text(x, y - 1, Value);
        canvas.set_text_color(Color::BLACK);

        canvas.text(x, y, Value);
      }

      drawn = true;
    }
  } else {
    brect.left = x - 2;
    brect.right = brect.left + tsize.cx + 4;
    brect.top = y + ((tsize.cy + 4) >> 3) - 2;
    brect.bottom = brect.top + 3 + tsize.cy - ((tsize.cy + 4) >> 3);

    if (label_block)
      notoverlapping = label_block->check(brect);
    else
      notoverlapping = true;

    if (notoverlapping) {
      if (is_embedded()) {
        canvas.text_opaque(x, y, Value);
      } else {
        canvas.background_transparent();
        canvas.text(x, y, Value);
      }

      drawn = true;
    }
  }

  return drawn;
}