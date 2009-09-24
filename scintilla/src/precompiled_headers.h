#pragma once
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
#include <BaseTsd.h>
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
#include <vector>
