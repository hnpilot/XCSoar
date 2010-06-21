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

#include "Screen/Bitmap.hpp"
#include "Interface.hpp"

#if defined(ENABLE_SDL) && !defined(WIN32)
#include "LocalPath.hpp"
#endif

#ifdef ENABLE_SDL
#include <SDL/SDL_endian.h>

  #ifdef WIN32
    #include <windows.h>
  #endif
#endif

#include <assert.h>

#ifndef ENABLE_SDL
struct DIBINFO : public BITMAPINFO
{
	RGBQUAD	 arColors[255];    // Color table info - adds an extra 255 entries to palette

	operator LPBITMAPINFO()          { return (LPBITMAPINFO) this; }
	operator LPBITMAPINFOHEADER()    { return &bmiHeader;          }
	RGBQUAD* ColorTable()            { return bmiColors;           }
};
#endif /* !ENABLE_SDL */

Bitmap::~Bitmap()
{
  reset();
}

bool
Bitmap::load(const TCHAR *name)
{
  reset();

#ifdef ENABLE_SDL
#ifdef WIN32
  HRSRC resource = FindResource(CommonInterface::hInst, name, RT_BITMAP);
  if (resource == NULL)
    return false;

  DWORD size = ::SizeofResource(CommonInterface::hInst, resource);
  if (size == 0)
    return false;

  HGLOBAL handle = ::LoadResource(CommonInterface::hInst, resource);
  if (handle == NULL)
    return false;

  LPVOID data = ::LockResource(handle);
  if (data == NULL)
    return false;

  const BITMAPINFO *info = (const BITMAPINFO *)data;
  if (size < sizeof(*info))
    return false;

  int pitch = (((info->bmiHeader.biWidth * info->bmiHeader.biBitCount) / 8 - 1) | 3) + 1;
  int data_size = pitch * info->bmiHeader.biHeight;

  /* duplicate the BMP file and re-insert the BITMAPFILEHEADER which
     is not included in this .EXE file */
  BITMAPFILEHEADER *header = (BITMAPFILEHEADER *)malloc(sizeof(*header) + size);
  if (header == NULL)
    /* out of memory */
    return false;

  /* byte order?  this constant is correct according to MSDN */
  header->bfType = 0x4D42;
  header->bfSize = sizeof(*header) + size;
  header->bfReserved1 = 0;
  header->bfReserved2 = 0;
  header->bfOffBits = sizeof(BITMAPFILEHEADER) + size - data_size;
  memcpy(header + 1, data, size);

  SDL_RWops *rw = SDL_RWFromMem(header, sizeof(*header) + size);
  surface = SDL_LoadBMP_RW(rw, 1);
  SDL_FreeRW(rw);
  free(header);

  return true;
#else
  surface = ::SDL_LoadBMP(name);
  return surface != NULL;
#endif
#else /* !ENABLE_SDL */
  bitmap = LoadBitmap(XCSoarInterface::hInst, name);
  return bitmap != NULL;
#endif /* !ENABLE_SDL */
}

#ifdef ENABLE_SDL
bool
Bitmap::load(unsigned id)
{
  // XXX

#ifdef WIN32
  TCHAR name[10];

  _stprintf(name, _T("%u"), (unsigned)id);
  return load(name);
#else
  TCHAR name[32];

  _stprintf(name, _T("resources/%u"), (unsigned)id);
  load(name);

  TCHAR path[MAX_PATH];
  LocalPath(path, name);

  return load(path);
#endif
}
#endif /* !ENABLE_SDL */

bool
Bitmap::load_stretch(unsigned id, unsigned zoom)
{
  assert(zoom > 0);

#ifdef ENABLE_SDL
  // XXX
  return load(id);
#else
  if (!load(id))
    return false;

  if (zoom <= 1)
    return true;

  SIZE src_size = get_size();
  SIZE dest_size;
  dest_size.cx = src_size.cx * zoom;
  dest_size.cy = src_size.cy * zoom;

  HDC dc = ::GetDC(NULL), src_dc = ::CreateCompatibleDC(dc),
    dest_dc = ::CreateCompatibleDC(dc);
  HBITMAP dest_bitmap = ::CreateCompatibleBitmap(dc,
                                                 dest_size.cx, dest_size.cy);
  ::ReleaseDC(NULL, dc);

  if (dest_bitmap == NULL) {
    ::DeleteDC(src_dc);
    ::DeleteDC(dest_dc);
    return false;
  }

  ::SelectObject(src_dc, bitmap);
  ::SelectObject(dest_dc, dest_bitmap);

  ::StretchBlt(dest_dc, 0, 0, dest_size.cx, dest_size.cy,
               src_dc, 0, 0, src_size.cx, src_size.cy,
               SRCCOPY);

  ::DeleteDC(src_dc);
  ::DeleteDC(dest_dc);

  ::DeleteObject(bitmap);
  bitmap = dest_bitmap;

  return true;
#endif
}

void *
Bitmap::create(unsigned width, unsigned height)
{
#ifdef ENABLE_SDL
  Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  surface = ::SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24,
                                   rmask, gmask, bmask, amask);
  assert(!SDL_MUSTLOCK(surface));

  return surface->pixels;
#else /* !ENABLE_SDL */
  DIBINFO bmi;

  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biCompression = 0;
  bmi.bmiHeader.biHeight = height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biSize = 40;
  bmi.bmiHeader.biSizeImage = width * height * 3;
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biXPelsPerMeter = 3780;
  bmi.bmiHeader.biYPelsPerMeter = 3780;
  bmi.bmiColors[0].rgbBlue = 0;
  bmi.bmiColors[0].rgbGreen = 0;
  bmi.bmiColors[0].rgbRed = 0;
  bmi.bmiColors[0].rgbReserved = 0;

  VOID *pvBits;
  HDC hDC = ::GetDC(NULL);
  bitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
  ::ReleaseDC(NULL, hDC);

  return pvBits;
#endif /* !ENABLE_SDL */
}

void
Bitmap::reset()
{
#ifdef ENABLE_SDL
  if (surface != NULL) {
    SDL_FreeSurface(surface);
    surface = NULL;
  }
#else /* !ENABLE_SDL */
  if (bitmap != NULL) {
    DeleteObject(bitmap);
    bitmap = NULL;
  }
#endif /* !ENABLE_SDL */
}

const SIZE
Bitmap::get_size() const
{
#ifdef ENABLE_SDL
  if (surface == NULL) {
    // XXX eliminate this case
    const SIZE size = { 0, 0 };
    return size;
  }

  const SIZE size = { surface->w, surface->h };
  return size;
#else /* !ENABLE_SDL */
  BITMAP bm;
  ::GetObject(bitmap, sizeof(bm), &bm);
  const SIZE size = { bm.bmWidth, bm.bmHeight };
  return size;
#endif /* !ENABLE_SDL */
}