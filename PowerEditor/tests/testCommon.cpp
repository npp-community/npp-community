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

	if (strRet != charRet || strPath != cPath)
	{
		_tprintf(_T("Testing |%s| append |%s|     String (ours) |%s| returned %d   Char (system) |%s| returned %d\r\n"), path, append, strPath.c_str(), strRet, cPath, charRet);
	}
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
	if (strPath != charPath || strRet != charRet)
	{
		_tprintf(_T("Testing |%s|    String(our version) |%s| returned %d     Char(sys version) |%s| returned %d\r\n"), toTest, strPath.c_str(), strRet, charPath, charRet);
	}

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

bool testPathCanonicalize(TCHAR *path)
{
	generic_string strPath(path);

	TCHAR output[MAX_PATH];
	BOOL charRet = PathCanonicalize(output, path);
	BOOL strRet = PathCanonicalize(strPath);

	if (charRet != strRet || strPath != output)
	{
		_tprintf(_T("Testing |%s|    String (ours) |%s| returned %d    Char (system) |%s| returned %d\r\n"), path, strPath.c_str(), strRet, output, charRet);
	}

	return ((charRet == strRet)
		   && (strPath == output));
}

TEST(PathCanonicalizeTest, Normal)
{
	EXPECT_TRUE(testPathCanonicalize(_T("abc\\def")));
	EXPECT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi")));
}

TEST(PathCanonicalizeTest, EndParent)
{
	EXPECT_TRUE(testPathCanonicalize(_T("abc\\def\\..")));
	EXPECT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi\\..")));
}

TEST(PathCanonicalizeTest, ParentMiddle)
{
	EXPECT_TRUE(testPathCanonicalize(_T("abc\\def\\..\\ghi")));
	EXPECT_TRUE(testPathCanonicalize(_T("abc\\..\\def\\ghi")));
}

TEST(PathCanonicalizeTest, ParentEndBS)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi\\..\\")));
}

TEST(PathCanonicalizeTest, ParentParentMiddleOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\..\\..\\def\\ghi")));
}

TEST(PathCanonicalizeTest, ParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, SameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\.\\def\\ghi")));
}

TEST(PathCanonicalizeTest, SameSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\.\\.\\def\\ghi")));
}

TEST(PathCanonicalizeTest, ParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("..\\abc\\def")));
}

TEST(PathCanonicalizeTest, ParentParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("..\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, SameParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T(".\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, SameDir)
{
	ASSERT_TRUE(testPathCanonicalize(_T(".\\abc\\def")));
}

TEST(PathCanonicalizeTest, SameSameDir)
{
	ASSERT_TRUE(testPathCanonicalize(_T(".\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, SameParentParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T(".\\..\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, ParentSameOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("..\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, ParentSameParentOverflow)
{
	EXPECT_TRUE(testPathCanonicalize(_T("..\\.\\..\\abc\\def")));
	EXPECT_TRUE(testPathCanonicalize(_T("..\\..\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, ParentParentSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi\\..\\..\\.\\jkl")));
}

TEST(PathCanonicalizeTest, ParentSameParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi\\..\\.\\..\\jkl")));
}

TEST(PathCanonicalizeTest, SameParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\ghi\\.\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, SameDirParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\.\\ghi\\..\\..\\jkl")));
}

// Single check for double backslashes
TEST(PathCanonicalizeTest, DoubleBackSlashes)
{
	ASSERT_TRUE(testPathCanonicalize(_T("abc\\def\\\\ghi")));
}

// Same tests but starting with a "\\"
TEST(PathCanonicalizeTest, BSNormalDirs)
{
	EXPECT_TRUE(testPathCanonicalize(_T("\\abc\\def")));
	EXPECT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi")));
}

TEST(PathCanonicalizeTest, BSEndParent)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\..")));
}

TEST(PathCanonicalizeTest, BSParentMiddle)
{
	EXPECT_TRUE(testPathCanonicalize(_T("\\abc\\def\\..\\ghi")));
	EXPECT_TRUE(testPathCanonicalize(_T("\\abc\\..\\def\\ghi")));
}

TEST(PathCanonicalizeTest, BSParentEnd)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi\\..")));
}

TEST(PathCanonicalizeTest, BSParentEndBS)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi\\..\\")));
}

TEST(PathCanonicalizeTest, BSParentParentMiddleOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\..\\..\\def\\ghi")));

}

TEST(PathCanonicalizeTest, BSSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\.\\def\\ghi")));
}

TEST(PathCanonicalizeTest, BSSameDirSameDirMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\.\\.\\def\\ghi")));
}

TEST(PathCanonicalizeTest, BSParentDir)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSParentParent)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\..\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSSameParentDir)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\.\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSSameDir)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSSameSameDir)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\.\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSSameParentParent)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\.\\..\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSParentSame)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\..\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSParentSameParent)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\..\\.\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSParentParentSame)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\..\\..\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, BSParentParentMiddleNoOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, BSParentParentSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi\\..\\..\\.\\jkl")));
}

TEST(PathCanonicalizeTest, BSParentSameParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi\\..\\.\\..\\jkl")));
}

TEST(PathCanonicalizeTest, BSSameParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\ghi\\.\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, BSSameDirParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("\\abc\\def\\.\\ghi\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, DriveNormal)
{
	EXPECT_TRUE(testPathCanonicalize(_T("C:\\abc\\def")));
	EXPECT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi")));
}

TEST(PathCanonicalizeTest, DriveParentEnd)
{
	EXPECT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\..")));
	EXPECT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi\\..")));
}

TEST(PathCanonicalizeTest, DriveParentMiddle)
{
	EXPECT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\..\\ghi")));
	EXPECT_TRUE(testPathCanonicalize(_T("C:\\abc\\..\\def\\ghi")));
}

TEST(PathCanonicalizeTest, DriveParentEndBS)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi\\..\\")));
}

TEST(PathCanonicalizeTest, DriveParentParentMiddleOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\..\\..\\def\\ghi")));
}

TEST(PathCanonicalizeTest, DriveSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\.\\def\\ghi")));
}

TEST(PathCanonicalizeTest, DriveSameSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\.\\.\\def\\ghi")));
}

TEST(PathCanonicalizeTest, DriveParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveParentParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\..\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveSameParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\.\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveSame)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveSameSame)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\.\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveSameParentParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\.\\..\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveParentSameOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\..\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveParentSameParentOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\..\\.\\..\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveParentParentSameOverflow)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\..\\..\\.\\abc\\def")));
}

TEST(PathCanonicalizeTest, DriveParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, DriveParentParentSameMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi\\..\\..\\.\\jkl")));
}

TEST(PathCanonicalizeTest, DriveParentSameParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi\\..\\.\\..\\jkl")));
}

TEST(PathCanonicalizeTest, DriveSameParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\ghi\\.\\..\\..\\jkl")));
}

TEST(PathCanonicalizeTest, DriveSameDirParentParentMiddle)
{
	ASSERT_TRUE(testPathCanonicalize(_T("C:\\abc\\def\\.\\ghi\\..\\..\\jkl")));
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

TEST(CompareNoCaseTest, CompareTwoEqualStringsDifferentCasesMixedCase)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("BlAh"));
	ASSERT_EQ(0, res);
}

TEST(CompareNoCaseTest, CompareTwoDifferentStringsFisrtSmaller)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("clah"));
	ASSERT_EQ(-1, res);
}

TEST(CompareNoCaseTest, CompareTwoDifferentStringsFirstBigger)
{
	int res = CompareNoCase(TEXT("blah"), TEXT("alah"));
	ASSERT_EQ(1, res);
}

#endif
