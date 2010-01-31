// This file was added to the Scintilla project.
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

#ifndef PRECOMPILED_HEADERS_H
#define PRECOMPILED_HEADERS_H

#include "targetver.h"

// Allow to track emplacement where leaked memory was allocated.
#define _CRTDBG_MAP_ALLOC

// C RunTime Header Files

// The next two files need to stay in that order, and need to be the
// first system includes of this file.
#include <stdlib.h>
#include <crtdbg.h>

#include <tchar.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#if _MSC_VER >= 1300
#include <basetsd.h>
#endif

// STL
#include <new>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

// Needed to be able to detect the memory leaks created by calls to 'new'
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include <windowsx.h>
#include <imm.h>

#include <zmouse.h>
#include <ole2.h>

// NPPCR
#include "MISC/Common/CommonGenerics.h"
#include "MISC/Debug/npp_debug.h"
#ifndef SHIPPING
	#include "MISC/Debug/FuncGuardsImport.h"
#endif

// Scintilla specific
#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

// Lexlib
#include "LexAccessor.h"
#include "Accessor.h"
#include "CharacterSet.h"
#include "PropSetSimple.h"
#include "LexerBase.h"
#include "LexerModule.h"
#include "LexerNoExceptions.h"
#include "LexerSimple.h"
#include "OptionSet.h"
#include "StyleContext.h"
#include "WordList.h"

// Google Test
#ifndef SHIPPING
	// Sadly, Google Test compiles at /W3 and has /W4 warnings for now.
	#pragma warning (push, 3)
	#include "gtest/gtest.h"
	#pragma warning (pop)
#endif

#endif // PRECOMPILED_HEADERS_H

