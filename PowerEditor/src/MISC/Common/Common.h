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

#ifndef M30_IDE_COMMUN_H
#define M30_IDE_COMMUN_H

#define NPP_CP_WIN_1250        1250
#define NPP_CP_WIN_1251        1251
#define NPP_CP_WIN_1252        1252
#define NPP_CP_WIN_1253        1253
#define NPP_CP_WIN_1254        1254
#define NPP_CP_WIN_1255        1255
#define NPP_CP_WIN_1256        1256
#define NPP_CP_WIN_1257        1257
#define NPP_CP_WIN_1258        1258

#define NPP_CP_BIG5            950
#define NPP_CP_EUC_KR          51949
#define NPP_CP_SHIFT_JIS       932
#define NPP_CP_GB2312          936
#define NPP_CP_ISO_8859_8      28598
#define NPP_CP_TIS_620         874


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

void folderBrowser(HWND parent, int outputCtrlID, const TCHAR *defaultStr = NULL);

void systemMessage(const TCHAR *title);
//DWORD ShortToLongPathName(LPCTSTR lpszShortPath, LPTSTR lpszLongPath, DWORD cchBuffer);
void printInt(int int2print);
void printStr(const TCHAR *str2print);

void writeLog(const TCHAR *logFileName, const char *log2write);
int filter(unsigned int code);
int getCpFromStringValue(const char * encodingStr);
generic_string purgeMenuItemString(const TCHAR * menuItemStr, bool keepAmpersand = false);
std::vector<generic_string> tokenizeString(const generic_string & tokenString, const char delim);

void ClientRectToScreenRect(HWND hWnd, RECT* rect);
void ScreenRectToClientRect(HWND hWnd, RECT* rect);

std::wstring string2wstring(const std::string & rString, UINT codepage);
std::string wstring2string(const std::wstring & rwString, UINT codepage);

TCHAR *BuildMenuFileName(TCHAR *buffer, int len, int pos, const TCHAR *filename);

int strVal(const TCHAR *str, int base);
int decStrVal(const TCHAR *str);
int hexStrVal(const TCHAR *str);

// test string case insensitive
// 0 if equal, <0 if name comes before, >0 if it comes after
int CompareNoCase(const generic_string& name1, const generic_string& name2);

// MS reimplementations with N++'s generic strings.
BOOL PathRemoveFileSpec(generic_string & path);
BOOL PathAppend(generic_string& path, const generic_string& more);
BOOL PathCanonicalize(generic_string& path);
BOOL PathCanonicalize(generic_string& path, generic_string& output);

class WcharMbcsConvertor
{
public:
	static WcharMbcsConvertor * getInstance() {return _pSelf;};
	static void destroyInstance() {delete _pSelf;};

	const wchar_t * char2wchar(const char *mbStr, UINT codepage);
	const wchar_t * char2wchar(const char *mbcs2Convert, UINT codepage, int *mstart, int *mend);
	const char * wchar2char(const wchar_t *wcStr, UINT codepage);
	const char * wchar2char(const wchar_t *wcStr, UINT codepage, long *mstart, long *mend);

	const char * encode(UINT fromCodepage, UINT toCodepage, const char *txt2Encode) {
        const wchar_t * strW = char2wchar(txt2Encode, fromCodepage);
        return wchar2char(strW, toCodepage);
    };
/*
	const char * encodeFromUtf8To(const char *txt2Encode) const {
        const wchar_t * strW = wmc->char2wchar(txt2Encode, SC_CP_UTF8);
        return wchar2char(strW, toCodepage);
    };
*/
protected:
	WcharMbcsConvertor();
	~WcharMbcsConvertor();

	static WcharMbcsConvertor * _pSelf;

	char *_multiByteStr;
	size_t _multiByteAllocLen;
	wchar_t *_wideCharStr;
	size_t _wideCharAllocLen;

private:
	// Since there's no public ctor, we need to void the default assignment operator.
	const WcharMbcsConvertor& operator= (const WcharMbcsConvertor&);
};

#define ERROR_MSG_SIZE 1024

#ifdef DEBUG
	#define NO_DEFAULT_CASE default: {\
		TCHAR errorMsg[ERROR_MSG_SIZE];\
		sprintf_s(errorMsg, ERROR_MSG_SIZE, "Unhanded default case in %s, line %d", __FILE__, __LINE__ );\
		::MessageBox(NULL, TEXT("Unhandled default case."), errorMsg, MB_OK|MB_ICONWARNING);\
		}\
		break
#else
	#define NO_DEFAULT_CASE default: break
#endif


#if _MSC_VER > 1400 // MS Compiler > VS 2005
#define REBARBAND_SIZE REBARBANDINFO_V3_SIZE
#else
#define REBARBAND_SIZE sizeof(REBARBANDINFO)
#endif

#endif //M30_IDE_COMMUN_H
