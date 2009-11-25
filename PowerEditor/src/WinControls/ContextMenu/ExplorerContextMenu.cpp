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

#include "precompiled_headers.h"

#include "ExplorerContextMenu.h"

// The best part of this code is the work was found in the WtlFileTreeCtrl.cpp file of the
// LightExplorer plugin for Notepad++ whose source was found at http://sourceforge.net/projects/npp-plugins/files/

#define CUT_MENU_ID 25
#define PASTE_MENU_ID 26
#define DELETE_MENU_ID 18
#define CREATE_SHORTCUT_MENU_ID 17

ExplorerContentMenu::ExplorerContentMenu() :
	m_psfFolder(NULL),
	m_pShellFolder(NULL),
	m_pidlArray(NULL),
	m_nItems(0),
	m_pContextMenu(NULL),
	m_hStandardMenu(0)
{
}

ExplorerContentMenu::~ExplorerContentMenu()
{
	if (m_hStandardMenu)
	{
		::DestroyMenu(m_hStandardMenu);
		m_hStandardMenu = 0;
	}

	FreePIDLArray (m_pidlArray);
	m_pidlArray = NULL;

	if (m_psfFolder)
	{
		m_psfFolder->Release();
		m_psfFolder = NULL;
	}

	if (m_pShellFolder)
	{
		m_pShellFolder->Release();
		m_pShellFolder = NULL;
	}

	if (m_pContextMenu)
	{
		m_pContextMenu->Release();
		m_pContextMenu = NULL;
	}
}

void ExplorerContentMenu::Load(const TCHAR* filename)
{
	std::vector<generic_string> vect;
	vect.push_back(filename);

	SetObjects(vect);

	if (m_pidlArray != NULL) {
		int iMenuType = 0;
		m_hStandardMenu = ::CreateMenu();

		if (!GetContextMenu((void**) &m_pContextMenu, iMenuType)) throw 0; // something went wrong

		// Let's fill out our popup menu
		m_pContextMenu->QueryContextMenu(m_hStandardMenu, ::GetMenuItemCount(m_hStandardMenu),
			IDM_MIN_EXPLORER_CONTEXT_MENU_ID, IDM_MAX_EXPLORER_CONTEXT_MENU_ID, CMF_NORMAL);

		CleanUpMenu();
	}
}

HMENU ExplorerContentMenu::GetHandle()
{
	return m_hStandardMenu;
}

void ExplorerContentMenu::CleanUpMenu()
{
	if (m_pidlArray != NULL && m_hStandardMenu != NULL)
	{
		// Disable a few standard file related items (cut, paste, etc) and hide
		// others that don't make sense in the context of a text editor.
		MENUITEMINFO info = {0};

		info.cbSize = sizeof(MENUITEMINFO);
		bool wasLastASeparator = false;
		for (int i = ::GetMenuItemCount(m_hStandardMenu) -1; i >= 0 ; --i)
		{
			info.fMask = MIIM_ID;
			#if WINVER <= 0x0600 // Anything before Vista
				// See http://msdn.microsoft.com/en-us/library/ms647578%28VS.85%29.aspx for more info.
				info.fMask |=MIIM_FTYPE;
			#else
				info.fMask |=MIIM_TYPE;
			#endif
			::GetMenuItemInfo(m_hStandardMenu, i, TRUE, &info);

			UINT menuID = info.wID;

			if (info.fType & MFT_SEPARATOR)
			{
				if (wasLastASeparator)
				{
					::RemoveMenu(m_hStandardMenu, i, MF_BYPOSITION);
				}
				wasLastASeparator = true;
				continue;
			}

			int originalID = menuID ? (menuID - IDM_MIN_EXPLORER_CONTEXT_MENU_ID + 1) : 0;

			if (originalID == CUT_MENU_ID ||
				originalID == PASTE_MENU_ID ||
				originalID == DELETE_MENU_ID ||
				originalID == CREATE_SHORTCUT_MENU_ID )
			{

					info.fMask = MIIM_STATE;
					info.fState = MFS_DISABLED;
					::SetMenuItemInfo(m_hStandardMenu, menuID, FALSE, &info);
			}
			else
			{
				TCHAR itemName[1024];
				::GetMenuString(m_hStandardMenu, menuID, itemName, 1024, MF_BYCOMMAND);
				if ( (_tcscmp(itemName, TEXT("&Open")) == 0) ||
					(_tcscmp(itemName, TEXT("&Edit")) == 0) ||
					(_tcscmp(itemName, TEXT("Edit with &Notepad++")) == 0) ||
					(_tcscmp(itemName, TEXT("Open Wit&h")) == 0) ||
					(_tcscmp(itemName, TEXT("Se&nd To")) == 0))
				{ // a name we need to remove
					::RemoveMenu(m_hStandardMenu, menuID, MF_BYCOMMAND);
					continue; // Since we just deleted an item, we won't change the "was last a separator" state
				}
			}
			wasLastASeparator = false;
		}
	}

}

void ExplorerContentMenu::InvokeCommand(int id)
{
	UINT idCommand = id - IDM_MIN_EXPLORER_CONTEXT_MENU_ID;

	CMINVOKECOMMANDINFOEX cmi = {0};
	cmi.cbSize = sizeof (CMINVOKECOMMANDINFOEX);
	cmi.lpVerb = (LPCSTR) MAKEINTRESOURCE (idCommand);
	cmi.lpVerbW = (LPWSTR) MAKEINTRESOURCE (idCommand);
	cmi.nShow = SW_SHOWNORMAL;

	m_pContextMenu->InvokeCommand ((LPCMINVOKECOMMANDINFO)&cmi);
}

void ExplorerContentMenu::FreePIDLArray(LPITEMIDLIST *pidlArray)
{
	if (!pidlArray)
	{
		return;
	}

	int iSize = (int)(_msize (pidlArray) / sizeof (LPITEMIDLIST));

	for (int i = 0; i < iSize; i++)
	{
		free (pidlArray[i]);
	}
	free (pidlArray);
}


int ExplorerContentMenu::GetPIDLCount (LPCITEMIDLIST pidl)
{
	if (!pidl)
	{
		return 0;
	}

	int nCount = 0;
	BYTE* pCur = (BYTE *) pidl;
	while (((LPCITEMIDLIST) pCur)->mkid.cb)
	{
		nCount++;
		pCur += ((LPCITEMIDLIST) pCur)->mkid.cb;
	}
	return nCount;
}

UINT ExplorerContentMenu::GetPIDLSize (LPCITEMIDLIST pidl)
{
	if (!pidl)
	{
		return 0;
	}
	int nSize = 0;
	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;
	while (pidlTemp->mkid.cb)
	{
		nSize += pidlTemp->mkid.cb;
		pidlTemp = (LPITEMIDLIST) (((LPBYTE) pidlTemp) + pidlTemp->mkid.cb);
	}
	return nSize;
}

LPITEMIDLIST ExplorerContentMenu::CopyPIDL (LPCITEMIDLIST pidl, int cb)
{
	if (cb == -1)
	{
		cb = GetPIDLSize (pidl); // Calculate size of list.
	}

	LPITEMIDLIST pidlRet = (LPITEMIDLIST) calloc (cb + sizeof (USHORT), sizeof (BYTE));
	if (pidlRet)
	{
		CopyMemory(pidlRet, pidl, cb);
	}

	return (pidlRet);
}

LPBYTE ExplorerContentMenu::GetPIDLPos (LPCITEMIDLIST pidl, int nPos)
{
	if (!pidl)
	{
		return 0;
	}
	int nCount = 0;

	BYTE * pCur = (BYTE *) pidl;
	while (((LPCITEMIDLIST) pCur)->mkid.cb)
	{
		if (nCount == nPos)
		{
			return pCur;
		}
		nCount++;
		pCur += ((LPCITEMIDLIST) pCur)->mkid.cb; // + sizeof(pidl->mkid.cb);
	}
	if (nCount == nPos)
		return pCur;
	return NULL;
}

HRESULT ExplorerContentMenu::SHBindToParentEx (LPCITEMIDLIST pidl, REFIID riid, VOID **ppv, LPCITEMIDLIST *ppidlLast)
{
	HRESULT hr = 0;
	if (!pidl || !ppv)
	{
		return E_POINTER;
	}

	int nCount = GetPIDLCount (pidl);
	if (nCount == 0) // desktop pidl of invalid pidl
	{
		return E_POINTER;
	}

	IShellFolder * psfDesktop = NULL;
	SHGetDesktopFolder (&psfDesktop);
	if (nCount == 1) // desktop pidl
	{
		if ((hr = psfDesktop->QueryInterface(riid, ppv)) == S_OK)
		{
			if (ppidlLast)
			{
				*ppidlLast = CopyPIDL (pidl);
			}
		}
		psfDesktop->Release ();
		return hr;
	}

	LPBYTE pRel = GetPIDLPos (pidl, nCount - 1);
	LPITEMIDLIST pidlParent = CopyPIDL (pidl, (int)(pRel - (LPBYTE) pidl));
	IShellFolder * psfFolder = NULL;

	if ((hr = psfDesktop->BindToObject (pidlParent, NULL, __uuidof (psfFolder), (void **) &psfFolder)) != S_OK)
	{
		free (pidlParent);
		psfDesktop->Release ();
		return hr;
	}
	if ((hr = psfFolder->QueryInterface (riid, ppv)) == S_OK)
	{
		if (ppidlLast)
		{
			*ppidlLast = CopyPIDL ((LPCITEMIDLIST) pRel);
		}
	}
	free (pidlParent);
	psfFolder->Release ();
	psfDesktop->Release ();
	return hr;
}

void ExplorerContentMenu::SetObjects(std::vector<generic_string> strArray)
{
	// store also the string for later menu use
	m_strArray = strArray;

	// free all allocated datas
	if (m_psfFolder)
	{
		m_psfFolder->Release ();
	}
	m_psfFolder = NULL;
	FreePIDLArray (m_pidlArray);
	m_pidlArray = NULL;

	// get IShellFolder interface of Desktop (root of shell namespace)
	IShellFolder* psfDesktop = NULL;
	SHGetDesktopFolder (&psfDesktop); // needed to obtain full qualified pidl

	// ParseDisplayName creates a PIDL from a file system path relative to the IShellFolder interface
	// but since we use the Desktop as our interface and the Desktop is the namespace root
	// that means that it's a fully qualified PIDL, which is what we need
	LPITEMIDLIST pidl = NULL;

	OLECHAR * olePath = (OLECHAR *) calloc (strArray[0].size() + 1, sizeof (OLECHAR));
	_tcscpy_s(olePath, strArray[0].size() + 1, strArray[0].c_str());
	psfDesktop->ParseDisplayName (NULL, 0, olePath, NULL, &pidl, NULL);
	free (olePath);

	if (pidl != NULL)
	{
		// now we need the parent IShellFolder interface of pidl, and the relative PIDL to that interface
		LPITEMIDLIST pidlItem = NULL; // relative pidl
		SHBindToParentEx (pidl, IID_IShellFolder, (void **) &m_psfFolder, NULL);
		free (pidlItem);
		// get interface to IMalloc (need to free the PIDLs allocated by the shell functions)
		LPMALLOC lpMalloc = NULL;
		SHGetMalloc (&lpMalloc);
		if (lpMalloc != NULL)
		{
			lpMalloc->Free (pidl);
		}

		// now we have the IShellFolder interface to the parent folder specified in the first element in strArray
		// since we assume that all objects are in the same folder (as it's stated in the MSDN)
		// we now have the IShellFolder interface to every objects parent folder

		IShellFolder * psfFolder = NULL;
		m_nItems = (int)strArray.size();
		for (int i = 0; i < m_nItems; i++)
		{
			olePath = (OLECHAR *) calloc (strArray[i].size() + 1, sizeof (OLECHAR));
			_tcscpy_s(olePath, strArray[i].size() + 1, strArray[i].c_str());
			psfDesktop->ParseDisplayName (NULL, 0, olePath, NULL, &pidl, NULL);
			free (olePath);

			m_pidlArray = (LPITEMIDLIST *) realloc (m_pidlArray, (i + 1) * sizeof (LPITEMIDLIST));
			// get relative pidl via SHBindToParent
			SHBindToParentEx (pidl, IID_IShellFolder, (void **) &psfFolder, (LPCITEMIDLIST *) &pidlItem);
			m_pidlArray[i] = CopyPIDL (pidlItem); // copy relative pidl to pidlArray
			free (pidlItem);
			// free pidl allocated by ParseDisplayName
			if (lpMalloc != NULL)
			{
				lpMalloc->Free (pidl);
			}
			if (psfFolder != NULL)
			{
				psfFolder->Release ();
			}
		}

		if (lpMalloc != NULL)
		{
			lpMalloc->Release ();
		}
	}
	if (psfDesktop != NULL)
	{
		psfDesktop->Release ();
	}
}

BOOL ExplorerContentMenu::GetContextMenu (void ** ppContextMenu, int & iMenuType)
{
	*ppContextMenu = NULL;
	LPCONTEXTMENU icm1 = NULL;

	// first we retrieve the normal IContextMenu interface (every object should have it)
	m_psfFolder->GetUIObjectOf (NULL, m_nItems, (LPCITEMIDLIST *) m_pidlArray, IID_IContextMenu, NULL, (void**) &icm1);

	if (icm1)
	{ // since we got an IContextMenu interface we can now obtain the higher version interfaces via that
		if (icm1->QueryInterface (IID_IContextMenu3, ppContextMenu) == NOERROR)
		{
			iMenuType = 3;
		}
		else if (icm1->QueryInterface (IID_IContextMenu2, ppContextMenu) == NOERROR)
		{
			iMenuType = 2;
		}

		if (*ppContextMenu)
		{
			icm1->Release(); // we can now release version 1 interface, cause we got a higher one
		}
		else
		{
			iMenuType = 1;
			*ppContextMenu = icm1; // since no higher versions were found
		} // redirect ppContextMenu to version 1 interface
	}
	else
	{
		return (FALSE); // something went wrong
	}

	return (TRUE); // success
}
