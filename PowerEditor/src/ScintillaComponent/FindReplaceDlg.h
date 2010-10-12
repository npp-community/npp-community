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

#ifndef SCINTILLACOMPONENT_FINDREPLACEDLG_H
#define SCINTILLACOMPONENT_FINDREPLACEDLG_H

#ifndef WINCONTROLS_STATICDIALOG_STATICDIALOG_H
#include "WinControls/StaticDialog/StaticDialog.h"
#endif

#ifndef SCINTILLACOMPONENT_BUFFERID_H
#include "ScintillaComponent/BufferID.h"
#endif

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

struct FindOption
{
	bool _isWholeWord;
	bool _isMatchCase;
	bool _isWrapAround;
	bool _whichDirection;
	SearchIncrementalType _incrementalType;
	SearchType _searchType;
	bool _doPurge;
	bool _doMarkLine;
	bool _doStyleFoundToken;
	bool _isInSelection;
	generic_string _str2Search;
	generic_string _str4Replace;
	generic_string _filters;
	generic_string _directory;
	bool _isRecursive;
	bool _isInHiddenDir;
	FindOption() :
		_isWholeWord(true),
		_isMatchCase(true),
		_isWrapAround(true),
		_whichDirection(DIR_DOWN),
		_incrementalType(NotIncremental),
		_searchType(FindNormal),
		_doPurge(false),
		_doMarkLine(false),
		_doStyleFoundToken(false),
		_isInSelection(false),
		_isRecursive(true),
		_isInHiddenDir(false)
	{}
};

enum FindStatus { FSFound, FSNotFound, FSTopReached, FSEndReached};

class FindReplaceDlg : public StaticDialog
{
friend class FindIncrementDlg;
public :
	static FindOption _options;
	static FindOption* _env;
	FindReplaceDlg();
	~FindReplaceDlg();

	virtual void destroy();

	void init(HINSTANCE hInst, HWND hParent, ScintillaEditView **ppEditView);

	virtual void create(int dialogID, bool isRTL = false);

	void initOptionsFromDlg();

	void doDialog(DIALOG_TYPE whichType, bool isRTL = false, bool toShow = true);
	bool processFindNext(const TCHAR *txt2find, const FindOption *options = NULL, FindStatus *oFindStatus = NULL);
	bool processReplace(const TCHAR *txt2find, const TCHAR *txt2replace, const FindOption *options = NULL);

	int markAll(const TCHAR *txt2find, int styleID);
	//int markAll2(const TCHAR *str2find);
	int markAllInc(const FindOption *opt);


	int processAll(ProcessOperation op, const FindOption *opt, bool isEntire = false, const TCHAR *fileName = NULL, int colourStyleID = -1);
//	int processAll(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, bool isEntire = false, const TCHAR *fileName = NULL, const FindOption *opt = NULL, int colourStyleID = -1);
	int processRange(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, int startRange, int endRange, const TCHAR *fileName = NULL, const FindOption *opt = NULL, int colourStyleID = -1);
	void replaceAllInOpenedDocs();
	void findAllIn(InWhat op);
	void setSearchText(TCHAR * txt2find);
	void gotoNextFoundResult(int direction = 0);

	void putFindResult(int result) {
		_findAllResult = result;
	};
	const TCHAR * getDir2Search() const {return _env->_directory.c_str();}

	void getPatterns(std::vector<generic_string> & patternVect);

	void launchFindInFilesDlg() {
		doDialog(FINDINFILES_DLG);
	};

	void setFindInFilesDirFilter(const TCHAR *dir, const TCHAR *filters);

	generic_string getText2search() const;

	const generic_string & getFilters() const {return _env->_filters;};
	const generic_string & getDirectory() const {return _env->_directory;};
	const FindOption & getCurrentOptions() const {return *_env;};
	bool isRecursive() const { return _env->_isRecursive; };
	bool isInHiddenDir() const { return _env->_isInHiddenDir; };
	void saveFindHistory();
	void changeTabName(DIALOG_TYPE index, const TCHAR *name2change);
	void beginNewFilesSearch();

	void finishFilesSearch(int count);

	void focusOnFinder();

	HWND getHFindResults();

	void updateFinderScintilla();
	void execSavedCommand(int cmd, int intValue, const generic_string& stringValue);

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void addText2Combo(const TCHAR * txt2add, HWND comboID, bool isUTF8 = false);
	generic_string getTextFromCombo(HWND hCombo, bool isUnicode = false) const;
	static LONG originalFinderProc;

	// Window procedure for the finder
	static LRESULT FAR PASCAL finderProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void combo2ExtendedMode(int comboID);

private :

	DIALOG_TYPE _currentStatus;
	RECT _findClosePos, _replaceClosePos, _findInFilesClosePos;

	ScintillaEditView **_ppEditView;
	Finder  *_pFinder;
	bool _isRTL;

	int _findAllResult;

	// JOCE either remove the magic number, or use a std::string if possible.
	TCHAR _findAllResultStr[1024];

	int _fileNameLenMax;
	char *_uniFileName;

	TabBar* _tab;

	void enableReplaceFunc(bool isEnable);
	void enableFindInFilesControls(bool isEnable = true);
	void enableFindInFilesFunc();

	void setDefaultButton(int nID);

	void gotoCorrectTab();

	bool isCheckedOrNot(int checkControlID) const {
		return (BST_CHECKED == ::SendMessage(::GetDlgItem(_hSelf, checkControlID), BM_GETCHECK, 0, 0));
	};

	void updateCombos();
	void updateCombo(int comboID);
	void fillFindHistory();
    void fillComboHistory(int id, const std::vector<generic_string> & strings);
    int saveComboHistory(int id, int maxcount, std::vector<generic_string> & strings);
	static const int FR_OP_FIND = 1;
	static const int FR_OP_REPLACE = 2;
	static const int FR_OP_FIF = 4;
	static const int FR_OP_GLOBAL = 8;
	void saveInMacro(int cmd, int cmdType);
	HWND _hMsgParent;
};

//FindIncrementDlg: incremental search dialog, docked in rebar
class FindIncrementDlg : public StaticDialog
{
public :
	FindIncrementDlg() : _pFRDlg(NULL), _pRebar(NULL), _FindStatus(FSFound) {};
	~FindIncrementDlg();
	void init(HINSTANCE hInst, HWND hParent, FindReplaceDlg *pFRDlg, bool isRTL = false);
	virtual void destroy();
	virtual void display(bool toShow = true) const;

	void setSearchText(const TCHAR * txt2find, bool isUTF8 = false);

	void setFindStatus(FindStatus iStatus);
	FindStatus getFindStatus() {
		return _FindStatus;
	}

	void addToRebar(ReBar * rebar);
private :
	bool _isRTL;
	FindReplaceDlg *_pFRDlg;
	FindStatus _FindStatus;

	ReBar * _pRebar;
	REBARBANDINFO _rbBand;

	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void markSelectedTextInc(bool enable, FindOption *opt = NULL);
};

#endif //SCINTILLACOMPONENT_FINDREPLACEDLG_H
