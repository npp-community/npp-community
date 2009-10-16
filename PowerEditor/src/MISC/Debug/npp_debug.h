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

#ifndef NPP_DEBUG_H
#define NPP_DEBUG_H

#ifndef SHIPPING

#define MAX_DEBUG_INDENT 1024
namespace NppDebug
{
typedef void (__stdcall *outputFunction)(const TCHAR*);

outputFunction setOutputFunction(outputFunction newOutputFunction);

void outputF(TCHAR* format, ...);

// JOCE: Nice to have: configurable indent character (' ', '\t', '_', '+', etc...)
class FuncGuard
{
public:
	FuncGuard(const TCHAR* funcsig, const TCHAR* funcname, const TCHAR* file, int line);
	~FuncGuard();

	void outputIndent();

private:
	void indent();
	void unindent();
	TCHAR* getIndent();
	generic_string _funcname;

	static int _depth;
	static TCHAR _indent[MAX_DEBUG_INDENT];
};

} // namespace NppDebug

#define debugf(format, ...) NppDebug::outputF(format, __VA_ARGS__)
// JOCE: modify to make that a single call to OutputF
#define line_debugf(format, ...) NppDebug::outputF(TEXT("%s(%d): "), TEXT(__FILE__), __LINE__); NppDebug::outputF(format, __VA_ARGS__)
// JOCE: modify to make that a single call to OutputF
#define guard_debugf(format, ...) __npp_func_guard__.outputIndent(); NppDebug::outputF(format, __VA_ARGS__)
#define func_guard() NppDebug::FuncGuard __npp_func_guard__(TEXT(__FUNCSIG__), TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__)

#else // if !SHIPPING
	#define debugf(format, ...) void(0)
	#define _debugf(format, ...) void(0)
	#define guard_func() void(0)
#endif // SHIPPING

#endif // NPP_DEBUG_H
