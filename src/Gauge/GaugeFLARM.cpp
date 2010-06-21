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

#include "Gauge/GaugeFLARM.hpp"
#include "Math/FastMath.h"
#include "Math/FastRotation.hpp"
#include "Math/Screen.hpp"
#include "Screen/Graphics.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Layout.hpp"
#include "Screen/BitmapCanvas.hpp"
#include "NMEA/Info.hpp"
#include "Units.hpp"
#include "resource.h"
#include "Dialogs.h"

#include <stdio.h>

#include <algorithm>

using std::min;
using std::max;

#define FLARMMAXRANGE 2000

/**
 * Returns the distance scaled at a quadratic(?) scale
 * @param d Distance to the own plane
 */
int
GaugeFLARM::RangeScale(double d)
{
  double drad = max(0.0, 1.0 - d / FLARMMAXRANGE);
  return iround(radius * (1.0 - drad * drad));
}

/**
 * Draws the FLARM gauge background bitmap to the given canvas
 * @param canvas Canvas for painting
 */
void
GaugeFLARM::RenderBg(Canvas &canvas)
{
  // Load the background bitmap
  BitmapCanvas hdcTemp(canvas, hRoseBitMap);

  // If it doesn't fit, make it fit
  if ((unsigned)hRoseBitMapSize.cx != canvas.get_width() ||
      (unsigned)hRoseBitMapSize.cy != canvas.get_height()) {
    canvas.stretch(0, 0, canvas.get_width(), canvas.get_height(), hdcTemp,
                   0, 0, hRoseBitMapSize.cx, hRoseBitMapSize.cy);
  } else {
    canvas.copy(0, 0, canvas.get_width(), canvas.get_height(), hdcTemp, 0, 0);
  }
}

/**
 * Renders the FLARM traffic to the given canvas
 * @param canvas Canvas for drawing
 * @param gps_info NMEA_INFO struct containing the FLARM targets
 */
void
GaugeFLARM::RenderTraffic(Canvas &canvas, const NMEA_INFO &gps_info)
{
  // TODO enhancement: support red/green Color blind pilots

  // Set font and colors
  canvas.select(TitleWindowFont);
  canvas.set_text_color(Color::BLACK);
  canvas.set_background_color(Color::WHITE);

  // Cycle through FLARM targets
  for (unsigned i = 0; i < FLARM_STATE::FLARM_MAX_TRAFFIC; ++i) {
    const FLARM_TRAFFIC &traffic = gps_info.flarm.FLARM_Traffic[i];

    if (!traffic.defined())
      continue;

    // Set the arrow color depending on alarm level
    switch (traffic.AlarmLevel) {
    case 1:
      canvas.select(MapGfx.WarningBrush);
      break;
    case 2:
    case 3:
      canvas.select(MapGfx.AlarmBrush);
      break;
    case 0:
    case 4:
      canvas.select(MapGfx.TrafficBrush);
      break;
    }

    double x, y;
    x = traffic.RelativeEast;
    y = -traffic.RelativeNorth;
    double d = hypot(x, y);
    if (d > 0) {
      x /= d;
      y /= d;
    } else {
      x = 0;
      y = 0;
    }
    double dh = traffic.RelativeAltitude;
    double slope;
    if (d > 0) {
      slope = atan2(dh, d) * 2.0 / M_PI; // (-1,1)
    } else {
      if (dh>0) {
        slope = 1;
      } else if (dh<0) {
        slope = -1;
      } else {
        slope = 0;
      }
    }
    slope = max(-1.0, min(1.0, slope * 2)); // scale so 45 degrees or more=90

    // display for FLARM gauge is always track up
    const Angle DisplayAngle = (Angle()-gps_info.TrackBearing).as_bearing();
    const Angle TrafficAngle = (traffic.TrackBearing + DisplayAngle).as_bearing();

    // or use .Heading? (no, because heading is not reliable)
    const FastRotation r(DisplayAngle);
    FastRotation::Pair p = r.Rotate(x, y);
    x = p.first;
    y = p.second;

    double scale = RangeScale(d);

    // Calculate screen coordinates
    POINT sc;
    sc.x = center.x + iround(x * scale);
    sc.y = center.y + iround(y * scale);

    if (traffic.AlarmLevel > 0) {
      // Draw line through target, from target to screen edge.
      // This is deliberate: when targets are close, there is no point drawing
      // a vector from the center to the target because it is not noticeable.
      // Having a line from the target to the edge is immediately visible ---
      // the first thing the pilot notices is the direction he needs to look,
      // which of course is the most important thing.
      //
      canvas.line(sc.x, sc.y, center.x + iround(radius * x),
                  center.y + iround(radius * y));

      // @todo: this should be width set by IBLSCALE, otherwise
      // with default pen it is too thin to be visible on 640x480 displays 
    }

    // Create an arrow polygon
    POINT Arrow[5];
    Arrow[0].x = -3;
    Arrow[0].y = 4;
    Arrow[1].x = 0;
    Arrow[1].y = -5;
    Arrow[2].x = 3;
    Arrow[2].y = 4;
    Arrow[3].x = 0;
    Arrow[3].y = 1;
    Arrow[4].x = -3;
    Arrow[4].y = 4;

    // Rotate and shift the arrow
    PolygonRotateShift(Arrow, 5, sc.x, sc.y, TrafficAngle);

    // Draw the polygon
    canvas.polygon(Arrow, 5);

    const short relalt = iround(Units::ToUserUnit(traffic.RelativeAltitude / 100,
                                                  Units::AltitudeUnit));

    // if (relative altitude is other than zero)
    if (relalt == 0)
      continue;

    // Write the relativ altitude devided by 100 to the Buffer
    TCHAR Buffer[10];
    _stprintf(Buffer, TEXT("%d"), abs(relalt));

    // Calculate size of the output string
    SIZE tsize = canvas.text_size(Buffer);
    tsize.cx = (tsize.cx + IBLSCALE(6)) / 2;

    // Draw string
    canvas.text(sc.x - tsize.cx + IBLSCALE(7),
                sc.y - tsize.cy - IBLSCALE(5),
                Buffer);

    // Set black brush for the up/down arrow
    canvas.black_brush();

    // Prepare the triangular polygon
    POINT triangle[4];
    triangle[0].x = 3;
    triangle[0].y = -3;
    triangle[1].x = 6;
    triangle[1].y = 1;
    triangle[2].x = 0;
    triangle[2].y = 1;

    // Flip = -1 for arrow pointing downwards
    short flip = 1;
    if (relalt < 0) {
      flip = -1;
    }

    // Shift the arrow to the right position
    for (int j = 0; j < 3; j++) {
      triangle[j].x = sc.x + IBLSCALE(triangle[j].x) - tsize.cx;
      triangle[j].y = sc.y + flip * IBLSCALE(triangle[j].y)
                      - tsize.cy / 2 - IBLSCALE(5);
    }
    triangle[3].x = triangle[0].x;
    triangle[3].y = triangle[0].y;

    // Draw the arrow
    canvas.polygon(triangle, 4);
  }
}

/**
 * Render the FLARM gauge to the buffer canvas
 * @param gps_info The NMEA_INFO struct containing the FLARM targets
 */
void
GaugeFLARM::Render(const NMEA_INFO &gps_info)
{
  // Render the background
  RenderBg(get_canvas());

  // Render the traffic on top
  RenderTraffic(get_canvas(), gps_info);

  // Draw buffer to the screen
  invalidate();
}

/**
 * Constructor of the GaugeFLARM class
 * @param parent Parent window
 * @param left Left edge of window pixel location
 * @param top Top edge of window pixel location
 * @param width Width of window (pixels)
 * @param height Height of window (pixels)
 */
GaugeFLARM::GaugeFLARM(ContainerWindow &parent,
                       int left, int top, unsigned width, unsigned height)
  :ForceVisible(false), Suppress(false)
{
  // start of new code for displaying FLARM window

  WindowStyle style;
  style.hide();

  set(parent, left, top, width, height, style);

  center.x = get_hmiddle();
  center.y = get_vmiddle();
  radius = min(get_right() - center.x, get_bottom() - center.y);

  // Load the background bitmap
  hRoseBitMap.load(IDB_FLARMROSE);

  // Save the size of the background bitmap
  hRoseBitMapSize = hRoseBitMap.get_size();

  // Render Background for the first time
  RenderBg(get_canvas());
}

void
GaugeFLARM::Update(bool enable, const NMEA_INFO &gps_info)
{
  // If FLARM alarm level higher then 0
  if (gps_info.flarm.FLARM_AlarmLevel > 0)
    // Show FLARM gauge and do not care about suppression
    Suppress = false;

  bool visible = ForceVisible ||
    (gps_info.flarm.FLARMTraffic && enable && !Suppress);
  if (visible) {
    Render(gps_info);
    send_user(MSG_SHOW);
  } else
    send_user(MSG_HIDE);
}

/**
 * This function is called when the mouse is pressed on the FLARM gauge and
 * opens the FLARM Traffic dialog
 * @param x x-Coordinate of the click
 * @param y x-Coordinate of the click
 * @return
 */
bool
GaugeFLARM::on_mouse_down(int x, int y)
{
  dlgFlarmTrafficShowModal();
  return true;
}

bool
GaugeFLARM::on_user(unsigned id)
{
  switch ((msg)id) {
  case MSG_SHOW:
    show();
    return true;

  case MSG_HIDE:
    hide();
    return true;
  }

  return BufferWindow::on_user(id);
}