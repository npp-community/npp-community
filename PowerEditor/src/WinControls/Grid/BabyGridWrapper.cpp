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

#include "precompiled_headers.h"
#include "BabyGridWrapper.h"

#include "babygrid.h"


const TCHAR *babyGridClassName = TEXT("BABYGRID");

bool BabyGridWrapper::_isRegistered = false;

void BabyGridWrapper::init(HINSTANCE hInst, HWND parent, int id)
{
	Window::init(hInst, parent);

	if (!_isRegistered)
		RegisterGridClass(_hInst);

	_hSelf = ::CreateWindowEx(WS_EX_CLIENTEDGE,
	                babyGridClassName,\
					TEXT(""),\
					WS_CHILD | WS_VISIBLE,\
					CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,\
					_hParent,\
					(HMENU)id,\
					_hInst,\
					(LPVOID)/*this*/NULL);
}

void BabyGridWrapper::setLineColNumber( size_t nbRow, size_t nbCol )
{
	::SendMessage(_hSelf, BGM_SETGRIDDIM, nbRow, nbCol);
}

void BabyGridWrapper::setCursorColour( COLORREF coulour )
{
	::SendMessage(_hSelf, BGM_SETCURSORCOLOR, (UINT)coulour, 0);
}

void BabyGridWrapper::setColsNumbered( bool isNumbered /*= true*/ )
{
	::SendMessage(_hSelf, BGM_SETCOLSNUMBERED, isNumbered?TRUE:FALSE, 0);
}

void BabyGridWrapper::setText( size_t row, size_t col, const TCHAR *text )
{
	_BGCELL cell;
	cell.row = row;
	cell.col = col;
	::SendMessage(_hSelf, BGM_SETCELLDATA, (UINT)&cell, (long)text);
}

void BabyGridWrapper::makeColAutoWidth( bool autoWidth /*= true*/ )
{
	::SendMessage(_hSelf, BGM_SETCOLAUTOWIDTH, autoWidth?TRUE:FALSE, 0);
}

int BabyGridWrapper::getSelectedRow()
{
	return ::SendMessage(_hSelf, BGM_GETROW, 0, 0);
}

void BabyGridWrapper::deleteCell( int row, int col )
{
	_BGCELL cell;
	cell.row = row;
	cell.col = col;
	::SendMessage(_hSelf, BGM_DELETECELL, (UINT)&cell, 0);
}

void BabyGridWrapper::setColWidth( unsigned int col, unsigned int width )
{
	::SendMessage(_hSelf, BGM_SETCOLWIDTH, col, width);
}

void BabyGridWrapper::clear()
{
	::SendMessage(_hSelf, BGM_CLEARGRID, 0, 0);
}
