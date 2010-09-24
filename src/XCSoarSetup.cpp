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
 * Defines the entry point for the DLL application.
 * @file XCSoarSetup.cpp
 * @see http://www.codeguru.com/Cpp/W-P/ce/networking/article.php/c9269/
 */

#include <windows.h>

#ifdef __GNUC__
/* cegcc doesn't provide ce_setup.h; the following enum values are
   taken from MSDN */
typedef enum {
  codeINSTALL_INIT_CONTINUE = 0,
  codeINSTALL_INIT_CANCEL
} codeINSTALL_INIT;

typedef enum {
  codeINSTALL_EXIT_DONE = 0,
  codeINSTALL_EXIT_UNINSTALL
} codeINSTALL_EXIT;

typedef enum {
  codeUNINSTALL_INIT_CONTINUE = 0,
  codeUNINSTALL_INIT_CANCEL
} codeUNINSTALL_INIT;

typedef enum {
  codeUNINSTALL_EXIT_DONE = 0
} codeUNINSTALL_EXIT;

/* disable C++ name mangling for exported functions */
extern "C" {
  DECLSPEC_EXPORT codeINSTALL_INIT
  Install_Init(HWND hwndparent, BOOL ffirstcall, BOOL fpreviouslyinstalled,
               LPCTSTR pszinstalldir);

  DECLSPEC_EXPORT codeINSTALL_EXIT
  Install_Exit(HWND hwndparent, LPCTSTR pszinstalldir,
               WORD cfaileddirs, WORD cfailedfiles, WORD cfailedregkeys,
               WORD cfailedregvals, WORD cfailedshortcuts);

  DECLSPEC_EXPORT codeUNINSTALL_INIT
  Uninstall_Init(HWND hwndparent, LPCTSTR pszinstalldir);

  DECLSPEC_EXPORT codeUNINSTALL_EXIT
  Uninstall_Exit(HWND hwndparent);

  DECLSPEC_EXPORT APIENTRY BOOL
  DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
};

#else
#if (WIN32_PLATFORM_PSPC == 1)
  #include <..\support\ActiveSync\inc\ce_setup.h>
#else
  #include <ce_setup.h>
#endif
#endif

//#define DebugMessage(Caption, Text)    MessageBox(NULL, Text, Caption, MB_ICONINFORMATION)
#define DebugMessage(Caption, Text)      ((void)0)

/**
 * Handles tasls done at start of installation
 */
codeINSTALL_INIT Install_Init(HWND hwndparent,
  BOOL ffirstcall, BOOL fpreviouslyinstalled, LPCTSTR pszinstalldir)
{
  HKEY hKey = NULL;

  DebugMessage("Setup", "Install_Init Entry");

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE ,
      TEXT("\\Software\\Microsoft\\Today\\Items\\XCSoar"),
      0, KEY_ALL_ACCESS, &hKey
    ) == ERROR_SUCCESS){

    DebugMessage("Setup", "Delete Value DLL");

    RegDeleteValue(hKey, TEXT("DLL"));

    DebugMessage("Setup", "Refresh Today");

    SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);

    RegCloseKey(hKey);

  }

  DebugMessage("Setup", "Install_Init Exit");

  return codeINSTALL_INIT_CONTINUE;
}

/**
 * Handles tasks done at end of installation
 */
codeINSTALL_EXIT Install_Exit(
    HWND hwndparent,LPCTSTR pszinstalldir,
    WORD cfaileddirs,WORD cfailedfiles,WORD cfailedregkeys,
    WORD cfailedregvals,
    WORD cfailedshortcuts)
{
  DebugMessage("Setup", "Install_Exit");

  SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);

  return codeINSTALL_EXIT_DONE;
}

/**
 * Handles tasks done at beginning of uninstallation
 */
codeUNINSTALL_INIT Uninstall_Init(
  HWND hwndparent,LPCTSTR pszinstalldir)
{
  HKEY hKey = NULL;

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE ,
      TEXT("\\Software\\Microsoft\\Today\\Items\\XCSoar"),
      0, KEY_ALL_ACCESS, &hKey
    ) == ERROR_SUCCESS){

    DebugMessage("Setup", "Delete Value DLL");

    RegDeleteValue(hKey, TEXT("DLL"));

    RegCloseKey(hKey);

    DebugMessage("Setup", "Refresh Today");

    SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);

  }

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE ,
      TEXT("\\Software\\Microsoft\\Today\\Items"),
      0, KEY_ALL_ACCESS, &hKey
    ) == ERROR_SUCCESS){

    DebugMessage("Setup", "Delete Key \\...Today\\XCSoar");

    RegDeleteKey(hKey, TEXT("XCSoar"));

    RegCloseKey(hKey);

  }

  return codeUNINSTALL_INIT_CONTINUE;
}

/**
 * Handles tasks done at end of uninstallation
 */
codeUNINSTALL_EXIT Uninstall_Exit(HWND hwndparent)
{
  //do nothing
  //return value
  return codeUNINSTALL_EXIT_DONE;
}

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  return TRUE;
}