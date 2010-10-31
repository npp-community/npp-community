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
// Uncomment/Comment out the desired guard definitions to enable/disable.  Use
// the provided func_guard() statement in the functions you want to trace.
//
// When new functions guards are created they should be added to this file
// in alphabetical order.
//
//
// IT IS RECOMMENDED TO USE THE POWERSHELL FUNCGUARD MODULE TO KEEP THE RELATED
// FILES IN SYNCH AND PROPERLY ORDERED!
//
//
///////////////////////////////////////////////////////////////////////////////

/*
	Usage:
	This skeleton file should be copied to FuncGuards.h by a pre-build event when
	FuncGuards.h doesn't exist.  It serves as your projects 'tracked' version of
	available function guards.

	ALL FUNCTION GUARDS SHOULD BE DISABLED (commented out) IN THIS FILE

	Notes:
		- Every define and usage needs to start with '//'

		- Cat is CamelCase with '_' spacing between either the filename or major
		  class category followed by a subclass or particular function area if
		  needed.

		- This is the validation regex used by the Powershell FuncGuard module:

			$Name -cmatch "^(\p{Lu}\p{Ll}+)+($|_((\p{Lu}\p{Ll}+)|\d)+|\d+)+$"

		- Entries in this file need to remain between the '#ifndef SHIPPING' and
		  '#endif // SHIPPING' statements and should have a single line before and
		  after the entry.

		- The format of the entries need to follow one of the following two
		  examples:
*/

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
// #define FUNCGUARD_PSUNIT_CLASS1

// func_guard(guardClass1Class2_Communications);
// Description: For tracing TestGuard1 and TestGuard2 communication.
/*
	This enables tracing of _only_ the communication functions of the
	TestGuard1 and TestGuard2 classes.

	Modify the local function area to filter messages, to output use:
		func_guard_debugf_cat(guardTestGuard_MultiClass);

	Enable by adding to the current func_guard(#cat) statement.
		example: func_guard(guardClass1 | guardClass1Class2_Communications);

	If both TestGuard1 and TestGuard2 are active then enabling this is not
	needed, but doesn't hurt anything.
*/
// #define FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardDllMain);
// Description: Traces main entry point of SciLexer.dll
//#define FUNCGUARD_NPPCR_DLLMAIN

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
