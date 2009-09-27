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

/* This file was originally hacked into winmain.cpp to run the tests
   Hopefully it'll be converted to whatever unit testing framework is in place, at some point
*/

#include "precompiled_headers.h"

void testPathRemoveFileSpec(TCHAR* charPath, generic_string strPath)
{
	BOOL strRet = PathRemoveFileSpec(strPath);
	BOOL charRet = PathRemoveFileSpec(charPath);

	// Fool the compiler in debug so the variables are used.
	// This will change anyway when the test framework is used.
	#ifdef NDEBUG
	strRet = strRet;
	charRet = charRet;
	#endif

	assert(strPath == charPath);
	assert(strRet == charRet);
}

void testPathFunctions()
{
	generic_string strPath;
	TCHAR charPath[MAX_PATH];

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\bar\\test.php"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);


	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\foo\\\\test"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:\\"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C:bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);


	_tcscpy_s(charPath, MAX_PATH, _T("C:"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\test\\foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\foo\\"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("\\foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("foo\\bar.ext"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("foo.exe"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("f"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://foo//test.exe"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("//foo//test.exe"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);


	_tcscpy_s(charPath, MAX_PATH, _T("C://foo//test"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://foo//"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://foo"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T("C://"));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

	_tcscpy_s(charPath, MAX_PATH, _T(""));
	strPath = charPath;
	testPathRemoveFileSpec(charPath, strPath);

}
