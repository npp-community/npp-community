/*
this file is part of Function List Plugin for Notepad++
Copyright (C)2005 Jens Lorenz <jens.plugin.npp@gmx.de>

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

#ifndef DOCKINGDLGINTERFACE_H
#define DOCKINGDLGINTERFACE_H

#include "StaticDialog.h"

struct tTbData;

class DockingDlgInterface : public StaticDialog
{
public:
	DockingDlgInterface(): StaticDialog() {};
	DockingDlgInterface(int dlgID): StaticDialog(),
		_dlgID(dlgID), _isFloating(TRUE), _iDockedPos(0) {};

	virtual void init(HINSTANCE hInst, HWND parent);

    void create(tTbData * data, bool isRTL = false);;

	virtual void updateDockingDlg();

	virtual void display(bool toShow = true) const;;

	const TCHAR * getPluginFileName() const {
		return _moduleName;
	};

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM /*wParam*/, LPARAM lParam);;

	// Handles
    HWND			_HSource;
	tTbData*		_data;
	int				_dlgID;
	bool            _isFloating;
	int				_iDockedPos;
	TCHAR            _moduleName[MAX_PATH];
	TCHAR			_pluginName[MAX_PATH];
};

#endif // DOCKINGDLGINTERFACE_H
