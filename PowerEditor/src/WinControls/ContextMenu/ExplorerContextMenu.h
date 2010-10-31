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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef EXPLORER_CONTEXT_MENU
#define EXPLORER_CONTEXT_MENU

#define IDM_MIN_EXPLORER_CONTEXT_MENU_ID 10000
#define IDM_MAX_EXPLORER_CONTEXT_MENU_ID 20000

class ExplorerContextMenu
{
public:
	ExplorerContextMenu();
	~ExplorerContextMenu();

	void Load(const TCHAR* filename);
	HMENU GetHandle();
	void InvokeCommand(int id);

private:

	void FreePIDLArray(LPITEMIDLIST *pidlArray);
	int GetPIDLCount (LPCITEMIDLIST pidl);
	UINT GetPIDLSize (LPCITEMIDLIST pidl);

	LPITEMIDLIST CopyPIDL (LPCITEMIDLIST pidl, int cb = -1);
	HRESULT SHBindToParentEx (LPCITEMIDLIST pidl, REFIID riid, VOID **ppv, LPCITEMIDLIST *ppidlLast);
	void SetObjects(std::vector<generic_string> strArray);
	LPBYTE GetPIDLPos (LPCITEMIDLIST pidl, int nPos);
	BOOL GetContextMenu (void ** ppContextMenu, int & iMenuType);

	void CleanUpMenu();

	IShellFolder* m_psfFolder;
	IShellFolder* m_pShellFolder;
	LPITEMIDLIST* m_pidlArray;
	int m_nItems;
	std::vector<generic_string> m_strArray;

	IContextMenu* m_pContextMenu;

	HMENU m_hStandardMenu;

};
#endif // EXPLORER_CONTEXT_MENU