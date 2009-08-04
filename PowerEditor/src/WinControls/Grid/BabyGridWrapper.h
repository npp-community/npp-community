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
#ifndef BABYGRIDWRAPPER
#define BABYGRIDWRAPPER

#ifndef WINDOW_CONTROL_H
#include "Window.h"
#endif

class BabyGridWrapper : public Window
{
public :
	BabyGridWrapper() : Window(){};
	//(Warning -- Member with different signature hides virtual member 'Window::init(struct HINSTANCE__ *, struct HWND__ *)'
	//lint -e1411
	void init(HINSTANCE hInst, HWND parent, int id);
	//lint +e1411
	void setLineColNumber(size_t nbRow, size_t nbCol);

	void setCursorColour(COLORREF coulour);

	void hideCursor() {
		setCursorColour(RGB(0, 0, 0));
	};

	void setColsNumbered(bool isNumbered = true);

	void setText(size_t row, size_t col, const TCHAR *text);

	void makeColAutoWidth(bool autoWidth = true);

	int getSelectedRow();

	void deleteCell(int row, int col);

	void setColWidth(unsigned int col, unsigned int width);

	void clear();

private :
	static bool _isRegistered;
/*
    static LRESULT CALLBACK staticWinProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
        return (((BabyGridWrapper *)(::GetWindowLongPtr(hwnd, GWL_USERDATA)))->runProc(Message, wParam, lParam));
    };
	LRESULT runProc(UINT Message, WPARAM wParam, LPARAM lParam);
*/
};

#endif //BABYGRIDWRAPPER

