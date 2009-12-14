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
DebugOutput defaultDebugOutput;
DebugOutput* g_debugOutput = &defaultDebugOutput;

DebugOutput::DebugOutput()
{
	resetValues();
}

void DebugOutput::printf(const TCHAR* format, ...)
{
	if (format)
	{
		va_list args;

		va_start( args, format );
		int nbWrittenChar = _vsntprintf_s( m_currentPtr, MAX_DEBUG_STR - m_currentLen, _TRUNCATE, format, args );
		if (nbWrittenChar >= 0)
		{
			m_currentLen += nbWrittenChar;
		}
		else // _vsntprintf_s returned -1, this means we were truncated.
		{
			m_currentLen = MAX_DEBUG_STR-1;
		}
		m_currentPtr = &m_outputStr[0] + m_currentLen;
	}
	else
	{
		*m_currentPtr = TEXT('\0');
	}
}

void DebugOutput::vsprintf(const TCHAR* format, va_list args)
{
	if (format)
	{
		int nbWrittenChar = _vsntprintf_s( m_currentPtr, MAX_DEBUG_STR - m_currentLen, _TRUNCATE, format, args );
		if (nbWrittenChar >= 0)
		{
			m_currentLen += nbWrittenChar;
		}
		else // _vsntprintf_s returned -1, this means we were truncated.
		{
			m_currentLen = MAX_DEBUG_STR-1;
		}
		m_currentPtr = &m_outputStr[0] + m_currentLen;
	}
	else
	{
		*m_currentPtr = TEXT('\0');
	}
}

void DebugOutput::flush()
{
	if (m_outputStr[0] != TEXT('\0'))
	{
		output(&m_outputStr[0]);
		resetValues();
	}
}

void DebugOutput::output(const TCHAR* str)
{
	OutputDebugString(str);
}

void DebugOutput::resetValues()
{
	m_outputStr[0] = TEXT('\0');
	m_currentPtr = &m_outputStr[0];
	m_currentLen = 0;
}

int FuncGuard::_depth = 0;
TCHAR FuncGuard::_indent[MAX_DEBUG_INDENT];

FuncGuard::FuncGuard( const TCHAR* funcsig, const TCHAR* funcname, const TCHAR* file, int line, State state, const TCHAR* category):
	_state(state)
{
	if (_state == Enabled)
	{
		_funcname = funcname;
		// JOCE: we currently don't do anything with the category variable. Should it be here at all?
		_category = category;

		// JOCE Only one call to OutputF.
		g_debugOutput->printf(TEXT("%s%s(%d):\n"),  getIndent(), file, line);
		g_debugOutput->printf(TEXT("%sEntering[ %s ]\n"),  getIndent(), funcsig);
		indent();
		g_debugOutput->flush();
	}
}

FuncGuard::~FuncGuard()
{
	if (_state == Enabled)
	{
		unindent();
		g_debugOutput->printf(TEXT("%sLeaving[ %s ]\n"), getIndent(), _funcname.c_str());
		g_debugOutput->flush();
	}
}

void FuncGuard::printf(State printfState, const TCHAR* format, ...)
{
	if (printfState == Enabled)
	{
		if (_state == Enabled)
		{
			g_debugOutput->printf(getIndent());
		}

		va_list args;
		va_start( args, format );
		g_debugOutput->vsprintf(format, args);
		g_debugOutput->flush();
		va_end(args);
	}
}

void FuncGuard::indent()
{
	if (_state == Enabled)
	{
		_depth++;
		if (_depth >= MAX_DEBUG_INDENT)
		{
			DebugBreak();
			_depth = MAX_DEBUG_INDENT-1;
		}
	}
}

void FuncGuard::unindent()
{
	if (_state == Enabled)
	{
		_depth--;
		if (_depth < 0)
		{
			DebugBreak();
			_depth = 0;
		}
	}
}

TCHAR* FuncGuard::getIndent()
{
	if (_state == Enabled)
	{
		int i = 0;;
		for (; i < _depth; i++)
		{
			_indent[i] = TEXT('\t');
		}
		_indent[i] = TEXT('\0');
		return _indent;
	}
	return TEXT("");
}

} // namespace Debug

#endif // #ifndef SHIPPING
