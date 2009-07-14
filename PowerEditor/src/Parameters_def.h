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

const bool POS_VERTICAL = true;
const bool POS_HORIZOTAL = false;

const int UDD_SHOW   = 1; // 0000 0001
const int UDD_DOCKED = 2; // 0000 0010

// 0 : 0000 0000 hide & undocked
// 1 : 0000 0001 show & undocked
// 2 : 0000 0010 hide & docked
// 3 : 0000 0011 show & docked

const int TAB_DRAWTOPBAR = 1;      //  0000 0001
const int TAB_DRAWINACTIVETAB = 2; //  0000 0010
const int TAB_DRAGNDROP = 4;       //  0000 0100
const int TAB_REDUCE = 8;	       //  0000 1000
const int TAB_CLOSEBUTTON = 16;    //  0001 0000
const int TAB_DBCLK2CLOSE = 32;    //  0010 0000
const int TAB_VERTICAL = 64;       //  0100 0000
const int TAB_MULTILINE = 128;     //  1000 0000
const int TAB_HIDE = 256;          //1 0000 0000

enum formatType {WIN_FORMAT, MAC_FORMAT, UNIX_FORMAT};
enum UniMode {uni8Bit=0, uniUTF8=1, uni16BE=2, uni16LE=3, uniCookie=4, uni7Bit=5, uni16BE_NoBOM=6, uni16LE_NoBOM=7, uniEnd};
enum ChangeDetect {cdDisabled=0, cdEnabled=1, cdAutoUpdate=2, cdGo2end=3, cdAutoUpdateGo2end=4};
enum BackupFeature {bak_none = 0, bak_simple = 1, bak_verbose = 2};
enum OpenSaveDirSetting {dir_followCurrent = 0, dir_last = 1, dir_userDef = 2};

const int LANG_INDEX_INSTR = 0;
const int LANG_INDEX_INSTR2 = 1;
const int LANG_INDEX_TYPE = 2;
const int LANG_INDEX_TYPE2 = 3;
const int LANG_INDEX_TYPE3 = 4;
const int LANG_INDEX_TYPE4 = 5;
const int LANG_INDEX_TYPE5 = 6;

const int COPYDATA_PARAMS = 0;
const int COPYDATA_FILENAMESA = 1;
const int COPYDATA_FILENAMESW = 2;


const bool SCIV_PRIMARY = false;
const bool SCIV_SECOND = true;

const TCHAR fontSizeStrs[][3] = {TEXT(""), TEXT("8"), TEXT("9"), TEXT("10"), TEXT("11"), TEXT("12"), TEXT("14"), TEXT("16"), TEXT("18"), TEXT("20"), TEXT("22"), TEXT("24"), TEXT("26"), TEXT("28")};

const TCHAR LINEDRAW_FONT[] =  TEXT("LINEDRAW.TTF");
const TCHAR localConfFile[] = TEXT("doLocalConf.xml");
const TCHAR notepadStyleFile[] = TEXT("asNotepad.xml");

#endif // PARAMETERS_DEF_H
