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

#include "npp_debug.h"

namespace NppDebug
{

static outputFunction debugOutput = OutputDebugString;

outputFunction setOutputFunction(outputFunction newOutputFunction)
{
	outputFunction currentOutputFunc = debugOutput;
	debugOutput = newOutputFunction;
	return currentOutputFunc;
}

void outputF(TCHAR* format, ...)
{
	va_list args;
	int len;
	TCHAR* buffer;

	va_start( args, format );
	len = _vsctprintf( format, args ) + 1; // _vscprintf doesn't count terminating '\0'
	buffer = new TCHAR[len];
	_vstprintf_s( buffer, len, format, args );
	debugOutput(buffer);
	delete [] buffer;
}

int FuncGuard::_depth = 0;
TCHAR FuncGuard::_indent[MAX_DEBUG_INDENT];

FuncGuard::FuncGuard(const TCHAR* funcsig, const TCHAR* funcname, const TCHAR* file, int line):
	_funcname(funcname)
{
	outputF(TEXT("%s%s(%d):\n"),  getIndent(), file, line);
	outputF(TEXT("%sEntering[ %s ]\n"),  getIndent(), funcsig);
	indent();
}

FuncGuard::~FuncGuard()
{
	unindent();
	outputF(TEXT("%sLeaving[ %s ]\n"), getIndent(), _funcname.c_str());
}

void FuncGuard::outputIndent()
{
	outputF(getIndent());
}

void FuncGuard::indent()
{
	_depth++;
	if (_depth >= MAX_DEBUG_INDENT)
	{
		DebugBreak();
		_depth = MAX_DEBUG_INDENT-1;
	}
}

void FuncGuard::unindent()
{
	_depth--;
	if (_depth < 0)
	{
		DebugBreak();
		_depth = 0;
	}
}

TCHAR* FuncGuard::getIndent()
{
	int i = 0;;
	for (; i < _depth; i++)
	{
		_indent[i] = TEXT('\t');
	}
	_indent[i] = TEXT('\0');
	return _indent;
}

} // namespace Debug

#endif // #ifndef SHIPPING
