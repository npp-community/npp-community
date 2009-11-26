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

#ifndef PRECOMPILEDHEADERS_H
#define PRECOMPILEDHEADERS_H

#include "targetver.h"

// Allow to track emplacement where leaked memory was allocated.
#define _CRTDBG_MAP_ALLOC

// C RunTime Header Files
// The next two files need to stay in that order, and need to be the
// first system includes of this file.
#include <stdlib.h>
#include <crtdbg.h>

#include <tchar.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <time.h>

// STL Headers
#include <algorithm>
#include <memory>
#include <iostream>
#include <functional>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <sstream>

// Needed to be able to detect the memory leaks created by calls to 'new'
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <dlgs.h>
#include <ShellAPI.h>
#include <dbghelp.h>
#include <eh.h>
#include <exception>		//default C++ exception
#include <shlobj.h>
#include <shlwapi.h>
#include <Oleacc.h>
#include <uxtheme.h>

// Google Test
#ifndef SHIPPING
	// Sadly, Google Test compiles at /W3 and has /W4 warnings for now.
	#pragma warning (push, 3)
	#include "gtest/gtest.h"
	#pragma warning (pop)
#endif

// Scintilla
#include "Scintilla.h"
#include "SciLexer.h"

// Power Editor
#include "MISC/Common/Common.h"

#endif // PRECOMPILEDHEADERS_H
