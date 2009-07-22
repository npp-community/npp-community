//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
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

#ifndef FIND_REPLACE_DLG_H
#define FIND_REPLACE_DLG_H

#include "StaticDialog.h"
#include "FindReplaceDlg_rc.h"
#include "BufferID.h"
#include "npp_winver.h"

// Forward declarations
class Finder;
class Searching;

class ScintillaEditView;
class TabBar;
class ReBar;

#define FIND_RECURSIVE 1
#define FIND_INHIDDENDIR 2

#define FINDREPLACE_MAXLENGTH 2048

enum DIALOG_TYPE {FIND_DLG, REPLACE_DLG, FINDINFILES_DLG};

#define DIR_DOWN true
#define DIR_UP false

//#define FIND_REPLACE_STR_MAX 256

enum InWhat{ALL_OPEN_DOCS, FILES_IN_DIR, CURRENT_DOC};

struct TargetRange {
	int targetStart;
	int targetEnd;
};

enum SearchIncrementalType { NotIncremental, FirstIncremental, NextIncremental };
enum SearchType { FindNormal, FindExtended, FindRegex };
enum ProcessOperation { ProcessFindAll, ProcessReplaceAll, ProcessCountAll, ProcessMarkAll, ProcessMarkAll_2, ProcessMarkAll_IncSearch, ProcessMarkAllExt };

struct FindOption {
	bool _isWholeWord;
	bool _isMatchCase;
	bool _isWrapAround;
	bool _whichDirection;
	SearchIncrementalType _incrementalType;
	SearchType _searchType;
	FindOption() :_isWholeWord(true), _isMatchCase(true), _searchType(FindNormal),\
		_isWrapAround(true), _whichDirection(DIR_DOWN), _incrementalType(NotIncremental){};
};

enum FindStatus { FSFound, FSNotFound, FSTopReached, FSEndReached};
//FindReplaceDialog: standard find/replace window
class FindReplaceDlg : public StaticDialog
{
friend class FindIncrementDlg;
public :
	FindReplaceDlg();
	~FindReplaceDlg();

	virtual void destroy();

	void init(HINSTANCE hInst, HWND hPere, ScintillaEditView **ppEditView) {
		Window::init(hInst, hPere);
		if (!ppEditView)
			throw int(9900);
		_ppEditView = ppEditView;
	};

	virtual void create(int dialogID, bool isRTL = false);

	void initOptionsFromDlg()	{
		_options._isWholeWord = isCheckedOrNot(IDWHOLEWORD);
		_options._isMatchCase = isCheckedOrNot(IDMATCHCASE);
		_options._searchType = isCheckedOrNot(IDREGEXP)?FindRegex:isCheckedOrNot(IDEXTENDED)?FindExtended:FindNormal;
		_options._isWrapAround = isCheckedOrNot(IDWRAP);
		_isInSelection = isCheckedOrNot(IDC_IN_SELECTION_CHECK);

		_doPurge = isCheckedOrNot(IDC_PURGE_CHECK);
		_doMarkLine = isCheckedOrNot(IDC_MARKLINE_CHECK);
		_doStyleFoundToken = isCheckedOrNot(IDC_STYLEFOUND_CHECK);

		::EnableWindow(::GetDlgItem(_hSelf, IDCMARKALL), (_doMarkLine || _doStyleFoundToken));
	};

	void doDialog(DIALOG_TYPE whichType, bool isRTL = false) {
		if (!isCreated())
		{
			create(IDD_FIND_REPLACE_DLG, isRTL);
			_isRTL = isRTL;
		}

		if (whichType == FINDINFILES_DLG)
			enableFindInFilesFunc();
		else
			enableReplaceFunc(whichType == REPLACE_DLG);

		::SetFocus(::GetDlgItem(_hSelf, IDFINDWHAT));
		display();
	};
	bool processFindNext(const TCHAR *txt2find, FindOption *options = NULL, FindStatus *oFindStatus = NULL);
	bool processReplace(const TCHAR *txt2find, const TCHAR *txt2replace, FindOption *options = NULL);

	int markAll(const TCHAR *txt2find, int styleID);
	int markAll2(const TCHAR *str2find);
	int markAllInc(const TCHAR *str2find, FindOption *opt);


	int processAll(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, bool isEntire = false, const TCHAR *fileName = NULL, FindOption *opt = NULL, int colourStyleID = -1);
	int processRange(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, int startRange, int endRange, const TCHAR *fileName = NULL, FindOption *opt = NULL, int colourStyleID = -1);
	void replaceAllInOpenedDocs();
	void findAllIn(InWhat op);

	void setSearchText(TCHAR * txt2find) {
		HWND hCombo = ::GetDlgItem(_hSelf, IDFINDWHAT);
		if (txt2find && txt2find[0])
		{
			// We got a valid search string
			::SendMessage(hCombo, CB_SETCURSEL, (WPARAM)-1, 0); // remove selection - to allow using down arrow to get to last searched word
			::SetDlgItemText(_hSelf, IDFINDWHAT, txt2find);
		}
		::SendMessage(hCombo, CB_SETEDITSEL, 0, MAKELPARAM(0, -1)); // select all text - fast edit
	}
	void gotoNextFoundResult(int direction = 0);;

	void putFindResult(int result) {
		_findAllResult = result;
	};
	const TCHAR * getDir2Search() const {return _directory.c_str();};

	void getPatterns(std::vector<std::generic_string> & patternVect);

	void launchFindInFilesDlg() {
		doDialog(FINDINFILES_DLG);
	};

	void setFindInFilesDirFilter(const TCHAR *dir, const TCHAR *filters) {
		if (dir)
		{
			_directory = dir;
			::SetDlgItemText(_hSelf, IDD_FINDINFILES_DIR_COMBO, dir);
		}
		if (filters)
		{
			_filters = filters;
			::SetDlgItemText(_hSelf, IDD_FINDINFILES_FILTERS_COMBO, filters);
		}
	};

	std::generic_string getText2search() const {
		return getTextFromCombo(::GetDlgItem(_hSelf, IDFINDWHAT));
	};

	const std::generic_string & getFilters() const {return _filters;};
	const std::generic_string & getDirectory() const {return _directory;};
	const FindOption & getCurrentOptions() const {return _options;};
	bool isRecursive() const { return _isRecursive; };
	bool isInHiddenDir() const { return _isInHiddenDir; };
	void saveFindHistory();
	void changeTabName(DIALOG_TYPE index, const TCHAR *name2change);
	void beginNewFilesSearch();

	void finishFilesSearch(int count);

	void focusOnFinder();;

	HWND getHFindResults();

	void updateFinderScintilla();;

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void addText2Combo(const TCHAR * txt2add, HWND comboID, bool isUTF8 = false);
	std::generic_string getTextFromCombo(HWND hCombo, bool isUnicode = false) const;
	static LONG originalFinderProc;

	// Window procedure for the finder
	static LRESULT FAR PASCAL finderProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void combo2ExtendedMode(int comboID);

private :
	DIALOG_TYPE _currentStatus;
	FindOption _options;

	bool _doPurge;
	bool _doMarkLine;
	bool _doStyleFoundToken;
	bool _isInSelection;


	RECT _findClosePos, _replaceClosePos, _findInFilesClosePos;

	ScintillaEditView **_ppEditView;
	Finder  *_pFinder;
	bool _isRTL;

	int _findAllResult;
	TCHAR _findAllResultStr[1024];

	std::generic_string _filters;
	std::generic_string _directory;
	bool _isRecursive;
	bool _isInHiddenDir;

	int _fileNameLenMax;
	char *_uniFileName;

	TabBar* _tab;
	winVer _winVer;

	void enableReplaceFunc(bool isEnable);
	void enableFindInFilesControls(bool isEnable = true);
	void enableFindInFilesFunc();;

	//////////////////

	void setDefaultButton(int nID)
	{
#if 0
		// There is a problem when you:
		// 1. open the find dialog
		// 2. press the "close" buttom
		// 3. open it again
		// 4. search for a non existing text
		// 5. when the "Can't find the text:" messagebox appears, hit "OK"
		// 6. now, the "Close" button looks like the default button. (but it only looks like that)
		//    if you hit "Enter" the "Find" button will be "pressed".
		// I thought this code might fix this but it doesn't
		// See: http://msdn.microsoft.com/en-us/library/ms645413(VS.85).aspx

		HWND pButton;
		DWORD dwDefInfo = SendMessage(_hSelf, DM_GETDEFID, 0, 0L);
		if (HIWORD(dwDefInfo) == DC_HASDEFID && (int)LOWORD(dwDefInfo) != nID)
		{
			// Reset 'DefButton' style
			pButton = GetDlgItem(_hSelf, (int)LOWORD(dwDefInfo));
			if (pButton)
				SendMessage(pButton, BM_SETSTYLE, LOWORD(BS_PUSHBUTTON | BS_RIGHT ), MAKELPARAM(TRUE, 0));
		}

		SendMessage(_hSelf, DM_SETDEFID, (WPARAM)nID, 0L);
		pButton = GetDlgItem(_hSelf, nID);
		if (pButton)
		{
			SendMessage(pButton, BM_SETSTYLE, LOWORD(BS_DEFPUSHBUTTON), MAKELPARAM(TRUE, 0));
		}
#endif
		SendMessage(_hSelf, DM_SETDEFID, (WPARAM)nID, 0L);
	}
	////////////////////////

	void gotoCorrectTab();;

	bool isCheckedOrNot(int checkControlID) const {
		return (BST_CHECKED == ::SendMessage(::GetDlgItem(_hSelf, checkControlID), BM_GETCHECK, 0, 0));
	};

	void updateCombos();
	void updateCombo(int comboID);;
	void fillFindHistory();
	void fillComboHistory(int id, int count, std::generic_string **pStrings);
	void saveComboHistory(int id, int maxcount, int& oldcount, std::generic_string **pStrings);
};

//FindIncrementDlg: incremental search dialog, docked in rebar
class FindIncrementDlg : public StaticDialog
{
public :
	FindIncrementDlg() : _pFRDlg(NULL), _pRebar(NULL) {};
	void init(HINSTANCE hInst, HWND hPere, FindReplaceDlg *pFRDlg, bool isRTL = false) {
		Window::init(hInst, hPere);
		if (!pFRDlg)
			throw int(9910);
		_pFRDlg = pFRDlg;
		create(IDD_INCREMENT_FIND, isRTL);
		_isRTL = isRTL;
	};
	virtual void destroy();
	virtual void display(bool toShow = true) const;

	void setSearchText(const TCHAR * txt2find, bool isUTF8 = false);
	void setFindStatus(FindStatus iStatus) {
		static TCHAR *findStatus[] = { TEXT(""), // FSFound
		                               TEXT("Phrase not found"), //FSNotFound
		                               TEXT("Reached top of page, continued from bottom"), // FSTopReached
		                               TEXT("Reached end of page, continued from top")}; // FSEndReached
		if (iStatus<0 || iStatus >= sizeof(findStatus)/sizeof(findStatus[0]))
			return; // out of range
		::SendDlgItemMessage(_hSelf, IDC_INCFINDSTATUS, WM_SETTEXT, 0, (LPARAM)findStatus[iStatus]);
	}

	void addToRebar(ReBar * rebar);
private :
	bool _isRTL;
	FindReplaceDlg *_pFRDlg;

	ReBar * _pRebar;
	REBARBANDINFO _rbBand;

	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void markSelectedTextInc(bool enable, FindOption *opt = NULL);
};

#endif //FIND_REPLACE_DLG_H
