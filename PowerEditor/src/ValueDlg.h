//this file is part of notepad++
//Copyright (C)2003 Don HO < donho@altern.org >
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef VALUEDLG_H
#define VALUEDLG_H

#ifndef WINCONTROLS_WINDOW_H
#include "WinControls/StaticDialog/StaticDialog.h"
#endif

#define DEFAULT_NB_NUMBER 2

// JOCE: Needs to be moved in WinControls somewhere.

class ValueDlg : public StaticDialog
{
public :
        ValueDlg() :
			_nbNumber(DEFAULT_NB_NUMBER),
			_defaultValue(0)
		{
			memset(&_p, 0, sizeof(POINT));
		}
		//(Warning -- Member with different signature hides virtual member 'Window::init(struct HINSTANCE__ *, struct HWND__ *)'
		//lint -e1411
        void init(HINSTANCE hInst, HWND parent, int valueToSet, const TCHAR *text);
		//lint +e1411
        int doDialog(POINT p, bool isRTL = false);
		void setNBNumber(int nbNumber);
		int reSizeValueBox();

protected :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM /*lParam*/);

private :
	int _nbNumber;
    int _defaultValue;
	generic_string _name;
	POINT _p;
};

// 0 : sans fullscreen
// 1 : fullscreen
// 2 : postit
#define buttonStatus_hidden 0
#define buttonStatus_fullscreen 1
#define buttonStatus_postit 2

class ButtonDlg : public StaticDialog
{
public :
    ButtonDlg();
    void doDialog(bool isRTL = false);
    int getButtonStatus() const;
    void setButtonStatus(int buttonStatus);
    void display(bool toShow = true) const;

protected :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
    int _buttonStatus;
};

#endif //VALUEDLG_H
