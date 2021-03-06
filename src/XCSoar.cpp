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

/**
 * This is the main entry point for the application
 * @file XCSoar.cpp
 */

#include "resource.h"
#include "Version.hpp"
#include "Protection.hpp"
#include "Components.hpp"
#include "LogFile.hpp"
#include "CommandLine.hpp"
#include "MainWindow.hpp"
#include "Asset.hpp"
#include "Interface.hpp"
#include "Compiler.h"

#ifndef ENABLE_SDL
#include <commctrl.h>
#endif /* !ENABLE_SDL */

/**
 * Main entry point for the whole XCSoar application
 */
#ifndef WIN32
int main(int argc, char **argv)
#else
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
#ifdef _WIN32_WCE
        gcc_unused LPWSTR lpCmdLine,
#else
        gcc_unused LPSTR lpCmdLine2,
#endif
        int nCmdShow)
#endif
{
  InitAsset();

  // Write startup note + version to logfile
  LogStartUp(_T("Starting XCSoar %s"), XCSoar_VersionString);

  // Read options from the command line
#ifndef WIN32
  HINSTANCE hInstance = NULL;
  const TCHAR *lpCmdLine = argc >= 2 ? argv[1] : _T("");
#elif !defined(_WIN32_WCE)
  /* on Windows (non-CE), the lpCmdLine argument is narrow, and we
     have to use GetCommandLine() to get the UNICODE string */
  LPCTSTR lpCmdLine = GetCommandLine();
#endif

  ParseCommandLine(lpCmdLine);

#ifndef ENABLE_SDL
  InitCommonControls();
#endif /* !ENABLE_SDL */

  // Write initialization note to logfile
  LogStartUp(_T("Initialise application instance"));

  // Perform application initialization and run loop
  if (!XCSoarInterface::Startup(hInstance))
    return 0;

  return CommonInterface::main_window.event_loop();
}
