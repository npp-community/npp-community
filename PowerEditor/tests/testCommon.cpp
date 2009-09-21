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

//////////////////////////////////////////////////////////////////////////
//
// Table of Content:
// - PathAppendTest
// - PathRemoveFileSpecTest
// - PathCanonicalizeTest
// - CompareNoCaseTest
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// PathAppendTest
//
//////////////////////////////////////////////////////////////////////////

bool testPathAppend(TCHAR *path, TCHAR *append)
{
	TCHAR cPath[MAX_PATH];

	generic_string strPath(path);
	generic_string strAppend(append);

	_tcscpy_s(cPath, MAX_PATH, path);

	BOOL strRet = PathAppend(strPath, strAppend);
	BOOL charRet = PathAppend(cPath, append);

	return ((strRet == charRet) &&
			(strPath == cPath));
}

TEST(PathAppendTest, TwoEmptyPaths)
{
	ASSERT_TRUE(testPathAppend(TEXT(""), TEXT("")));
}

TEST(PathAppendTest, RootAndEmptyPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\"), TEXT("")));
}

TEST(PathAppendTest, EmptyPathAndRoot)
{
	ASSERT_TRUE(testPathAppend(TEXT(""), TEXT("\\")));
}

TEST(PathAppendTest, RootAndRoot)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\"), TEXT("\\")));
}

TEST(PathAppendTest, basicNameAndRoot)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc"), TEXT("\\")));
}

TEST(PathAppendTest, basicPathAndRoot)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\"), TEXT("\\")));
}

TEST(PathAppendTest, basicNameAndEmptyPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc"), TEXT("")));
}

TEST(PathAppendTest, basicPathAndEmptyPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\"), TEXT("")));
}

TEST(PathAppendTest, EmptyPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT(""), TEXT("abc")));
}

TEST(PathAppendTest, RootAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\"), TEXT("abc")));
}

TEST(PathAppendTest, RootAndBasicRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\"), TEXT("\\abc")));
}

TEST(PathAppendTest, RootAndBasicPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\"), TEXT("abc\\")));
}

TEST(PathAppendTest, BasicNameAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc"), TEXT("def")));
}

TEST(PathAppendTest, BasicPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\"), TEXT("def")));
}

TEST(PathAppendTest, BasicNameAndRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc"), TEXT("\\def")));
}

TEST(PathAppendTest, BasicPathAndRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\"), TEXT("\\def")));
}

TEST(PathAppendTest, FullRootPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\abc\\"), TEXT("def")));
}

TEST(PathAppendTest, FullRootPathAndRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\abc\\"), TEXT("\\def")));
}

TEST(PathAppendTest, FullRootPathAndFullRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\abc\\"), TEXT("\\def\\")));
}

TEST(PathAppendTest, CurrentBasedBasicPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT(".\\abc"), TEXT("def")));
}

TEST(PathAppendTest, CurrentBasedFullPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT(".\\abc\\"), TEXT("def")));
}

TEST(PathAppendTest, ParentBasedBasicPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc"), TEXT("def")));
}

TEST(PathAppendTest, ParentBasedFullPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc\\"), TEXT("def")));
}

TEST(PathAppendTest, ParentBasedBasicPathAndRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc"), TEXT("\\def")));
}

TEST(PathAppendTest, ParentBasedFullPathAndRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc\\"), TEXT("\\def")));
}

TEST(PathAppendTest, ParentBasedBasicPathAndParentBasedBasicPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc"), TEXT("..\\def")));
}

TEST(PathAppendTest, ParentOfParentBasedBasicPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc\\.."), TEXT("def")));
}

TEST(PathAppendTest, ParentBasedBasicPathAndParentOfRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc"), TEXT("\\..\\def")));
}

TEST(PathAppendTest, ParentOfRootPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\..\\abc"), TEXT("def")));
}

TEST(PathAppendTest, ParentOfRootPathAndParentBasedBasicPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\..\\abc"), TEXT("..\\def")));
}

TEST(PathAppendTest, ParentOfRootPathAndParentOfRootPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("\\..\\abc"), TEXT("\\..\\def")));
}

TEST(PathAppendTest, ThreeDeepBasicPathAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\def\\ghi"), TEXT("jkl")));
}

TEST(PathAppendTest, ThreeDeepPathBackTwoParentsAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\def\\ghi\\..\\.."), TEXT("jkl")));
}

TEST(PathAppendTest, ParentOfTwoDeepPathBackOneParentAndBasicName)
{
	ASSERT_TRUE(testPathAppend(TEXT("..\\abc\\def\\..\\"), TEXT("ghi")));
}

TEST(PathAppendTest, ThreeDeepPathAndParentBasedBasicPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\def\\ghi"), TEXT("..\\jkl")));
}

TEST(PathAppendTest, ThreeDeepPathBackOneParentAndParentBasedBasicPath)
{
	ASSERT_TRUE(testPathAppend(TEXT("abc\\def\\ghi\\.."), TEXT("..\\jkl")));
}


//////////////////////////////////////////////////////////////////////////
//
// PathRemoveFileSpecTest
//
//////////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////////
//
// PathCanonicalizeTest
//
//////////////////////////////////////////////////////////////////////////

void assertPathCanonicalize(TCHAR *path)
{
	generic_string strPath(path);

	TCHAR output[MAX_PATH];
	BOOL charRet = PathCanonicalize(output, path);
	BOOL strRet = PathCanonicalize(strPath);

	assert(charRet == strRet);
	assert(strPath == output);
}

// TODO: Needs to be ported to Google Test framework
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


//////////////////////////////////////////////////////////////////////////
//
// CompareNoCaseTest
//
//////////////////////////////////////////////////////////////////////////

TEST(CompareNoCaseTest, CompareTwoEmptyStrings)
{
	int res = CompareNoCase(TEXT(""), TEXT(""));
	ASSERT_EQ(0, res);
}

TEST(CompareNoCaseTest, CompareTwoEqualStrings)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("blah"));
	ASSERT_EQ(0, res);
}

TEST(CompareNoCaseTest, CompareTwoEqualStringsDifferentCases)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("BLAH"));
	ASSERT_EQ(0, res);
}

TEST(CompareNoCaseTest, CompareTwoDifferentStringsFisrtSmaller)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("clah"));
	ASSERT_EQ(-1, res);
}

TEST(CompareNoCaseTest, CompareTwoDifferentStringsFisrtBigger)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("alah"));
	ASSERT_EQ(1, res);
}

#endif
