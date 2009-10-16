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
// - FuncGuardTest
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// FuncGuardTest
//
//////////////////////////////////////////////////////////////////////////

static generic_string s_outputResult;

static void __stdcall testOutputFunction(const TCHAR* input)
{
	s_outputResult.append(input);
}

class FuncGuardTest : public ::testing::Test
{
public:
	FuncGuardTest() :
		originalOutput(NppDebug::setOutputFunction(testOutputFunction))
	{
		s_outputResult = TEXT("");
	}

	~FuncGuardTest()
	{
		NppDebug::setOutputFunction(originalOutput);
	}

	NppDebug::outputFunction originalOutput;
};


// JOCE: If we could make the "expected" string looking more like what the actual output is, that'd be nice.
TEST_F(FuncGuardTest, guardSimple)
{
	{
		NppDebug::FuncGuard guard(TEXT("sig"), TEXT("name"), TEXT("file"), 1);
		ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n"), s_outputResult);
	}
	ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\nLeaving[ name ]\n"), s_outputResult);
}

TEST_F(FuncGuardTest, guardSimpleIndent)
{
	{
		NppDebug::FuncGuard guard1(TEXT("sig"), TEXT("name"), TEXT("file"), 1);
		ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n"), s_outputResult);
		{
			NppDebug::FuncGuard guard2(TEXT("sig2"), TEXT("name2"), TEXT("file2"), 2);
			ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n\tfile2(2):\n\tEntering[ sig2 ]\n"), s_outputResult);
		}
		ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n\tfile2(2):\n\tEntering[ sig2 ]\n\tLeaving[ name2 ]\n"), s_outputResult);
	}
	ASSERT_EQ(TEXT("file(1):\nEntering[ sig ]\n\tfile2(2):\n\tEntering[ sig2 ]\n\tLeaving[ name2 ]\nLeaving[ name ]\n"), s_outputResult);
}

static TCHAR TestGuardFuncOneGuardLine[8];
void TestGuardFuncOne()
{
	func_guard(); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
}

static TCHAR TestGuardFuncTwoGuardLine[8];
void TestGuardFuncTwo()
{
	func_guard(); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
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
	ASSERT_EQ(expected, s_outputResult);
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
	ASSERT_EQ(expected, s_outputResult);
}

void TestGuardFuncWithCommentOne()
{
	func_guard(); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
	_itot_s(line, TestGuardFuncOneGuardLine, 10);
	guard_debugf(TEXT("We print a number: %d\n"), 42);
}

void TestGuardFuncWithCommentTwo()
{
	func_guard(); int line = __LINE__; // This declaration needs to be on the same line as func_guard to get the right line number.
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
	ASSERT_EQ(expected, s_outputResult);
}


#endif // SHIPPING