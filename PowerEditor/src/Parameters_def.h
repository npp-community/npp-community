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

#ifndef PARAMETERS_DEF_H
#define PARAMETERS_DEF_H


#define POS_HORIZOTAL false
#define POS_VERTICAL true

// 0 : 0000 0000 hide & undocked
// 1 : 0000 0001 show & undocked
// 2 : 0000 0010 hide & docked
// 3 : 0000 0011 show & docked
#define UDD_SHOW  1 // 0000 0001
#define UDD_DOCKED  2 // 0000 0010

enum TabMessage
{
	TAB_DRAWTOPBAR = 1,      //  0000 0001
	TAB_DRAWINACTIVETAB = 2, //  0000 0010
	TAB_DRAGNDROP = 4,       //  0000 0100
	TAB_REDUCE = 8,	         //  0000 1000
	TAB_CLOSEBUTTON = 16,    //  0001 0000
	TAB_DBCLK2CLOSE = 32,    //  0010 0000
	TAB_VERTICAL = 64,       //  0100 0000
	TAB_MULTILINE = 128,     //  1000 0000
	TAB_HIDE = 256           //1 0000 0000
};

enum formatType {WIN_FORMAT, MAC_FORMAT, UNIX_FORMAT};
enum UniMode {uni8Bit=0, uniUTF8=1, uni16BE=2, uni16LE=3, uniCookie=4, uni7Bit=5, uni16BE_NoBOM=6, uni16LE_NoBOM=7, uniEnd};
enum ChangeDetect {cdDisabled=0, cdEnabled=1, cdAutoUpdate=2, cdGo2end=3, cdAutoUpdateGo2end=4};
enum BackupFeature {bak_none = 0, bak_simple = 1, bak_verbose = 2};
enum OpenSaveDirSetting {dir_followCurrent = 0, dir_last = 1, dir_userDef = 2};

enum LangIndex
{
	LANG_INDEX_INSTR = 0,
	LANG_INDEX_INSTR2 = 1,
	LANG_INDEX_TYPE = 2,
	LANG_INDEX_TYPE2 = 3,
	LANG_INDEX_TYPE3 = 4,
	LANG_INDEX_TYPE4 = 5,
	LANG_INDEX_TYPE5 = 6
};

enum CopyData
{
	COPYDATA_PARAMS = 0,
	COPYDATA_FILENAMESA = 1,
	COPYDATA_FILENAMESW = 2
};

#define SCIV_PRIMARY false
#define SCIV_SECOND true

extern const TCHAR fontSizeStrs[14][3];

extern const TCHAR LINEDRAW_FONT[];
extern const TCHAR localConfFile[];
extern const TCHAR notepadStyleFile[];

#endif // PARAMETERS_DEF_H
