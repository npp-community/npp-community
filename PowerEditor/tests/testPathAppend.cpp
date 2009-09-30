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

#endif
