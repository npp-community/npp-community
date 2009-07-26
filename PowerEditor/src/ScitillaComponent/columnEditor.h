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

#ifndef COLUMNEDITOR_H
#define COLUMNEDITOR_H

#include "StaticDialog.h"

// Forward declarations
class ScintillaEditView;

class ColumnEditorDlg : public StaticDialog
{
public :
	ColumnEditorDlg() : StaticDialog() {};

	void init(HINSTANCE hInst, HWND hPere, ScintillaEditView **ppEditView);
	virtual void create(int dialogID, bool isRTL = false);
	void doDialog(bool isRTL = false);
    virtual void display(bool toShow = true) const;
	void switchTo(bool toText);

	UCHAR getFormat();

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :

    ScintillaEditView **_ppEditView;
};
#endif// COLUMNEDITOR_H
