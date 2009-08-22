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

#ifndef NDEBUG

void assertPathCanonicalize(TCHAR *path)
{
	generic_string strPath(path);

	TCHAR output[MAX_PATH];
	BOOL charRet = PathCanonicalize(output, path);
	BOOL strRet = PathCanonicalize(strPath);

	assert(charRet == strRet);
	assert(strPath == output);
}


void testPathCanonicalize()
{
	generic_string strPath;

	assertPathCanonicalize(TEXT("abc\\def"));
	assertPathCanonicalize(TEXT("abc\\def\\.."));
	assertPathCanonicalize(TEXT("abc\\def\\..\\ghi"));
	assertPathCanonicalize(TEXT("abc\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("abc\\def\\ghi\\.."));
	assertPathCanonicalize(TEXT("abc\\def\\ghi\\..\\"));
	assertPathCanonicalize(TEXT("abc\\..\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("abc\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("abc\\.\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("abc\\def\\ghi"));
	assertPathCanonicalize(TEXT("..\\abc\\def"));
	assertPathCanonicalize(TEXT("..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT(".\\..\\abc\\def"));
	assertPathCanonicalize(TEXT(".\\abc\\def"));
	assertPathCanonicalize(TEXT(".\\.\\abc\\def"));
	assertPathCanonicalize(TEXT(".\\..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("..\\.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("..\\..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("abc\\def\\ghi\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("abc\\def\\ghi\\..\\..\\.\\jkl"));
	assertPathCanonicalize(TEXT("abc\\def\\ghi\\..\\.\\..\\jkl"));
	assertPathCanonicalize(TEXT("abc\\def\\ghi\\.\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("abc\\def\\.\\ghi\\..\\..\\jkl"));


	// Single check for double backslashes
	assertPathCanonicalize(TEXT("abc\\def\\\\ghi"));



	// Same tests but starting with a "\\"
	assertPathCanonicalize(TEXT("\\abc\\def"));
	assertPathCanonicalize(TEXT("\\abc\\def\\.."));
	assertPathCanonicalize(TEXT("\\abc\\def\\..\\ghi"));
	assertPathCanonicalize(TEXT("\\abc\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi\\.."));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi\\..\\"));
	assertPathCanonicalize(TEXT("\\abc\\..\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("\\abc\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("\\abc\\.\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi"));
	assertPathCanonicalize(TEXT("\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("\\..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("\\.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("\\.\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("\\.\\..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("\\..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("\\..\\.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("\\..\\..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi\\..\\..\\.\\jkl"));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi\\..\\.\\..\\jkl"));
	assertPathCanonicalize(TEXT("\\abc\\def\\ghi\\.\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("\\abc\\def\\.\\ghi\\..\\..\\jkl"));

	// And now with a drive letter
	assertPathCanonicalize(TEXT("C:\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\.."));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\..\\ghi"));
	assertPathCanonicalize(TEXT("C:\\abc\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi\\.."));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi\\..\\"));
	assertPathCanonicalize(TEXT("C:\\abc\\..\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:\\abc\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:\\abc\\.\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\.\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\.\\..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\..\\.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\..\\..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi\\..\\..\\.\\jkl"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi\\..\\.\\..\\jkl"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\ghi\\.\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("C:\\abc\\def\\.\\ghi\\..\\..\\jkl"));


    // And the really ugly one, drive letter relative path
	// (win32 version gets this wrong, so we have to too, "C:..\\abc\\def" becomes "\abc\def")
	assertPathCanonicalize(TEXT("C:abc\\def"));
	assertPathCanonicalize(TEXT("C:abc\\def\\.."));
	assertPathCanonicalize(TEXT("C:abc\\def\\..\\ghi"));
	assertPathCanonicalize(TEXT("C:abc\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi\\.."));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi\\..\\"));
	assertPathCanonicalize(TEXT("C:abc\\..\\..\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:abc\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:abc\\.\\.\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi"));
	assertPathCanonicalize(TEXT("C:..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:.\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:.\\..\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:..\\.\\..\\abc\\def"));
	assertPathCanonicalize(TEXT("C:..\\..\\.\\abc\\def"));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi\\..\\..\\.\\jkl"));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi\\..\\.\\..\\jkl"));
	assertPathCanonicalize(TEXT("C:abc\\def\\ghi\\.\\..\\..\\jkl"));
	assertPathCanonicalize(TEXT("C:abc\\def\\.\\ghi\\..\\..\\jkl"));

}

#endif