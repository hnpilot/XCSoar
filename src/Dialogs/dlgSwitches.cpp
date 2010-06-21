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

#include "Dialogs/Internal.hpp"
#include "Blackboard.hpp"
#include "Units.hpp"
#include "InputEvents.h"
#include "DataField/Base.hpp"
#include "MainWindow.hpp"

static WndForm *wf=NULL;

static void
Update(const TCHAR *name, bool value)
{
  WndProperty *wp = (WndProperty*)wf->FindByName(name);
  if (wp == NULL)
    return;

  DataField *df = wp->GetDataField();
  if (df->GetAsBoolean() != value) {
    wp->GetDataField()->Set(value);
    wp->RefreshDisplay();
  }
}

static void UpdateValues() {
  const SWITCH_INFO &switches = XCSoarInterface::Basic().SwitchState;

  Update(_T("prpFlapLanding"), switches.FlapLanding);
  Update(_T("prpAirbrakeExtended"), switches.AirbrakeLocked);
  Update(_T("prpFlapPositive"), switches.FlapPositive);
  Update(_T("prpFlapNeutral"), switches.FlapNeutral);
  Update(_T("prpFlapNegative"), switches.FlapNegative);
  Update(_T("prpGearExtended"), switches.GearExtended);
  Update(_T("prpAcknowledge"), switches.Acknowledge);
  Update(_T("prpRepeat"), switches.Repeat);
  Update(_T("prpSpeedCommand"), switches.SpeedCommand);
  Update(_T("prpUserSwitchUp"), switches.UserSwitchUp);
  Update(_T("prpUserSwitchMiddle"), switches.UserSwitchMiddle);
  Update(_T("prpUserSwitchDown"), switches.UserSwitchDown);
  Update(_T("prpVarioCircling"), switches.VarioCircling);
}

static int OnTimerNotify(WindowControl * Sender) {
	(void)Sender;
  UpdateValues();
  return 0;
}

static void OnCloseClicked(WindowControl * Sender){
	(void)Sender;
  wf->SetModalResult(mrOK);
}


static CallBackTableEntry_t CallBackTable[]={
  DeclareCallBackEntry(OnCloseClicked),
  DeclareCallBackEntry(NULL)
};



void dlgSwitchesShowModal(void){
  wf = dlgLoadFromXML(CallBackTable,
                      _T("dlgSwitches.xml"),
		      XCSoarInterface::main_window,
		      _T("IDR_XML_SWITCHES"));
  if (wf == NULL)
    return;

  wf->SetTimerNotify(OnTimerNotify);

  UpdateValues();
  wf->ShowModal();

  delete wf;
}