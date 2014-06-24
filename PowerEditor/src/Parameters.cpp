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

#include "precompiled_headers.h"

#include "Parameters.h"
#include "WinControls/OpenSaveFileDialog/FileDialog.h"
#include "ScintillaComponent/ScintillaEditView.h"

#include "keys.h"
#include "MenuCmdID.h"

#include "resource.h"
#include "ScintillaComponent/colors.h"

#include "MISC/Common/npp_winver.h"
#include "MISC/Common/npp_session.h"

struct WinMenuKeyDefinition {	//more or less matches accelerator table definition, easy copy/paste
	//const TCHAR * name;	//name retrieved from menu?
	UCHAR vKey;
	int functionId;
	bool isCtrl;
	bool isAlt;
	bool isShift;
	TCHAR * specialName;		//Used when no real menu name exists (in case of toggle for example)
};


struct ScintillaKeyDefinition {
	const TCHAR * name;
	int functionId;
	bool isCtrl;
	bool isAlt;
	bool isShift;
	UCHAR vKey;
	int redirFunctionId;	//this gets set  when a function is being redirected through Notepad++ if Scintilla doesnt do it properly :)
};

WinMenuKeyDefinition winKeyDefs[] = {
//array of accelerator keys for all std menu items, values can be 0 for vKey, which means its unused
//================================================================================
//  {V_KEY,		COMMAND_ID,				 			Ctrl,  Alt,   Shift, cmdName},
//================================================================================
	{VK_N,		IDM_FILE_NEW,			 			true,  false, false, NULL},
	{VK_O,		IDM_FILE_OPEN,						true,  false, false, NULL},
	{VK_NULL,	IDM_FILE_RELOAD,					false, false, false, NULL},
	{VK_S,		IDM_FILE_SAVE,						true,  false, false, NULL},
	{VK_S,		IDM_FILE_SAVEAS,					true,  true,  false, NULL},
	{VK_NULL,	IDM_FILE_SAVECOPYAS,				false, false, false, NULL},
	{VK_S,		IDM_FILE_SAVEALL,					true,  false, true,  NULL},
	{VK_W,	 	IDM_FILE_CLOSE,						true,  false, false, NULL},
	{VK_NULL,	IDM_FILE_CLOSEALL,					false, false, false, NULL},
	{VK_NULL,	IDM_FILE_CLOSEALL_BUT_CURRENT,		false, false, false, NULL},
	{VK_NULL,	IDM_FILE_DELETE,					false, false, false, NULL},
	{VK_NULL,	IDM_FILE_RENAME,					false, false, false, NULL},
	{VK_NULL,	IDM_FILE_LOADSESSION,				false, false, false, NULL},
	{VK_NULL,	IDM_FILE_SAVESESSION,				false, false, false, NULL},
	{VK_P,		IDM_FILE_PRINT,						true,  false, false, NULL},
	{VK_NULL,	IDM_FILE_PRINTNOW,					false, false, false, NULL},
	{VK_F4,		IDM_FILE_EXIT,						false, true,  false, NULL},

//	{VK_NULL,	IDM_EDIT_UNDO,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_REDO,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_CUT,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_COPY,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_PASTE,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_DELETE,	 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_SELECTALL,					false, false, false, NULL},

	{VK_NULL,	IDM_EDIT_SETREADONLY,				false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_CLEARREADONLY,				false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_FULLPATHTOCLIP,			false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_FILENAMETOCLIP,			false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_CURRENTDIRTOCLIP,			false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_INS_TAB,					false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_RMV_TAB,					false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_DUP_LINE,					false, false, false, NULL},
	{VK_I,		IDM_EDIT_SPLIT_LINES,		 		true,  false, false, NULL},
	{VK_J,		IDM_EDIT_JOIN_LINES,		 		true,  false, false, NULL},
	{VK_UP,		IDM_EDIT_LINE_UP,					true,  false, true,  NULL},
	{VK_DOWN,	IDM_EDIT_LINE_DOWN,					true,  false, true,  NULL},
	{VK_NULL,	IDM_EDIT_TRIMTRAILING,		 		false, false, false, NULL},
	{VK_C,		IDM_EDIT_COLUMNMODE,				false, true,  false, NULL},
	{VK_U, 		IDM_EDIT_UPPERCASE,					true,  false, true,  NULL},
	{VK_U, 		IDM_EDIT_LOWERCASE,					true,  false, false, NULL},
	{VK_Q,		IDM_EDIT_BLOCK_COMMENT,				true,  false, false, NULL},
	{VK_K,		IDM_EDIT_BLOCK_COMMENT_SET,			true,  false, false, NULL},
	{VK_K,		IDM_EDIT_BLOCK_UNCOMMENT,			true,  false, true,  NULL},
	{VK_Q,		IDM_EDIT_STREAM_COMMENT, 			true,  false, true,  NULL},
	{VK_SPACE,	IDM_EDIT_AUTOCOMPLETE,				true,  false, false, NULL},
	{VK_RETURN,	IDM_EDIT_AUTOCOMPLETE_CURRENTFILE,	true,  false, false, NULL},
	{VK_SPACE,	IDM_EDIT_FUNCCALLTIP,				true,  false, true,  NULL},
	{VK_R,		IDM_EDIT_RTL,						true,  true,  false, NULL},
	{VK_L,		IDM_EDIT_LTR,						true,  true,  false, NULL},

	{VK_F,		IDM_SEARCH_FIND,					true,  false, false, NULL},
	{VK_F,		IDM_SEARCH_FINDINFILES,				true,  false, true,  NULL},
	{VK_F3,		IDM_SEARCH_FINDNEXT,				false, false, false, NULL},
	{VK_F3,		IDM_SEARCH_FINDPREV,				false, false, true,  NULL},
	{VK_F3,		IDM_SEARCH_VOLATILE_FINDNEXT,		true,  true, false, NULL},
	{VK_F3,		IDM_SEARCH_VOLATILE_FINDPREV,		true,  true, true,  NULL},
    {VK_F3,		IDM_SEARCH_SETANDFINDNEXT,          true,  false, false, NULL},
	{VK_F3,		IDM_SEARCH_SETANDFINDPREV,          true,  false, true,  NULL},
	{VK_F4,		IDM_SEARCH_GOTONEXTFOUND,           false, false, false, NULL},
	{VK_F4,		IDM_SEARCH_GOTOPREVFOUND,           false, false, true,  NULL},
	{VK_F7,		IDM_FOCUS_ON_FOUND_RESULTS,         false, false, false, NULL},
	{VK_H,		IDM_SEARCH_REPLACE,					true,  false, false, NULL},
	{VK_I,		IDM_SEARCH_FINDINCREMENT,			true,  true,  false, NULL},
	{VK_G,		IDM_SEARCH_GOTOLINE,		 		true,  false, false, NULL},
	{VK_B,		IDM_SEARCH_GOTOMATCHINGBRACE,		true,  false, false, NULL},
	{VK_F2,		IDM_SEARCH_TOGGLE_BOOKMARK,			true,  false, false, NULL},
	{VK_F2,		IDM_SEARCH_NEXT_BOOKMARK, 			false, false, false, NULL},
	{VK_F2,		IDM_SEARCH_PREV_BOOKMARK, 			false, false, true,  NULL},
	{VK_NULL,	IDM_SEARCH_CLEAR_BOOKMARKS, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_CUTMARKEDLINES, 			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_COPYMARKEDLINES, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_PASTEMARKEDLINES, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_DELETEMARKEDLINES, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_MARKALLEXT1,			    false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_MARKALLEXT2,		    	false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_MARKALLEXT3,			    false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_MARKALLEXT4, 			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_MARKALLEXT5,	    		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_UNMARKALLEXT1,	    	false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_UNMARKALLEXT2,			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_UNMARKALLEXT3,			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_UNMARKALLEXT4,			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_UNMARKALLEXT5,			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_CLEARALLMARKS,			false, false, false, NULL},
	{VK_1,		IDM_SEARCH_GOPREVMARKER1,			true,  false, true,  NULL},
	{VK_2,		IDM_SEARCH_GOPREVMARKER2,			true,  false, true,  NULL},
	{VK_3,		IDM_SEARCH_GOPREVMARKER3,			true,  false, true,  NULL},
	{VK_4,		IDM_SEARCH_GOPREVMARKER4,			true,  false, true,  NULL},
	{VK_5,		IDM_SEARCH_GOPREVMARKER5,			true,  false, true,  NULL},
	{VK_0,		IDM_SEARCH_GOPREVMARKER_DEF,		true,  false, true,  NULL},

	{VK_F11,	IDM_VIEW_FULLSCREENTOGGLE,			false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_ALWAYSONTOP,				false, false, false, NULL},
	{VK_F12,	IDM_VIEW_POSTIT,					false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_TAB_SPACE,					false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_EOL,						false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_ALL_CHARACTERS,			false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_INDENT_GUIDE,				false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_WRAP,						false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_WRAP_SYMBOL,				false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_USER_DLG,					false, false, false, NULL},
	//{VK_NULL,	IDM_VIEW_ZOOMIN,					false, false, false, NULL},
	//{VK_NULL,	IDM_VIEW_ZOOMOUT,					false, false, false, NULL},
	//{VK_NULL,	IDM_VIEW_ZOOMRESTORE,				false, false, false, NULL},
	{VK_0,		IDM_VIEW_TOGGLE_FOLDALL, 			false, true,  false, NULL},
	{VK_F,		IDM_VIEW_FOLD_CURRENT,				true,  true,  false, NULL},
	{VK_1,		IDM_VIEW_FOLD_1, 					false, true,  false, NULL},
	{VK_2,		IDM_VIEW_FOLD_2, 					false, true,  false, NULL},
	{VK_3,		IDM_VIEW_FOLD_3, 					false, true,  false, NULL},
	{VK_4,		IDM_VIEW_FOLD_4, 					false, true,  false, NULL},
	{VK_5,		IDM_VIEW_FOLD_5, 					false, true,  false, NULL},
	{VK_6,		IDM_VIEW_FOLD_6, 					false, true,  false, NULL},
	{VK_7,		IDM_VIEW_FOLD_7, 					false, true,  false, NULL},
	{VK_8,		IDM_VIEW_FOLD_8, 					false, true,  false, NULL},
	{VK_F,		IDM_VIEW_UNFOLD_CURRENT,			true,  true,  true,  NULL},
	{VK_1,		IDM_VIEW_UNFOLD_1,					false, true,  true,  NULL},
	{VK_2,		IDM_VIEW_UNFOLD_2,					false, true,  true,  NULL},
	{VK_3,		IDM_VIEW_UNFOLD_3,					false, true,  true,  NULL},
	{VK_4,		IDM_VIEW_UNFOLD_4,					false, true,  true,  NULL},
	{VK_5,		IDM_VIEW_UNFOLD_5,					false, true,  true,  NULL},
	{VK_6,		IDM_VIEW_UNFOLD_6,					false, true,  true,  NULL},
	{VK_7,		IDM_VIEW_UNFOLD_7,					false, true,  true,  NULL},
	{VK_8,		IDM_VIEW_UNFOLD_8,					false, true,  true,  NULL},
	{VK_0,		IDM_VIEW_TOGGLE_UNFOLDALL,			false, true,  true,  NULL},
	{VK_H,		IDM_VIEW_HIDELINES,					false, true,  false, NULL},
	{VK_NULL,	IDM_VIEW_GOTO_ANOTHER_VIEW,			false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_CLONE_TO_ANOTHER_VIEW,		false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_SYNSCROLLV,				false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_SYNSCROLLH,				false, false, false, NULL},
	{VK_F8,		IDM_VIEW_SWITCHTO_OTHER_VIEW,		false, false, false, NULL},

	{VK_NULL, 	IDM_FORMAT_TODOS,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_TOUNIX,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_TOMAC,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_ANSI,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_UTF_8,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_AS_UTF_8,				false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_UCS_2BE,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_UCS_2LE,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_ANSI,				false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_AS_UTF_8,			false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_UTF_8,				false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_UCS_2BE,			false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_UCS_2LE,			false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_6,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1256,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_13,             false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1257,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_14,             false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_5,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_MAC_CYRILLIC,            false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_KOI8R_CYRILLIC,          false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_KOI8U_CYRILLIC,          false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1251,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1250,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_437,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_720,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_737,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_775,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_850,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_852,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_855,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_857,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_858,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_860,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_861,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_862,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_863,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_865,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_866,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_DOS_869,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_BIG5,                    false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_GB2312,                  false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_2,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_7,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1253,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_8,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1255,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_SHIFT_JIS,               false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_EUC_KR,                  false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_10,             false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_15,             false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_4,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_16,             false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_3,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_11,             false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_TIS_620,                 false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_9,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1254,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1252,                false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_ISO_8859_1,              false, false, false, NULL},
    {VK_NULL,   IDM_FORMAT_WIN_1258,                false, false, false, NULL},

	{VK_NULL, 	IDM_SETTING_PREFERENCE,				false, false, false, NULL},
	{VK_NULL, 	IDM_LANGSTYLE_CONFIG_DLG,			false, false, false, NULL},
	{VK_NULL, 	IDM_SETTING_SHORTCUT_MAPPER,		false, false, false, NULL},

	{VK_R,		IDC_EDIT_TOGGLEMACRORECORDING,		true,  false, true,  TEXT("Toggle macro record")},
	{VK_P,		IDM_MACRO_PLAYBACKRECORDEDMACRO, 	true,  false, true,  NULL},
	{VK_NULL,	IDM_MACRO_SAVECURRENTMACRO, 		false, false, false, NULL},
	{VK_NULL,	IDM_MACRO_RUNMULTIMACRODLG, 		false, false, false, NULL},

	{VK_F5,		IDM_EXECUTE,						false, false, false, NULL},

	{VK_NULL,	IDM_HOMESWEETHOME, 					false, false, false, NULL},
	{VK_NULL,	IDM_PROJECTPAGE, 					false, false, false, NULL},
	{VK_NULL,	IDM_ONLINEHELP, 					false, false, false, NULL},
	{VK_NULL,	IDM_FORUM, 							false, false, false, NULL},
	{VK_NULL,	IDM_PLUGINSHOME, 					false, false, false, NULL},
	{VK_F1,		IDM_ABOUT, 							false, false, false, NULL},
	{VK_F1,		IDM_HELP, 							false, false, true,  NULL},

	{VK_TAB,	IDC_PREV_DOC,						true,  false, true, TEXT("Switch to previous document")},
	{VK_TAB,	IDC_NEXT_DOC,						true,  false, false, TEXT("Switch to next document")},

	{VK_1,		IDM_SEARCH_GONEXTMARKER1,			true,  false, false, NULL},
	{VK_2,		IDM_SEARCH_GONEXTMARKER2,			true,  false, false, NULL},
	{VK_3,		IDM_SEARCH_GONEXTMARKER3,			true,  false, false, NULL},
	{VK_4,		IDM_SEARCH_GONEXTMARKER4,			true,  false, false, NULL},
	{VK_5,		IDM_SEARCH_GONEXTMARKER5,			true,  false, false, NULL},
	{VK_0,		IDM_SEARCH_GONEXTMARKER_DEF,		true,  false, false, NULL}

};


ScintillaKeyDefinition scintKeyDefs[] = {	//array of accelerator keys for all possible scintilla functions, values can be 0 for vKey, which means its unused
	{TEXT("SCI_CUT"),					SCI_CUT,					true,  false, false, VK_X, 		IDM_EDIT_CUT},
	{TEXT(""),							SCI_CUT,					false, false, true,  VK_DELETE, 0},
	{TEXT("SCI_COPY"),					SCI_COPY,					true,  false, false, VK_C, 		IDM_EDIT_COPY},
	{TEXT(""),							SCI_COPY,					true,  false, false, VK_INSERT, 0},
	{TEXT("SCI_PASTE"),					SCI_PASTE,					true,  false, false, VK_V, 		IDM_EDIT_PASTE},
	{TEXT(""),							SCI_PASTE,					false, false, true,  VK_INSERT, 0},
	{TEXT("SCI_SELECTALL"),				SCI_SELECTALL,				true,  false, false, VK_A, 		IDM_EDIT_SELECTALL},
	{TEXT("SCI_CLEAR"),					SCI_CLEAR,					false, false, false, VK_DELETE, IDM_EDIT_DELETE},
	{TEXT("SCI_CLEARALL"),				SCI_CLEARALL,				false, false, false, 0,			0},
	{TEXT("SCI_UNDO"),					SCI_UNDO,					true,  false, false, VK_Z, 		IDM_EDIT_UNDO},
	{TEXT(""),							SCI_UNDO,					false, true,  false, VK_BACK, 	0},
	{TEXT("SCI_REDO"),					SCI_REDO,					true,  false, false, VK_Y, 		IDM_EDIT_REDO},
	{TEXT("SCI_NEWLINE"),					SCI_NEWLINE,				false, false, false, VK_RETURN, 0},
	{TEXT(""),							SCI_NEWLINE,				false, false, true,  VK_RETURN, 0},
	{TEXT("SCI_TAB"),						SCI_TAB,					false, false, false, VK_TAB,	IDM_EDIT_INS_TAB},
	{TEXT("SCI_BACKTAB"),					SCI_BACKTAB,				false, false, true,  VK_TAB,	IDM_EDIT_RMV_TAB},
	{TEXT("SCI_FORMFEED"),				SCI_FORMFEED,				false, false, false, 0,			0},
	{TEXT("SCI_ZOOMIN"),					SCI_ZOOMIN,					true,  false, false, VK_ADD, 	IDM_VIEW_ZOOMIN},
	{TEXT("SCI_ZOOMOUT"),					SCI_ZOOMOUT,				true,  false, false, VK_SUBTRACT,IDM_VIEW_ZOOMOUT},
	{TEXT("SCI_SETZOOM"),					SCI_SETZOOM,				true,  false, false, VK_DIVIDE,	IDM_VIEW_ZOOMRESTORE},
	{TEXT("SCI_SELECTIONDUPLICATE"),		SCI_SELECTIONDUPLICATE,		true,  false, false, VK_D, 		IDM_EDIT_DUP_LINE},
	{TEXT("SCI_LINESJOIN"),				SCI_LINESJOIN,				false, false, false, 0,			0},
	{TEXT("SCI_SCROLLCARET"),				SCI_SCROLLCARET,			false, false, false, 0,			0},
	{TEXT("SCI_EDITTOGGLEOVERTYPE"),		SCI_EDITTOGGLEOVERTYPE,		false, false, false, VK_INSERT, 0},
	{TEXT("SCI_MOVECARETINSIDEVIEW"),		SCI_MOVECARETINSIDEVIEW,	false, false, false, 0,			0},
	{TEXT("SCI_LINEDOWN"),				SCI_LINEDOWN,				false, false, false, VK_DOWN,	0},
	{TEXT("SCI_LINEDOWNEXTEND"),			SCI_LINEDOWNEXTEND,			false, false, true,  VK_DOWN,	0},
	{TEXT("SCI_LINEDOWNRECTEXTEND"),		SCI_LINEDOWNRECTEXTEND,		false, true,  true,  VK_DOWN,	0},
	{TEXT("SCI_LINESCROLLDOWN"),			SCI_LINESCROLLDOWN,			true,  false, false, VK_DOWN,	0},
	{TEXT("SCI_LINEUP"),					SCI_LINEUP,					false, false, false, VK_UP,		0},
	{TEXT("SCI_LINEUPEXTEND"),			SCI_LINEUPEXTEND,			false, false, true,  VK_UP,		0},
	{TEXT("SCI_LINEUPRECTEXTEND"),		SCI_LINEUPRECTEXTEND,		false, true,  true,  VK_UP,		0},
	{TEXT("SCI_LINESCROLLUP"),			SCI_LINESCROLLUP,			true,  false, false, VK_UP,		0},
	{TEXT("SCI_PARADOWN"),				SCI_PARADOWN,				true,  false, false, VK_OEM_6,	0},
	{TEXT("SCI_PARADOWNEXTEND"),			SCI_PARADOWNEXTEND,			true,  false, true,  VK_OEM_6,	0},
	{TEXT("SCI_PARAUP"),					SCI_PARAUP,					true,  false, false, VK_OEM_4,	0},
	{TEXT("SCI_PARAUPEXTEND"),			SCI_PARAUPEXTEND,			true,  false, true,  VK_OEM_4,	0},
	{TEXT("SCI_CHARLEFT"),				SCI_CHARLEFT,				false, false, false, VK_LEFT,	0},
	{TEXT("SCI_CHARLEFTEXTEND"),			SCI_CHARLEFTEXTEND,			false, false, true,  VK_LEFT,	0},
	{TEXT("SCI_CHARLEFTRECTEXTEND"),		SCI_CHARLEFTRECTEXTEND,		false, true,  true,  VK_LEFT,	0},
	{TEXT("SCI_CHARRIGHT"),				SCI_CHARRIGHT,				false, false, false, VK_RIGHT,	0},
	{TEXT("SCI_CHARRIGHTEXTEND"),			SCI_CHARRIGHTEXTEND,		false, false, true,  VK_RIGHT,	0},
	{TEXT("SCI_CHARRIGHTRECTEXTEND"),		SCI_CHARRIGHTRECTEXTEND,	false, true,  true,  VK_RIGHT,	0},
	{TEXT("SCI_WORDLEFT"),				SCI_WORDLEFT,				true,  false, false, VK_LEFT,	0},
	{TEXT("SCI_WORDLEFTEXTEND"),			SCI_WORDLEFTEXTEND,			true,  false, true,  VK_LEFT,	0},
	{TEXT("SCI_WORDRIGHT"),				SCI_WORDRIGHT,				true,  false, false, VK_RIGHT,	0},
	{TEXT("SCI_WORDRIGHTEXTEND"),			SCI_WORDRIGHTEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_WORDLEFTEND"),				SCI_WORDLEFTEND,			false, false, false, 0,			0},
	{TEXT("SCI_WORDLEFTENDEXTEND"),		SCI_WORDLEFTENDEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_WORDRIGHTEND"),			SCI_WORDRIGHTEND,			false, false, false, 0,			0},
	{TEXT("SCI_WORDRIGHTENDEXTEND"),		SCI_WORDRIGHTENDEXTEND,		true,  false, true,  VK_RIGHT,	0},
	{TEXT("SCI_WORDPARTLEFT"),			SCI_WORDPARTLEFT,			true,  false, false, VK_OEM_2,	0},
	{TEXT("SCI_WORDPARTLEFTEXTEND"),		SCI_WORDPARTLEFTEXTEND,		true,  false, true,  VK_OEM_2,	0},
	{TEXT("SCI_WORDPARTRIGHT"),			SCI_WORDPARTRIGHT,			true,  false, false, VK_OEM_5,	0},
	{TEXT("SCI_WORDPARTRIGHTEXTEND"),		SCI_WORDPARTRIGHTEXTEND,	true,  false, true,  VK_OEM_5,	0},
	{TEXT("SCI_HOME"),					SCI_HOME,					false, false, false, 0,			0},
	{TEXT("SCI_HOMEEXTEND"),				SCI_HOMEEXTEND,				false, false, false, 0,			0},
	{TEXT("SCI_HOMERECTEXTEND"),			SCI_HOMERECTEXTEND,			false, false, false, 0,			0},
	{TEXT("SCI_HOMEDISPLAY"),				SCI_HOMEDISPLAY,			false, true,  false, VK_HOME, 	0},
	{TEXT("SCI_HOMEDISPLAYEXTEND"),		SCI_HOMEDISPLAYEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_HOMEWRAP"),				SCI_HOMEWRAP,				false, false, false, 0,			0},
	{TEXT("SCI_HOMEWRAPEXTEND"),			SCI_HOMEWRAPEXTEND,			false, false, false, 0,			0},
	{TEXT("SCI_VCHOME"),					SCI_VCHOME,					false, false, false, 0,			0},
	{TEXT("SCI_VCHOMEEXTEND"),			SCI_VCHOMEEXTEND,			false, false, true,  VK_HOME, 	0},
	{TEXT("SCI_VCHOMERECTEXTEND"),		SCI_VCHOMERECTEXTEND,		false, true,  true,  VK_HOME,	0},
	{TEXT("SCI_VCHOMEWRAP"),				SCI_VCHOMEWRAP,				false, false, false, VK_HOME,	0},
	{TEXT("SCI_VCHOMEWRAPEXTEND"),		SCI_VCHOMEWRAPEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_LINEEND"),					SCI_LINEEND,				false, false, false, 0,			0},
	{TEXT("SCI_LINEENDEXTEND"),			SCI_LINEENDEXTEND,			false, false, true,  VK_END,	0},
	{TEXT("SCI_LINEENDRECTEXTEND"),		SCI_LINEENDRECTEXTEND,		false, true,  true,  VK_END,	0},
	{TEXT("SCI_LINEENDDISPLAY"),			SCI_LINEENDDISPLAY,			false, true,  false, VK_END, 	0},
	{TEXT("SCI_LINEENDDISPLAYEXTEND"), 	SCI_LINEENDDISPLAYEXTEND,	false, false, false, 0,			0},
	{TEXT("SCI_LINEENDWRAP"),				SCI_LINEENDWRAP,			false, false, false, VK_END,	0},
	{TEXT("SCI_LINEENDWRAPEXTEND"),		SCI_LINEENDWRAPEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_DOCUMENTSTART"),			SCI_DOCUMENTSTART,			true,  false, false, VK_HOME, 	0},
	{TEXT("SCI_DOCUMENTSTARTEXTEND"),		SCI_DOCUMENTSTARTEXTEND,	true,  false, true,  VK_HOME, 	0},
	{TEXT("SCI_DOCUMENTEND"),				SCI_DOCUMENTEND,			true,  false, false, VK_END, 	0},
	{TEXT("SCI_DOCUMENTENDEXTEND"),		SCI_DOCUMENTENDEXTEND,		true,  false, true,  VK_END, 	0},
	{TEXT("SCI_PAGEUP"),					SCI_PAGEUP,					false, false, false, VK_PRIOR,	0},
	{TEXT("SCI_PAGEUPEXTEND"),			SCI_PAGEUPEXTEND,			false, false, true,  VK_PRIOR,	0},
	{TEXT("SCI_PAGEUPRECTEXTEND"),		SCI_PAGEUPRECTEXTEND,		false, true,  true,  VK_PRIOR,	0},
	{TEXT("SCI_PAGEDOWN"),				SCI_PAGEDOWN,				false, false, false, VK_NEXT, 	0},
	{TEXT("SCI_PAGEDOWNEXTEND"),			SCI_PAGEDOWNEXTEND,			false, false, true,  VK_NEXT, 	0},
	{TEXT("SCI_PAGEDOWNRECTEXTEND"),		SCI_PAGEDOWNRECTEXTEND,		false, true,  true,  VK_NEXT,	0},
	{TEXT("SCI_STUTTEREDPAGEUP"),			SCI_STUTTEREDPAGEUP,		false, false, false, 0,			0},
	{TEXT("SCI_STUTTEREDPAGEUPEXTEND"),	SCI_STUTTEREDPAGEUPEXTEND,	false, false, false, 0,			0},
	{TEXT("SCI_STUTTEREDPAGEDOWN"),		SCI_STUTTEREDPAGEDOWN,		false, false, false, 0,			0},
	{TEXT("SCI_STUTTEREDPAGEDOWNEXTEND"), SCI_STUTTEREDPAGEDOWNEXTEND,false, false, false, 0,			0},
	{TEXT("SCI_DELETEBACK"),				SCI_DELETEBACK,				false, false, false, VK_BACK,	0},
	{TEXT(""),							SCI_DELETEBACK,				false, false, true,  VK_BACK,	0},
	{TEXT("SCI_DELETEBACKNOTLINE"),		SCI_DELETEBACKNOTLINE,		false, false, false, 0,			0},
	{TEXT("SCI_DELWORDLEFT"),				SCI_DELWORDLEFT,			true,  false, false, VK_BACK,	0},
	{TEXT("SCI_DELWORDRIGHT"),			SCI_DELWORDRIGHT,			true,  false, false, VK_DELETE, 0},
	{TEXT("SCI_DELLINELEFT"),				SCI_DELLINELEFT,			true,  false, true,  VK_BACK,	0},
	{TEXT("SCI_DELLINERIGHT"),			SCI_DELLINERIGHT,			true,  false, true,  VK_DELETE,	0},
	{TEXT("SCI_LINEDELETE"),				SCI_LINEDELETE,				true,  false, true,  VK_L, 		0},
	{TEXT("SCI_LINECUT"),					SCI_LINECUT,				true,  false, false, VK_L, 		0},
	{TEXT("SCI_LINECOPY"),				SCI_LINECOPY,				true,  false, true,  VK_T, 		0},
	{TEXT("SCI_LINETRANSPOSE"),			SCI_LINETRANSPOSE,			true,  false, false, VK_T, 		0},
	{TEXT("SCI_LINEDUPLICATE"),			SCI_LINEDUPLICATE,			false, false, false, 0,			0},
	{TEXT("SCI_CANCEL"),					SCI_CANCEL,					false, false, false, VK_ESCAPE, 0},
	{TEXT("SCI_SWAPMAINANCHORCARET"),	SCI_SWAPMAINANCHORCARET,			false, false, false, 0,	0},
	{TEXT("SCI_ROTATESELECTION"),		SCI_ROTATESELECTION,					false, false, false, 0, 0}

	//{TEXT("SCI_EMPTYUNDOBUFFER"),		SCI_EMPTYUNDOBUFFER,		false, false, false, 0,			0},
	//{TEXT("SCI_TOGGLECARETSTICKY"),		SCI_TOGGLECARETSTICKY,		false, false, false, 0,			0},
	//{TEXT("SCI_CALLTIPCANCEL"),			SCI_CALLTIPCANCEL,			false, false, false, 0,			0},
	//{TEXT("SCI_SETSAVEPOINT"),			SCI_SETSAVEPOINT,			false, false, false, 0,			0},
	//{TEXT("SCI_CLEARDOCUMENTSTYLE"),	SCI_CLEARDOCUMENTSTYLE,		false, false, false, 0,			0},
	//
	//
	//{TEXT("SCI_CHOOSECARETX"),			SCI_CHOOSECARETX,			false, false, false, 0,			0},
	//{TEXT("SCI_AUTOCCOMPLETE"),			SCI_AUTOCCOMPLETE,			false, false, false, 0,			0},
	//{TEXT("SCI_AUTOCCANCEL"),			SCI_AUTOCCANCEL,			false, false, false, 0,			0},
	//{TEXT("SCI_CLEARREGISTEREDIMAGES"), SCI_CLEARREGISTEREDIMAGES,	false, false, false, 0,			0},
	//{TEXT("SCI_HOMEDISPLAYEXTEND"),		SCI_HOMEDISPLAYEXTEND,		false, true,  true,  VK_HOME,	0},
	//{TEXT("SCI_LINEENDDISPLAYEXTEND"),	SCI_LINEENDDISPLAYEXTEND,	false, true,  true,  VK_END,	0},
	//
	//{TEXT("SCI_DELWORDRIGHTEND"),		SCI_DELWORDRIGHTEND,		false, false, false, 0,			0},
	//{TEXT("SCI_LOWERCASE"),				SCI_LOWERCASE,				false, false, false, 0,			0},
	//{TEXT("SCI_UPPERCASE"),				SCI_UPPERCASE,				false, false, false, 0,			0},
	//{TEXT("SCI_LOWERCASE"),				SCI_LOWERCASE,				true,  false, false, VK_U, 		0},
	//{TEXT("SCI_UPPERCASE"),				SCI_UPPERCASE,				true,  false, true,  VK_U, 		0},
	//
	//{TEXT("SCI_FORMFEED"),				SCI_FORMFEED,				true,  false, false, VK_L, 		0},
	//{TEXT("SCI_CLEARALLCMDKEYS"),		SCI_CLEARALLCMDKEYS,		false, false, false, 0,			0},
	//{TEXT("SCI_STARTRECORD"),			SCI_STARTRECORD,			false, false, false, 0,			0},
	//{TEXT("SCI_STOPRECORD"),			SCI_STOPRECORD,				false, false, false, 0,			0},
	//{TEXT("SCI_SEARCHANCHOR"),			SCI_SEARCHANCHOR,			false, false, false, 0,			0},
	//{TEXT("SCI_TARGETFROMSELECTION"),	SCI_TARGETFROMSELECTION,	false, false, false, 0,			0},
	//{TEXT("SCI_STYLERESETDEFAULT"),		SCI_STYLERESETDEFAULT,		false, false, false, 0,			0},
	//{TEXT("SCI_STYLECLEARALL"),			SCI_STYLECLEARALL,			false, false, false, 0,			0},
	//
};


static int getKwClassFromName(const TCHAR *str) {
	if (!lstrcmp(TEXT("instre1"), str)) return LANG_INDEX_INSTR;
	if (!lstrcmp(TEXT("instre2"), str)) return LANG_INDEX_INSTR2;
	if (!lstrcmp(TEXT("type1"), str)) return LANG_INDEX_TYPE;
	if (!lstrcmp(TEXT("type2"), str)) return LANG_INDEX_TYPE2;
	if (!lstrcmp(TEXT("type3"), str)) return LANG_INDEX_TYPE3;
	if (!lstrcmp(TEXT("type4"), str)) return LANG_INDEX_TYPE4;
	if (!lstrcmp(TEXT("type5"), str)) return LANG_INDEX_TYPE5;

	if ((str[1] == '\0') && (str[0] >= '0') && (str[0] <= '8')) // up to KEYWORDSET_MAX
		return str[0] - '0';

	return -1;
};

#ifdef UNICODE
#include "localizationString.h"

std::wstring LocalizationSwitcher::getLangFromXmlFileName(const wchar_t *fn) const
{
	size_t nbItem = sizeof(localizationDefs)/sizeof(LocalizationSwitcher::LocalizationDefinition);
	for (size_t i = 0 ; i < nbItem ; i++)
	{
		if (_wcsicmp(fn, localizationDefs[i]._xmlFileName) == 0)
			return localizationDefs[i]._langName;
	}
	return TEXT("");
}

std::wstring LocalizationSwitcher::getXmlFilePathFromLangName(const wchar_t *langName) const
{
	for (size_t i = 0 ; i < _localizationList.size() ; i++)
	{
		if (_wcsicmp(langName, _localizationList[i].first.c_str()) == 0)
			return _localizationList[i].second;
	}
	return TEXT("");
}

bool LocalizationSwitcher::addLanguageFromXml(std::wstring xmlFullPath)
{
	wchar_t * fn = ::PathFindFileNameW(xmlFullPath.c_str());
	std::wstring foundLang = getLangFromXmlFileName(fn);
	if (foundLang != TEXT(""))
	{
		_localizationList.push_back(std::pair<std::wstring, std::wstring>(foundLang, xmlFullPath));
		return true;
	}
	return false;
}

bool LocalizationSwitcher::switchToLang(wchar_t *lang2switch) const
{
	std::wstring langPath = getXmlFilePathFromLangName(lang2switch);
	if (langPath == TEXT(""))
		return false;

	return ::CopyFileW(langPath.c_str(), _nativeLangPath.c_str(), FALSE) != FALSE;
}

#endif


generic_string ThemeSwitcher::getThemeFromXmlFileName(const TCHAR *xmlFullPath) const
{
	if (!xmlFullPath || !xmlFullPath[0])
	{
		return TEXT("");
	}
	TCHAR fn[MAX_PATH];
	lstrcpy(fn, ::PathFindFileName(xmlFullPath));
	PathRemoveExtension(fn);
	generic_string themeName = fn;
	return themeName;
}

NppParameters * NppParameters::_pSelf = NULL;
int FileDialog::_dialogFileBoxId = getWinVersion() < WV_W2K?edt1:cmb13;

NppParameters::NppParameters() :
	_isTaskListRBUTTONUP_Active(false),
	_isFindReplacing(false),
	_pXmlDoc(NULL), _pXmlUserDoc(NULL), _pXmlUserStylerDoc(NULL),
	_pXmlUserLangDoc(NULL), _pXmlToolIconsDoc(NULL), _pXmlShortcutDoc(NULL),
	_pXmlContextMenuDoc(NULL), _pXmlSessionDoc(NULL), _pXmlBlacklistDoc(NULL),
	_pXmlNativeLangDocA(NULL), _nbMaxFile(10),
	_fileSaveDlgFilterIndex(-1),
	_hUser32(NULL), _hUXTheme(NULL),
	_transparentFuncAddr(NULL), _enableThemeDialogTextureFuncAddr(NULL),
	_session(new Session()),
	_pAccelerator(NULL), _pScintAccelerator(NULL),
	_asNotepadStyle(false)
{
	// Prepare for default path
	TCHAR nppPath[MAX_PATH];
	::GetModuleFileName(NULL, nppPath, MAX_PATH);

	PathRemoveFileSpec(nppPath);
	_nppPath = nppPath;

	//Initialize current directory to startup directory
	TCHAR curDir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, curDir);
	_currentDirectory = curDir;

	_appdataNppDir = TEXT("");
	generic_string notepadStylePath(_nppPath);
	PathAppend(notepadStylePath, notepadStyleFile);

	_asNotepadStyle = (PathFileExists(notepadStylePath.c_str()) == TRUE);

	//Load initial accelerator key definitions
	initMenuKeys();
	initScintillaKeys();
}

NppParameters::~NppParameters()
{
	if (_pXmlDoc != NULL)
	{
		if (_pXmlDoc->isDirty())
			_pXmlDoc->SaveFile();
		delete _pXmlDoc;
	}

	if (_pXmlUserDoc != NULL)
	{
		_pXmlUserDoc->SaveFile();
		delete _pXmlUserDoc;
	}
	if (_pXmlUserStylerDoc)
		delete _pXmlUserStylerDoc;

	if (_pXmlUserLangDoc)
	{
		delete _pXmlUserLangDoc;
	}
	if (_pXmlNativeLangDocA)
		delete _pXmlNativeLangDocA;

	if (_pXmlToolIconsDoc)
		delete _pXmlToolIconsDoc;

	if (_pXmlShortcutDoc)
		delete _pXmlShortcutDoc;

	if (_pXmlContextMenuDoc)
		delete _pXmlContextMenuDoc;

	if (_pXmlSessionDoc)
		delete _pXmlSessionDoc;

	for( std::vector<Lang *>::const_iterator it = _langList.begin(), end = _langList.end();
		it != end;
		++it)
	{
		delete (*it);
	}
	for (std::vector<TiXmlDocument*>::iterator it = _importedUDL.begin(), end = _importedUDL.end();
		it != end;
		++it)
	{
		delete (*it);
	}
	for (std::vector<UserLangContainer *>::iterator it = _userLangArray.begin(), end = _userLangArray.end();
		it != end;
		++it)
	{
		delete (*it);
	}
	if (_hUser32)
		FreeLibrary(_hUser32);
	if (_hUXTheme)
		FreeLibrary(_hUXTheme);

	// JOCE: These were NOT allocated here, and should NOT be freed here either...  :-/
	for (std::vector<TiXmlDocument *>::iterator it = _pXmlExternalLexerDoc.begin(),
												end = _pXmlExternalLexerDoc.end();
		it != end;
		++it )
	{
		delete (*it);
	}

	_pXmlExternalLexerDoc.clear();

	delete _session;
}

void cutString(const generic_string& str2cut, std::vector<generic_string> & patternVect)
{
	TCHAR str2scan[MAX_PATH];
	lstrcpy(str2scan, str2cut.c_str());
	size_t len = lstrlen(str2scan);
	bool isProcessing = false;
	TCHAR *pBegin = NULL;
	for (size_t i = 0 ; i <= len ; i++)
	{
		switch(str2scan[i])
		{
		case ' ':
		case '\0':
			{
				if (isProcessing)
				{
					str2scan[i] = '\0';
					patternVect.push_back(pBegin);
					isProcessing = false;
				}
				break;
			}

		default :
			if (!isProcessing)
			{
				isProcessing = true;
				pBegin = str2scan+i;
			}
		}
	}
}

bool NppParameters::reloadStylers(TCHAR *stylePath)
{
	if (_pXmlUserStylerDoc)
		delete _pXmlUserStylerDoc;

	_pXmlUserStylerDoc = new TiXmlDocument(stylePath?stylePath:_stylerPath);
	bool loadOkay = _pXmlUserStylerDoc->LoadFile();
	if (!loadOkay)
	{
		::MessageBox(NULL, TEXT("Load stylers.xml failed!"), stylePath, MB_OK);
		delete _pXmlUserStylerDoc;
		_pXmlUserStylerDoc = NULL;
		return false;
	}
	_lexerStylerArray.eraseAll();
	_widgetStyleArray.setNbStyler( 0 );

	getUserStylersFromXmlTree();

	//  Reload plugin styles.
	for( size_t i = 0; i < getExternalLexerDoc()->size(); i++)
	{
		getExternalLexerFromXmlTree( getExternalLexerDoc()->at(i) );
	}
	return true;
}

bool NppParameters::reloadLang()
{
	generic_string nativeLangPath(_nppPath);
	PathAppend(nativeLangPath, generic_string(TEXT("nativeLang.xml")));

	if (!PathFileExists(nativeLangPath.c_str()))
	{
		nativeLangPath = _userPath;
		PathAppend(nativeLangPath, generic_string(TEXT("nativeLang.xml")));
		if (!PathFileExists(nativeLangPath.c_str()))
			return false;
	}

	if (_pXmlNativeLangDocA)
		delete _pXmlNativeLangDocA;

	_pXmlNativeLangDocA = new TiXmlDocumentA();

	bool loadOkay = _pXmlNativeLangDocA->LoadUnicodeFilePath(nativeLangPath.c_str());
	if (!loadOkay)
	{
		delete _pXmlNativeLangDocA;
		_pXmlNativeLangDocA = NULL;
		return false;
	}
	return loadOkay;
}

bool NppParameters::load()
{
	L_END = L_EXTERNAL;
	bool isAllLaoded = true;
	assert(getNbLang() == 0);

	// Make localConf.xml path
	generic_string localConfPath(_nppPath);
	PathAppend(localConfPath, localConfFile);

	// Test if localConf.xml exist
	bool isLocal = (PathFileExists(localConfPath.c_str()) == TRUE);

	if (isLocal)
	{
		_userPath = _nppPath;
	}
	else
	{
		ITEMIDLIST *pidl;
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		TCHAR tmp[MAX_PATH];
		SHGetPathFromIDList(pidl, tmp);
		_userPath = tmp;

		PathAppend(_userPath, TEXT("Notepad++"));
		_appdataNppDir = _userPath;

		if (!PathFileExists(_userPath.c_str()))
		{
			::CreateDirectory(_userPath.c_str(), NULL);
		}
	}

	//-------------------------------------//
	// Transparent function for w2k and xp //
	//-------------------------------------//
	_hUser32 = ::GetModuleHandle(TEXT("User32"));
	if (_hUser32)
		_transparentFuncAddr = (WNDPROC)::GetProcAddress(_hUser32, "SetLayeredWindowAttributes");

	//---------------------------------------------//
	// Dlg theme texture function for xp and vista //
	//---------------------------------------------//
	_hUXTheme = ::LoadLibrary(TEXT("uxtheme.dll"));
	if (_hUXTheme)
		_enableThemeDialogTextureFuncAddr = (WNDPROC)::GetProcAddress(_hUXTheme, "EnableThemeDialogTexture");

	//---------------------------------------//
	// langs.xml : for every user statically //
	//---------------------------------------//
	generic_string langs_xml_path(_nppPath);
	PathAppend(langs_xml_path, TEXT("langs.xml"));

    BOOL doRecover = FALSE;
    if (::PathFileExists(langs_xml_path.c_str()))
    {
        struct _stat buf;

        if (generic_stat(langs_xml_path.c_str(), &buf)==0)
            if (buf.st_size == 0)
                doRecover = ::MessageBox(NULL, TEXT("Load langs.xml failed!\rDo you want to recover your langs.xml?"), TEXT("Configurator"),MB_YESNO);
    }
    else
        doRecover = true;

    if (doRecover)
	{
		generic_string srcLangsPath(_nppPath);
		PathAppend(srcLangsPath, TEXT("langs.model.xml"));
		::CopyFile(srcLangsPath.c_str(), langs_xml_path.c_str(), FALSE);
	}

	_pXmlDoc = new TiXmlDocument(langs_xml_path);
	bool loadOkay = _pXmlDoc->LoadFile();
	if (!loadOkay)
	{
		::MessageBox(NULL, TEXT("Load langs.xml failed!"), TEXT("Configurator"),MB_OK);
		delete _pXmlDoc;
		_pXmlDoc = NULL;
		isAllLaoded = false;
	}
	else
		getLangKeywordsFromXmlTree();

	//---------------------------//
	// config.xml : for per user //
	//---------------------------//
	generic_string configPath(_userPath);
	PathAppend(configPath, TEXT("config.xml"));

	generic_string srcConfigPath(_nppPath);
	PathAppend(srcConfigPath, TEXT("config.model.xml"));

	if (!::PathFileExists(configPath.c_str()))
		::CopyFile(srcConfigPath.c_str(), configPath.c_str(), FALSE);

	_pXmlUserDoc = new TiXmlDocument(configPath);
	loadOkay = _pXmlUserDoc->LoadFile();
	if (!loadOkay)
	{
		int res = ::MessageBox(NULL, TEXT("Load config.xml failed!\rDo you want to recover your config.xml?"), TEXT("Configurator"),MB_YESNO);
		if (res ==IDYES)
		{
			::CopyFile(srcConfigPath.c_str(), configPath.c_str(), FALSE);

			loadOkay = _pXmlUserDoc->LoadFile();
			if (!loadOkay)
			{
				::MessageBox(NULL, TEXT("Recover config.xml failed!"), TEXT("Configurator"),MB_OK);
				delete _pXmlUserDoc;
				_pXmlUserDoc = NULL;
				isAllLaoded = false;
			}
			else
				getUserParametersFromXmlTree();
		}
		else
		{
			delete _pXmlUserDoc;
			_pXmlUserDoc = NULL;
			isAllLaoded = false;
		}
	}
	else
		getUserParametersFromXmlTree();

	//----------------------------//
	// stylers.xml : for per user //
	//----------------------------//

	_stylerPath = _userPath;
	PathAppend(_stylerPath, TEXT("stylers.xml"));

	if (!PathFileExists(_stylerPath.c_str()))
	{
		generic_string srcStylersPath(_nppPath);
		PathAppend(srcStylersPath, TEXT("stylers.model.xml"));

		::CopyFile(srcStylersPath.c_str(), _stylerPath.c_str(), TRUE);
	}

	if ( _nppGUI._themeName.empty() || (!PathFileExists(_nppGUI._themeName.c_str())) )
	{
		_nppGUI._themeName.assign(_stylerPath);
	}

	_pXmlUserStylerDoc = new TiXmlDocument(_nppGUI._themeName.c_str());

	loadOkay = _pXmlUserStylerDoc->LoadFile();
	if (!loadOkay)
	{
		::MessageBox(NULL, TEXT("Load stylers.xml failed!"), _stylerPath.c_str(), MB_OK);
		delete _pXmlUserStylerDoc;
		_pXmlUserStylerDoc = NULL;
		isAllLaoded = false;
	}
	else
		getUserStylersFromXmlTree();

	_themeSwitcher._stylesXmlPath = _stylerPath;
	// Firstly, add the default theme
	_themeSwitcher.addDefaultThemeFromXml(_stylerPath);

	//-----------------------------------//
	// userDefineLang.xml : for per user //
	//-----------------------------------//
	_userDefineLangPath = _userPath;
	PathAppend(_userDefineLangPath, TEXT("userDefineLang.xml"));

	_pXmlUserLangDoc = new TiXmlDocument(_userDefineLangPath);
	loadOkay = _pXmlUserLangDoc->LoadFile();
	if (!loadOkay)
	{
		delete _pXmlUserLangDoc;
		_pXmlUserLangDoc = NULL;
		isAllLaoded = false;
	}
	else
		getUserDefineLangsFromXmlTree();

	//----------------------------------------------//
	// nativeLang.xml : for per user                //
	// In case of absence of user's nativeLang.xml, //
	// We'll look in the Notepad++ Dir.             //
	//----------------------------------------------//
	generic_string nativeLangPath(_userPath);
	PathAppend(nativeLangPath, TEXT("nativeLang.xml"));

	if (!PathFileExists(nativeLangPath.c_str()))
	{
		nativeLangPath = _nppPath;
		PathAppend(nativeLangPath, TEXT("nativeLang.xml"));
	}

	_pXmlNativeLangDocA = new TiXmlDocumentA();

	loadOkay = _pXmlNativeLangDocA->LoadUnicodeFilePath(nativeLangPath.c_str());
	if (!loadOkay)
	{
		delete _pXmlNativeLangDocA;
		_pXmlNativeLangDocA = NULL;
		isAllLaoded = false;
	}
#ifdef UNICODE
	_localizationSwitcher._nativeLangPath = nativeLangPath;
#endif
	//---------------------------------//
	// toolbarIcons.xml : for per user //
	//---------------------------------//
	generic_string toolbarIconsPath(_userPath);
	PathAppend(toolbarIconsPath, TEXT("toolbarIcons.xml"));

	_pXmlToolIconsDoc = new TiXmlDocument(toolbarIconsPath);
	loadOkay = _pXmlToolIconsDoc->LoadFile();
	if (!loadOkay)
	{
		delete _pXmlToolIconsDoc;
		_pXmlToolIconsDoc = NULL;
		isAllLaoded = false;
	}

	//------------------------------//
	// shortcuts.xml : for per user //
	//------------------------------//
	_shortcutsPath = _userPath;
	PathAppend(_shortcutsPath, TEXT("shortcuts.xml"));

	if (!PathFileExists(_shortcutsPath.c_str()))
	{
		generic_string srcShortcutsPath(_nppPath);
		PathAppend(srcShortcutsPath, TEXT("shortcuts.xml"));

		::CopyFile(srcShortcutsPath.c_str(), _shortcutsPath.c_str(), TRUE);
	}

	_pXmlShortcutDoc = new TiXmlDocument(_shortcutsPath);
	loadOkay = _pXmlShortcutDoc->LoadFile();
	if (!loadOkay)
	{
		delete _pXmlShortcutDoc;
		_pXmlShortcutDoc = NULL;
		isAllLaoded = false;
	}
	else
	{
		getShortcutsFromXmlTree();
		getMacrosFromXmlTree();
		getUserCmdsFromXmlTree();

		// fill out _scintillaModifiedKeys :
		// those user defined Scintilla key will be used remap Scintilla Key Array
		getScintKeysFromXmlTree();
	}

	//---------------------------------//
	// contextMenu.xml : for per user //
	//---------------------------------//
	_contextMenuPath = _userPath;
	PathAppend(_contextMenuPath, TEXT("contextMenu.xml"));

	if (!PathFileExists(_contextMenuPath.c_str()))
	{
		generic_string srcContextMenuPath(_nppPath);
		PathAppend(srcContextMenuPath, TEXT("contextMenu.xml"));

		::CopyFile(srcContextMenuPath.c_str(), _contextMenuPath.c_str(), TRUE);
	}

	_pXmlContextMenuDoc = new TiXmlDocument(_contextMenuPath);
	loadOkay = _pXmlContextMenuDoc->LoadFile();
	if (!loadOkay)
	{
		delete _pXmlContextMenuDoc;
		_pXmlContextMenuDoc = NULL;
		isAllLaoded = false;
	}
	//else
		//getContextMenuFromXmlTree();

	//----------------------------//
	// session.xml : for per user //
	//----------------------------//
	_sessionPath = _userPath;
	PathAppend(_sessionPath, TEXT("session.xml"));

	// Don't load session.xml if not required in order to speed up!!
	const NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
	if (nppGUI._rememberLastSession)
	{
		_pXmlSessionDoc = new TiXmlDocument(_sessionPath);
		loadOkay = _pXmlSessionDoc->LoadFile();
		if (!loadOkay)
			isAllLaoded = false;
		else
			getSessionFromXmlTree();

		delete _pXmlSessionDoc;
		for (size_t i = 0 ; i < _pXmlExternalLexerDoc.size() ; i++)
			if (_pXmlExternalLexerDoc[i])
				delete _pXmlExternalLexerDoc[i];

		_pXmlExternalLexerDoc.clear();
		_pXmlSessionDoc = NULL;
	}

    //------------------------------//
	// blacklist.xml : for per user //
	//------------------------------//
	_blacklistPath = _userPath;
	PathAppend(_blacklistPath, TEXT("blacklist.xml"));

    if (PathFileExists(_blacklistPath.c_str()))
	{
        _pXmlBlacklistDoc = new TiXmlDocument(_blacklistPath);
        loadOkay = _pXmlBlacklistDoc->LoadFile();
        if (loadOkay)
        {
            getBlackListFromXmlTree();
        }
    }
	return isAllLaoded;
}

void NppParameters::destroyInstance()
{
	delete _pSelf;
	_pSelf = NULL;
}

void NppParameters::setFontList(HWND hWnd)
{
	//---------------//
	// Sys font list //
	//---------------//

	LOGFONT lf;
	_fontlist.clear();
	_fontlist.push_back(TEXT(""));

	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfFaceName[0]='\0';
	lf.lfPitchAndFamily = 0;
	HDC hDC = ::GetDC(hWnd);

	::EnumFontFamiliesEx(hDC,
						&lf,
						(FONTENUMPROC) EnumFontFamExProc,
						(LPARAM) &_fontlist, 0);
}

void NppParameters::getLangKeywordsFromXmlTree()
{
	TiXmlNode *root =
        _pXmlDoc->FirstChild(TEXT("NotepadPlus"));
		if (!root) return;
	feedKeyWordsParameters(root);
}

void NppParameters::getExternalLexerFromXmlTree(TiXmlDocument *doc)
{
	TiXmlNode *root = doc->FirstChild(TEXT("NotepadPlus"));
		if (!root) return;
	feedKeyWordsParameters(root);
	feedStylerArray(root);
}

int NppParameters::addExternalLangToEnd(ExternalLangContainer * externalLang)
{
	_externalLangArray.push_back(externalLang);
	L_END++;
	return _externalLangArray.size();
}

bool NppParameters::getUserStylersFromXmlTree()
{
	TiXmlNode *root = _pXmlUserStylerDoc->FirstChild(TEXT("NotepadPlus"));
		if (!root) return false;
	return feedStylerArray(root);
}

bool NppParameters::getUserParametersFromXmlTree()
{
	if (!_pXmlUserDoc)
		return false;

	TiXmlNode *root = _pXmlUserDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root) return false;

	// GUI
	feedGUIParameters(root);

	//History
	feedFileListParameters(root);

	// Raser tout
	TiXmlNode *node = root->FirstChildElement(TEXT("History"));
	root->RemoveChild(node);

	// Repartir de zero
	TiXmlElement HistoryNode(TEXT("History"));

	root->InsertEndChild(HistoryNode);

	//Find history
	feedFindHistoryParameters(root);

	return true;
}

bool NppParameters::getUserDefineLangsFromXmlTree(TiXmlDocument *tixmldoc)
{
	if (!tixmldoc)
		return false;

	TiXmlNode *root = tixmldoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	return feedUserLang(root);
}



bool NppParameters::getShortcutsFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	feedShortcut(root);
	return true;
}

bool NppParameters::getMacrosFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	feedMacros(root);
	return true;
}

bool NppParameters::getUserCmdsFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	feedUserCmds(root);
	return true;
}


bool NppParameters::getPluginCmdsFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	feedPluginCustomizedCmds(root);
	return true;
}


bool NppParameters::getScintKeysFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	feedScintKeys(root);
	return true;
}

bool NppParameters::getBlackListFromXmlTree()
{
    if (!_pXmlBlacklistDoc)
		return false;

	TiXmlNode *root = _pXmlBlacklistDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	return feedBlacklist(root);
}

void NppParameters::initMenuKeys()
{
	int nrCommands = sizeof(winKeyDefs)/sizeof(WinMenuKeyDefinition);
	WinMenuKeyDefinition wkd;
	for(int i = 0; i < nrCommands; i++)
	{
		wkd = winKeyDefs[i];
		Shortcut sc( (wkd.specialName?wkd.specialName:TEXT("")), wkd.isCtrl, wkd.isAlt, wkd.isShift, wkd.vKey);
		_shortcuts.push_back( CommandShortcut(sc, wkd.functionId) );
	}
}

void NppParameters::initScintillaKeys() {

	int nrCommands = sizeof(scintKeyDefs)/sizeof(ScintillaKeyDefinition);

	//Warning! Matching function have to be consecutive
	ScintillaKeyDefinition skd;
	int prevIndex = -1;
	int prevID = -1;
	for(int i = 0; i < nrCommands; i++) {
		skd = scintKeyDefs[i];
		if (skd.functionId == prevID) {
			KeyCombo kc;
			kc._isCtrl = skd.isCtrl;
			kc._isAlt = skd.isAlt;
			kc._isShift = skd.isShift;
			kc._key = skd.vKey;
			_scintillaKeyCommands[prevIndex].addKeyCombo(kc);
		} else {
			_scintillaKeyCommands.push_back(ScintillaKeyMap(Shortcut(skd.name, skd.isCtrl, skd.isAlt, skd.isShift, skd.vKey), skd.functionId, skd.redirFunctionId));
			prevIndex++;
		}
		prevID = skd.functionId;
	}
}
bool NppParameters::reloadContextMenuFromXmlTree(HMENU mainMenuHadle)
{
	_contextMenuItems.clear();
	return getContextMenuFromXmlTree(mainMenuHadle);
}

bool NppParameters::getContextMenuFromXmlTree(HMENU mainMenuHadle)
{
	if (!_pXmlContextMenuDoc)
		return false;
	TiXmlNode *root = _pXmlContextMenuDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;
	TiXmlNode *contextMenuRoot = root->FirstChildElement(TEXT("ScintillaContextMenu"));
	if (contextMenuRoot)
	{
		for (TiXmlNode *childNode = contextMenuRoot->FirstChildElement(TEXT("Item"));
			childNode ;
			childNode = childNode->NextSibling(TEXT("Item")) )
		{
			int id;
			const TCHAR *idStr = (childNode->ToElement())->Attribute(TEXT("id"), &id);
			if (idStr)
			{
				_contextMenuItems.push_back(MenuItemUnit(id, TEXT("")));
			}
			else
			{
				const TCHAR *menuEntryName = (childNode->ToElement())->Attribute(TEXT("MenuEntryName"));
				const TCHAR *menuItemName = (childNode->ToElement())->Attribute(TEXT("MenuItemName"));
				if (menuEntryName && menuItemName)
				{
					int nbMenuEntry = ::GetMenuItemCount(mainMenuHadle);
					for (int i = 0 ; i < nbMenuEntry ; i++)
					{
						TCHAR menuEntryString[64];
						::GetMenuString(mainMenuHadle, i, menuEntryString, 64, MF_BYPOSITION);
						if (generic_stricmp(menuEntryName, purgeMenuItemString(menuEntryString).c_str()) == 0)
						{
							std::vector< std::pair<HMENU, int> > parentMenuPos;
							HMENU topMenu = ::GetSubMenu(mainMenuHadle, i);
							int maxTopMenuPos = ::GetMenuItemCount(topMenu);
							HMENU currMenu = topMenu;
							int currMaxMenuPos = maxTopMenuPos;

							int currMenuPos = 0;
							bool notFound = false;

							do {
								if ( ::GetSubMenu( currMenu, currMenuPos ) ) {
									//  Go into sub menu
									parentMenuPos.push_back( std::make_pair( currMenu, currMenuPos ) );
									currMenu = ::GetSubMenu( currMenu, currMenuPos );
									currMenuPos = 0;
									currMaxMenuPos = ::GetMenuItemCount(currMenu);
								}
								else {
									//  Check current menu position.
									TCHAR cmdStr[256];
									::GetMenuString(currMenu, currMenuPos, cmdStr, 256, MF_BYPOSITION);
									if (generic_stricmp(menuItemName, purgeMenuItemString(cmdStr).c_str()) == 0)
									{
										int cmdId = ::GetMenuItemID(currMenu, currMenuPos);
										_contextMenuItems.push_back(MenuItemUnit(cmdId, TEXT("")));
										break;
									}

									if ( ( currMenuPos >= currMaxMenuPos ) && ( parentMenuPos.size() > 0 ) ) {
										currMenu = parentMenuPos.back().first;
										currMenuPos = parentMenuPos.back().second;
										parentMenuPos.pop_back();
										currMaxMenuPos = ::GetMenuItemCount( currMenu );
									}

									if ( ( currMenu == topMenu ) && ( currMenuPos >= maxTopMenuPos ) ) {
										notFound = true;
									}
									else {
										currMenuPos++;
									}
								}
							} while (! notFound );
							break;
						}
					}
				}
				else
				{
					const TCHAR *pluginName = (childNode->ToElement())->Attribute(TEXT("PluginEntryName"));
					const TCHAR *pluginCmdName = (childNode->ToElement())->Attribute(TEXT("pluginCommandItemName"));
					if (pluginName && pluginCmdName)
					{
						HMENU pluginsMenu = ::GetSubMenu(mainMenuHadle, MENUINDEX_PLUGINS);
						int nbPlugins = ::GetMenuItemCount(pluginsMenu);
						for (int i = 0 ; i < nbPlugins ; i++)
						{
							TCHAR menuItemString[256];
							::GetMenuString(pluginsMenu, i, menuItemString, 256, MF_BYPOSITION);
							if (generic_stricmp(pluginName, purgeMenuItemString(menuItemString).c_str()) == 0)
							{
								HMENU pluginMenu = ::GetSubMenu(pluginsMenu, i);
								int nbPluginCmd = ::GetMenuItemCount(pluginMenu);
								for (int j = 0 ; j < nbPluginCmd ; j++)
								{
									TCHAR pluginCmdStr[256];
									::GetMenuString(pluginMenu, j, pluginCmdStr, 256, MF_BYPOSITION);
									if (generic_stricmp(pluginCmdName, purgeMenuItemString(pluginCmdStr).c_str()) == 0)
									{
										int pluginCmdId = ::GetMenuItemID(pluginMenu, j);
										_contextMenuItems.push_back(MenuItemUnit(pluginCmdId, TEXT("")));
										break;
									}
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

void NppParameters::setWorkingDir(const TCHAR * newPath)
{
	if (newPath && newPath[0])
	{
		_currentDirectory = newPath;
	}
	else
	{
		if (PathFileExists(_nppGUI._defaultDirExp))
		{
			_currentDirectory = _nppGUI._defaultDirExp;
		}
		else
		{
			_currentDirectory = _nppPath.c_str();
		}
	}
}

bool NppParameters::loadSession(Session* session, const TCHAR *sessionFileName)
{
	TiXmlDocument *pXmlSessionDocument = new TiXmlDocument(sessionFileName);
	bool loadOkay = pXmlSessionDocument->LoadFile();
	if (loadOkay)
		loadOkay = getSessionFromXmlTree(pXmlSessionDocument, session);

	delete pXmlSessionDocument;
	return loadOkay;
}

static void parseSessionView(TiXmlNode *viewRoot, size_t& ioActiveIndex, std::vector<sessionFileInfo>& sessionFiles)
{
	if (viewRoot)
	{
		size_t index;
		TiXmlElement *actIndex = viewRoot->ToElement();
		const TCHAR* str = actIndex->Attribute(TEXT("activeIndex"), (int *)&index);
		if (str)
		{
			ioActiveIndex = index;
		}
		for (TiXmlNode *childNode = viewRoot->FirstChildElement(TEXT("File"));
			childNode ;
			childNode = childNode->NextSibling(TEXT("File")) )
		{
			const TCHAR *fileName = (childNode->ToElement())->Attribute(TEXT("filename"));
			if (fileName)
			{
				Position position;
				(childNode->ToElement())->Attribute(TEXT("firstVisibleLine"), &position._firstVisibleLine);
				(childNode->ToElement())->Attribute(TEXT("xOffset"), &position._xOffset);
				(childNode->ToElement())->Attribute(TEXT("startPos"), &position._startPos);
				(childNode->ToElement())->Attribute(TEXT("endPos"), &position._endPos);
				(childNode->ToElement())->Attribute(TEXT("selMode"), &position._selMode);
				(childNode->ToElement())->Attribute(TEXT("scrollWidth"), &position._scrollWidth);

				const TCHAR *langName;
				langName = (childNode->ToElement())->Attribute(TEXT("lang"));
				int encoding = -1;
				const TCHAR *encStr = (childNode->ToElement())->Attribute(TEXT("encoding"), &encoding);
				sessionFileInfo sfi(fileName, langName, encStr?encoding:-1, position);

				for (TiXmlNode *markNode = childNode->FirstChildElement(TEXT("Mark"));
					markNode ;
					markNode = markNode->NextSibling(TEXT("Mark")))
				{
					int lineNumber;
					const TCHAR *lineNumberStr = (markNode->ToElement())->Attribute(TEXT("line"), &lineNumber);
					if (lineNumberStr)
					{
						sfi.marks.push_back(lineNumber);
					}
				}
				sessionFiles.push_back(sfi);
			}
		}
	}
}

bool NppParameters::getSessionFromXmlTree(TiXmlDocument *pSessionDoc, Session *pSession)
{
	if ((pSessionDoc) && (!pSession))
		return false;

	TiXmlDocument **ppSessionDoc = &_pXmlSessionDoc;
	Session *ptrSession = _session;

	if (pSessionDoc)
	{
		ppSessionDoc = &pSessionDoc;
		ptrSession = pSession;
	}

	if (!*ppSessionDoc)
		return false;

	TiXmlNode *root = (*ppSessionDoc)->FirstChild(TEXT("NotepadPlus"));
	if (!root)
		return false;

	TiXmlNode *sessionRoot = root->FirstChildElement(TEXT("Session"));
	if (!sessionRoot)
		return false;

	TiXmlNode *mainviewRoot = sessionRoot->FirstChildElement(TEXT("mainView"));
	// JOCE: The code blocks for mainviewRoot and subviewRoot are almost identical. This can be factored.
	parseSessionView(mainviewRoot, ptrSession->_activeMainIndex, ptrSession->_mainViewFiles);

	TiXmlNode *subviewRoot = sessionRoot->FirstChildElement(TEXT("subView"));
	// If the main view is empty, let's not fill the sub view. Let's fill the main view instead.
	if (ptrSession->_mainViewFiles.size() > 0)
	{
		parseSessionView(subviewRoot, ptrSession->_activeSubIndex, ptrSession->_subViewFiles);
	}
	else
	{
		parseSessionView(subviewRoot, ptrSession->_activeMainIndex, ptrSession->_mainViewFiles);
	}

	// At this point, if _subViewFiles.size == 0, it means the active we will automatically be _mainViewFiles (idx 0)
	if (ptrSession->_subViewFiles.size() == 0 )
	{
		ptrSession->_activeView = 0;
	}
	else
	{
		TiXmlElement *actView = sessionRoot->ToElement();
		size_t index;
		const TCHAR *str = actView->Attribute(TEXT("activeView"), (int *)&index);
		if (str)
		{
			ptrSession->_activeView = index;
		}
	}

	return true;
}

void NppParameters::feedFileListParameters(TiXmlNode *node)
{
	_nbMaxFile = 10;

	TiXmlNode *historyRoot = node->FirstChildElement(TEXT("History"));
	if (!historyRoot) return;

	(historyRoot->ToElement())->Attribute(TEXT("nbMaxFile"), &_nbMaxFile);
	if ((_nbMaxFile < 0) || (_nbMaxFile > NB_MAX_LRF_FILE))
		return;

	for (TiXmlNode *childNode = historyRoot->FirstChildElement(TEXT("File"));
		childNode && (getNbLRFile() < NB_MAX_LRF_FILE);
		childNode = childNode->NextSibling(TEXT("File")) )
	{
		const TCHAR *filePath = (childNode->ToElement())->Attribute(TEXT("filename"));
		if (filePath)
		{
			_LRFileList.push_back(filePath);
		}
	}
}

void NppParameters::feedFindHistoryParameters(TiXmlNode *node)
{
	TiXmlNode *findHistoryRoot = node->FirstChildElement(TEXT("FindHistory"));
	if (!findHistoryRoot) return;

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryPath"), &_findHistory._nbMaxFindHistoryPath);
	if ((_findHistory._nbMaxFindHistoryPath > 0) && (_findHistory._nbMaxFindHistoryPath <= NB_MAX_FINDHISTORY_PATH))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Path"));
            childNode && (_findHistory._findHistoryPaths.size() < NB_MAX_FINDHISTORY_PATH);
			childNode = childNode->NextSibling(TEXT("Path")) )
		{
			const TCHAR *filePath = (childNode->ToElement())->Attribute(TEXT("name"));
			if (filePath)
			{
                _findHistory._findHistoryPaths.push_back(generic_string(filePath));
			}
		}
	}

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryFilter"), &_findHistory._nbMaxFindHistoryFilter);
	if ((_findHistory._nbMaxFindHistoryFilter > 0) && (_findHistory._nbMaxFindHistoryFilter <= NB_MAX_FINDHISTORY_FILTER))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Filter"));
			childNode && (_findHistory._findHistoryFilters.size() < NB_MAX_FINDHISTORY_FILTER);
			childNode = childNode->NextSibling(TEXT("Filter")))
		{
			const TCHAR *fileFilter = (childNode->ToElement())->Attribute(TEXT("name"));
			if (fileFilter)
			{
				_findHistory._findHistoryFilters.push_back(generic_string(fileFilter));
			}
		}
	}

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryFind"), &_findHistory._nbMaxFindHistoryFind);
	if ((_findHistory._nbMaxFindHistoryFind > 0) && (_findHistory._nbMaxFindHistoryFind <= NB_MAX_FINDHISTORY_FIND))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Find"));
			childNode && (_findHistory._findHistoryFinds.size() < NB_MAX_FINDHISTORY_FIND);
			childNode = childNode->NextSibling(TEXT("Find")))
		{
			const TCHAR *fileFind = (childNode->ToElement())->Attribute(TEXT("name"));
			if (fileFind)
			{
				_findHistory._findHistoryFinds.push_back(generic_string(fileFind));
			}
		}
	}

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryReplace"), &_findHistory._nbMaxFindHistoryReplace);
	if ((_findHistory._nbMaxFindHistoryReplace > 0) && (_findHistory._nbMaxFindHistoryReplace <= NB_MAX_FINDHISTORY_REPLACE))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Replace"));
			childNode && (_findHistory._findHistoryReplaces.size() < NB_MAX_FINDHISTORY_REPLACE);
			childNode = childNode->NextSibling(TEXT("Replace")))
		{
			const TCHAR *fileReplace = (childNode->ToElement())->Attribute(TEXT("name"));
			if (fileReplace)
			{
				_findHistory._findHistoryReplaces.push_back(generic_string(fileReplace));
			}
		}
	}

	const TCHAR *boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("matchWord"));
	if (boolStr)
		_findHistory._isMatchWord = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("matchCase"));
	if (boolStr)
		_findHistory._isMatchCase = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("wrap"));
	if (boolStr)
		_findHistory._isWrap = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("directionDown"));
	if (boolStr)
		_findHistory._isDirectionDown = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("fifRecuisive"));
	if (boolStr)
		_findHistory._isFifRecuisive = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("fifInHiddenFolder"));
	if (boolStr)
		_findHistory._isFifInHiddenFolder = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("dlgAlwaysVisible"));
	if (boolStr)
		_findHistory._isDlgAlwaysVisible = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("fifFilterFollowsDoc"));
	if (boolStr)
		_findHistory._isFilterFollowDoc = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("fifFolderFollowsDoc"));
	if (boolStr)
		_findHistory._isFolderFollowDoc = !lstrcmp(TEXT("yes"), boolStr);

	int mode = 0;
	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("searchMode"), &mode);
	if (boolStr)
		_findHistory._searchMode = (FindHistory::searchMode)mode;

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("transparencyMode"), &mode);
	if (boolStr)
		_findHistory._transparencyMode = (FindHistory::transparencyMode)mode;

	(findHistoryRoot->ToElement())->Attribute(TEXT("transparency"), &_findHistory._transparency);
	if (_findHistory._transparency <= 0 || _findHistory._transparency > 200)
		_findHistory._transparency = 150;
}

void NppParameters::feedShortcut(TiXmlNode *node)
{
	TiXmlNode *shortcutsRoot = node->FirstChildElement(TEXT("InternalCommands"));
	if (!shortcutsRoot) return;

	for (TiXmlNode *childNode = shortcutsRoot->FirstChildElement(TEXT("Shortcut"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Shortcut")) )
	{
		int id;
		const TCHAR *idStr = (childNode->ToElement())->Attribute(TEXT("id"), &id);
		if (idStr)
		{
			//find the commandid that matches this Shortcut sc and alter it, push back its index in the modified list, if not present
			int len = (int)_shortcuts.size();
			for(int i = 0; i < len; i++)
			{
				if (_shortcuts[i].getID() == id)
				{	//found our match
					getShortcuts(childNode, _shortcuts[i]);
					addUserModifiedIndex(i);
				}
			}
		}
	}
}

void NppParameters::feedMacros(TiXmlNode *node)
{
	TiXmlNode *macrosRoot = node->FirstChildElement(TEXT("Macros"));
	if (!macrosRoot) return;

	for (TiXmlNode *childNode = macrosRoot->FirstChildElement(TEXT("Macro"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Macro")) )
	{
		Shortcut sc;
		if (getShortcuts(childNode, sc))// && sc.isValid())
		{
			Macro macro;
			getActions(childNode, macro);
			int cmdID = ID_MACRO + _macros.size();
			MacroShortcut ms(sc, macro, cmdID);
			//if (ms.isValid())
			_macros.push_back(ms);
		}
	}
}


void NppParameters::getActions(TiXmlNode *node, Macro & macro)
{
	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("Action"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Action")) )
	{
		int type;
		const TCHAR *typeStr = (childNode->ToElement())->Attribute(TEXT("type"), &type);
		if ((!typeStr) || (type > 3)) // JOCE: Magic number: fix this.
			continue;

		int msg = 0;
		(childNode->ToElement())->Attribute(TEXT("message"), &msg);

		int wParam = 0;
		(childNode->ToElement())->Attribute(TEXT("wParam"), &wParam);

		int lParam = 0;
		(childNode->ToElement())->Attribute(TEXT("lParam"), &lParam);

		const TCHAR *sParam = (childNode->ToElement())->Attribute(TEXT("sParam"));
		if (!sParam)
			sParam = TEXT("");
		recordedMacroStep step(msg, wParam, lParam, sParam, type);
		if (step.isValid())
			macro.push_back(step);

	}
}

void NppParameters::feedUserCmds(TiXmlNode *node)
{
	TiXmlNode *userCmdsRoot = node->FirstChildElement(TEXT("UserDefinedCommands"));
	if (!userCmdsRoot) return;

	for (TiXmlNode *childNode = userCmdsRoot->FirstChildElement(TEXT("Command"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Command")) )
	{
		Shortcut sc;
		if (getShortcuts(childNode, sc))// && sc.isValid())
		{
			TiXmlNode *aNode = childNode->FirstChild();
			if (aNode)
			{
				const TCHAR *cmdStr = aNode->Value();
				if (cmdStr)
				{
					int cmdID = ID_USER_CMD + _userCommands.size();
					UserCommand uc(sc, cmdStr, cmdID);
					//if (uc.isValid())
					_userCommands.push_back(uc);
				}
			}
		}
	}
}

void NppParameters::feedPluginCustomizedCmds(TiXmlNode *node)
{
	TiXmlNode *pluginCustomizedCmdsRoot = node->FirstChildElement(TEXT("PluginCommands"));
	if (!pluginCustomizedCmdsRoot) return;

	for (TiXmlNode *childNode = pluginCustomizedCmdsRoot->FirstChildElement(TEXT("PluginCommand"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("PluginCommand")) )
	{
		const TCHAR *moduleName = (childNode->ToElement())->Attribute(TEXT("moduleName"));
		if (!moduleName)
			continue;

		int internalID = -1;
		const TCHAR *internalIDStr = (childNode->ToElement())->Attribute(TEXT("internalID"), &internalID);

		if (!internalIDStr)
			continue;

		//Find the corresponding plugincommand and alter it, put the index in the list
		int len = (int)_pluginCommands.size();
		for(int i = 0; i < len; i++)
		{
			PluginCmdShortcut & pscOrig = _pluginCommands[i];
			if (!generic_strnicmp(pscOrig.getModuleName(), moduleName, lstrlen(moduleName)) && pscOrig.getInternalID() == internalID)
			{
				//Found matching command
				getShortcuts(childNode, _pluginCommands[i]);
				addPluginModifiedIndex(i);
				break;
			}
		}
	}
}

void NppParameters::feedScintKeys(TiXmlNode *node)
{
	TiXmlNode *scintKeysRoot = node->FirstChildElement(TEXT("ScintillaKeys"));
	if (!scintKeysRoot) return;

	for (TiXmlNode *childNode = scintKeysRoot->FirstChildElement(TEXT("ScintKey"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("ScintKey")) )
	{
		int scintKey;
		const TCHAR *keyStr = (childNode->ToElement())->Attribute(TEXT("ScintID"), &scintKey);
		if (!keyStr)
			continue;

		int menuID;
		keyStr = (childNode->ToElement())->Attribute(TEXT("menuCmdID"), &menuID);
		if (!keyStr)
			continue;

		//Find the corresponding scintillacommand and alter it, put the index in the list
		size_t len = _scintillaKeyCommands.size();
		for(size_t i = 0; i < len; i++)
		{
			ScintillaKeyMap & skmOrig = _scintillaKeyCommands[i];
			if (skmOrig.getScintillaKeyID() == scintKey && skmOrig.getMenuCmdID() == menuID)
			{
				//Found matching command
				_scintillaKeyCommands[i].clearDups();
				getShortcuts(childNode, _scintillaKeyCommands[i]);
				_scintillaKeyCommands[i].setKeyComboByIndex(0, _scintillaKeyCommands[i].getKeyCombo());
				addScintillaModifiedIndex(i);
				KeyCombo kc;
				for (TiXmlNode *nextNode = childNode->FirstChildElement(TEXT("NextKey"));
					nextNode ;
					nextNode = childNode->NextSibling(TEXT("NextKey")) )
				{
					const TCHAR *str = (nextNode->ToElement())->Attribute(TEXT("Ctrl"));
					if (!str)
						continue;
					kc._isCtrl = !lstrcmp(TEXT("yes"), str);

					str = (nextNode->ToElement())->Attribute(TEXT("Alt"));
					if (!str)
						continue;
					kc._isAlt = !lstrcmp(TEXT("yes"), str);

					str = (nextNode->ToElement())->Attribute(TEXT("Shift"));
					if (!str)
						continue;
					kc._isShift = !lstrcmp(TEXT("yes"), str);

					int key;
					str = (nextNode->ToElement())->Attribute(TEXT("Key"), &key);
					if (!str)
						continue;
					assert(key >= 0 && key < 256);
					kc._key = (UCHAR)key; // safe, since => assert(key >= 0 && key < 256)
					_scintillaKeyCommands[i].addKeyCombo(kc);
				}
				break;
			}
		}
	}
}

bool NppParameters::feedBlacklist(TiXmlNode *node)
{
	TiXmlNode *blackListRoot = node->FirstChildElement(TEXT("PluginBlackList"));
	if (!blackListRoot) return false;

	for (TiXmlNode *childNode = blackListRoot->FirstChildElement(TEXT("Plugin"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Plugin")) )
	{
        const TCHAR *name = (childNode->ToElement())->Attribute(TEXT("name"));
	    if (name)
        {
            _blacklist.push_back(name);
        }
    }
    return true;
}

bool NppParameters::getShortcuts(TiXmlNode *node, Shortcut & sc)
{
	if (!node) return false;

	const TCHAR *name = (node->ToElement())->Attribute(TEXT("name"));
	if (!name)
		name = TEXT("");

	bool isCtrl = false;
	const TCHAR *isCtrlStr = (node->ToElement())->Attribute(TEXT("Ctrl"));
	if (isCtrlStr)
		isCtrl = !lstrcmp(TEXT("yes"), isCtrlStr);

	bool isAlt = false;
	const TCHAR *isAltStr = (node->ToElement())->Attribute(TEXT("Alt"));
	if (isAltStr)
		isAlt = !lstrcmp(TEXT("yes"), isAltStr);

	bool isShift = false;
	const TCHAR *isShiftStr = (node->ToElement())->Attribute(TEXT("Shift"));
	if (isShiftStr)
		isShift = !lstrcmp(TEXT("yes"), isShiftStr);

	int key;
	const TCHAR *keyStr = (node->ToElement())->Attribute(TEXT("Key"), &key);
	if (!keyStr)
		return false;

	assert(key >= 0 && key < 256);
	sc = Shortcut(name, isCtrl, isAlt, isShift, (UCHAR)key);
	return true;
}


bool NppParameters::feedUserLang(TiXmlNode *node)
{
    bool isEverythingOK = true;
    bool hasFoundElement = false;

	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("UserLang"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("UserLang")) )
	{
		const TCHAR *name = (childNode->ToElement())->Attribute(TEXT("name"));
		const TCHAR *ext = (childNode->ToElement())->Attribute(TEXT("ext"));
        hasFoundElement = true;
		UserLangContainer* newLangContainer = new UserLangContainer(name, ext);
		try {
			if (!name || !name[0] || !ext)
				throw std::runtime_error("NppParameters::feedUserLang : UserLang name is missing");

			TiXmlNode *settingsRoot = childNode->FirstChildElement(TEXT("Settings"));
			if (!settingsRoot)
				throw std::runtime_error("NppParameters::feedUserLang : Settings node is missing");
			newLangContainer->feedUserSettings(settingsRoot);

			TiXmlNode *keywordListsRoot = childNode->FirstChildElement(TEXT("KeywordLists"));
			if (!keywordListsRoot)
				throw std::runtime_error("NppParameters::feedUserLang : KeywordLists node is missing");

			newLangContainer->feedUserKeywordList(keywordListsRoot);

			TiXmlNode *stylesRoot = childNode->FirstChildElement(TEXT("Styles"));
			if (!stylesRoot)
				throw std::runtime_error("NppParameters::feedUserLang : Styles node is missing");

			newLangContainer->feedUserStyles(stylesRoot);

		} catch (std::exception e) {
			delete newLangContainer;
			newLangContainer = NULL;
			isEverythingOK = false;
		}

		if (newLangContainer)
		{
			_userLangArray.push_back(newLangContainer);
		}
	}

    if (isEverythingOK)
	{
        isEverythingOK = hasFoundElement;
	}

    return isEverythingOK;
}

bool NppParameters::importUDLFromFile(generic_string sourceFile)
{
    TiXmlDocument *pXmlUserLangDoc = new TiXmlDocument(sourceFile);
	bool loadOkay = pXmlUserLangDoc->LoadFile();
	if (loadOkay)
	{
		loadOkay = getUserDefineLangsFromXmlTree(pXmlUserLangDoc);
    }
	if (loadOkay)
	{
		_importedUDL.push_back(pXmlUserLangDoc);
	}
    return loadOkay;
}

bool NppParameters::exportUDLToFile(int langIndex2export, generic_string fileName2save)
{
    if (langIndex2export != -1 && langIndex2export >= (int)_userLangArray.size())
        return false;

    TiXmlDocument *pNewXmlUserLangDoc = new TiXmlDocument(fileName2save);
    TiXmlNode *newRoot2export = pNewXmlUserLangDoc->InsertEndChild(TiXmlElement(TEXT("NotepadPlus")));

    insertUserLang2Tree(newRoot2export, _userLangArray[langIndex2export]);
    bool result = pNewXmlUserLangDoc->SaveFile();

    delete pNewXmlUserLangDoc;
    return result;
}

LangType NppParameters::getLangFromExt(const TCHAR *ext)
{
	int i = getNbLang();
	i--;
	while (i >= 0)
	{
		Lang *l = getLangFromIndex(i--);

		const TCHAR *defList = l->getDefaultExtList();
		const TCHAR *userList = NULL;

		LexerStylerArray &lsa = getLStylerArray();
		const TCHAR *lName = l->getLangName();
		LexerStyler *pLS = lsa.getLexerStylerByName(lName);

		if (pLS)
			userList = pLS->getLexerUserExt();

		generic_string list(TEXT(""));
		if (defList)
			list += defList;
		if (userList)
		{
			list += TEXT(" ");
			list += userList;
		}
		if (isInList(ext, list.c_str()))
			return l->getLangID();
	}
	return L_TEXT;
}

void NppParameters::writeUserDefinedLang()
{
	if (!_pXmlUserLangDoc)
	{
		//do the treatment
		_pXmlUserLangDoc = new TiXmlDocument(_userDefineLangPath);
	}

	TiXmlNode *root = _pXmlUserLangDoc->FirstChild(TEXT("NotepadPlus"));
	if (root)
	{
		_pXmlUserLangDoc->RemoveChild(root);
	}

	_pXmlUserLangDoc->InsertEndChild(TiXmlElement(TEXT("NotepadPlus")));

	root = _pXmlUserLangDoc->FirstChild(TEXT("NotepadPlus"));

	for (std::vector<UserLangContainer *>::iterator it = _userLangArray.begin(), end = _userLangArray.end();
		it != end;
		++it)
	{
		insertUserLang2Tree(root, (*it));
	}
	_pXmlUserLangDoc->SaveFile();
}

void NppParameters::insertCmd(TiXmlNode *shortcutsRoot, const CommandShortcut & cmd)
{
	const KeyCombo & key = cmd.getKeyCombo();
	TiXmlNode *sc = shortcutsRoot->InsertEndChild(TiXmlElement(TEXT("Shortcut")));
	sc->ToElement()->SetAttribute(TEXT("id"), cmd.getID());
	sc->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	sc->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	sc->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	sc->ToElement()->SetAttribute(TEXT("Key"), key._key);
}

void NppParameters::insertMacro(TiXmlNode *macrosRoot, const MacroShortcut & macro)
{
	const KeyCombo & key = macro.getKeyCombo();
	TiXmlNode *macroRoot = macrosRoot->InsertEndChild(TiXmlElement(TEXT("Macro")));
	macroRoot->ToElement()->SetAttribute(TEXT("name"), macro.getMenuName());
	macroRoot->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	macroRoot->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	macroRoot->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	macroRoot->ToElement()->SetAttribute(TEXT("Key"), key._key);
	for (size_t i = 0 ; i < macro._macro.size() ; i++)
	{
		TiXmlNode *actionNode = macroRoot->InsertEndChild(TiXmlElement(TEXT("Action")));
		const recordedMacroStep & action = macro._macro[i];
		actionNode->ToElement()->SetAttribute(TEXT("type"), action.MacroType);
		actionNode->ToElement()->SetAttribute(TEXT("message"), action.message);
		actionNode->ToElement()->SetAttribute(TEXT("wParam"), action.wParameter);
		actionNode->ToElement()->SetAttribute(TEXT("lParam"), action.lParameter);
		actionNode->ToElement()->SetAttribute(TEXT("sParam"), action.sParameter.c_str());
	}
}

void NppParameters::insertUserCmd(TiXmlNode *userCmdRoot, const UserCommand & userCmd)
{
	const KeyCombo & key = userCmd.getKeyCombo();
	TiXmlNode *cmdRoot = userCmdRoot->InsertEndChild(TiXmlElement(TEXT("Command")));
	cmdRoot->ToElement()->SetAttribute(TEXT("name"), userCmd.getMenuName());
	cmdRoot->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	cmdRoot->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	cmdRoot->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	cmdRoot->ToElement()->SetAttribute(TEXT("Key"), key._key);
	cmdRoot->InsertEndChild(TiXmlText(userCmd._cmd.c_str()));
}

void NppParameters::insertPluginCmd(TiXmlNode *pluginCmdRoot, const PluginCmdShortcut & pluginCmd)
{
	const KeyCombo & key = pluginCmd.getKeyCombo();
	TiXmlNode *pluginCmdNode = pluginCmdRoot->InsertEndChild(TiXmlElement(TEXT("PluginCommand")));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("moduleName"), pluginCmd.getModuleName());
	pluginCmdNode->ToElement()->SetAttribute(TEXT("internalID"), pluginCmd.getInternalID());
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Key"), key._key);
}

void NppParameters::insertScintKey(TiXmlNode *scintKeyRoot, const ScintillaKeyMap & scintKeyMap)
{
	TiXmlNode *keyRoot = scintKeyRoot->InsertEndChild(TiXmlElement(TEXT("ScintKey")));
	keyRoot->ToElement()->SetAttribute(TEXT("ScintID"), scintKeyMap.getScintillaKeyID());
	keyRoot->ToElement()->SetAttribute(TEXT("menuCmdID"), scintKeyMap.getMenuCmdID());

	//Add main shortcut
	KeyCombo key = scintKeyMap.getKeyComboByIndex(0);
	keyRoot->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	keyRoot->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	keyRoot->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	keyRoot->ToElement()->SetAttribute(TEXT("Key"), key._key);

	//Add additional shortcuts
	size_t size = scintKeyMap.getSize();
	if (size > 1) {
		TiXmlNode * keyNext;
		for(size_t i = 1; i < size; i++) {
			keyNext = keyRoot->InsertEndChild(TiXmlElement(TEXT("NextKey")));
			key = scintKeyMap.getKeyComboByIndex(i);
			keyNext->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
			keyNext->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
			keyNext->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
			keyNext->ToElement()->SetAttribute(TEXT("Key"), key._key);
		}
	}
}

void NppParameters::writeSession(const Session* session, const TCHAR *fileName)
{
	assert(session);
	const TCHAR *pathName = fileName?fileName:_sessionPath.c_str();

	_pXmlSessionDoc = new TiXmlDocument(pathName);
	TiXmlNode *root = _pXmlSessionDoc->InsertEndChild(TiXmlElement(TEXT("NotepadPlus")));

	if (root)
	{
		TiXmlNode *sessionNode = root->InsertEndChild(TiXmlElement(TEXT("Session")));
		(sessionNode->ToElement())->SetAttribute(TEXT("activeView"), (int)session->_activeView);

		TiXmlNode *mainViewNode = sessionNode->InsertEndChild(TiXmlElement(TEXT("mainView")));
		(mainViewNode->ToElement())->SetAttribute(TEXT("activeIndex"), (int)session->_activeMainIndex);
		for (size_t i = 0 ; i < session->_mainViewFiles.size() ; i++)
		{
			TiXmlNode *fileNameNode = mainViewNode->InsertEndChild(TiXmlElement(TEXT("File")));

			(fileNameNode->ToElement())->SetAttribute(TEXT("firstVisibleLine"), session->_mainViewFiles[i]._firstVisibleLine);
			(fileNameNode->ToElement())->SetAttribute(TEXT("xOffset"), session->_mainViewFiles[i]._xOffset);
			(fileNameNode->ToElement())->SetAttribute(TEXT("scrollWidth"), session->_mainViewFiles[i]._scrollWidth);
			(fileNameNode->ToElement())->SetAttribute(TEXT("startPos"), session->_mainViewFiles[i]._startPos);
			(fileNameNode->ToElement())->SetAttribute(TEXT("endPos"), session->_mainViewFiles[i]._endPos);
			(fileNameNode->ToElement())->SetAttribute(TEXT("selMode"), session->_mainViewFiles[i]._selMode);
			(fileNameNode->ToElement())->SetAttribute(TEXT("lang"), session->_mainViewFiles[i]._langName.c_str());
			(fileNameNode->ToElement())->SetAttribute(TEXT("encoding"), session->_mainViewFiles[i]._encoding);
			(fileNameNode->ToElement())->SetAttribute(TEXT("filename"), session->_mainViewFiles[i]._fileName.c_str());

			for (size_t j = 0 ; j < session->_mainViewFiles[i].marks.size() ; j++)
			{
				size_t markLine = session->_mainViewFiles[i].marks[j];
				TiXmlNode *markNode = fileNameNode->InsertEndChild(TiXmlElement(TEXT("Mark")));
				markNode->ToElement()->SetAttribute(TEXT("line"), markLine);
			}
		}

		TiXmlNode *subViewNode = sessionNode->InsertEndChild(TiXmlElement(TEXT("subView")));
		(subViewNode->ToElement())->SetAttribute(TEXT("activeIndex"), (int)session->_activeSubIndex);
		for (size_t i = 0 ; i < session->_subViewFiles.size() ; i++)
		{
			TiXmlNode *fileNameNode = subViewNode->InsertEndChild(TiXmlElement(TEXT("File")));

			(fileNameNode->ToElement())->SetAttribute(TEXT("firstVisibleLine"), session->_subViewFiles[i]._firstVisibleLine);
			(fileNameNode->ToElement())->SetAttribute(TEXT("xOffset"), session->_subViewFiles[i]._xOffset);
			(fileNameNode->ToElement())->SetAttribute(TEXT("scrollWidth"), session->_subViewFiles[i]._scrollWidth);
			(fileNameNode->ToElement())->SetAttribute(TEXT("startPos"), session->_subViewFiles[i]._startPos);
			(fileNameNode->ToElement())->SetAttribute(TEXT("endPos"), session->_subViewFiles[i]._endPos);
			(fileNameNode->ToElement())->SetAttribute(TEXT("selMode"), session->_subViewFiles[i]._selMode);
			(fileNameNode->ToElement())->SetAttribute(TEXT("lang"), session->_subViewFiles[i]._langName.c_str());
			(fileNameNode->ToElement())->SetAttribute(TEXT("encoding"), session->_subViewFiles[i]._encoding);
			(fileNameNode->ToElement())->SetAttribute(TEXT("filename"), session->_subViewFiles[i]._fileName.c_str());

			for (size_t j = 0 ; j < session->_subViewFiles[i].marks.size() ; j++)
			{
				size_t markLine = session->_subViewFiles[i].marks[j];
				TiXmlNode *markNode = fileNameNode->InsertEndChild(TiXmlElement(TEXT("Mark")));
				markNode->ToElement()->SetAttribute(TEXT("line"), markLine);
			}
		}
	}
	_pXmlSessionDoc->SaveFile();

}

void NppParameters::writeShortcuts()
{
	if (!_pXmlShortcutDoc)
	{
		//do the treatment
		_pXmlShortcutDoc = new TiXmlDocument(_shortcutsPath);
	}

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("NotepadPlus"));
	if (!root)
	{
		root = _pXmlShortcutDoc->InsertEndChild(TiXmlElement(TEXT("NotepadPlus")));
	}

	TiXmlNode *cmdRoot = root->FirstChild(TEXT("InternalCommands"));
	if (cmdRoot)
		root->RemoveChild(cmdRoot);

	cmdRoot = root->InsertEndChild(TiXmlElement(TEXT("InternalCommands")));
	for (size_t i = 0 ; i < _customizedShortcuts.size() ; i++)
	{
		int index = _customizedShortcuts[i];
		CommandShortcut csc = _shortcuts[index];
		insertCmd(cmdRoot, csc);
	}

	TiXmlNode *macrosRoot = root->FirstChild(TEXT("Macros"));
	if (macrosRoot)
		root->RemoveChild(macrosRoot);

	macrosRoot = root->InsertEndChild(TiXmlElement(TEXT("Macros")));

	for (size_t i = 0 ; i < _macros.size() ; i++)
	{
		insertMacro(macrosRoot, _macros[i]);
	}

	TiXmlNode *userCmdRoot = root->FirstChild(TEXT("UserDefinedCommands"));
	if (userCmdRoot)
		root->RemoveChild(userCmdRoot);

	userCmdRoot = root->InsertEndChild(TiXmlElement(TEXT("UserDefinedCommands")));

	for (size_t i = 0 ; i < _userCommands.size() ; i++)
	{
		insertUserCmd(userCmdRoot, _userCommands[i]);
	}

	TiXmlNode *pluginCmdRoot = root->FirstChild(TEXT("PluginCommands"));
	if (pluginCmdRoot)
		root->RemoveChild(pluginCmdRoot);

	pluginCmdRoot = root->InsertEndChild(TiXmlElement(TEXT("PluginCommands")));
	for (size_t i = 0 ; i < _pluginCustomizedCmds.size() ; i++)
	{
		insertPluginCmd(pluginCmdRoot, _pluginCommands[_pluginCustomizedCmds[i]]);
	}

	TiXmlNode *scitillaKeyRoot = root->FirstChild(TEXT("ScintillaKeys"));
	if (scitillaKeyRoot)
		root->RemoveChild(scitillaKeyRoot);

	scitillaKeyRoot = root->InsertEndChild(TiXmlElement(TEXT("ScintillaKeys")));
	for (size_t i = 0 ; i < _scintillaModifiedKeyIndices.size() ; i++)
	{
		insertScintKey(scitillaKeyRoot, _scintillaKeyCommands[_scintillaModifiedKeyIndices[i]]);
	}
	_pXmlShortcutDoc->SaveFile();
}

int NppParameters::addUserLangToEnd(const UserLangContainer & userLang, const TCHAR *newName)
{
	if (isExistingUserLangName(newName))
		return -1;
	UserLangContainer* newLangContainer = new UserLangContainer();
	(*newLangContainer) = userLang;
	newLangContainer->setName(newName);
	_userLangArray.push_back(newLangContainer);

	return _userLangArray.size()-1;
}

void NppParameters::removeUserLang(int index)
{
	// JOCE Can we possibly move index to a size_t?
	if (index >= (int)_userLangArray.size() )
		return;
	delete _userLangArray[index];
	_userLangArray.erase(_userLangArray.begin() + index);
}

int NppParameters::getIndexFromKeywordListName(const TCHAR *name)
{
	if (!name) return -1;
	if (!lstrcmp(name, TEXT("Folder+")))	return 1;
	else if (!lstrcmp(name, TEXT("Folder-")))	return 2;
	else if (!lstrcmp(name, TEXT("Operators")))return 3;
	else if (!lstrcmp(name, TEXT("Comment")))	return 4;
	else if (!lstrcmp(name, TEXT("Words1")))	return 5;
	else if (!lstrcmp(name, TEXT("Words2")))	return 6;
	else if (!lstrcmp(name, TEXT("Words3")))	return 7;
	else if (!lstrcmp(name, TEXT("Words4")))	return 8;
	else if (!lstrcmp(name, TEXT("Delimiters")))	return 0;
	else return -1;
}

bool UserLangContainer::feedUserSettings(TiXmlNode *settingsRoot)
{
	if(!settingsRoot)
	{
		return false;
	}

	const TCHAR *boolStr;
	TiXmlNode *globalSettingNode = settingsRoot->FirstChildElement(TEXT("Global"));
	if (globalSettingNode)
	{
		boolStr = (globalSettingNode->ToElement())->Attribute(TEXT("caseIgnored"));
		if (boolStr)
			setIsCaseIgnored(lstrcmp(TEXT("yes"), boolStr) == 0);
		boolStr = (globalSettingNode->ToElement())->Attribute(TEXT("escapeChar"));
		_escapeChar[0] = (boolStr) ? boolStr[0] : 0;
	}
	TiXmlNode *treatAsSymbolNode = settingsRoot->FirstChildElement(TEXT("TreatAsSymbol"));
	if (treatAsSymbolNode)
	{
		boolStr = (treatAsSymbolNode->ToElement())->Attribute(TEXT("comment"));
		if (boolStr)
			setIsCommentSymbol(!lstrcmp(TEXT("yes"), boolStr));

		boolStr = (treatAsSymbolNode->ToElement())->Attribute(TEXT("commentLine"));
		if (boolStr)
			setIsCommentLineSymbol(!lstrcmp(TEXT("yes"), boolStr));
	}
	TiXmlNode *prefixNode = settingsRoot->FirstChildElement(TEXT("Prefix"));
	if (prefixNode)
	{
		TCHAR names[nbPrefixListAllowed][7] = {TEXT("words1"), TEXT("words2"), TEXT("words3"), TEXT("words4")};
		for (int i = 0 ; i < nbPrefixListAllowed ; i++)
		{
			boolStr = (prefixNode->ToElement())->Attribute(names[i]);
			if (boolStr)
				setIsPrefix(i, !lstrcmp(TEXT("yes"), boolStr));
		}
	}
	return true;
}

bool UserLangContainer::feedUserKeywordList(TiXmlNode *node)
{
	if (!node)
	{
		return false;
	}

	NppParameters* params = NppParameters::getInstance();

	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("Keywords"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Keywords")))
	{
		const TCHAR *keywordsName = (childNode->ToElement())->Attribute(TEXT("name"));
		int i = params->getIndexFromKeywordListName(keywordsName);
		if (i != -1)
		{
			TiXmlNode *valueNode = childNode->FirstChild();
			if (valueNode)
			{
				const TCHAR *kwl = (valueNode)?valueNode->Value():(lstrcmp(keywordsName, TEXT("Delimiters"))?TEXT(""):TEXT("000000"));
				setKeywordList(i, kwl);
			}
		}
	}
	return true;
}

bool UserLangContainer::feedUserStyles(TiXmlNode *node)
{
	if (!node)
	{
		return false;
	}

	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("WordsStyle"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("WordsStyle")))
	{
		int id;
		const TCHAR *styleIDStr = (childNode->ToElement())->Attribute(TEXT("styleID"), &id);
		if (styleIDStr)
		{
			_styleArray.addStyler(id, childNode);
		}
	}
	return true;
}

bool NppParameters::feedStylerArray(TiXmlNode *node)
{
    TiXmlNode *styleRoot = node->FirstChildElement(TEXT("LexerStyles"));
    if (!styleRoot) return false;

    // For each lexer
    for (TiXmlNode *childNode = styleRoot->FirstChildElement(TEXT("LexerType"));
		 childNode ;
		 childNode = childNode->NextSibling(TEXT("LexerType")) )
    {
		if (!_lexerStylerArray.hasEnoughSpace()) return false;

	    TiXmlElement *element = childNode->ToElement();
	    const TCHAR *lexerName = element->Attribute(TEXT("name"));
		const TCHAR *lexerDesc = element->Attribute(TEXT("desc"));
		const TCHAR *lexerUserExt = element->Attribute(TEXT("ext"));
		const TCHAR *lexerExcluded = element->Attribute(TEXT("excluded"));
	    if (lexerName)
	    {
		    _lexerStylerArray.addLexerStyler(lexerName, lexerDesc, lexerUserExt, childNode);
			if (lexerExcluded != NULL && !lstrcmp(lexerExcluded, TEXT("yes")))
			{
				int index = getExternalLangIndexFromName(lexerName);
				if (index != -1)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)(index + L_EXTERNAL)));
			}
	    }
    }

    // The global styles for all lexers
    TiXmlNode *globalStyleRoot = node->FirstChildElement(TEXT("GlobalStyles"));
    if (!globalStyleRoot) return false;

    for (TiXmlNode *childNode = globalStyleRoot->FirstChildElement(TEXT("WidgetStyle"));
		 childNode ;
		 childNode = childNode->NextSibling(TEXT("WidgetStyle")) )
    {
		if (!_widgetStyleArray.hasEnoughSpace()) return false;

	    TiXmlElement *element = childNode->ToElement();
	    const TCHAR *styleIDStr = element->Attribute(TEXT("styleID"));

        int styleID = decStrVal(styleIDStr);
		if (styleID != -1)
		{
		    _widgetStyleArray.addStyler(styleID, childNode);
        }
    }
	return true;
}

void LexerStylerArray::addLexerStyler(const TCHAR *lexerName, const TCHAR *lexerDesc, const TCHAR *lexerUserExt , TiXmlNode *lexerNode)
{
    LexerStyler & ls = _lexerStylerArray[_nbLexerStyler++];
    ls.setLexerName(lexerName);
	if (lexerDesc)
		ls.setLexerDesc(lexerDesc);

	if (lexerUserExt)
		ls.setLexerUserExt(lexerUserExt);

    for (TiXmlNode *childNode = lexerNode->FirstChildElement(TEXT("WordsStyle"));
		 childNode ;
		 childNode = childNode->NextSibling(TEXT("WordsStyle")) )
    {

		if (!ls.hasEnoughSpace()) return;

		TiXmlElement *element = childNode->ToElement();
		const TCHAR *styleIDStr = element->Attribute(TEXT("styleID"));

		if (styleIDStr)
		{
			int styleID = decStrVal(styleIDStr);
			if (styleID != -1)
			{
				ls.addStyler(styleID, childNode);
			}
		}
    }
}

void LexerStylerArray::eraseAll()
{

	for (int i = 0 ; i < _nbLexerStyler ; i++)
	{
		_lexerStylerArray[i].setNbStyler( 0 );
	}

	_nbLexerStyler = 0;
}

bool NppParameters::writeHistory(const TCHAR *fullpath)
{
	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("NotepadPlus"));
	if (!nppRoot) return false;

	TiXmlNode *historyNode = nppRoot->FirstChildElement(TEXT("History"));
	if (!historyNode) return false;

	TiXmlElement recentFileNode(TEXT("File"));
	(recentFileNode.ToElement())->SetAttribute(TEXT("filename"), fullpath);

	(historyNode->ToElement())->InsertEndChild(recentFileNode);
	return true;
}

TiXmlNode * NppParameters::getChildElementByAttribut(TiXmlNode *pere, const TCHAR *childName,\
			const TCHAR *attributName, const TCHAR *attributVal) const
{
	for (TiXmlNode *childNode = pere->FirstChildElement(childName);
		childNode ;
		childNode = childNode->NextSibling(childName))
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *val = element->Attribute(attributName);
		if (val)
		{
			if (!lstrcmp(val, attributVal))
				return childNode;
		}
	}
	return NULL;
}

NppParameters* NppParameters::getInstance()
{
	if(!_pSelf)
	{
		_pSelf = new NppParameters;
	}
	return _pSelf;
}

// 2 restes : L_H, L_USER
LangType NppParameters::getLangIDFromStr(const TCHAR *langName)
{
	int lang = (int)L_TEXT;
	for(; lang < L_EXTERNAL; lang++) {
		const TCHAR * name = ScintillaEditView::langNames[lang].lexerName;
		if (!lstrcmp(name, langName)) {	//found lang?
			return (LangType)lang;
		}
	}

	//Cannot find language, check if its an external one

	LangType l = (LangType)lang;
	if (l == L_EXTERNAL) {	//try find external lexer
		int id = _pSelf->getExternalLangIndexFromName(langName);
		if (id != -1) return (LangType)(id + L_EXTERNAL);
	}

	return L_TEXT;
}

void NppParameters::feedKeyWordsParameters(TiXmlNode *node)
{

	TiXmlNode *langRoot = node->FirstChildElement(TEXT("Languages"));
	if (!langRoot) return;

	for (TiXmlNode *langNode = langRoot->FirstChildElement(TEXT("Language"));
		langNode ;
		langNode = langNode->NextSibling(TEXT("Language")) )
	{
		TiXmlElement *element = langNode->ToElement();
		const TCHAR *name = element->Attribute(TEXT("name"));
		if (name)
		{
			Lang* newLang = new Lang(getLangIDFromStr(name), name);
			//_langList[_nbLang] = new Lang(getLangIDFromStr(name), name);
			newLang->setDefaultExtList(element->Attribute(TEXT("ext")));
			newLang->setCommentLineSymbol(element->Attribute(TEXT("commentLine")));
			newLang->setCommentStart(element->Attribute(TEXT("commentStart")));
			newLang->setCommentEnd(element->Attribute(TEXT("commentEnd")));
            int i;
            if (element->Attribute(TEXT("tabSettings"), &i))
                newLang->setTabInfo(i);

			for (TiXmlNode *kwNode = langNode->FirstChildElement(TEXT("Keywords"));
				kwNode ;
				kwNode = kwNode->NextSibling(TEXT("Keywords")) )
			{
				const TCHAR *indexName = (kwNode->ToElement())->Attribute(TEXT("name"));
				TiXmlNode *kwVal = kwNode->FirstChild();
				const TCHAR *keyWords = TEXT("");
				if ((indexName) && (kwVal))
					keyWords = kwVal->Value();

				int i = getKwClassFromName(indexName);

				if (i >= 0 && i <= KEYWORDSET_MAX)
					newLang->setWords(keyWords, i);
			}
			_langList.push_back(newLang);
		}
	}
}

void NppParameters::feedGUIParameters(TiXmlNode *node)
{
	TiXmlNode *GUIRoot = node->FirstChildElement(TEXT("GUIConfigs"));
	if (!GUIRoot) return;

	for (TiXmlNode *childNode = GUIRoot->FirstChildElement(TEXT("GUIConfig"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("GUIConfig")) )
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));
		if (!nm) continue;

		const TCHAR *val;

		if (!lstrcmp(nm, TEXT("ToolBar")))
		{
			val = element->Attribute(TEXT("visible"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("no")))
					_nppGUI._toolbarShow = false;
				else// if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._toolbarShow = true;
			}
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("small")))
						_nppGUI._toolBarStatus = TB_SMALL;
					else if (!lstrcmp(val, TEXT("large")))
						_nppGUI._toolBarStatus = TB_LARGE;
					else// if (!lstrcmp(val, TEXT("standard")))	//assume standard in all other cases
						_nppGUI._toolBarStatus = TB_STANDARD;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("StatusBar")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("hide")))
						_nppGUI._statusBarShow = false;
					else if (!lstrcmp(val, TEXT("show")))
						_nppGUI._statusBarShow = true;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("MenuBar")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("hide")))
						_nppGUI._menuBarShow = false;
					else if (!lstrcmp(val, TEXT("show")))
						_nppGUI._menuBarShow = true;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("TabBar")))
		{
			bool isFailed = false;
			int oldValue = _nppGUI._tabStatus;
			val = element->Attribute(TEXT("dragAndDrop"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus = TAB_DRAGNDROP;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus = 0;
				else
					isFailed = true;
			}

			val = element->Attribute(TEXT("drawTopBar"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_DRAWTOPBAR;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}

			val = element->Attribute(TEXT("drawInactiveTab"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_DRAWINACTIVETAB;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}

			val = element->Attribute(TEXT("reduce"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_REDUCE;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}

			val = element->Attribute(TEXT("closeButton"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_CLOSEBUTTON;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}

			val = element->Attribute(TEXT("doubleClick2Close"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_DBCLK2CLOSE;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}
			val = element->Attribute(TEXT("vertical"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_VERTICAL;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}

			val = element->Attribute(TEXT("multiLine"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_MULTILINE;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}
			val = element->Attribute(TEXT("hide"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("yes")))
					_nppGUI._tabStatus |= TAB_HIDE;
				else if (!lstrcmp(val, TEXT("no")))
					_nppGUI._tabStatus |= 0;
				else
					isFailed = true;
			}
			if (isFailed)
				_nppGUI._tabStatus = oldValue;


		}
		else if (!lstrcmp(nm, TEXT("Auto-detection")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._fileAutoDetection = cdEnabled;
					else if (!lstrcmp(val, TEXT("auto")))
						_nppGUI._fileAutoDetection = cdAutoUpdate;
					else if (!lstrcmp(val, TEXT("Update2End")))
						_nppGUI._fileAutoDetection = cdGo2end;
					else if (!lstrcmp(val, TEXT("autoUpdate2End")))
						_nppGUI._fileAutoDetection = cdAutoUpdateGo2end;
					else //(!lstrcmp(val, TEXT("no")))
						_nppGUI._fileAutoDetection = cdDisabled;

					_nppGUI._fileAutoDetectionOriginalValue = _nppGUI._fileAutoDetection;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("TrayIcon")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._isMinimizedToTray = true;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("RememberLastSession")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._rememberLastSession = true;
					else
						_nppGUI._rememberLastSession = false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("MaitainIndent")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._maitainIndent = true;
					else
						_nppGUI._maitainIndent = false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("SmartHighLight")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._enableSmartHilite = true;
					else
						_nppGUI._enableSmartHilite = false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("EnableWheelZoom")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._enableMouseWheelZoom = true;
					else
						_nppGUI._enableMouseWheelZoom = false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("TagsMatchHighLight")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					_nppGUI._enableTagsMatchHilite = !lstrcmp(val, TEXT("yes"));
					const TCHAR *tahl = element->Attribute(TEXT("TagAttrHighLight"));
					if (tahl)
						_nppGUI._enableTagAttrsHilite = !lstrcmp(tahl, TEXT("yes"));

					tahl = element->Attribute(TEXT("HighLightNonHtmlZone"));
					if (tahl)
						_nppGUI._enableHiliteNonHTMLZone = !lstrcmp(tahl, TEXT("yes"));
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("TaskList")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					_nppGUI._doTaskList = (!lstrcmp(val, TEXT("yes")))?true:false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("MRU")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					_nppGUI._styleMRU = (!lstrcmp(val, TEXT("yes")))?true:false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("URL")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("1")))
						_nppGUI._styleURL = 1;
					else if (!lstrcmp(val, TEXT("2")))
						_nppGUI._styleURL = 2;
					else
						_nppGUI._styleURL = 0;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("CheckHistoryFiles")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("no")))
						_nppGUI._checkHistoryFiles = false;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("ScintillaViewsSplitter")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("vertical")))
						_nppGUI._splitterPos = POS_VERTICAL;
					else if (!lstrcmp(val, TEXT("horizontal")))
						_nppGUI._splitterPos = POS_HORIZOTAL;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("UserDefineDlg")))
		{
			bool isFailed = false;
			int oldValue = _nppGUI._userDefineDlgStatus;

			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("hide")))
						_nppGUI._userDefineDlgStatus = 0;
					else if (!lstrcmp(val, TEXT("show")))
						_nppGUI._userDefineDlgStatus = UDD_SHOW;
					else
						isFailed = true;
				}
			}
			val = element->Attribute(TEXT("position"));
			if (val)
			{
				if (!lstrcmp(val, TEXT("docked")))
					_nppGUI._userDefineDlgStatus |= UDD_DOCKED;
				else if (!lstrcmp(val, TEXT("undocked")))
					_nppGUI._userDefineDlgStatus |= 0;
				else
					isFailed = true;
			}
			if (isFailed)
				_nppGUI._userDefineDlgStatus = oldValue;
		}
		else if (!lstrcmp(nm, TEXT("TabSetting")))
		{
			int i;
			val = element->Attribute(TEXT("size"), &i);
			if (val)
				_nppGUI._tabSize = i;

			if ((_nppGUI._tabSize == -1) || (_nppGUI._tabSize == 0))
				_nppGUI._tabSize = 8;

			val = element->Attribute(TEXT("replaceBySpace"));
			if (val)
				_nppGUI._tabReplacedBySpace = (!lstrcmp(val, TEXT("yes")));
		}

		else if (!lstrcmp(nm, TEXT("Caret")))
		{
			int i;
			val = element->Attribute(TEXT("width"), &i);
			if (val)
				_nppGUI._caretWidth = i;

			val = element->Attribute(TEXT("blinkRate"), &i);
			if (val)
				_nppGUI._caretBlinkRate = i;
		}

        else if (!lstrcmp(nm, TEXT("ScintillaGlobalSettings")))
		{
			val = element->Attribute(TEXT("enableMultiSelection"));
			if (val && lstrcmp(val, TEXT("yes")) == 0)
				_nppGUI._enableMultiSelection = true;
		}

		else if (!lstrcmp(nm, TEXT("AppPosition")))
		{
			RECT oldRect = _nppGUI._appPos;
			bool fuckUp = true;
			int i;

			if (element->Attribute(TEXT("x"), &i))
			{
				_nppGUI._appPos.left = i;

				if (element->Attribute(TEXT("y"), &i))
				{
					_nppGUI._appPos.top = i;

					if (element->Attribute(TEXT("width"), &i))
					{
						_nppGUI._appPos.right = i;

						if (element->Attribute(TEXT("height"), &i))
						{
							_nppGUI._appPos.bottom = i;
							fuckUp = false;
						}
					}
				}
			}
			if (fuckUp)
				_nppGUI._appPos = oldRect;

			val = element->Attribute(TEXT("isMaximized"));
			if (val)
			{
				_nppGUI._isMaximized = (lstrcmp(val, TEXT("yes")) == 0);
			}
		}
		else if (!lstrcmp(nm, TEXT("NewDocDefaultSettings")))
		{
			int i;
			if (element->Attribute(TEXT("format"), &i))
				_nppGUI._newDocDefaultSettings._format = (formatType)i;

			if (element->Attribute(TEXT("encoding"), &i))
				_nppGUI._newDocDefaultSettings._encoding = (UniMode)i;

			if (element->Attribute(TEXT("lang"), &i))
				_nppGUI._newDocDefaultSettings._lang = (LangType)i;

			val = element->Attribute(TEXT("openAnsiAsUTF8"));
			if (val)
				_nppGUI._newDocDefaultSettings._openAnsiAsUtf8 = (lstrcmp(val, TEXT("yes")) == 0);

		}
		else if (!lstrcmp(nm, TEXT("langsExcluded")))
		{
			int g0 = 0; // up to  8
			int g1 = 0; // up to 16
			int g2 = 0; // up to 24
			int g3 = 0; // up to 32
			int g4 = 0; // up to 40
			int g5 = 0; // up to 48
			int g6 = 0; // up to 56
			int g7 = 0; // up to 64
			const int nbMax = 64;

			int i;
			if (element->Attribute(TEXT("gr0"), &i))
				if (i <= 255)
					g0 = i;
			if (element->Attribute(TEXT("gr1"), &i))
				if (i <= 255)
					g1 = i;
			if (element->Attribute(TEXT("gr2"), &i))
				if (i <= 255)
					g2 = i;
			if (element->Attribute(TEXT("gr3"), &i))
				if (i <= 255)
					g3 = i;
			if (element->Attribute(TEXT("gr4"), &i))
				if (i <= 255)
					g4 = i;
			if (element->Attribute(TEXT("gr5"), &i))
				if (i <= 255)
					g5 = i;
			if (element->Attribute(TEXT("gr6"), &i))
				if (i <= 255)
					g6 = i;
			if (element->Attribute(TEXT("gr7"), &i))
				if (i <= 255)
					g7 = i;

			bool langArray[nbMax];
			for (int i = 0 ; i < nbMax ; i++) langArray[i] = false;

			UCHAR mask = 1;
			for (int i = 0 ; i < 8 ; i++)
			{
				if (mask & g0)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 8 ; i < 16 ; i++)
			{
				if (mask & g1)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 16 ; i < 24 ; i++)
			{
				if (mask & g2)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 24 ; i < 32 ; i++)
			{
				if (mask & g3)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 32 ; i < 40 ; i++)
			{
				if (mask & g4)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 40 ; i < 48 ; i++)
			{
				if (mask & g5)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 48 ; i < 56 ; i++)
			{
				if (mask & g6)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (int i = 56 ; i < 64 ; i++)
			{
				if (mask & g7)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			val = element->Attribute(TEXT("langMenuCompact"));
			if (val)
				_nppGUI._isLangMenuCompact = (!lstrcmp(val, TEXT("yes")));
		}

		else if (!lstrcmp(nm, TEXT("Print")))
		{
			val = element->Attribute(TEXT("lineNumber"));
			if (val)
				_nppGUI._printSettings._printLineNumber = (!lstrcmp(val, TEXT("yes")));

			int i;
			if (element->Attribute(TEXT("printOption"), &i))
				_nppGUI._printSettings._printOption = i;

			val = element->Attribute(TEXT("headerLeft"));
			if (val)
				_nppGUI._printSettings._headerLeft = val;

			val = element->Attribute(TEXT("headerMiddle"));
			if (val)
				_nppGUI._printSettings._headerMiddle = val;

			val = element->Attribute(TEXT("headerRight"));
			if (val)
				_nppGUI._printSettings._headerRight = val;


			val = element->Attribute(TEXT("footerLeft"));
			if (val)
				_nppGUI._printSettings._footerLeft = val;

			val = element->Attribute(TEXT("footerMiddle"));
			if (val)
				_nppGUI._printSettings._footerMiddle = val;

			val = element->Attribute(TEXT("footerRight"));
			if (val)
				_nppGUI._printSettings._footerRight = val;


			val = element->Attribute(TEXT("headerFontName"));
			if (val)
				_nppGUI._printSettings._headerFontName = val;

			val = element->Attribute(TEXT("footerFontName"));
			if (val)
				_nppGUI._printSettings._footerFontName = val;

			if (element->Attribute(TEXT("headerFontStyle"), &i))
				_nppGUI._printSettings._headerFontStyle = i;

			if (element->Attribute(TEXT("footerFontStyle"), &i))
				_nppGUI._printSettings._footerFontStyle = i;

			if (element->Attribute(TEXT("headerFontSize"), &i))
				_nppGUI._printSettings._headerFontSize = i;

			if (element->Attribute(TEXT("footerFontSize"), &i))
				_nppGUI._printSettings._footerFontSize = i;

			// JOCE: I can't rename the "marge" attributes to "margin" just yet. How annoying.
			if (element->Attribute(TEXT("margeLeft"), &i))
				_nppGUI._printSettings._margin.left = i;

			if (element->Attribute(TEXT("margeTop"), &i))
				_nppGUI._printSettings._margin.top = i;

			if (element->Attribute(TEXT("margeRight"), &i))
				_nppGUI._printSettings._margin.right = i;

			if (element->Attribute(TEXT("margeBottom"), &i))
				_nppGUI._printSettings._margin.bottom = i;
		}

		else if (!lstrcmp(nm, TEXT("ScintillaPrimaryView")))
		{
			feedScintillaParam(element);
		}

		else if (!lstrcmp(nm, TEXT("Backup")))
		{
			int i;
			if (element->Attribute(TEXT("action"), &i))
				_nppGUI._backup = (BackupFeature)i;

			const TCHAR *bDir = element->Attribute(TEXT("useCustumDir"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._useDir = true;
			}
			const TCHAR *pDir = element->Attribute(TEXT("dir"));
			if (pDir)
				_nppGUI._backupDir = pDir;
		}
		else if (!lstrcmp(nm, TEXT("DockingManager")))
		{
			feedDockingManager(element);
		}

		else if (!lstrcmp(nm, TEXT("globalOverride")))
		{
			const TCHAR *bDir = element->Attribute(TEXT("fg"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableFg = true;
			}

			bDir = element->Attribute(TEXT("bg"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableBg = true;
			}

			bDir = element->Attribute(TEXT("font"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableFont = true;
			}

			bDir = element->Attribute(TEXT("fontSize"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableFontSize = true;
			}

			bDir = element->Attribute(TEXT("bold"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableBold = true;
			}

			bDir = element->Attribute(TEXT("italic"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableItalic = true;
			}

			bDir = element->Attribute(TEXT("underline"));
			if (bDir && !lstrcmp(bDir, TEXT("yes")))
			{
				_nppGUI._globalOverride.enableUnderLine = true;
			}
		}
		else if (!lstrcmp(nm, TEXT("auto-completion")))
		{
			int i;
			if (element->Attribute(TEXT("autoCAction"), &i))
				_nppGUI._autocStatus = (NppGUI::AutocStatus)i;

			if (element->Attribute(TEXT("triggerFromNbChar"), &i))
				_nppGUI._autocFromLen = i;

			const TCHAR * funcParams = element->Attribute(TEXT("funcParams"));
			if (funcParams && !lstrcmp(funcParams, TEXT("yes")))
			{
				_nppGUI._funcParams = true;
		}
		}
		else if (!lstrcmp(nm, TEXT("sessionExt")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
					_nppGUI._definedSessionExt = val;
			}
		}
		else if (!lstrcmp(nm, TEXT("noUpdate")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
                    _nppGUI._autoUpdateOpt._doAutoUpdate = (!lstrcmp(val, TEXT("yes")))?false:true;
				}

                int i;
                val = element->Attribute(TEXT("intervalDays"), &i);
                if (val)
				{
                    _nppGUI._autoUpdateOpt._intervalDays = i;
				}

                val = element->Attribute(TEXT("nextUpdateDate"));
                if (val)
				{
                    _nppGUI._autoUpdateOpt._nextUpdateDate = Date(val);
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("openSaveDir")))
		{
			const TCHAR * value = element->Attribute(TEXT("value"));
			if (value && value[0])
			{
				if (lstrcmp(value, TEXT("1")) == 0)
					_nppGUI._openSaveDir = dir_last;
				else if (lstrcmp(value, TEXT("2")) == 0)
					_nppGUI._openSaveDir = dir_userDef;
				else
					_nppGUI._openSaveDir = dir_followCurrent;
			}
			const TCHAR * path = element->Attribute(TEXT("defaultDirPath"));
			if (path && path[0])
			{
				lstrcpyn(_nppGUI._defaultDir, path, MAX_PATH);
				::ExpandEnvironmentStrings(_nppGUI._defaultDir, _nppGUI._defaultDirExp, 500);
			}
		}
		else if (!lstrcmp(nm, TEXT("titleBar")))
		{
			const TCHAR * value = element->Attribute(TEXT("short"));
			_nppGUI._shortTitlebar = false;	//default state
			if (value && value[0])
			{
				if (lstrcmp(value, TEXT("yes")) == 0)
					_nppGUI._shortTitlebar = true;
				else if (lstrcmp(value, TEXT("no")) == 0)
					_nppGUI._shortTitlebar = false;
			}
		}
		else if (!lstrcmp(nm, TEXT("stylerTheme")))
		{
			const TCHAR *themePath = element->Attribute(TEXT("path"));
			if (themePath != NULL && themePath[0])
				_nppGUI._themeName.assign(themePath);
		}
	}
}

void NppParameters::feedScintillaParam(TiXmlNode *node)
{
    TiXmlElement *element = node->ToElement();

    // Line Number Margin
    const TCHAR *nm = element->Attribute(TEXT("lineNumberMargin"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp._lineNumberMarginShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._lineNumberMarginShow = false;
	}

	// Bookmark Margin
	nm = element->Attribute(TEXT("bookMarkMargin"));
	if (nm)
	{

		if (!lstrcmp(nm, TEXT("show")))
			_svp._bookMarkMarginShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._bookMarkMarginShow = false;
	}
/*
	// doc change state Margin
	nm = element->Attribute(TEXT("docChangeStateMargin"));
	if (nm)
	{

		if (!lstrcmp(nm, TEXT("show")))
			_svp._docChangeStateMarginShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._docChangeStateMarginShow = false;
	}
*/
    // Indent GuideLine
    nm = element->Attribute(TEXT("indentGuideLine"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp._indentGuideLineShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._indentGuideLineShow= false;
	}

    // Folder Mark Style
    nm = element->Attribute(TEXT("folderMarkStyle"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("box")))
			_svp._folderStyle = FOLDER_STYLE_BOX;
		else if (!lstrcmp(nm, TEXT("circle")))
			_svp._folderStyle = FOLDER_STYLE_CIRCLE;
		else if (!lstrcmp(nm, TEXT("arrow")))
			_svp._folderStyle = FOLDER_STYLE_ARROW;
		else if (!lstrcmp(nm, TEXT("simple")))
			_svp._folderStyle = FOLDER_STYLE_SIMPLE;
		else if (!lstrcmp(nm, TEXT("none")))
			_svp._folderStyle = FOLDER_STYLE_NONE;
	}

	// Line Wrap method
	nm = element->Attribute(TEXT("lineWrapMethod"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("default")))
			_svp._lineWrapMethod = LINEWRAP_DEFAULT;
		else if (!lstrcmp(nm, TEXT("aligned")))
			_svp._lineWrapMethod = LINEWRAP_ALIGNED;
		else if (!lstrcmp(nm, TEXT("indent")))
			_svp._lineWrapMethod = LINEWRAP_INDENT;
	}

    // Current Line Highlighting State
    nm = element->Attribute(TEXT("currentLineHilitingShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp._currentLineHilitingShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._currentLineHilitingShow = false;
	}

    // Current wrap symbol visibility State
    nm = element->Attribute(TEXT("wrapSymbolShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp._wrapSymbolShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._wrapSymbolShow = false;
	}

	// Do Wrap
    nm = element->Attribute(TEXT("Wrap"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("yes")))
			_svp._doWrap = true;
		else if (!lstrcmp(nm, TEXT("no")))
			_svp._doWrap = false;
	}

	// Do Edge
    nm = element->Attribute(TEXT("edge"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("background")))
			_svp._edgeMode = EDGE_BACKGROUND;
		else if (!lstrcmp(nm, TEXT("line")))
			_svp._edgeMode = EDGE_LINE;
		else
			_svp._edgeMode = EDGE_NONE;
	}

	int val;
	nm = element->Attribute(TEXT("edgeNbColumn"), &val);
	if (nm)
	{
		_svp._edgeNbColumn = val;
	}

	nm = element->Attribute(TEXT("zoom"), &val);
	if (nm)
	{
		_svp._zoom = val;
	}

	nm = element->Attribute(TEXT("zoom2"), &val);
	if (nm)
	{
		_svp._zoom2 = val;
	}

	// White Space visibility State
    nm = element->Attribute(TEXT("whiteSpaceShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp._whiteSpaceShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._whiteSpaceShow = false;
	}

	// EOL visibility State
    nm = element->Attribute(TEXT("eolShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp._eolShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp._eolShow = false;
	}
}


void NppParameters::feedDockingManager(TiXmlNode *node)
{
    TiXmlElement *element = node->ToElement();

    int i;
	if (element->Attribute(TEXT("leftWidth"), &i))
		_nppGUI._dockingData._leftWidth = i;

	if (element->Attribute(TEXT("rightWidth"), &i))
		_nppGUI._dockingData._rightWidth = i;

	if (element->Attribute(TEXT("topHeight"), &i))
		_nppGUI._dockingData._topHeight = i;

	if (element->Attribute(TEXT("bottomHeight"), &i))
		_nppGUI._dockingData._bottomHight = i;



	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("FloatingWindow"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("FloatingWindow")) )
	{
		TiXmlElement *floatElement = childNode->ToElement();
		int cont;
		if (floatElement->Attribute(TEXT("cont"), &cont))
		{
			int x = 0;
			int y = 0;
			int w = 100;
			int h = 100;

			floatElement->Attribute(TEXT("x"), &x);
			floatElement->Attribute(TEXT("y"), &y);
			floatElement->Attribute(TEXT("width"), &w);
			floatElement->Attribute(TEXT("height"), &h);
			_nppGUI._dockingData._flaotingWindowInfo.push_back(FloatingWindowInfo(cont, x, y, w, h));
		}
	}

	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("PluginDlg"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("PluginDlg")) )
	{
		TiXmlElement *dlgElement = childNode->ToElement();
		const TCHAR *name = dlgElement->Attribute(TEXT("pluginName"));

		int id;
		const TCHAR *idStr = dlgElement->Attribute(TEXT("id"), &id);
		if (name && idStr)
		{
			int curr = 0; // on left
			int prev = 0; // on left

			dlgElement->Attribute(TEXT("curr"), &curr);
			dlgElement->Attribute(TEXT("prev"), &prev);

			bool isVisible = false;
			const TCHAR *val = dlgElement->Attribute(TEXT("isVisible"));
			if (val)
			{
				isVisible = (lstrcmp(val, TEXT("yes")) == 0);
			}

			_nppGUI._dockingData._pluginDockInfo.push_back(PlugingDlgDockingInfo(name, id, curr, prev, isVisible));
		}
	}

	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("ActiveTabs"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("ActiveTabs")) )
	{
		TiXmlElement *dlgElement = childNode->ToElement();

		int cont;
		if (dlgElement->Attribute(TEXT("cont"), &cont))
		{
			int activeTab = 0;
			dlgElement->Attribute(TEXT("activeTab"), &activeTab);
			_nppGUI._dockingData._containerTabInfo.push_back(ContainerTabInfo(cont, activeTab));
		}
	}
}

bool NppParameters::writeScintillaParams(const ScintillaViewParams & svp)
{
	if (!_pXmlUserDoc) return false;

	const TCHAR *pViewName = TEXT("ScintillaPrimaryView");
	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("NotepadPlus"));
	if (!nppRoot) return false;

	TiXmlNode *configsRoot = nppRoot->FirstChildElement(TEXT("GUIConfigs"));
	if (!configsRoot) return false;

	TiXmlNode *scintNode = getChildElementByAttribut(configsRoot, TEXT("GUIConfig"), TEXT("name"), pViewName);
	if (!scintNode) return false;

	(scintNode->ToElement())->SetAttribute(TEXT("lineNumberMargin"), svp._lineNumberMarginShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("bookMarkMargin"), svp._bookMarkMarginShow?TEXT("show"):TEXT("hide"));
	//(scintNode->ToElement())->SetAttribute(TEXT("docChangeStateMargin"), svp._docChangeStateMarginShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("indentGuideLine"), svp._indentGuideLineShow?TEXT("show"):TEXT("hide"));
	const TCHAR *pFolderStyleStr = (svp._folderStyle == FOLDER_STYLE_SIMPLE)?TEXT("simple"):
									(svp._folderStyle == FOLDER_STYLE_ARROW)?TEXT("arrow"):
										(svp._folderStyle == FOLDER_STYLE_CIRCLE)?TEXT("circle"):
										(svp._folderStyle == FOLDER_STYLE_NONE)?TEXT("none"):TEXT("box");
	(scintNode->ToElement())->SetAttribute(TEXT("folderMarkStyle"), pFolderStyleStr);

	const TCHAR *pWrapMethodStr = (svp._lineWrapMethod == LINEWRAP_ALIGNED)?TEXT("aligned"):
								(svp._lineWrapMethod == LINEWRAP_INDENT)?TEXT("indent"):TEXT("default");
	(scintNode->ToElement())->SetAttribute(TEXT("lineWrapMethod"), pWrapMethodStr);

	(scintNode->ToElement())->SetAttribute(TEXT("currentLineHilitingShow"), svp._currentLineHilitingShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("wrapSymbolShow"), svp._wrapSymbolShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("Wrap"), svp._doWrap?TEXT("yes"):TEXT("no"));
	const TCHAR *edgeStr = NULL;
	if (svp._edgeMode == EDGE_NONE)
		edgeStr = TEXT("no");
	else if (svp._edgeMode == EDGE_LINE)
		edgeStr = TEXT("line");
	else
		edgeStr = TEXT("background");
	(scintNode->ToElement())->SetAttribute(TEXT("edge"), edgeStr);
	(scintNode->ToElement())->SetAttribute(TEXT("edgeNbColumn"), svp._edgeNbColumn);
	(scintNode->ToElement())->SetAttribute(TEXT("zoom"), svp._zoom);
	(scintNode->ToElement())->SetAttribute(TEXT("zoom2"), svp._zoom2);
	(scintNode->ToElement())->SetAttribute(TEXT("whiteSpaceShow"), svp._whiteSpaceShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("eolShow"), svp._eolShow?TEXT("show"):TEXT("hide"));

	return true;
}

bool NppParameters::writeGUIParams()
{
	if (!_pXmlUserDoc) return false;

	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("NotepadPlus"));
	if (!nppRoot) return false;

	TiXmlNode *GUIRoot = nppRoot->FirstChildElement(TEXT("GUIConfigs"));
	if (!GUIRoot) return false;

	bool autoDetectionExist = false;
	bool checkHistoryFilesExist = false;
	bool trayIconExist = false;
	bool rememberLastSessionExist = false;
	bool newDocDefaultSettingsExist = false;
	bool langsExcludedLstExist = false;
	bool printSettingExist = false;
	bool doTaskListExist = false;
	bool maitainIndentExist = false;
	bool MRUExist = false;
	bool backExist = false;
	bool URLExist = false;
	bool globalOverrideExist = false;
	bool autocExist = false;
	bool sessionExtExist = false;
	bool noUpdateExist = false;
	bool menuBarExist = false;
	bool smartHighLightExist = false;
	bool tagsMatchHighLightExist = false;
	bool mouseWheelZoomExist = false;
	bool caretExist = false;
    bool ScintillaGlobalSettingsExist = false;
	bool openSaveDirExist = false;
	bool titleBarExist = false;
	bool stylerThemeExist = false;

	TiXmlNode *dockingParamNode = NULL;

	for (TiXmlNode *childNode = GUIRoot->FirstChildElement(TEXT("GUIConfig"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("GUIConfig")))
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));
		if (!nm) continue;

		if (!lstrcmp(nm, TEXT("ToolBar")))
		{
			const TCHAR *pStr = (_nppGUI._toolbarShow)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("visible"), pStr);

			pStr = _nppGUI._toolBarStatus == TB_SMALL?TEXT("small"):(_nppGUI._toolBarStatus == TB_STANDARD?TEXT("standard"):TEXT("large"));
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("StatusBar")))
		{
			const TCHAR *pStr = _nppGUI._statusBarShow?TEXT("show"):TEXT("hide");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("MenuBar")))
		{
			const TCHAR *pStr = _nppGUI._menuBarShow?TEXT("show"):TEXT("hide");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));

			menuBarExist = true;
		}
		else if (!lstrcmp(nm, TEXT("TabBar")))
		{
			const TCHAR *pStr = (_nppGUI._tabStatus & TAB_DRAWTOPBAR)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("dragAndDrop"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_DRAGNDROP)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("drawTopBar"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_DRAWINACTIVETAB)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("drawInactiveTab"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_REDUCE)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("reduce"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_CLOSEBUTTON)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("closeButton"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_DBCLK2CLOSE)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("doubleClick2Close"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_VERTICAL)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("vertical"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_MULTILINE)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("multiLine"), pStr);

			pStr = (_nppGUI._tabStatus & TAB_HIDE)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("hide"), pStr);

		}
		else if (!lstrcmp(nm, TEXT("ScintillaViewsSplitter")))
		{
			const TCHAR *pStr = _nppGUI._splitterPos == POS_VERTICAL?TEXT("vertical"):TEXT("horizontal");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("UserDefineDlg")))
		{
			const TCHAR *pStr = _nppGUI._userDefineDlgStatus & UDD_SHOW?TEXT("show"):TEXT("hide");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));

			pStr = (_nppGUI._userDefineDlgStatus & UDD_DOCKED)?TEXT("docked"):TEXT("undocked");
			element->SetAttribute(TEXT("position"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("TabSetting")))
		{
			const TCHAR *pStr = _nppGUI._tabReplacedBySpace?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("replaceBySpace"), pStr);
			element->SetAttribute(TEXT("size"), _nppGUI._tabSize);
		}
		else if (!lstrcmp(nm, TEXT("Caret")))
		{
			caretExist = true;
			element->SetAttribute(TEXT("width"), _nppGUI._caretWidth);
			element->SetAttribute(TEXT("blinkRate"), _nppGUI._caretBlinkRate);
		}
        else if (!lstrcmp(nm, TEXT("ScintillaGlobalSettings")))
		{
			ScintillaGlobalSettingsExist = true;
            element->SetAttribute(TEXT("enableMultiSelection"), _nppGUI._enableMultiSelection?TEXT("yes"):TEXT("no"));
		}
		else if (!lstrcmp(nm, TEXT("Auto-detection")))
		{
			autoDetectionExist = true;
			const TCHAR *pStr = TEXT("no");
			switch (_nppGUI._fileAutoDetection)
			{
				case cdEnabled:
					pStr = TEXT("yes");
					break;
				case cdAutoUpdate:
					pStr = TEXT("auto");
					break;
				case cdGo2end:
					pStr = TEXT("Update2End");
					break;
				case cdAutoUpdateGo2end:
					pStr = TEXT("autoUpdate2End");
					break;
			}
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("TrayIcon")))
		{
			trayIconExist = true;
			const TCHAR *pStr = _nppGUI._isMinimizedToTray?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("RememberLastSession")))
		{
			rememberLastSessionExist = true;
			const TCHAR *pStr = _nppGUI._rememberLastSession?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}

		else if (!lstrcmp(nm, TEXT("MaitainIndent")))
		{
			maitainIndentExist = true;
			const TCHAR *pStr = _nppGUI._maitainIndent?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("SmartHighLight")))
		{
			smartHighLightExist = true;
			const TCHAR *pStr = _nppGUI._enableSmartHilite?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("EnableWheelZoom")))
		{
			mouseWheelZoomExist = true;
			const TCHAR *pStr = _nppGUI._enableMouseWheelZoom?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("TagsMatchHighLight")))
		{
			tagsMatchHighLightExist = true;
			const TCHAR *pStr = _nppGUI._enableTagsMatchHilite?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));

			(childNode->ToElement())->SetAttribute(TEXT("TagAttrHighLight"), _nppGUI._enableTagAttrsHilite?TEXT("yes"):TEXT("no"));
			(childNode->ToElement())->SetAttribute(TEXT("HighLightNonHtmlZone"), _nppGUI._enableHiliteNonHTMLZone?TEXT("yes"):TEXT("no"));
		}

		else if (!lstrcmp(nm, TEXT("TaskList")))
		{
			doTaskListExist = true;
			const TCHAR *pStr = _nppGUI._doTaskList?TEXT("yes"):TEXT("no");

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("CheckHistoryFiles")))
		{
			checkHistoryFilesExist = true;
			const TCHAR *pStr = _nppGUI._checkHistoryFiles?TEXT("yes"):TEXT("no");

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("AppPosition")))
		{
			element->SetAttribute(TEXT("x"), _nppGUI._appPos.left);
			element->SetAttribute(TEXT("y"), _nppGUI._appPos.top);
			element->SetAttribute(TEXT("width"), _nppGUI._appPos.right);
			element->SetAttribute(TEXT("height"), _nppGUI._appPos.bottom);
			element->SetAttribute(TEXT("isMaximized"), _nppGUI._isMaximized?TEXT("yes"):TEXT("no"));
		}
		else if (!lstrcmp(nm, TEXT("NewDocDefaultSettings")))
		{
			element->SetAttribute(TEXT("format"), _nppGUI._newDocDefaultSettings._format);
			element->SetAttribute(TEXT("encoding"), _nppGUI._newDocDefaultSettings._encoding);
			element->SetAttribute(TEXT("lang"), _nppGUI._newDocDefaultSettings._lang);
			element->SetAttribute(TEXT("openAnsiAsUTF8"), _nppGUI._newDocDefaultSettings._openAnsiAsUtf8?TEXT("yes"):TEXT("no"));
			newDocDefaultSettingsExist = true;
		}
		else if (!lstrcmp(nm, TEXT("langsExcluded")))
		{
			writeExcludedLangList(element);
			element->SetAttribute(TEXT("langMenuCompact"), _nppGUI._isLangMenuCompact?TEXT("yes"):TEXT("no"));
			langsExcludedLstExist = true;
		}
		else if (!lstrcmp(nm, TEXT("Print")))
		{
			writePrintSetting(element);
			printSettingExist = true;
		}
		else if (!lstrcmp(nm, TEXT("Backup")))
		{
			element->SetAttribute(TEXT("action"), _nppGUI._backup);
			element->SetAttribute(TEXT("useCustumDir"), _nppGUI._useDir?TEXT("yes"):TEXT("no"));
			element->SetAttribute(TEXT("dir"), _nppGUI._backupDir.c_str());
			backExist = true;
		}
		else if (!lstrcmp(nm, TEXT("MRU")))
		{
			MRUExist = true;
			const TCHAR *pStr = _nppGUI._styleMRU?TEXT("yes"):TEXT("no");

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("URL")))
		{
			URLExist = true;
			const TCHAR *pStr = TEXT("0");
			if (_nppGUI._styleURL == 1)
				pStr = TEXT("1");
			else if (_nppGUI._styleURL == 2)
				pStr = TEXT("2");

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("DockingManager")))
		{
			dockingParamNode = childNode;
		}
		else if (!lstrcmp(nm, TEXT("globalOverride")))
		{
			globalOverrideExist = true;
			const TCHAR *pStr = _nppGUI._globalOverride.enableFg?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("fg"), pStr);

			pStr = (_nppGUI._globalOverride.enableBg)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("bg"), pStr);

			pStr = _nppGUI._globalOverride.enableFont?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("font"), pStr);

			pStr = _nppGUI._globalOverride.enableFontSize?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("fontSize"), pStr);

			pStr = _nppGUI._globalOverride.enableBold?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("bold"), pStr);

			pStr = _nppGUI._globalOverride.enableItalic?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("italic"), pStr);

			pStr = _nppGUI._globalOverride.enableUnderLine?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("underline"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("auto-completion")))
		{
			autocExist = true;
			element->SetAttribute(TEXT("autoCAction"), _nppGUI._autocStatus);
			element->SetAttribute(TEXT("triggerFromNbChar"), _nppGUI._autocFromLen);
			const TCHAR * pStr = _nppGUI._funcParams?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("funcParams"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("sessionExt")))
		{
			sessionExtExist = true;
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(_nppGUI._definedSessionExt.c_str());
			else
				childNode->InsertEndChild(TiXmlText(_nppGUI._definedSessionExt.c_str()));
		}
		else if (!lstrcmp(nm, TEXT("noUpdate")))
		{
			noUpdateExist = true;
            const TCHAR *pStr = _nppGUI._autoUpdateOpt._doAutoUpdate?TEXT("no"):TEXT("yes");

            element->SetAttribute(TEXT("intervalDays"), _nppGUI._autoUpdateOpt._intervalDays);
            element->SetAttribute(TEXT("nextUpdateDate"), _nppGUI._autoUpdateOpt._nextUpdateDate.toString().c_str());

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("openSaveDir")))
		{
			openSaveDirExist = true;
			element->SetAttribute(TEXT("value"), _nppGUI._openSaveDir);
			element->SetAttribute(TEXT("defaultDirPath"), _nppGUI._defaultDir);
		}
		else if (!lstrcmp(nm, TEXT("titleBar")))
		{
			titleBarExist = true;
			const TCHAR *pStr = (_nppGUI._shortTitlebar)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("short"), pStr);

			//pStr = (_nppGUI._showDirty)?TEXT("yes"):TEXT("no");
			//element->SetAttribute(TEXT("showDirty"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("stylerTheme")))
		{
			stylerThemeExist = true;
			element->SetAttribute(TEXT("path"), _nppGUI._themeName.c_str());
		}
	}

	if (!noUpdateExist)
	{
        insertGUIConfigBoolNode(GUIRoot, TEXT("noUpdate"), _nppGUI._autoUpdateOpt._doAutoUpdate);
	}

	if (!autoDetectionExist)
	{
		const TCHAR *pStr = TEXT("no");
		switch (_nppGUI._fileAutoDetection)
		{
			case cdEnabled:
				pStr = TEXT("yes");
				break;
			case cdAutoUpdate:
				pStr = TEXT("auto");
				break;
			case cdGo2end:
				pStr = TEXT("Update2End");
				break;
			case cdAutoUpdateGo2end:
				pStr = TEXT("autoUpdate2End");
				break;
		}
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Auto-detection"));
		GUIConfigElement->InsertEndChild(TiXmlText(pStr));
	}
	if (!checkHistoryFilesExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("CheckHistoryFiles"), _nppGUI._checkHistoryFiles);
	}
	if (!trayIconExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("TrayIcon"), _nppGUI._isMinimizedToTray);
	}

	if (!maitainIndentExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("MaitainIndent"), _nppGUI._maitainIndent);
	}

	if (!smartHighLightExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("SmartHighLight"), _nppGUI._enableSmartHilite);
	}
	if (!mouseWheelZoomExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("EnableWheelZoom"), _nppGUI._enableMouseWheelZoom);
	}
	if (!tagsMatchHighLightExist)
	{
		TiXmlElement * ele = insertGUIConfigBoolNode(GUIRoot, TEXT("TagsMatchHighLight"), _nppGUI._enableTagsMatchHilite);
		ele->SetAttribute(TEXT("TagAttrHighLight"), _nppGUI._enableTagAttrsHilite?TEXT("yes"):TEXT("no"));
		ele->SetAttribute(TEXT("HighLightNonHtmlZone"), _nppGUI._enableHiliteNonHTMLZone?TEXT("yes"):TEXT("no"));
	}
	if (!rememberLastSessionExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("RememberLastSession"), _nppGUI._rememberLastSession);
	}

	if (!newDocDefaultSettingsExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("NewDocDefaultSettings"));
		GUIConfigElement->SetAttribute(TEXT("format"), _nppGUI._newDocDefaultSettings._format);
		GUIConfigElement->SetAttribute(TEXT("encoding"), _nppGUI._newDocDefaultSettings._encoding);
		GUIConfigElement->SetAttribute(TEXT("lang"), _nppGUI._newDocDefaultSettings._lang);
		GUIConfigElement->SetAttribute(TEXT("openAnsiAsUTF8"), _nppGUI._newDocDefaultSettings._openAnsiAsUtf8?TEXT("yes"):TEXT("no"));
	}

	if (!langsExcludedLstExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("langsExcluded"));
		writeExcludedLangList(GUIConfigElement);
		GUIConfigElement->SetAttribute(TEXT("langMenuCompact"), _nppGUI._isLangMenuCompact?TEXT("yes"):TEXT("no"));
	}

	if (!printSettingExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Print"));
		writePrintSetting(GUIConfigElement);
	}
	if (!backExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Backup"));
		GUIConfigElement->SetAttribute(TEXT("action"), _nppGUI._backup);
		GUIConfigElement->SetAttribute(TEXT("useCustumDir"), _nppGUI._useDir?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("dir"), _nppGUI._backupDir.c_str());
	}

	if (!doTaskListExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("TaskList"), _nppGUI._doTaskList);
	}

	if (!MRUExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("MRU"), _nppGUI._styleMRU);
	}

	if (!URLExist)
	{
		const TCHAR *pStr = TEXT("0");
		if (_nppGUI._styleURL == 1)
			pStr = TEXT("1");
		else if (_nppGUI._styleURL == 2)
			pStr = TEXT("2");

		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("URL"));
		GUIConfigElement->InsertEndChild(TiXmlText(pStr));
	}

	if (!globalOverrideExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("globalOverride"));
		GUIConfigElement->SetAttribute(TEXT("fg"), _nppGUI._globalOverride.enableFg?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("bg"), _nppGUI._globalOverride.enableBg?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("font"), _nppGUI._globalOverride.enableFont?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("fontSize"), _nppGUI._globalOverride.enableFontSize?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("bold"), _nppGUI._globalOverride.enableBold?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("italic"), _nppGUI._globalOverride.enableItalic?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("underline"), _nppGUI._globalOverride.enableUnderLine?TEXT("yes"):TEXT("no"));
	}

	if (!autocExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("auto-completion"));
		GUIConfigElement->SetAttribute(TEXT("autoCAction"), _nppGUI._autocStatus);
		GUIConfigElement->SetAttribute(TEXT("triggerFromNbChar"), _nppGUI._autocFromLen);
		const TCHAR * pStr = _nppGUI._funcParams?TEXT("yes"):TEXT("no");
		GUIConfigElement->SetAttribute(TEXT("funcParams"), pStr);
	}

	if (dockingParamNode)
	{
		// Rase tout
		GUIRoot->RemoveChild(dockingParamNode);
	}

	if (!sessionExtExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("sessionExt"));
		GUIConfigElement->InsertEndChild(TiXmlText(_nppGUI._definedSessionExt.c_str()));
	}

	if (!menuBarExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("MenuBar"));
		GUIConfigElement->InsertEndChild(TiXmlText(_nppGUI._menuBarShow?TEXT("show"):TEXT("hide")));
	}

	if (!caretExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Caret"));
		GUIConfigElement->SetAttribute(TEXT("width"), _nppGUI._caretWidth);
		GUIConfigElement->SetAttribute(TEXT("blinkRate"), _nppGUI._caretBlinkRate);
	}

    if (!ScintillaGlobalSettingsExist)
    {
        TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("ScintillaGlobalSettings"));
        GUIConfigElement->SetAttribute(TEXT("enableMultiSelection"), _nppGUI._enableMultiSelection?TEXT("yes"):TEXT("no"));
    }

	if (!openSaveDirExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("openSaveDir"));
		GUIConfigElement->SetAttribute(TEXT("value"), _nppGUI._openSaveDir);
		GUIConfigElement->SetAttribute(TEXT("defaultDirPath"), _nppGUI._defaultDir);
	}

	if (!titleBarExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("titleBar"));
		const TCHAR *pStr = (_nppGUI._shortTitlebar)?TEXT("yes"):TEXT("no");
		GUIConfigElement->SetAttribute(TEXT("short"), pStr);
	}
	if (!stylerThemeExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("stylerTheme"));
		GUIConfigElement->SetAttribute(TEXT("path"), _nppGUI._themeName.c_str());
	}

	insertDockingParamNode(GUIRoot);
	return true;
}

bool NppParameters::writeFindHistory()
{
	if (!_pXmlUserDoc) return false;

	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("NotepadPlus"));
	if (!nppRoot) return false;

	TiXmlNode *findHistoryRoot = nppRoot->FirstChildElement(TEXT("FindHistory"));
	if (!findHistoryRoot)
	{
		TiXmlElement element(TEXT("FindHistory"));
		findHistoryRoot = nppRoot->InsertEndChild(element);
	}
	findHistoryRoot->Clear();

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryPath"),    _findHistory._nbMaxFindHistoryPath);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryFilter"),  _findHistory._nbMaxFindHistoryFilter);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryFind"),    _findHistory._nbMaxFindHistoryFind);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryReplace"), _findHistory._nbMaxFindHistoryReplace);

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("matchWord"),				_findHistory._isMatchWord?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("matchCase"),				_findHistory._isMatchCase?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("wrap"),					_findHistory._isWrap?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("directionDown"),			_findHistory._isDirectionDown?TEXT("yes"):TEXT("no"));

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("fifRecuisive"),			_findHistory._isFifRecuisive?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("fifInHiddenFolder"),		_findHistory._isFifInHiddenFolder?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("dlgAlwaysVisible"),		_findHistory._isDlgAlwaysVisible?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("fifFilterFollowsDoc"),	_findHistory._isFilterFollowDoc?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("fifFolderFollowsDoc"),	_findHistory._isFolderFollowDoc?TEXT("yes"):TEXT("no"));

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("searchMode"), _findHistory._searchMode);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("transparencyMode"), _findHistory._transparencyMode);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("transparency"), _findHistory._transparency);

	TiXmlElement hist_element(TEXT(""));

	hist_element.SetValue(TEXT("Path"));
    for (size_t i = 0; i < _findHistory._findHistoryPaths.size(); i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._findHistoryPaths[i].c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	hist_element.SetValue(TEXT("Filter"));
	for (size_t i = 0; i < _findHistory._findHistoryFilters.size(); i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._findHistoryFilters[i].c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	hist_element.SetValue(TEXT("Find"));
	for (size_t i = 0; i < _findHistory._findHistoryFinds.size(); i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._findHistoryFinds[i].c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	hist_element.SetValue(TEXT("Replace"));
	for (size_t i = 0; i < _findHistory._findHistoryReplaces.size(); i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._findHistoryReplaces[i].c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	return true;
}

void NppParameters::insertDockingParamNode(TiXmlNode *GUIRoot)
{
	TiXmlElement DMNode(TEXT("GUIConfig"));
	DMNode.SetAttribute(TEXT("name"), TEXT("DockingManager"));
	DMNode.SetAttribute(TEXT("leftWidth"), _nppGUI._dockingData._leftWidth);
	DMNode.SetAttribute(TEXT("rightWidth"), _nppGUI._dockingData._rightWidth);
	DMNode.SetAttribute(TEXT("topHeight"), _nppGUI._dockingData._topHeight);
	DMNode.SetAttribute(TEXT("bottomHeight"), _nppGUI._dockingData._bottomHight);

	for (size_t i = 0 ; i < _nppGUI._dockingData._flaotingWindowInfo.size() ; i++)
	{
		FloatingWindowInfo & fwi = _nppGUI._dockingData._flaotingWindowInfo[i];
		TiXmlElement FWNode(TEXT("FloatingWindow"));
		FWNode.SetAttribute(TEXT("cont"), fwi._cont);
		FWNode.SetAttribute(TEXT("x"), fwi._pos.left);
		FWNode.SetAttribute(TEXT("y"), fwi._pos.top);
		FWNode.SetAttribute(TEXT("width"), fwi._pos.right);
		FWNode.SetAttribute(TEXT("height"), fwi._pos.bottom);

		DMNode.InsertEndChild(FWNode);
	}

	for (size_t i = 0 ; i < _nppGUI._dockingData._pluginDockInfo.size() ; i++)
	{
		PlugingDlgDockingInfo & pdi = _nppGUI._dockingData._pluginDockInfo[i];
		TiXmlElement PDNode(TEXT("PluginDlg"));
		PDNode.SetAttribute(TEXT("pluginName"), pdi._name);
		PDNode.SetAttribute(TEXT("id"), pdi._internalID);
		PDNode.SetAttribute(TEXT("curr"), pdi._currContainer);
		PDNode.SetAttribute(TEXT("prev"), pdi._prevContainer);
		PDNode.SetAttribute(TEXT("isVisible"), pdi._isVisible?TEXT("yes"):TEXT("no"));

		DMNode.InsertEndChild(PDNode);
	}

	for (size_t i = 0 ; i < _nppGUI._dockingData._containerTabInfo.size() ; i++)
	{
		ContainerTabInfo & cti = _nppGUI._dockingData._containerTabInfo[i];
		TiXmlElement CTNode(TEXT("ActiveTabs"));
		CTNode.SetAttribute(TEXT("cont"), cti._cont);
		CTNode.SetAttribute(TEXT("activeTab"), cti._activeTab);
		DMNode.InsertEndChild(CTNode);
	}

	GUIRoot->InsertEndChild(DMNode);
}

void NppParameters::writePrintSetting(TiXmlElement *element)
{
	const TCHAR *pStr = _nppGUI._printSettings._printLineNumber?TEXT("yes"):TEXT("no");
	element->SetAttribute(TEXT("lineNumber"), pStr);

	element->SetAttribute(TEXT("printOption"), _nppGUI._printSettings._printOption);

	element->SetAttribute(TEXT("headerLeft"), _nppGUI._printSettings._headerLeft.c_str());
	element->SetAttribute(TEXT("headerMiddle"), _nppGUI._printSettings._headerMiddle.c_str());
	element->SetAttribute(TEXT("headerRight"), _nppGUI._printSettings._headerRight.c_str());
	element->SetAttribute(TEXT("footerLeft"), _nppGUI._printSettings._footerLeft.c_str());
	element->SetAttribute(TEXT("footerMiddle"), _nppGUI._printSettings._footerMiddle.c_str());
	element->SetAttribute(TEXT("footerRight"), _nppGUI._printSettings._footerRight.c_str());

	element->SetAttribute(TEXT("headerFontName"), _nppGUI._printSettings._headerFontName.c_str());
	element->SetAttribute(TEXT("headerFontStyle"), _nppGUI._printSettings._headerFontStyle);
	element->SetAttribute(TEXT("headerFontSize"), _nppGUI._printSettings._headerFontSize);
	element->SetAttribute(TEXT("footerFontName"), _nppGUI._printSettings._footerFontName.c_str());
	element->SetAttribute(TEXT("footerFontStyle"), _nppGUI._printSettings._footerFontStyle);
	element->SetAttribute(TEXT("footerFontSize"), _nppGUI._printSettings._footerFontSize);

	// JOCE: I can't rename the "marge" attributes to "margin" just yet. How annoying.
	element->SetAttribute(TEXT("margeLeft"), _nppGUI._printSettings._margin.left);
	element->SetAttribute(TEXT("margeRight"), _nppGUI._printSettings._margin.right);
	element->SetAttribute(TEXT("margeTop"), _nppGUI._printSettings._margin.top);
	element->SetAttribute(TEXT("margeBottom"), _nppGUI._printSettings._margin.bottom);
}

void NppParameters::writeExcludedLangList(TiXmlElement *element)
{
	int g0 = 0; // up to  8
	int g1 = 0; // up to 16
	int g2 = 0; // up to 24
	int g3 = 0; // up to 32
	int g4 = 0; // up to 40
	int g5 = 0; // up to 48
	int g6 = 0; // up to 56
	int g7 = 0; // up to 64

	for (size_t i = 0 ; i < _nppGUI._excludedLangList.size() ; i++)
	{
		LangType langType = _nppGUI._excludedLangList[i]._langType;
		if (langType >= L_EXTERNAL && langType < L_END)
			continue;

		int nGrp = langType / 8;
		int nMask = 1 << langType % 8;


		switch (nGrp)
		{
			// JOCE Plain numerical values?? makes sense?
			case 0 :
				g0 |= nMask;
				break;
			case 1 :
				g1 |= nMask;
				break;
			case 2 :
				g2 |= nMask;
				break;
			case 3 :
				g3 |= nMask;
				break;
			case 4 :
				g4 |= nMask;
				break;
			case 5 :
				g5 |= nMask;
				break;
			case 6 :
				g6 |= nMask;
				break;
			case 7 :
				g7 |= nMask;
				break;

			NO_DEFAULT_CASE;
		}
	}

	element->SetAttribute(TEXT("gr0"), g0);
	element->SetAttribute(TEXT("gr1"), g1);
	element->SetAttribute(TEXT("gr2"), g2);
	element->SetAttribute(TEXT("gr3"), g3);
	element->SetAttribute(TEXT("gr4"), g4);
	element->SetAttribute(TEXT("gr5"), g5);
	element->SetAttribute(TEXT("gr6"), g6);
	element->SetAttribute(TEXT("gr7"), g7);
}

TiXmlElement * NppParameters::insertGUIConfigBoolNode(TiXmlNode *r2w, const TCHAR *name, bool bVal)
{
	const TCHAR *pStr = bVal?TEXT("yes"):TEXT("no");
	TiXmlElement *GUIConfigElement = (r2w->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
	GUIConfigElement->SetAttribute(TEXT("name"), name);
	GUIConfigElement->InsertEndChild(TiXmlText(pStr));
	return GUIConfigElement;
}

int RGB2int(COLORREF color) {
    return (((((DWORD)color) & 0x0000FF) << 16) | ((((DWORD)color) & 0x00FF00)) | ((((DWORD)color) & 0xFF0000) >> 16));
}

int NppParameters::langTypeToCommandID(LangType lt) const
{
	int id;
	switch (lt)
	{
		case L_C :
			id = IDM_LANG_C; break;
		case L_CPP :
			id = IDM_LANG_CPP; break;
		case L_JAVA :
			id = IDM_LANG_JAVA;	break;
		case L_CS :
			id = IDM_LANG_CS; break;
		case L_OBJC :
			id = IDM_LANG_OBJC;	break;
		case L_HTML :
			id = IDM_LANG_HTML;	break;
		case L_XML :
			id = IDM_LANG_XML; break;
		case L_JS :
			id = IDM_LANG_JS; break;
		case L_PHP :
			id = IDM_LANG_PHP; break;
		case L_ASP :
			id = IDM_LANG_ASP; break;
        case L_JSP :
			id = IDM_LANG_JSP; break;
		case L_CSS :
			id = IDM_LANG_CSS; break;
		case L_LUA :
			id = IDM_LANG_LUA; break;
		case L_PERL :
			id = IDM_LANG_PERL; break;
		case L_PYTHON :
			id = IDM_LANG_PYTHON; break;
		case L_BATCH :
			id = IDM_LANG_BATCH; break;
		case L_PASCAL :
			id = IDM_LANG_PASCAL; break;
		case L_MAKEFILE :
			id = IDM_LANG_MAKEFILE;	break;
		case L_INI :
			id = IDM_LANG_INI; break;
		case L_ASCII :
			id = IDM_LANG_ASCII; break;
		case L_RC :
			id = IDM_LANG_RC; break;
		case L_TEX :
			id = IDM_LANG_TEX; break;
		case L_FORTRAN :
			id = IDM_LANG_FORTRAN; break;
		case L_BASH :
			id = IDM_LANG_BASH; break;
		case L_FLASH :
			id = IDM_LANG_FLASH; break;
		case L_NSIS :
			id = IDM_LANG_NSIS; break;
		case L_USER :
			id = IDM_LANG_USER; break;
        case L_SQL :
            id = IDM_LANG_SQL; break;
        case L_VB :
            id = IDM_LANG_VB; break;
		case L_TCL :
            id = IDM_LANG_TCL; break;

		case L_LISP :
            id = IDM_LANG_LISP; break;
		case L_SCHEME :
            id = IDM_LANG_SCHEME; break;
		case L_ASM :
            id = IDM_LANG_ASM; break;
		case L_DIFF :
            id = IDM_LANG_DIFF; break;
		case L_PROPS :
            id = IDM_LANG_PROPS; break;
		case L_PS :
            id = IDM_LANG_PS; break;
		case L_RUBY :
            id = IDM_LANG_RUBY; break;
		case L_SMALLTALK :
            id = IDM_LANG_SMALLTALK; break;
		case L_VHDL :
            id = IDM_LANG_VHDL; break;

		case L_ADA :
            id = IDM_LANG_ADA; break;
		case L_MATLAB :
            id = IDM_LANG_MATLAB; break;

		case L_HASKELL :
            id = IDM_LANG_HASKELL; break;

		case L_KIX :
            id = IDM_LANG_KIX; break;
		case L_AU3 :
            id = IDM_LANG_AU3; break;
		case L_VERILOG :
            id = IDM_LANG_VERILOG; break;
		case L_CAML :
            id = IDM_LANG_CAML; break;

		case L_INNO :
            id = IDM_LANG_INNO; break;

		case L_CMAKE :
            id = IDM_LANG_CMAKE; break;

		case L_YAML :
            id = IDM_LANG_YAML; break;

		case L_COBOL :
            id = IDM_LANG_COBOL; break;

		case L_D :
            id = IDM_LANG_D; break;

		case L_GUI4CLI :
            id = IDM_LANG_GUI4CLI; break;

		case L_POWERSHELL :
            id = IDM_LANG_POWERSHELL; break;

		case L_R :
            id = IDM_LANG_R; break;

		case L_REBOL :
            id = IDM_LANG_REBOL; break;
		
		case L_RSP :
            id = IDM_LANG_RSP; break;
		
		case L_SEARCHRESULT :
			id = -1;	break;

		case L_TEXT :
			id = IDM_LANG_TEXT;	break;

		default :
			if(lt >= L_EXTERNAL && lt < L_END)
				id = lt - L_EXTERNAL + IDM_LANG_EXTERNAL;
			else
				id = IDM_LANG_TEXT;
	}
	return id;
}

void NppParameters::writeStyles(LexerStylerArray & lexersStylers, StyleArray & globalStylers)
{
	TiXmlNode *lexersRoot = (_pXmlUserStylerDoc->FirstChild(TEXT("NotepadPlus")))->FirstChildElement(TEXT("LexerStyles"));
	for (TiXmlNode *childNode = lexersRoot->FirstChildElement(TEXT("LexerType"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("LexerType")))
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));

		LexerStyler *pLs = _lexerStylerArray.getLexerStylerByName(nm);
        LexerStyler *pLs2 = lexersStylers.getLexerStylerByName(nm);

		if (pLs)
		{
			const TCHAR *extStr = pLs->getLexerUserExt();
			element->SetAttribute(TEXT("ext"), extStr);
			for (TiXmlNode *grChildNode = childNode->FirstChildElement(TEXT("WordsStyle"));
					grChildNode ;
					grChildNode = grChildNode->NextSibling(TEXT("WordsStyle")))
			{
				TiXmlElement *grElement = grChildNode->ToElement();
                const TCHAR *styleName = grElement->Attribute(TEXT("name"));

                int i = pLs->getStylerIndexByName(styleName);
                if (i != -1)
				{
                    Style & style = pLs->getStyler(i);
                    Style & style2Sync = pLs2->getStyler(i);

                    writeStyle2Element(style, style2Sync, grElement);
                }
			}
		}
	}

	for(size_t x = 0; x < _pXmlExternalLexerDoc.size(); x++)
	{
		TiXmlNode *lexersRoot = ( _pXmlExternalLexerDoc[x]->FirstChild(TEXT("NotepadPlus")))->FirstChildElement(TEXT("LexerStyles"));
		for (TiXmlNode *childNode = lexersRoot->FirstChildElement(TEXT("LexerType"));
			childNode ;
			childNode = childNode->NextSibling(TEXT("LexerType")))
		{
			TiXmlElement *element = childNode->ToElement();
			const TCHAR *nm = element->Attribute(TEXT("name"));

			LexerStyler *pLs = _lexerStylerArray.getLexerStylerByName(nm);
			LexerStyler *pLs2 = lexersStylers.getLexerStylerByName(nm);

			if (pLs)
			{
				const TCHAR *extStr = pLs->getLexerUserExt();
				element->SetAttribute(TEXT("ext"), extStr);

				for (TiXmlNode *grChildNode = childNode->FirstChildElement(TEXT("WordsStyle"));
						grChildNode ;
						grChildNode = grChildNode->NextSibling(TEXT("WordsStyle")))
				{
					TiXmlElement *grElement = grChildNode->ToElement();
					const TCHAR *styleName = grElement->Attribute(TEXT("name"));

					int i = pLs->getStylerIndexByName(styleName);
					if (i != -1)
					{
						Style & style = pLs->getStyler(i);
						Style & style2Sync = pLs2->getStyler(i);

						writeStyle2Element(style, style2Sync, grElement);
					}
				}
			}
		}
		_pXmlExternalLexerDoc[x]->SaveFile();
	}

	TiXmlNode *globalStylesRoot = (_pXmlUserStylerDoc->FirstChild(TEXT("NotepadPlus")))->FirstChildElement(TEXT("GlobalStyles"));

    for (TiXmlNode *childNode = globalStylesRoot->FirstChildElement(TEXT("WidgetStyle"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("WidgetStyle")))
    {
        TiXmlElement *pElement = childNode->ToElement();
        const TCHAR *styleName = pElement->Attribute(TEXT("name"));
        int i = _widgetStyleArray.getStylerIndexByName(styleName);

        if (i != -1)
		{
            Style & style = _widgetStyleArray.getStyler(i);
            Style & style2Sync = globalStylers.getStyler(i);

            writeStyle2Element(style, style2Sync, pElement);
        }
    }

	_pXmlUserStylerDoc->SaveFile();
}


bool NppParameters::insertTabInfo(const TCHAR *langName, int tabInfo)
{
    if (!_pXmlDoc) return false;
    TiXmlNode *langRoot = (_pXmlDoc->FirstChild(TEXT("NotepadPlus")))->FirstChildElement(TEXT("Languages"));
    for (TiXmlNode *childNode = langRoot->FirstChildElement(TEXT("Language"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Language")))
	{
        TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));
        if (nm && lstrcmp(langName, nm) == 0)
        {
            childNode->ToElement()->SetAttribute(TEXT("tabSettings"), tabInfo);
            _pXmlDoc->makeDirty();
            return true;
        }
    }
    return false;
}

void NppParameters::writeStyle2Element(Style & style2Wite, Style & style2Sync, TiXmlElement *element)
{
    if (HIBYTE(HIWORD(style2Wite._fgColor)) != 0xFF)
    {
        int rgbVal = RGB2int(style2Wite._fgColor);
	    TCHAR fgStr[7];
	    wsprintf(fgStr, TEXT("%.6X"), rgbVal);
	    element->SetAttribute(TEXT("fgColor"), fgStr);
    }

    if (HIBYTE(HIWORD(style2Wite._bgColor)) != 0xFF)
    {
        int rgbVal = RGB2int(style2Wite._bgColor);
	    TCHAR bgStr[7];
	    wsprintf(bgStr, TEXT("%.6X"), rgbVal);
	    element->SetAttribute(TEXT("bgColor"), bgStr);
    }

	if (style2Wite._colorStyle != COLORSTYLE_ALL)
    {
	    element->SetAttribute(TEXT("colorStyle"), style2Wite._colorStyle);
    }

    if (!style2Wite._fontName.empty())
    {
        const TCHAR *oldFontName = element->Attribute(TEXT("fontName"));
        if (style2Wite._fontName != oldFontName)
        {
		    element->SetAttribute(TEXT("fontName"), style2Wite._fontName);
            style2Sync._fontName = style2Wite._fontName = element->Attribute(TEXT("fontName"));
        }
    }

    if (style2Wite._fontSize != -1)
    {
        if (!style2Wite._fontSize)
            element->SetAttribute(TEXT("fontSize"), TEXT(""));
        else
		    element->SetAttribute(TEXT("fontSize"), style2Wite._fontSize);
    }

    if (style2Wite._fontStyle != -1)
    {
	    element->SetAttribute(TEXT("fontStyle"), style2Wite._fontStyle);
    }


	if (style2Wite._keywords)
    {
		TiXmlNode *teteDeNoeud = element->LastChild();

		if (teteDeNoeud)
			teteDeNoeud->SetValue(style2Wite._keywords->c_str());
		else
			element->InsertEndChild(TiXmlText(style2Wite._keywords->c_str()));
    }
}

void NppParameters::insertUserLang2Tree(TiXmlNode *node, UserLangContainer *userLang)
{
	TiXmlElement *rootElement = (node->InsertEndChild(TiXmlElement(TEXT("UserLang"))))->ToElement();

	rootElement->SetAttribute(TEXT("name"), userLang->getName());
	rootElement->SetAttribute(TEXT("ext"), userLang->getExtension());
	TiXmlElement *settingsElement = (rootElement->InsertEndChild(TiXmlElement(TEXT("Settings"))))->ToElement();
	{
		TiXmlElement *globalElement = (settingsElement->InsertEndChild(TiXmlElement(TEXT("Global"))))->ToElement();
		globalElement->SetAttribute(TEXT("caseIgnored"), userLang->isCaseIgnored()?TEXT("yes"):TEXT("no"));
		if (userLang->_escapeChar[0])
			globalElement->SetAttribute(TEXT("escapeChar"), userLang->_escapeChar);

		TiXmlElement *treatAsSymbolElement = (settingsElement->InsertEndChild(TiXmlElement(TEXT("TreatAsSymbol"))))->ToElement();
		treatAsSymbolElement->SetAttribute(TEXT("comment"), userLang->isCommentSymbol()?TEXT("yes"):TEXT("no"));
		treatAsSymbolElement->SetAttribute(TEXT("commentLine"), userLang->isCommentLineSymbol()?TEXT("yes"):TEXT("no"));

		TiXmlElement *prefixElement = (settingsElement->InsertEndChild(TiXmlElement(TEXT("Prefix"))))->ToElement();
		TCHAR names[nbPrefixListAllowed][7] = {TEXT("words1"), TEXT("words2"), TEXT("words3"), TEXT("words4")};
		for (int i = 0 ; i < nbPrefixListAllowed ; i++)
			prefixElement->SetAttribute(names[i], userLang->isPrefix(i)?TEXT("yes"):TEXT("no"));
	}

	TiXmlElement *kwlElement = (rootElement->InsertEndChild(TiXmlElement(TEXT("KeywordLists"))))->ToElement();

	const int nbKWL = 9;
	TCHAR kwn[nbKWL][16] = {TEXT("Delimiters"), TEXT("Folder+"), TEXT("Folder-"), TEXT("Operators"), TEXT("Comment"), TEXT("Words1"), TEXT("Words2"), TEXT("Words3"), TEXT("Words4")};
	for (int i = 0 ; i < nbKWL ; i++)
	{
		TiXmlElement *kwElement = (kwlElement->InsertEndChild(TiXmlElement(TEXT("Keywords"))))->ToElement();
		kwElement->SetAttribute(TEXT("name"), kwn[i]);
		kwElement->InsertEndChild(TiXmlText(userLang->getKeywordList(i)));
	}

	TiXmlElement *styleRootElement = (rootElement->InsertEndChild(TiXmlElement(TEXT("Styles"))))->ToElement();

	for (int i = 0 ; i < userLang->_styleArray.getNbStyler() ; i++)
	{
		TiXmlElement *styleElement = (styleRootElement->InsertEndChild(TiXmlElement(TEXT("WordsStyle"))))->ToElement();
		Style style2Wite = userLang->_styleArray.getStyler(i);

		styleElement->SetAttribute(TEXT("name"), style2Wite._styleDesc);

		styleElement->SetAttribute(TEXT("styleID"), style2Wite._styleID);

		//if (HIBYTE(HIWORD(style2Wite._fgColor)) != 0xFF)
		{
			int rgbVal = RGB2int(style2Wite._fgColor);
			TCHAR fgStr[7];
			wsprintf(fgStr, TEXT("%.6X"), rgbVal);
			styleElement->SetAttribute(TEXT("fgColor"), fgStr);
		}

		//if (HIBYTE(HIWORD(style2Wite._bgColor)) != 0xFF)
		{
			int rgbVal = RGB2int(style2Wite._bgColor);
			TCHAR bgStr[7];
			wsprintf(bgStr, TEXT("%.6X"), rgbVal);
			styleElement->SetAttribute(TEXT("bgColor"), bgStr);
		}

		if (style2Wite._colorStyle != COLORSTYLE_ALL)
		{
			styleElement->SetAttribute(TEXT("colorStyle"), style2Wite._colorStyle);
		}

		if (!style2Wite._fontName.empty())
		{
			styleElement->SetAttribute(TEXT("fontName"), style2Wite._fontName);
		}

		if (style2Wite._fontStyle == -1)
		{
			styleElement->SetAttribute(TEXT("fontStyle"), TEXT("0"));
		}
		else
		{
			styleElement->SetAttribute(TEXT("fontStyle"), style2Wite._fontStyle);
		}

		if (style2Wite._fontSize != -1)
		{
			if (!style2Wite._fontSize)
				styleElement->SetAttribute(TEXT("fontSize"), TEXT(""));
			else
				styleElement->SetAttribute(TEXT("fontSize"), style2Wite._fontSize);
		}
	}
}

void NppParameters::addUserModifiedIndex(int index)
{
	size_t len = _customizedShortcuts.size();
	bool found = false;
	for(size_t i = 0; i < len; i++)
	{
		if (_customizedShortcuts[i] == index)
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		_customizedShortcuts.push_back(index);
	}
}

void NppParameters::addPluginModifiedIndex(int index)
{
	size_t len = _pluginCustomizedCmds.size();
	bool found = false;
	for(size_t i = 0; i < len; i++)
	{
		if (_pluginCustomizedCmds[i] == index)
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		_pluginCustomizedCmds.push_back(index);
	}
}

void NppParameters::addScintillaModifiedIndex(int index)
{
	size_t len = _scintillaModifiedKeyIndices.size();
	bool found = false;
	for(size_t i = 0; i < len; i++)
	{
		if (_scintillaModifiedKeyIndices[i] == index)
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		_scintillaModifiedKeyIndices.push_back(index);
	}
}

bool NppParameters::writeNbHistoryFile( int nb )
{
	if (!_pXmlUserDoc) return false;

	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("NotepadPlus"));
	if (!nppRoot) return false;

	TiXmlNode *historyNode = nppRoot->FirstChildElement(TEXT("History"));
	if (!historyNode) return false;

	(historyNode->ToElement())->SetAttribute(TEXT("nbMaxFile"), nb);
	return true;
}

const char * NppParameters::getNativeLangMenuStringA( int itemID )
{
	if (!_pXmlNativeLangDocA)
		return NULL;

	TiXmlNodeA * node =  _pXmlNativeLangDocA->FirstChild("NotepadPlus");
	if (!node) return NULL;

	node = node->FirstChild("Native-Langue");
	if (!node) return NULL;

	node = node->FirstChild("Menu");
	if (!node) return NULL;

	node = node->FirstChild("Main");
	if (!node) return NULL;

	node = node->FirstChild("Commands");
	if (!node) return NULL;

	for (TiXmlNodeA *childNode = node->FirstChildElement("Item");
		childNode ;
		childNode = childNode->NextSibling("Item") )
	{
		TiXmlElementA *element = childNode->ToElement();
		int id;
		if (element->Attribute("id", &id) && (id == itemID))
		{
			return element->Attribute("name");
		}
	}
	return NULL;
}

UserLangContainer::UserLangContainer():
	_name(TEXT("new user define")),
	_ext(TEXT("")),
	_isCaseIgnored(false),
	_isCommentLineSymbol(false),
	_isCommentSymbol(false)
{
	for (int i = 0; i < nbPrefixListAllowed; i++)
	{
		_isPrefix[i] = false;
	}

	_escapeChar[0] = TEXT('\0');
	_escapeChar[1] = TEXT('\0');
}

UserLangContainer::UserLangContainer( const TCHAR *name, const TCHAR *ext ) :
	_name(name),
	_ext(ext),
	_isCaseIgnored(false),
	_isCommentLineSymbol(false),
	_isCommentSymbol(false)
{
	for (int i = 0; i < nbPrefixListAllowed; i++)
	{
		_isPrefix[i] = false;
	}

	_escapeChar[0] = TEXT('\0');
	_escapeChar[1] = TEXT('\0');
}

const UserLangContainer & UserLangContainer::operator=( const UserLangContainer & ulc )
{
	if (this != &ulc)
	{
		_name = ulc._name;
		_ext = ulc._ext;
		_escapeChar[0] = ulc._escapeChar[0];
		_escapeChar[1] = '\0';
		_isCaseIgnored = ulc._isCaseIgnored;
		_styleArray = ulc._styleArray;
		int nbStyler = _styleArray.getNbStyler();
		for (int i = 0 ; i < nbStyler ; i++)
		{
			Style & st = _styleArray.getStyler(i);
			if (st._bgColor == COLORREF(-1))
			{
				st._bgColor = white;
			}
			if (st._fgColor == COLORREF(-1))
			{
				st._fgColor = black;
			}
		}
		for (int i = 0 ; i < getNbKeywordList() ; i++)
		{
			_keywordLists[i] = ulc._keywordLists[i];
		}

		_isCaseIgnored = ulc._isCaseIgnored;
		_isCommentLineSymbol = ulc._isCommentLineSymbol;
		_isCommentSymbol = ulc._isCommentSymbol;
		for (int i = 0; i < nbPrefixListAllowed; i++)
		{
			_isPrefix[i] = ulc._isPrefix[i];
		}
	}
	return *this;
}
