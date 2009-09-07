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

namespace Debug
{
	void OutputF(TCHAR* format, ...)
	{
		va_list args;
		int len;
		TCHAR* buffer;

		va_start( args, format );
		len = _vsctprintf( format, args ) + 1; // _vscprintf doesn't count terminating '\0'
		buffer = new TCHAR[len];
		_vstprintf_s( buffer, len, format, args );
		OutputDebugString(buffer);
		delete [] buffer;
	}
} // namespace Debug

#endif // #ifndef SHIPPING