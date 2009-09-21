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