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

#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

struct MenuItemUnit {
	unsigned long _cmdID;
	generic_string _itemName;
	generic_string _subMenuName;
	MenuItemUnit();
	MenuItemUnit(unsigned long cmdID, generic_string itemName, generic_string subMenuName=TEXT(""));
	MenuItemUnit(unsigned long cmdID, const TCHAR *itemName, const TCHAR *subMenuName=NULL);
};

class ContextMenu {
public:
	ContextMenu();
	~ContextMenu();
	void create(HWND hParent, const std::vector<MenuItemUnit> & menuItemArray);
	bool isCreated() const {return _hMenu != NULL;}

	void display(const POINT & p) const;
	void enableItem(int cmdID, bool doEnable) const;
	void checkItem(int cmdID, bool doCheck) const;

	HMENU getMenuHandle() {return _hMenu;}

private:
	HWND _hParent;
	HMENU _hMenu;
};

#endif //CONTEXTMENU_h
