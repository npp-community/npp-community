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

#ifndef WINCONTROLS_TASKLIST_TASKLISTDLG_H
#define WINCONTROLS_TASKLIST_TASKLISTDLG_H

#ifndef WINCONTROLS_STATICDIALOG_STATICDIALOG_H
#include "WinControls/StaticDialog/StaticDialog.h"
#endif

#define	TASKLIST_USER    (WM_USER + 8000)
#define WM_GETTASKLISTINFO (TASKLIST_USER + 01)

class TaskList;

struct TaskLstFnStatus {
	int _iView;
	int _docIndex;
	generic_string _fn;
	int _status;
	TaskLstFnStatus(generic_string str, int status) : _fn(str), _status(status){};
	TaskLstFnStatus(int iView, int docIndex, generic_string str, int status) : _iView(iView), _docIndex(docIndex), _fn(str), _status(status) {};
};

struct TaskListInfo {
	std::vector<TaskLstFnStatus> _tlfsLst;
	int _currentIndex;
};

class TaskListDlg : public StaticDialog
{
public :
    TaskListDlg();
	~TaskListDlg();

	void init(HINSTANCE hInst, HWND parent, HIMAGELIST hImgLst, bool dir);

    int doDialog(bool isRTL = false);

protected :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);

private :
	TaskList* _taskList;
	TaskListInfo _taskListInfo;
	HIMAGELIST _hImalist;
	bool _initDir;
	HHOOK _hHooker;

	void drawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};


#endif // WINCONTROLS_TASKLIST_TASKLISTDLG_H
