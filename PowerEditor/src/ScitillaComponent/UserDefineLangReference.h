/*
this file is part of Notepad++
Copyright (C)2003 Don HO <donho@altern.org>

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

#ifndef USER_DEFINE_LANG_REFERENCE_H
#define USER_DEFINE_LANG_REFERENCE_H

#define langNameLenMax 16
#define extsLenMax 256

//const int max_char = 4096;
#define max_char  1024*30

#define nbPrefixListAllowed 4

enum UDD_KeywordList
{
	KWL_DELIM_INDEX = 0,
	KWL_FOLDER_OPEN_INDEX = 1,
	KWL_FOLDER_CLOSE_INDEX = 2,
	KWL_OPERATOR_INDEX = 3,
	KWL_COMMENT_INDEX = 4,
	KWL_KW1_INDEX = 5,
	KWL_KW2_INDEX = 6,
	KWL_KW3_INDEX = 7,
	KWL_KW4_INDEX = 8,
	KWL_NB_KEYWORD_LISTS
};

enum UDD_Style
{
	STYLE_DEFAULT_INDEX = 0,
	STYLE_BLOCK_OPEN_INDEX = 1,
	STYLE_BLOCK_CLOSE_INDEX = 2,
	STYLE_WORD1_INDEX = 3,
	STYLE_WORD2_INDEX = 4,
	STYLE_WORD3_INDEX = 5,
	STYLE_WORD4_INDEX = 6,
	STYLE_COMMENT_INDEX = 7,
	STYLE_COMMENTLINE_INDEX = 8,
	STYLE_NUMBER_INDEX = 9,
	STYLE_OPERATOR_INDEX = 10,
	STYLE_DELIM2_INDEX = 11,
	STYLE_DELIM3_INDEX = 12
};

#endif //USER_DEFINE_LANG_REFERENCE_H


