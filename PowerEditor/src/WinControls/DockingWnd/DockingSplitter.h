//this file is part of docking functionality for Notepad++
//Copyright (C)2006 Jens Lorenz <jens.plugin.npp@gmx.de>
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


#ifndef WINCONTROLS_DOCKINGWND_DOCKINGSPLITTER_H
#define WINCONTROLS_DOCKINGWND_DOCKINGSPLITTER_H

#ifndef WINCONTROLS_WINDOW_H
#include "WinControls/Window.h"
#endif

#define	DMS_VERTICAL		0x00000001
#define	DMS_HORIZONTAL		0x00000002

class DockingSplitter : public Window
{
public :
	DockingSplitter() : _hMessage(NULL), _isLeftButtonDown(FALSE), _flags(0)
	{
		_ptOldPos.x = 0;
		_ptOldPos.y = 0;
	}

public:
	//(Warning -- Member with different signature hides virtual member 'Window::init(struct HINSTANCE__ *, struct HWND__ *)'
	//lint -e1411
	void init(HINSTANCE hInst, HWND hWnd, HWND hMessage, UINT flags);
	//lint +e1411

protected:

	static LRESULT CALLBACK staticWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND				_hMessage;

	BOOL				_isLeftButtonDown;
	POINT				_ptOldPos;
	UINT				_flags;

	static BOOL			_isVertReg;
	static BOOL			_isHoriReg;
};

#endif // WINCONTROLS_DOCKINGWND_DOCKINGSPLITTER_H
