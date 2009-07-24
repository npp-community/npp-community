// This file is part of notepad++
// Copyright (C)2009 The Notepad++ Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "precompiled_headers.h"

#include "DockingDlgInterface.h"
#include "dockingResource.h"
#include "Docking.h"
#include "Notepad_plus_msgs.h"

void DockingDlgInterface::init( HINSTANCE hInst, HWND parent )
{
	StaticDialog::init(hInst, parent);
	::GetModuleFileName((HMODULE)hInst, _moduleName, MAX_PATH);
	lstrcpy(_moduleName, PathFindFileName(_moduleName));
}

void DockingDlgInterface::create( tTbData * data, bool isRTL /*= false*/ )
{
	StaticDialog::create(_dlgID, isRTL);
	::GetWindowText(_hSelf, _pluginName, MAX_PATH);

	// user information
	data->hClient		= _hSelf;
	data->pszName		= _pluginName;

	// supported features by plugin
	data->uMask			= 0;

	// additional info
	data->pszAddInfo	= NULL;

	_data = data;
}

void DockingDlgInterface::updateDockingDlg()
{
	::SendMessage(_hParent, NPPM_DMMUPDATEDISPINFO, 0, (LPARAM)_hSelf);
}

void DockingDlgInterface::display( bool toShow /*= true*/ ) const
{
	::SendMessage(_hParent, toShow?NPPM_DMMSHOW:NPPM_DMMHIDE, 0, (LPARAM)_hSelf);
}

BOOL CALLBACK DockingDlgInterface::run_dlgProc( UINT message, WPARAM /*wParam*/, LPARAM lParam )
{
	switch (message)
	{

	case WM_NOTIFY:
		{
			LPNMHDR	pnmh	= (LPNMHDR)lParam;

			if (pnmh->hwndFrom == _hParent)
			{
				switch (LOWORD(pnmh->code))
				{
				case DMN_CLOSE:
					{
						//::MessageBox(_hSelf, TEXT("Close Dialog"), TEXT("Plugin Message"), MB_OK);
						break;
					}
				case DMN_FLOAT:
					{
						//::MessageBox(_hSelf, TEXT("Float Dialog"), TEXT("Plugin Message"), MB_OK);
						_isFloating = true;
						break;
					}
				case DMN_DOCK:
					{
						//TCHAR test[256];
						//wsprintf(test, TEXT("Dock Dialog to %d"), HIWORD(pnmh->code));
						//::MessageBox(_hSelf, test, TEXT("Plugin Message"), MB_OK);
						_iDockedPos = HIWORD(pnmh->code);
						_isFloating = false;
						break;
					}
				default:
					break;
				}
			}
			break;
		}
	default:
		break;
	}
	return FALSE;
}
