//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
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

#ifndef WINCONTROLS_TOOLBAR_TOOLBAR_H
#define WINCONTROLS_TOOLBAR_TOOLBAR_H

#ifndef WINCONTROLS_WINDOW_H
#include "WinControls/Window.h"
#endif

#define REBAR_BAR_TOOLBAR		0
#define REBAR_BAR_SEARCH		1

#define REBAR_BAR_EXTERNAL		10
#ifndef _WIN32_IE
#define _WIN32_IE	0x0600
#endif //_WIN32_IE

enum toolBarStatusType {TB_SMALL, TB_LARGE, TB_STANDARD};

typedef struct {
	UINT		message;		// identification of icon in tool bar (menu ID)
	HBITMAP		hBmp;			// bitmap for toolbar
	HICON		hIcon;			// icon for toolbar
} tDynamicList;

// Forward declarations
class ReBar;
struct ToolBarButtonUnit;
struct toolbarIcons;
class ToolBarIcons;

class ToolBar : public Window
{
public :
	ToolBar();
	~ToolBar();

	virtual bool init(HINSTANCE hInst, HWND hParent, toolBarStatusType type,
		ToolBarButtonUnit *buttonUnitArray, int arraySize);

	virtual void destroy();
	void enable(int cmdID, bool doEnable) const {
		::SendMessage(_hSelf, TB_ENABLEBUTTON, cmdID, (LPARAM)doEnable);
	};

	int getWidth() const;
	int getHeight() const;

	void reduce();
	void enlarge();
	void setToUglyIcons();

	bool getCheckState(int ID2Check) const;

	void setCheck(int ID2Check, bool willBeChecked) const;

	toolBarStatusType getState() const {
		return _state;
	};

	bool changeIcons(int whichLst, int iconIndex, const TCHAR *iconLocation);

	void registerDynBtn(UINT message, toolbarIcons* hBmp);

	void doPopop(POINT chevPoint);	//show the popup if buttons are hidden

	void addToRebar(ReBar * rebar);

private :
	TBBUTTON *_pTBB;
	ToolBarIcons* _toolBarIcons;
	toolBarStatusType _state;
	std::vector<tDynamicList> _vDynBtnReg;
	size_t _nrButtons;
	size_t _nrDynButtons;
	size_t _nrTotalButtons;
	size_t _nrCurrentButtons;
	ReBar * _pRebar;
	REBARBANDINFO _rbBand;

	void setDefaultImageList();
	void setHotImageList();
	void setDisableImageList();

	void reset(bool create = false);
	void setState(toolBarStatusType state) {
		_state = state;
	}

};

class ReBar : public Window
{
public :
	ReBar():Window() { usedIDs.clear(); };
	~ReBar();

	virtual void destroy() {
		Window::destroy();
		usedIDs.clear();
	};

	void init(HINSTANCE hInst, HWND hParent);
	bool addBand(REBARBANDINFO * rBand, bool useID);	//useID true if ID from info should be used (false for plugins). wID in bandinfo will be set to used ID
	void reNew(int id, REBARBANDINFO * rBand);					//wID from bandinfo is used for update
	void removeBand(int id);

	void setIDVisible(int id, bool show);
	bool getIDVisible(int id);

private:
	std::vector<int> usedIDs;

	int getNewID();
	void releaseID(int id);
	bool isIDTaken(int id);
};

#endif // WINCONTROLS_TOOLBAR_TOOLBAR_H
