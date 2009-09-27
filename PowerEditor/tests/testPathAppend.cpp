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

#ifndef NDEBUG

void assertPathAppend(TCHAR *path, TCHAR *append)
{
	TCHAR cPath[MAX_PATH];


	generic_string strPath(path);
	generic_string strAppend(append);

	_tcscpy_s(cPath, MAX_PATH, path);

	BOOL strRet = PathAppend(strPath, strAppend);
	BOOL charRet = PathAppend(cPath, append);

	assert(strRet == charRet);
	assert(strPath == cPath);
}

void testPathAppend()
{
	generic_string strPath;
	generic_string strAppend;


	// First the blank and backslash only cases
	assertPathAppend(TEXT(""), TEXT(""));			// "\\"
	assertPathAppend(TEXT("\\"), TEXT(""));			// "\\"
	assertPathAppend(TEXT(""), TEXT("\\"));			// "\\"
	assertPathAppend(TEXT("\\"), TEXT("\\"));   	// "\\"
	assertPathAppend(TEXT("abc"), TEXT("\\"));  	// "abc"
	assertPathAppend(TEXT("abc\\"), TEXT("\\"));  	// "abc\\"
	assertPathAppend(TEXT("abc"), TEXT(""));		// "abc"
	assertPathAppend(TEXT("abc\\"), TEXT(""));		// "abc\\"
	assertPathAppend(TEXT(""), TEXT("abc"));		// "abc"
	assertPathAppend(TEXT("\\"), TEXT("abc"));		// "\\abc"
	assertPathAppend(TEXT("\\"), TEXT("\\abc"));	// "\\abc"

	// now the normal cases
	assertPathAppend(TEXT("abc"), TEXT("def"));
	assertPathAppend(TEXT("abc\\"), TEXT("def"));
	assertPathAppend(TEXT("abc"), TEXT("\\def"));
	assertPathAppend(TEXT("abc\\"), TEXT("\\def"));
	assertPathAppend(TEXT("\\abc\\"), TEXT("def"));
	assertPathAppend(TEXT("\\abc\\"), TEXT("\\def"));
	assertPathAppend(TEXT("\\abc\\"), TEXT("\\def\\"));

	// and lastly the cases that need canonicalizing
	assertPathAppend(TEXT(".\\abc"), TEXT("def"));
	assertPathAppend(TEXT(".\\abc\\"), TEXT("def"));
	assertPathAppend(TEXT("..\\abc"), TEXT("def"));
	assertPathAppend(TEXT("..\\abc\\"), TEXT("def"));
	assertPathAppend(TEXT("..\\abc\\"), TEXT("\\def"));
	assertPathAppend(TEXT("..\\abc"), TEXT("\\def"));
	assertPathAppend(TEXT("..\\abc\\.."), TEXT("def"));
	assertPathAppend(TEXT("..\\abc"), TEXT("..\\def"));
	assertPathAppend(TEXT("..\\abc"), TEXT("\\..\\def"));
	assertPathAppend(TEXT("\\..\\abc"), TEXT("def"));
	assertPathAppend(TEXT("\\..\\abc"), TEXT("..\\def"));
	assertPathAppend(TEXT("\\..\\abc"), TEXT("\\..\\def"));
	assertPathAppend(TEXT("abc\\def\\ghi"), TEXT("jkl"));
	assertPathAppend(TEXT("abc\\def\\ghi\\..\\.."), TEXT("jkl"));
	assertPathAppend(TEXT("..\\abc\\def\\..\\"), TEXT("ghi"));
	assertPathAppend(TEXT("abc\\def\\ghi"), TEXT("..\\jkl"));
	assertPathAppend(TEXT("abc\\def\\ghi\\.."), TEXT("..\\jkl"));


}

#endif

