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

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include <windowsx.h>
#include <imm.h>

#include <commctrl.h>
#include <zmouse.h>
#include <ole2.h>

#if _MSC_VER >= 1300
#include <basetsd.h>
#endif

// Standard headers.
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

// STL
#include <new>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#endif // PRECOMPILED_HEADERS_H

