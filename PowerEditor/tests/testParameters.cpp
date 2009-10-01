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
#include "parameters.h"

//////////////////////////////////////////////////////////////////////////
//
// Table of Content:
// - CutStringSTLStringTest
// - UserLangContainerTest
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// CutStringSTLStringTest
//
//////////////////////////////////////////////////////////////////////////

TEST(CutStringSTLStringTest, EmptyString)
{
	std::vector<generic_string> patternVect;
	generic_string str(TEXT(""));
	cutString(str, patternVect);
	ASSERT_EQ(0, patternVect.size());
}

TEST(CutStringSTLStringTest, OneToken)
{
	std::vector<generic_string> patternVect;
	generic_string str(TEXT("foo"));
	cutString(str, patternVect);
	ASSERT_EQ(1, patternVect.size());
}

TEST(CutStringSTLStringTest, TwoSpaceSeparatedTokens)
{
	std::vector<generic_string> patternVect;
	generic_string str(TEXT("foo bar"));
	cutString(str, patternVect);
	ASSERT_EQ(2, patternVect.size());
}


//////////////////////////////////////////////////////////////////////////
//
// UserLangContainerTest
//
//////////////////////////////////////////////////////////////////////////

TEST(UserLangContainerTest, FeedUserSettingNull)
{
	UserLangContainer container;
	bool feedUserSetting = container.feedUserSettings(NULL);
	ASSERT_EQ(false, feedUserSetting);
	EXPECT_EQ(false, container.isCaseIgnored());
	EXPECT_EQ(TEXT('\0'), container._escapeChar[0]);
	EXPECT_EQ(false, container.isCommentSymbol());
	EXPECT_EQ(false, container.isCommentLineSymbol());
	for (int i = 0 ; i < nbPrefixListAllowed ; i++)
	{
		EXPECT_EQ(false, container.isPrefix(i));
	}
}

TEST(UserLangContainerTest, feedUserKeywordListNull)
{
	UserLangContainer container;
	bool feedUserKeywordList = container.feedUserKeywordList(NULL);
	ASSERT_EQ(false, feedUserKeywordList);
	for (int i = 0; i < container.getNbKeywordList(); i++)
	{
		EXPECT_EQ(TEXT(""), container.getKeywordList(i));
	}
}

TEST(UserLangContainerTest, feedUserStylesNull)
{
	UserLangContainer container;
	bool feedUserStyles = container.feedUserStyles(NULL);
	ASSERT_EQ(false, feedUserStyles);
	EXPECT_EQ(0, container._styleArray.getNbStyler());
}

// JOCE Need to include XML snippet to run the following tests.
//
//TEST(UserLangContainerTest, FeedUserSettingNormal)
//{
//	UserLangContainer container;
//	container.feedUserSettings(TiXmlNode *settingsRoot);
//}
//
//TEST(UserLangContainerTest, feedUserKeywordListNormal)
//{
//	container.feedUserKeywordList(TiXmlNode *node);
//}
//
//TEST(UserLangContainerTest, feedUserStylesNormal)
//{
//	container.feedUserStyles(TiXmlNode *node);
//}

#endif
// Lint seems flabbergasted with parameters.h because of the #ifndef SHIPPING.
//lint -e766 Info -- Header file '..\src\parameters.h' not used in module '..\tests\testParameters.cpp'
