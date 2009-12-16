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

// JOCE tests would be needed for debugf and _debugf as well.

//////////////////////////////////////////////////////////////////////////
//
// Table of Content:
// - DebugOutputtTest
// - FuncGuardTest
//
//////////////////////////////////////////////////////////////////////////



// Test debug output tools for this file.
static generic_string s_testDebugOutputStr;

class TestDebugOutput : public NppDebug::DebugOutput
{
public:
	int getCurrentLen() { return m_currentLen; }
	const TCHAR* getCurrentPtr() { return m_currentPtr; }
	const TCHAR* getOutputStr() { return m_outputStr; }

protected:
	virtual void output(const TCHAR* str)
	{
		s_testDebugOutputStr += str;
	}
};



//////////////////////////////////////////////////////////////////////////
//
// DebugOutputTest
//
//////////////////////////////////////////////////////////////////////////

class DebugOutputTest : public ::testing::Test
{
public:
	DebugOutputTest()
	{
		s_testDebugOutputStr = TEXT("");
	}
protected:
	TestDebugOutput debugOutput;
};

TEST_F(DebugOutputTest, inputSimple)
{
	const TCHAR* str1 = TEXT("Hello World");
	debugOutput.printf(str1);
	EXPECT_EQ(_tcslen(str1), debugOutput.getCurrentLen());
	debugOutput.flush();
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	EXPECT_EQ(debugOutput.getOutputStr(), debugOutput.getCurrentPtr());
	ASSERT_EQ(str1, s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, inputMultipleStrings)
{
	const TCHAR* str1 = TEXT("Hello World\n");
	const TCHAR* str2 = TEXT("It's a nice day\n");
	generic_string resStr = str1;
	resStr += str2;
	debugOutput.printf(str1);
	debugOutput.printf(str2);
	debugOutput.flush();
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	ASSERT_EQ(resStr, s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, noInput)
{
	debugOutput.flush();
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	EXPECT_EQ(debugOutput.getOutputStr(), debugOutput.getCurrentPtr());
	ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, flushTwice)
{
	const TCHAR* str1 = TEXT("Hello World\n");
	debugOutput.printf(str1);
	debugOutput.flush();
	s_testDebugOutputStr = TEXT("");
	debugOutput.flush();
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	EXPECT_EQ(debugOutput.getOutputStr(), debugOutput.getCurrentPtr());
	ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, inputTooLarge)
{
	generic_string strTooLong(MAX_DEBUG_STR*2, TEXT('A'));

	debugOutput.printf(strTooLong.c_str());
	EXPECT_EQ(MAX_DEBUG_STR-1, debugOutput.getCurrentLen());
	debugOutput.flush();
	generic_string strExpected(MAX_DEBUG_STR-1, TEXT('A'));
	ASSERT_EQ(strExpected, s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, inputTooLargeBecauseOfArguments)
{
	TCHAR str[MAX_DEBUG_STR];

	for (int i = 0; i < MAX_DEBUG_STR -3; i++)
	{
		str[i] = TEXT('A');
	}

	str[MAX_DEBUG_STR-3] = TEXT('%');
	str[MAX_DEBUG_STR-2] = TEXT('d');
	str[MAX_DEBUG_STR-1] = TEXT('\0');

	debugOutput.printf(str,55555555);
	EXPECT_EQ(MAX_DEBUG_STR-1, debugOutput.getCurrentLen());
	debugOutput.flush();
	str[MAX_DEBUG_STR-3] = TEXT('5');
	str[MAX_DEBUG_STR-2] = TEXT('5');
	ASSERT_EQ(str, s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, inputNULL)
{
	debugOutput.printf(NULL);
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	debugOutput.flush();
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	EXPECT_EQ(debugOutput.getOutputStr(), debugOutput.getCurrentPtr());
	ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
}

TEST_F(DebugOutputTest, inputNULLAfterLegalInput)
{
	const TCHAR* str1 = TEXT("Hello World");
	debugOutput.printf(str1);
	debugOutput.printf(NULL);
	EXPECT_EQ(_tcslen(str1), debugOutput.getCurrentLen());
	debugOutput.flush();
	EXPECT_EQ(0, debugOutput.getCurrentLen());
	EXPECT_EQ(debugOutput.getOutputStr(), debugOutput.getCurrentPtr());
	ASSERT_EQ(str1, s_testDebugOutputStr);
}

//////////////////////////////////////////////////////////////////////////
//
// FuncGuardTest
//
//////////////////////////////////////////////////////////////////////////

class FuncGuardTest : public ::testing::Test
{
public:
	FuncGuardTest() :
		m_originalOutput(NppDebug::g_debugOutput)
	{
		NppDebug::g_debugOutput = &m_debugOutput;
		s_testDebugOutputStr = TEXT("");
	}

	~FuncGuardTest()
	{
		NppDebug::g_debugOutput = m_originalOutput;
	}

	NppDebug::DebugOutput* m_originalOutput;
	TestDebugOutput m_debugOutput;
};

// JOCE: If we could make the "expected" string looking more like what the actual output is, that'd be nice.
TEST_F(FuncGuardTest, guardSimple)
{
	{
		NppDebug::FuncGuard guard(TEXT("sig"), TEXT("name"), TEXT("file"), 1, NppDebug::FuncGuard::Enabled, TEXT("test"));
		ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n"), s_testDebugOutputStr);
	}
	ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\nLeaving[ name ]\n"), s_testDebugOutputStr);
}

TEST_F(FuncGuardTest, guardSimpleIndent)
{
	{
		NppDebug::FuncGuard guard1(TEXT("sig"), TEXT("name"), TEXT("file"), 1, NppDebug::FuncGuard::Enabled, TEXT("test"));
		ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n"), s_testDebugOutputStr);
		{
			NppDebug::FuncGuard guard2(TEXT("sig2"), TEXT("name2"), TEXT("file2"), 2, NppDebug::FuncGuard::Enabled, TEXT("test"));
			ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n\tfile2(2):\n\tEntering[ sig2 ]\n"), s_testDebugOutputStr);
		}
		ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n\tfile2(2):\n\tEntering[ sig2 ]\n\tLeaving[ name2 ]\n"), s_testDebugOutputStr);
	}
	ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n\tfile2(2):\n\tEntering[ sig2 ]\n\tLeaving[ name2 ]\nLeaving[ name ]\n"), s_testDebugOutputStr);
}

TEST_F(FuncGuardTest, guardSimpleDisabled)
{
	{
		NppDebug::FuncGuard guard(TEXT("sig"), TEXT("name"), TEXT("file"), 1, NppDebug::FuncGuard::Disabled, TEXT("test"));
		ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
	}
	ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
}

TEST_F(FuncGuardTest, guardSimpleIndentDisabled)
{
	{
		NppDebug::FuncGuard guard1(TEXT("sig"), TEXT("name"), TEXT("file"), 1, NppDebug::FuncGuard::Disabled, TEXT("test"));
		ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
		{
			NppDebug::FuncGuard guard2(TEXT("sig2"), TEXT("name2"), TEXT("file2"), 2, NppDebug::FuncGuard::Disabled, TEXT("test"));
			ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
		}
		ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
	}
	ASSERT_EQ(TEXT(""), s_testDebugOutputStr);
}

// Enable the 'test' function guard category
func_guard_enable_cat(test);


static TCHAR TestGuardFuncOneGuardLine[8];
void TestGuardFuncOne()
{
	func_guard(test); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
}

static TCHAR TestGuardFuncTwoGuardLine[8];
void TestGuardFuncTwo()
{
	func_guard(test); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncTwoGuardLine, 10);
	TestGuardFuncOne();
}

TEST_F(FuncGuardTest, guardFunc)
{
	TestGuardFuncOne();
	generic_string expected = TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncOneGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("Entering[ void __cdecl TestGuardFuncOne(void) ]\n");
	expected += TEXT("Leaving[ TestGuardFuncOne ]\n");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

TEST_F(FuncGuardTest, guardFuncIndent)
{
	TestGuardFuncTwo();
	generic_string expected = TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncTwoGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("Entering[ void __cdecl TestGuardFuncTwo(void) ]\n");
	expected += TEXT("\t");
	expected += TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncOneGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("\tEntering[ void __cdecl TestGuardFuncOne(void) ]\n");
	expected += TEXT("\tLeaving[ TestGuardFuncOne ]\n");
	expected += TEXT("Leaving[ TestGuardFuncTwo ]\n");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

void TestGuardFuncWithCommentOne()
{
	func_guard(test); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
	guard_debugf(TEXT("We print a number: %d\n"), 42);
}

void TestGuardFuncWithCommentTwo()
{
	func_guard(test); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncTwoGuardLine, 10);
	TestGuardFuncWithCommentOne();
}

TEST_F(FuncGuardTest, guardFuncIndentWithDebugComment)
{
	TestGuardFuncWithCommentTwo();
	generic_string expected = TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncTwoGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("Entering[ void __cdecl TestGuardFuncWithCommentTwo(void) ]\n");
	expected += TEXT("\t");
	expected += TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncOneGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("\tEntering[ void __cdecl TestGuardFuncWithCommentOne(void) ]\n");
	expected += TEXT("\t\tWe print a number: 42\n");
	expected += TEXT("\tLeaving[ TestGuardFuncWithCommentOne ]\n");
	expected += TEXT("Leaving[ TestGuardFuncWithCommentTwo ]\n");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

void TestGuardFuncWithCategoryCommentOne()
{
	func_guard(test); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
	guard_debugf_cat(test, TEXT("%s %d\n"), TEXT("We print a number:"), 42);
}

void TestGuardFuncWithCategoryCommentTwo()
{
	func_guard(test); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncTwoGuardLine, 10);
	TestGuardFuncWithCategoryCommentOne();
}

TEST_F(FuncGuardTest, guardFuncIndentWithCategoryDebugComment)
{
	TestGuardFuncWithCategoryCommentTwo();
	generic_string expected = TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncTwoGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("Entering[ void __cdecl TestGuardFuncWithCategoryCommentTwo(void) ]\n");
	expected += TEXT("\t");
	expected += TEXT(__FILE__);
	expected += TEXT('(');
	expected += TestGuardFuncOneGuardLine;
	expected += TEXT("):\n");
	expected += TEXT("\tEntering[ void __cdecl TestGuardFuncWithCategoryCommentOne(void) ]\n");
	expected += TEXT("\t\tWe print a number: 42\n");
	expected += TEXT("\tLeaving[ TestGuardFuncWithCategoryCommentOne ]\n");
	expected += TEXT("Leaving[ TestGuardFuncWithCategoryCommentTwo ]\n");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

// Disable the 'test_disable' function guard category
func_guard_disable_cat(test_disable);

void TestGuardFuncDisabledOne()
{
	func_guard(test_disable);
}

void TestGuardFuncDisabledTwo()
{
	func_guard(test_disable);
	TestGuardFuncDisabledOne();
}

TEST_F(FuncGuardTest, guardFuncDisabled)
{
	TestGuardFuncDisabledOne();
	generic_string expected = TEXT("");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

TEST_F(FuncGuardTest, guardFuncIndentDisabled)
{
	TestGuardFuncDisabledTwo();
	generic_string expected = TEXT("");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

void TestGuardFuncDisabledWithCommentOne()
{
	func_guard(test_disable); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
	guard_debugf(TEXT("%s %d\n"), TEXT("We print a number:"), 42);
}

void TestGuardFuncDisabledWithCommentTwo()
{
	func_guard(test_disable); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncTwoGuardLine, 10);
	TestGuardFuncDisabledWithCommentOne();
}

TEST_F(FuncGuardTest, guardFuncDisabledDebugComment)
{
	TestGuardFuncDisabledWithCommentTwo();
	generic_string expected = TEXT("We print a number: 42\n");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

void TestGuardFuncDisabledWithCategoryCommentOne()
{
	func_guard(test_disable); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
	guard_debugf_cat(test_disable, TEXT("%s %d\n"), TEXT("We print a number:"), 42);
}

void TestGuardFuncDisabledWithCategoryCommentTwo()
{
	func_guard(test_disable); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncTwoGuardLine, 10);
	TestGuardFuncDisabledWithCategoryCommentOne();
}

TEST_F(FuncGuardTest, guardFuncDisabledDebugCategoryComment)
{
	TestGuardFuncDisabledWithCategoryCommentTwo();
	generic_string expected = TEXT("");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

void TestGuardFuncDisabledWithCategoryCommentEnabledOne()
{
	func_guard(test_disable); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
	guard_debugf_cat(test, TEXT("%s %d\n"), TEXT("We print a number:"), 42);
}

void TestGuardFuncDisabledWithCategoryCommentEnabledTwo()
{
	func_guard(test_disable); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncTwoGuardLine, 10);
	TestGuardFuncDisabledWithCategoryCommentEnabledOne();
}

TEST_F(FuncGuardTest, guardFuncDisabledDebugCategoryCommentEnabled)
{
	TestGuardFuncDisabledWithCategoryCommentEnabledTwo();
	generic_string expected = TEXT("We print a number: 42\n");
	ASSERT_EQ(expected, s_testDebugOutputStr);
}

#endif // SHIPPING
