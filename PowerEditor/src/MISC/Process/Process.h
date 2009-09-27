//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef PROCESSUS_H
#define PROCESSUS_H

enum progType {WIN32_PROG, CONSOLE_PROG};

class Process
{
public:
    Process(progType pt = WIN32_PROG);
    Process(const TCHAR *cmd, const TCHAR *args, const TCHAR *cDir, progType pt = WIN32_PROG);

    BOOL run();

	const TCHAR * getStdout() const {
		return _stdoutStr.c_str();
	};

	const TCHAR * getStderr() const {
		return _stderrStr.c_str();
	};

	int getExitCode() const {
		return _exitCode;
	};

	bool hasStdout() {
		return _stdoutStr != TEXT("");
	};

	bool hasStderr() {
		return _stderrStr != TEXT("");
	};

protected:
    progType _type;

	// INPUTS
    generic_string _command;
	generic_string _args;
	generic_string _curDir;

	// OUTPUTS
	generic_string _stdoutStr;
	generic_string _stderrStr;
	int _exitCode;

	// HANDLES
    HANDLE _hPipeOutR;
	HANDLE _hPipeErrR;
	HANDLE _hProcess;
	HANDLE _hProcessThread;

	BOOL	_bProcessEnd;

	static DWORD WINAPI staticListenerStdOut(void * myself){
		((Process *)myself)->listenerStdOut();
		return 0;
	};
	static DWORD WINAPI staticListenerStdErr(void * myself) {
		((Process *)myself)->listenerStdErr();
		return 0;
	};
	static DWORD WINAPI staticWaitForProcessEnd(void * myself) {
		((Process *)myself)->waitForProcessEnd();
		return 0;
	};

	void listenerStdOut();
	void listenerStdErr();
	void waitForProcessEnd();

	void error(const TCHAR *txt2display, BOOL & returnCode, int errCode);
};

#endif //PROCESSUS_H

