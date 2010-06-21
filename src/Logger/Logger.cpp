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

#include "Logger/Logger.hpp"
#include "Logger/LoggerImpl.hpp"
#include "Device/Declaration.hpp"
#include "NMEA/Info.hpp"
#include "Language.hpp"
#include "Dialogs/Message.hpp"
#include "Components.hpp"
#include "LogFile.hpp"
#include "TaskClientUI.hpp"
#include "Asset.hpp"

/**
 * Constructor of the Logger class
 */
Logger::Logger()
{
  _logger = new LoggerImpl();
}

/**
 * Destructor of the Logger class
 */
Logger::~Logger()
{
  Poco::ScopedRWLock protect(lock, true);
  delete _logger;
}

void
Logger::LogPoint(const NMEA_INFO &gps_info)
{
  // don't hold up the calculation thread if it's locked
  // by another process (most likely the logger gui message)

  if (lock.tryWriteLock()) {
    _logger->LogPoint(gps_info);
    lock.unlock();
  }
}

/**
 * Checks whether a Task is declared to the Logger.
 * If so, asks whether to invalidate the declaration.
 * @return True if a Task is NOT declared to the Logger, False otherwise
 */
bool
Logger::CheckDeclaration(void)
{
  return _logger->CheckDeclaration();
}

bool
Logger::isTaskDeclared()
{
  return _logger->isTaskDeclared();
}

bool
Logger::isLoggerActive()
{
  Poco::ScopedRWLock protect(lock, false);
  return _logger->isLoggerActive();
}

bool
Logger::LoggerClearFreeSpace(const NMEA_INFO &gps_info)
{
  Poco::ScopedRWLock protect(lock, true);
  return _logger->LoggerClearFreeSpace(gps_info);
}


void
Logger::guiStartLogger(const NMEA_INFO& gps_info,
                    const SETTINGS_COMPUTER& settings,
                    bool noAsk)
{
  if (isLoggerActive() || gps_info.gps.Replay)
    return;

  OrderedTask* task = task_ui.task_clone();
  const Declaration decl(task);

  if (task) {
    delete task;

    if (!noAsk) {
      TCHAR TaskMessage[1024];
      _tcscpy(TaskMessage, _T("Start Logger With Declaration\r\n"));
      
      if (decl.size()) {
        for (unsigned i = 0; i< decl.size(); ++i) {
          _tcscat(TaskMessage, decl.get_name(i));
          _tcscat(TaskMessage, _T("\r\n"));
        }
      } else {
        _tcscat(TaskMessage, _T("None"));
      }
      
      if (MessageBoxX(TaskMessage, gettext(_T("Start Logger")),
                      MB_YESNO | MB_ICONQUESTION) != IDYES)
        return;
    }
  }

  if (!LoggerClearFreeSpace(gps_info)) {
    MessageBoxX(gettext(_T("Logger inactive, insufficient storage!")),
                gettext(_T("Logger Error")), MB_OK| MB_ICONERROR);
    LogStartUp(_T("Logger not started: Insufficient Storage"));
    return;
  }

  Poco::ScopedRWLock protect(lock, true);
  _logger->StartLogger(gps_info, settings, strAssetNumber, decl);
}

void
Logger::guiToggleLogger(const NMEA_INFO& gps_info,
                     const SETTINGS_COMPUTER& settings,
                     bool noAsk)
{
  if (isLoggerActive()) {
    guiStopLogger(gps_info, noAsk);
  } else {
    guiStartLogger(gps_info, settings, noAsk);
  }
}

void
Logger::guiStopLogger(const NMEA_INFO &gps_info,
                   bool noAsk)
{
  if (!isLoggerActive())
    return;

  if (noAsk || (MessageBoxX(gettext(_T("Stop Logger")),
                            gettext(_T("Stop Logger")),
                            MB_YESNO | MB_ICONQUESTION) == IDYES)) {
    Poco::ScopedRWLock protect(lock, true);
    _logger->StopLogger(gps_info);
  }
}

void
Logger::LoggerDeviceDeclare(const OrderedTask& task)
{
  return _logger->LoggerDeviceDeclare(task);
}

void
Logger::LoggerNote(const TCHAR *text)
{
  Poco::ScopedRWLock protect(lock, true);
  return _logger->LoggerNote(text);
}

void
Logger::clearBuffer()
{
  Poco::ScopedRWLock protect(lock, true);
  return _logger->clearBuffer();
}