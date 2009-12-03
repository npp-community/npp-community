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

#define MAX_DEBUG_INDENT 32
#define MAX_DEBUG_STR 1024
namespace NppDebug
{
	class DebugOutput
	{
	public:
		DebugOutput();
		void printf(const TCHAR* format, ...);
		void flush();
	protected:
		virtual void output(const TCHAR* str);
		void resetValues();
		TCHAR m_outputStr[MAX_DEBUG_STR];
		TCHAR* m_currentPtr;
		int m_currentLen;
	};

	extern DebugOutput* g_debugOutput;

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

// debugf() works just at printf(), except that it outputs to the debug console in MSVC
#define debugf(format, ...) NppDebug::g_debugOutput->printf(format, __VA_ARGS__); NppDebug::g_debugOutput->flush()

// line_debugf() works just as debugf, but it prefixes the text message with the file name
// and line number of the call in a way that MSVC recognizes so you can double click on the
// line in the debug output and be taken to the source file.
//
// For example:
// source\folder\file.cpp(42): Hello World!!!

#define line_debugf(format, ...) \
	NppDebug::g_debugOutput->printf(TEXT("%s(%d): "), TEXT(__FILE__), __LINE__); \
	NppDebug::g_debugOutput->printf(format, __VA_ARGS__); \
	NppDebug::g_debugOutput->flush()

//
//  Debug Guards
// ==============
//
// Debug guards are debugging tools meant to help the programmers figure out the path the
// code is taking in real time by displaying info in the debug console.
// The different guarded functions will show up in the console as the program enters them.
// They will also be indented in a way that a called function will be indented in from a
// caller function.
//
// For example, given the following function calls:
//
// void FooOne()
// {
//     func_guard(foo); // The 'foo' parameter is explained below...
// }
// void FooTwo()
// {
//    func_guard(foo); // The 'foo' parameter is explained below...
//    // ...
//    FooOne();
// }
//
// one would get the following output:
//
// file.cpp(42):
// Entering[ void __cdecl FooTwo(void) ]
//   file.cpp(21):
//   Entering[ void __cdecl FooOne(void) ]
//   Leaving[ FooOne ]
// Leaving[ FooTwo ]
//
//
// Now, it's also understandable that programmers who sprinkled func_guard()'s all over the place
// will not want to remove these calls before commiting a branch, just to have to put them back
// for the next feature they'll start. But on the other hand, if the output console is flooded with
// calls from all around the place, this tool loses it utility as the programmer will not be able
// to tell the function that he cares about from the noise. This is why the function guards can be
// turned on and off simply by declaring a category to which a guard belongs as enabled or disabled.
//
// For example:
//
// func_guard_enable_cat(foo); // enables the 'foo' category
// func_guard_disable_cat(bar); // disable the 'bar' category
//
// void FooOne()
// {
//     func_guard(foo); // This guard is enabled.
// }
//
// void BarOne()
// {
//     func_guard(bar); // This guard is disabled.
// }
//
// Right now (Nov 2009), the categories are mainly expected to match the files they're declared in one-to-one or
// one-to-many; i.e. In foo.cpp, you are expected to declare one or more guards to be used in that file only.
// However, there's a provision for categories that would span more that one file. The programmer would
// declare a category using func_guard_enable_cat(foo) or func_guard_disable_cat(foo) in one source file, and
// would be able to use that same category in another file using func_guard_import_cat(foo).
// This could be useful in case you have a few tightly coupled classes across a few files for which one would
// want to turn the tracing on or off by changing a single declaration instead of many.
//
// To be noted: in SHIPPING mode, the debug guards are simply compiled out. In all other cases, the guards will
// create a small object on the stack. The disabled ones simply won't output to the debug console.

#define func_guard_import_cat(cat) \
	extern NppDebug::FuncGuard::State __func_guard_category_##cat
#define func_guard_enable_cat(cat) \
	NppDebug::FuncGuard::State __func_guard_category_##cat = NppDebug::FuncGuard::Enabled
#define func_guard_disable_cat(cat) \
	NppDebug::FuncGuard::State __func_guard_category_##cat = NppDebug::FuncGuard::Disabled
#define func_guard(cat) \
	NppDebug::FuncGuard __npp_func_guard__( TEXT(__FUNCSIG__), \
											TEXT(__FUNCTION__), \
											TEXT(__FILE__), \
											__LINE__, \
											__func_guard_category_##cat, \
											TEXT( #cat ))

// guard_debugf() acts just like debugf() described above, with the exception that it will indent the comment
// to match the current indentation of the function guards, if enabled. Otherwise, it will act just as a normal
// debugf().
//
// For example, given this:
//
// void FooOne()
// {
//     func_guard(foo);
//     int val = 42;
//     guard_debugf("Hello World! My favorite number is %d\n", val);
// }
// void FooTwo()
// {
//    func_guard(foo);
//    // ...
//    FooOne();
// }
//
// one would get the following output:
//
// file.cpp(42):
// Entering[ void __cdecl FooTwo(void) ]
//   file.cpp(21):
//   Entering[ void __cdecl FooOne(void) ]
//     Hello World! My favorite number is 42
//   Leaving[ FooOne ]
// Leaving[ FooTwo ]

#define guard_debugf(format, ...) \
	__npp_func_guard__.outputIndent(); \
	NppDebug::g_debugOutput->printf(format, __VA_ARGS__); \
	NppDebug::g_debugOutput->flush()

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
