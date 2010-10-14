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

#ifndef MISC_COMMON_COMMONGENERICS_H
#define MISC_COMMON_COMMONGENERICS_H

#define CP_ANSI_LATIN_1 1252
#define CP_BIG5 950

#ifdef UNICODE
	#define NppMainEntry wWinMain
	#define generic_strtol wcstol
	#define generic_strncpy(dest, src, size_d) wcsncpy_s(dest, size_d, src, _TRUNCATE)
	#define generic_stricmp _wcsicmp
	#define generic_strncmp wcsncmp
	#define generic_strnicmp _wcsnicmp
	#define generic_strncat wcsncat
	#define generic_strchr wcschr
	#define generic_atoi _wtoi
	#define generic_itoa _itow_s
	#define generic_atof _wtof
	#define generic_strtok wcstok_s
	#define generic_strftime wcsftime
	#define generic_fprintf fwprintf
	#define generic_sscanf swscanf_s
	#define generic_fopen(pFile, filename, mode) if(_wfopen_s(&pFile, filename, mode) != 0){pFile = NULL;}
	#define generic_fgets fgetws
	#define generic_stat _wstat
	#define generic_sprintf swprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESW
#else
	#define NppMainEntry WinMain
	#define generic_strtol strtol
	#define generic_strncpy(dest, src, size_d) strncpy_s(dest, size_d, src, _TRUNCATE)
	#define generic_stricmp _stricmp
	#define generic_strncmp strncmp
	#define generic_strnicmp _strnicmp
	#define generic_strncat strncat
	#define generic_strchr strchr
	#define generic_atoi atoi
	#define generic_itoa _itoa_s
	#define generic_atof atof
	#define generic_strtok strtok_s
	#define generic_strftime strftime
	#define generic_fprintf fprintf
	#define generic_sscanf sscanf_s
	#define generic_fopen(pFile, filename, mode) if(fopen_s(&pFile, filename, mode) != 0){pFile = NULL;}
	#define generic_fgets fgets
	#define generic_stat _stat
	#define generic_sprintf sprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESA
#endif

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > generic_string;
typedef std::basic_ostringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > generic_stringstream;

#endif // MISC_COMMON_COMMONGENERICS_H
