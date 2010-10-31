// This file is part of Notepad++ Community Release's FuncGuard project.
// Copyright (C)2009 The Notepad++ Community
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

///////////////////////////////////////////////////////////////////////////////
//
// All function guards should be listed here to make them available in any
// source.  The simplest method is by using the precompiled headers.
//
// When new functions guards are created they should be added to this file
// in alphabetical order.
//
//
// IT IS RECOMMENDED TO USE THE POWERSHELL FUNCGUARD MODULE TO KEEP THE RELATED
// FILES IN SYNCH AND PROPERLY ORDERED!
//
//
// Any desired documentation for the function guard belongs in FuncGuards.h
//
///////////////////////////////////////////////////////////////////////////////

/*
	Usage:
		- Entries in this file need to remain between the '#ifndef' and '#endif'
		  statements for FUNCGUARDIMPORT and should have no spacing between the
		  entries.

	Example:
		#ifndef MISC_DEBUG_FUNCGUARDIMPORT_H
		#define MISC_DEBUG_FUNCGUARDIMPORT_H

		func_guard_import_cat(guardClass1);
		func_guard_import_cat(guardClass1Class2_Communications);
		func_guard_import_cat(guardClass2);

		#endif // MISC_DEBUG_FUNCGUARDS_H
*/

#ifndef MISC_DEBUG_FUNCGUARDIMPORT_H
#define MISC_DEBUG_FUNCGUARDIMPORT_H

func_guard_import_cat(guardDllMain);

#endif // MISC_DEBUG_FUNCGUARDS_H
