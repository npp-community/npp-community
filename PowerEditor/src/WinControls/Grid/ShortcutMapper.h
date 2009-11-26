/*
this file is part of notepad++
Copyright (C)2003 Don HO ( donho@altern.org )

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

#ifndef WINCONTROLS_GRID_SHORTCUTMAPPER_H
#define WINCONTROLS_GRID_SHORTCUTMAPPER_H

#ifndef WINCONTROLS_STATICDIALOG_STATICDIALOG_H
#include "WinControls/StaticDialog/StaticDialog.h"
#endif

enum GridState {STATE_MENU, STATE_MACRO, STATE_USER, STATE_PLUGIN, STATE_SCINTILLA};

class BabyGridWrapper;
class ContextMenu;

class ShortcutMapper : public StaticDialog {
public:
	ShortcutMapper();
	~ShortcutMapper();
	//void init(HINSTANCE hInst, HWND parent) {};
	void destroy();
	void doDialog(bool isRTL = false);
	void getClientRect(RECT & rc) const {
		Window::getClientRect(rc);
		rc.top += 40;
		rc.bottom -= 20;
		rc.left += 5;
	};

	void translateTab(int index, const TCHAR * newname);

protected :
	BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	static const int maxTabName = 64;
	BabyGridWrapper* _babygrid;
	ContextMenu* _rightClickMenu;

	GridState _currentState;
	HWND _hTabCtrl;

	TCHAR tabNames[5][maxTabName];

	void initTabs();
	void initBabyGrid();
	void fillOutBabyGrid();
};

#endif //WINCONTROLS_GRID_SHORTCUTMAPPER_H
