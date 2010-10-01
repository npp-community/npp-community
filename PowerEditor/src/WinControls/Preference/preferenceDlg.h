/*
this file is part of notepad++
Copyright (C)2003 Don HO < donho@altern.org >

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef WINCONTROLS_PREFERENCE_PREFERENCEDLG_H
#define WINCONTROLS_PREFERENCE_PREFERENCEDLG_H

#ifndef WINCONTROLS_STATICDIALOG_STATICDIALOG_H
#include "WinControls/StaticDialog/StaticDialog.h"
#endif

class ControlsTab;
struct PreferenceDlgWindows;

class PreferenceDlg : public StaticDialog
{
// JOCE: What NativeLangSpeaker does here, PreferenceDlg should do it.
friend class NativeLangSpeaker;

public :
	PreferenceDlg();

	~PreferenceDlg();
    void doDialog(bool isRTL = false);
	virtual void destroy();

private :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
	ControlsTab* _ctrlTab;
	PreferenceDlgWindows* _wVector;

	// Panes
	StaticDialog* _barsDlg;
	StaticDialog* _marginsDlg;
	StaticDialog* _settingsDlg;
	StaticDialog* _fileAssocDlg;
	StaticDialog* _langMenuDlg;
	StaticDialog* _printSettingsDlg;
	StaticDialog* _defaultNewDocDlg;
	StaticDialog* _backupDlg;
};

#endif //WINCONTROLS_PREFERENCE_PREFERENCEDLG_H
