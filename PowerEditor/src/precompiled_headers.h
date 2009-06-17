#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <dlgs.h>
#include <ShellAPI.h>
#include <dbghelp.h>

#include <exception>		//default C++ exception

#include <shlobj.h>
#include <shlwapi.h>

#include <Oleacc.h>

#include <uxtheme.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#ifdef UNICODE
#include <wchar.h>
#endif

// STL Headers
#include <algorithm>
#include <memory>
#include <iostream>
#include <functional>
#include <vector>
#include <deque>

#ifdef TIXMLA_USE_STL
#include <string>
#include <sstream>
#endif
