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
	namespace Debug
	{
		void OutputF(TCHAR* format, ...);
	}

	#define debugf(format, ...) Debug::OutputF(format, __VA_ARGS__)
	#define _debugf(format, ...) Debug::OutputF(TEXT("%s(%d): "), TEXT(__FILE__), __LINE__); Debug::OutputF(format, __VA_ARGS__)
#else
	#define debugf(format, ...) void(0)
	#define _debugf(format, ...) void(0)
#endif

#ifdef _DEBUG
	#ifdef NPPCR_DEBUG_NO_DEFAULT_CASE
		#define NO_DEFAULT_CASE default: {\
			Debug::OutputF(TEXT("%s(%d): %s"), TEXT(__FILE__), __LINE__, TEXT("Unhandled default case.\n"));\
			TCHAR errorMsg[ERROR_MSG_SIZE];\
			_stprintf_s(errorMsg, ERROR_MSG_SIZE, TEXT("Unhandled default case in %s, line %d\n\n\nDo you want to break?"), TEXT(__FILE__), __LINE__ );\
			int ret = ::MessageBox(NULL, errorMsg, TEXT("Unhandled default case."), MB_YESNO|MB_ICONWARNING);\
			if (ret == IDYES)\
			{\
				/* JOCE: could throw an exception instead and be properly tested. */ \
				DebugBreak(); \
			}\
		}\
		break
	#else
		#define NO_DEFAULT_CASE default: {\
		Debug::OutputF(TEXT("%s(%d): %s"), TEXT(__FILE__), __LINE__, TEXT("Unhandled default case.\n"));\
		}\
		break
	#endif
#else
	#define NO_DEFAULT_CASE default: break
#endif

#endif // NPP_DEBUG_H
