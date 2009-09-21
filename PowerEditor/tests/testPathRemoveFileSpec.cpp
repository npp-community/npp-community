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

#ifndef SHIPPING

// JOCE: Should move to testCommon.cpp
bool testPathRemoveFileSpec(TCHAR* toTest)
{
	generic_string strPath;
	TCHAR charPath[MAX_PATH];

	_tcscpy_s(charPath, MAX_PATH, toTest);
	strPath = charPath;

	BOOL strRet = PathRemoveFileSpec(strPath);
	BOOL charRet = PathRemoveFileSpec(charPath);

	return ((strPath == charPath) &&
		    (strRet == charRet));
}

TEST(PathRemoveFileSpecTest, BasicPath)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:\\foo\\bar.ext")));
}

TEST(PathRemoveFileSpecTest, BasicTwoDeepPath)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:\\foo\\bar\\test.php")));
}

TEST(PathRemoveFileSpecTest, BasicPathNoFileWithEndSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:\\foo\\")));
}

TEST(PathRemoveFileSpecTest, BasicPathNoExtensionWithoutEndSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:\\foo")));
}

TEST(PathRemoveFileSpecTest, BasicPathNoFileDoubleMiddleSlashWithoutEndSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:\\foo\\\\test")));
}

TEST(PathRemoveFileSpecTest, DriveLetterWithRootPath)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:\\")));
}

TEST(PathRemoveFileSpecTest, DriveLetterAndFileNoSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:bar.ext")));
}

TEST(PathRemoveFileSpecTest, DriveLetter)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:")));
}

TEST(PathRemoveFileSpecTest, NoDriveLetterBasicPath)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("\\foo\\bar.ext")));
}

TEST(PathRemoveFileSpecTest, NoDriveLetterBasicPathTwoDeep)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("\\test\\foo\\bar.ext")));
}

TEST(PathRemoveFileSpecTest, NoDriveLetterBasicPathNoFileWithEndSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("\\foo\\")));
}

TEST(PathRemoveFileSpecTest, NoDriveLetterBasicPathNoFileWithoutEndSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("\\foo")));
}

TEST(PathRemoveFileSpecTest, NoDriveLetterNoStartSlashBasicPath)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("foo\\bar.ext")));
}

TEST(PathRemoveFileSpecTest, BasicFileName)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("foo.exe")));
}

TEST(PathRemoveFileSpecTest, BasicFileNameNoExtention)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("foo")));
}

TEST(PathRemoveFileSpecTest, OneLetterFileName)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("f")));
}

TEST(PathRemoveFileSpecTest, EmptyString)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("")));
}

TEST(PathRemoveFileSpecTest, BasicPathForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:/foo/test.exe")));
}

TEST(PathRemoveFileSpecTest, NoDriveForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("/foo/test.exe")));
}

TEST(PathRemoveFileSpecTest, NoDriveDoubleForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("//foo//test.exe")));
}

TEST(PathRemoveFileSpecTest, NoExtensionForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:/foo/test")));
}

TEST(PathRemoveFileSpecTest, NoFileForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:/foo/")));
}

TEST(PathRemoveFileSpecTest, DriveLetterAndFileNoExtensionForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:/foo")));
}

TEST(PathRemoveFileSpecTest, DriveLetterForwardSlash)
{
	ASSERT_TRUE(testPathRemoveFileSpec(_T("C:/")));
}

#endif
