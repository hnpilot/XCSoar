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

#ifndef XCSOAR_TERRAIN_RENDERER_HPP
#define XCSOAR_TERRAIN_RENDERER_HPP

#include "Navigation/GeoPoint.hpp"

#include <windef.h>

#define NUM_COLOR_RAMP_LEVELS 13

class RawBitmap;
class Canvas;
struct BGRColor;
class RasterMap;
class Projection;
class RasterTerrain;
class RasterRounding;
struct COLORRAMP;

class TerrainRenderer {
public:
  TerrainRenderer(const RasterTerrain *_terrain,
                  const RECT &rc);
  virtual ~TerrainRenderer();

public:
  POINT spot_max_pt;
  POINT spot_min_pt;
  short spot_max_val;
  short spot_min_val;

protected:
  bool is_terrain;
  bool do_shading;
  bool do_water;
  unsigned int height_scale;
  const COLORRAMP *color_ramp;
  const COLORRAMP *last_color_ramp;
  RasterMap *DisplayMap;
  int interp_levels;

  virtual bool SetMap();

private:
  const RasterTerrain *terrain;
  const RECT rect_big;

  const RECT BorderSlope(const RECT& rect_quantised, const int edge) const;

  // screen dimensions in coarse pixels
  unsigned int width_sub, height_sub;
  unsigned int quantisation_pixels;
  // step size used for slope calculations
  unsigned int quantisation_effective;

  RawBitmap *sbuf;

  fixed pixelsize_d;

  int oversampling;
  int blursize;

  unsigned short *hBuf;
  BGRColor *colorBuf;
  RasterRounding *rounding;

  short TerrainRamp;
  short TerrainContrast;
  short TerrainBrightness;

  const RECT Height(const Projection &map_projection);
  void ScanSpotHeights(const RECT& rect);
  void FillHeightBuffer(const Projection &map_projection,
                        const RECT& rect);
  void Slope(const RECT& rect_quantised, const int sx, const int sy, const int sz);
  void ColorTable();
  void Draw(Canvas &canvas, RECT src_rect, RECT dest_rect);

  virtual bool do_scan_spot();

public:
  void
  SetSettings(short _TerrainRamp, short _TerrainContrast,
      short _TerrainBrightness)
  {
    TerrainRamp = _TerrainRamp;
    TerrainContrast = _TerrainContrast;
    TerrainBrightness = _TerrainBrightness;
  }

  /**
   * @param day_time the UTC time, in seconds since midnight
   */
  bool Draw(Canvas &canvas, const Projection &map_projection,
      const Angle sunazimuth, const Angle sunelevation);
};

#endif