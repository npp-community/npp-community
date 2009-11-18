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
		enum State
		{
			Disabled,
			Enabled
		};

		FuncGuard(const TCHAR* funcsig, const TCHAR* funcname, const TCHAR* file, int line, State state, const TCHAR* category);
		~FuncGuard();

		void outputIndent();

	private:
		void indent();
		void unindent();
		TCHAR* getIndent();
		generic_string _funcname;
		generic_string _category;
		State _state;

		static int _depth;
		static TCHAR _indent[MAX_DEBUG_INDENT];
	};

	} // namespace NppDebug

	#define debugf(format, ...) NppDebug::outputF(format, __VA_ARGS__)
	// JOCE: modify to make that a single call to OutputF
	#define line_debugf(format, ...) NppDebug::outputF(TEXT("%s(%d): "), TEXT(__FILE__), __LINE__); NppDebug::outputF(format, __VA_ARGS__)

	#define func_guard_declare_cat(cat) extern NppDebug::FuncGuard::State __func_guard_category_##cat
	#define func_guard_enable_cat(cat) NppDebug::FuncGuard::State __func_guard_category_##cat = NppDebug::FuncGuard::Enabled
	#define func_guard_disable_cat(cat) NppDebug::FuncGuard::State __func_guard_category_##cat = NppDebug::FuncGuard::Disabled

	#define func_guard(cat) NppDebug::FuncGuard __npp_func_guard__( TEXT(__FUNCSIG__), TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__, __func_guard_category_##cat, TEXT( #cat ))

	// JOCE: modify to make that a single call to OutputF (should make guard_debugf part of the function guard
	#define guard_debugf(format, ...) __npp_func_guard__.outputIndent(); NppDebug::outputF(format, __VA_ARGS__)

#else // if !SHIPPING

	#define debugf(format, ...) void(0)
	#define line_debugf(format, ...) void(0)

	#define func_guard_declare_cat(cat) void(0)
	#define func_guard_enable_cat(cat) void(0)
	#define func_guard_disable_cat(cat) void(0)

	#define func_guard(cat) void(0)

	#define guard_debugf(format, ...) void(0)
#endif // SHIPPING

#endif // NPP_DEBUG_H
