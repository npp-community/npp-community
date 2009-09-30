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

#ifndef DOCKINGMANAGER_H
#define DOCKINGMANAGER_H

#ifndef WINDOW_CONTROL_H
#include "Window.h"
#endif

#define DSPC_CLASS_NAME TEXT("dockingManager")

#define	CONT_MAP_MAX	50

class DockingSplitter;
class DockingCont;
struct tTbData;
struct tDockMgr;

class DockingManager : public Window
{
public :
	DockingManager();
	~DockingManager();

	void init(HINSTANCE hInst, HWND hWnd, Window ** ppWin);
	virtual void reSizeTo(RECT & rc);

	void setClientWnd(Window ** ppWin) {
		_ppWindow = ppWin;
		_ppMainWindow = ppWin;
	};

	void showContainer(HWND hCont, BOOL view = TRUE);
	void showContainer(UINT	uCont, BOOL view = TRUE);

	void updateContainerInfo(HWND hClient);

	void createDockableDlg(tTbData* data, int iCont, bool isVisible = false);
	void setActiveTab(int iCont, int iItem);

	void showDockableDlg(HWND hDlg, BOOL view);
	void showDockableDlg(TCHAR* pszName, BOOL view);

	DockingCont* toggleActiveTb(DockingCont* pContSrc, UINT message, BOOL bNew = FALSE, LPRECT rcFloat = NULL);
	DockingCont* toggleVisTb(DockingCont* pContSrc, UINT message, LPRECT rcFloat = NULL);
	void		 toggleActiveTb(DockingCont* pContSrc, DockingCont* pContTgt);
	void		 toggleVisTb(DockingCont* pContSrc, DockingCont* pContTgt);

	// get number of container
	int  GetContainer(DockingCont* pCont);

	/* get all container in vector */
	std::vector<DockingCont*> & getContainerInfo() {
		return _vContainer;
	};
	/* get dock data (sized areas) */
	void getDockInfo(tDockMgr *pDockData);

	/* setting styles of docking */
	void setStyleCaption(BOOL captionOnTop);

	int getDockedContSize(int iCont);
	void setDockedContSize(int iCont, int iSize);

	virtual void destroy();

private :

	static LRESULT CALLBACK staticWinProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	void onSize();

	void	toggleTb(DockingCont* pContSrc, DockingCont* pContTgt, tTbData* TbData);

	// test if container exists
	BOOL ContExists(size_t iCont);
	int	 FindEmptyContainer();
	LRESULT SendNotify(HWND hWnd, UINT message);

private:
	Window						**_ppWindow;
	RECT						_rcWork;
	RECT						_rect;
	Window						**_ppMainWindow;

	/* handles all the icons */
	std::vector<HWND>			_vImageList;
	HIMAGELIST					_hImageList;
	std::vector<DockingCont*>	_vContainer;
	tDockMgr*					_dockData;

	static BOOL					_isRegistered;
	BOOL						_isInitialized;
	int							_iContMap[CONT_MAP_MAX];

	/* splitter data */
	std::vector<DockingSplitter*>	_vSplitter;
};

#endif //DOCKINGMANAGER_H
