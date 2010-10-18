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
#include "WinControls/TrayIcon/trayIconControler.h"
trayIconControler::trayIconControler(HWND hwnd, UINT uID, UINT uCBMsg, HICON hicon, TCHAR *tip)
{
  _nid.cbSize = sizeof(_nid);
  _nid.hWnd = hwnd;
  _nid.uID = uID;
  _nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  _nid.uCallbackMessage = uCBMsg;
  _nid.hIcon = hicon;
  lstrcpy(_nid.szTip, tip);

  ::RegisterWindowMessage(TEXT("TaskbarCreated"));
  _isIconShowed = false;
}

int trayIconControler::doTrayIcon(DWORD op)
{
  if ((op != TRAYICON_ADD)&&(op != TRAYICON_REMOVE)) return TRAYICON_INCORRECT_OPERATION;
  if (((_isIconShowed)&&(op == TRAYICON_ADD))||((!_isIconShowed)&&(op == TRAYICON_REMOVE)))
    return TRAYICON_OPERATION_INCOHERENT;
  ::Shell_NotifyIcon(op, &_nid);
  _isIconShowed = !_isIconShowed;

  return 0;
}
