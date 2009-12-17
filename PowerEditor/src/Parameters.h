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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#ifndef SCINTILLA_REF_H
#include "ScintillaRef.h"
#endif

#ifndef TOOL_BAR_H
#include "ToolBar.h"
#endif

#ifndef USER_DEFINE_LANG_REFERENCE_H
#include "UserDefineLangReference.h"
#endif

#ifndef SHORTCUTS_H
#include "shortcut.h"
#endif

#ifndef CONTEXTMENU_H
#include "ContextMenu.h"
#endif

#ifndef NOTEPAD_PLUS_MSGS_H
#include "Notepad_plus_msgs.h"
#endif

#ifndef PARAMETERS_DEF_H
#include "Parameters_def.h"
#endif

// JOCE: this needs to be taken out at some point...
#ifndef NPP_STYLES
#include "npp_styles.h"
#endif

// JOCE: this needs to be taken out at some point...
#ifndef NPP_DATE
#include "npp_date.h"
#endif

// JOCE: this needs to be taken out at some point...
#ifndef USER_DEFINE_DIALOG_H
#include "UserDefineDialog.h"
#endif

// Forward declarations
class TiXmlDocument;
class TiXmlDocumentA;
class TiXmlElement;

struct Session;

void cutString(const generic_string& str2cut, std::vector<generic_string> & patternVect);
/*
struct HeaderLineState {
	HeaderLineState() : _headerLineNumber(0), _isCollapsed(false){};
	HeaderLineState(int lineNumber, bool isFoldUp) : _headerLineNumber(lineNumber), _isCollapsed(isFoldUp){};
	int _headerLineNumber;
	bool _isCollapsed;
};
*/
struct CmdLineParams {
	bool _isNoPlugin;
	bool _isReadOnly;
	bool _isNoSession;
	bool _isNoTab;
	bool _isPreLaunch;

	int _line2go;
    int _column2go;

    POINT _point;
	bool _isPointXValid;
	bool _isPointYValid;
	bool isPointValid() {
		return _isPointXValid && _isPointXValid;
	};

	LangType _langType;
	CmdLineParams() : _isNoPlugin(false), _isReadOnly(false), _isNoSession(false), _isNoTab(false),
        _line2go(-1), _column2go(-1), _isPointXValid(false), _isPointYValid(false), _langType(L_EXTERNAL)
    {
        _point.x = 0;
        _point.y = 0;
    }
};

struct FloatingWindowInfo {
	int _cont;
	RECT _pos;
	FloatingWindowInfo(int cont, int x, int y, int w, int h) : _cont(cont) {
		_pos.left	= x;
		_pos.top	= y;
		_pos.right	= w;
		_pos.bottom = h;
	};
};

struct PlugingDlgDockingInfo {
	generic_string _name;
	int _internalID;

	int _currContainer;
	int _prevContainer;
	bool _isVisible;

	PlugingDlgDockingInfo(const TCHAR *pluginName, int id, int curr, int prev, bool isVis) : _name(pluginName), _internalID(id), _currContainer(curr), _prevContainer(prev), _isVisible(isVis){};

	friend inline const bool operator==(const PlugingDlgDockingInfo & a, const PlugingDlgDockingInfo & b) {
		if ((a._name == b._name) && (a._internalID == b._internalID))
			return true;
		else
			return false;
	};
};

struct ContainerTabInfo {
	int _cont;
	int _activeTab;

	ContainerTabInfo(int cont, int activeTab) : _cont(cont), _activeTab(activeTab) {};
};

struct DockingManagerData {
	int _leftWidth;
	int _rightWidth;
	int _topHeight;
	int _bottomHight;

	DockingManagerData() : _leftWidth(200), _rightWidth(200), _topHeight(200), _bottomHight(200) {};

	std::vector<FloatingWindowInfo>		_flaotingWindowInfo;
	std::vector<PlugingDlgDockingInfo>	_pluginDockInfo;
	std::vector<ContainerTabInfo>		_containerTabInfo;

	RECT * getFloatingRCFrom(int floatCont) {
		for (size_t i = 0 ; i < _flaotingWindowInfo.size() ; i++)
		{
			if (_flaotingWindowInfo[i]._cont == floatCont)
				return &(_flaotingWindowInfo[i]._pos);
		}
		return NULL;
	}
};

struct NewDocDefaultSettings
{
	formatType _format;
	UniMode _encoding;
	bool _openAnsiAsUtf8;
	LangType _lang;
	NewDocDefaultSettings():_format(WIN_FORMAT), _encoding(uni8Bit), _openAnsiAsUtf8(false), _lang(L_TXT){};
};

struct LangMenuItem {
	LangType _langType;
	int	_cmdID;
	generic_string _langName;

	LangMenuItem(LangType lt, int cmdID = 0, generic_string langName = TEXT("")):
	_langType(lt), _cmdID(cmdID), _langName(langName){};
};

struct PrintSettings {
	bool _printLineNumber;
	int _printOption;

	generic_string _headerLeft;
	generic_string _headerMiddle;
	generic_string _headerRight;
	generic_string _headerFontName;
	int _headerFontStyle;
	int _headerFontSize;

	generic_string _footerLeft;
	generic_string _footerMiddle;
	generic_string _footerRight;
	generic_string _footerFontName;
	int _footerFontStyle;
	int _footerFontSize;

	RECT _marge;

	PrintSettings() : _printLineNumber(true), _printOption(SC_PRINT_NORMAL), _headerLeft(TEXT("")), _headerMiddle(TEXT("")), _headerRight(TEXT("")),\
		_headerFontName(TEXT("")), _headerFontStyle(0), _headerFontSize(0),  _footerLeft(TEXT("")), _footerMiddle(TEXT("")), _footerRight(TEXT("")),\
		_footerFontName(TEXT("")), _footerFontStyle(0), _footerFontSize(0) {
			_marge.left = 0; _marge.top = 0; _marge.right = 0; _marge.bottom = 0;
		};

	bool isHeaderPresent() const {
		return ((_headerLeft != TEXT("")) || (_headerMiddle != TEXT("")) || (_headerRight != TEXT("")));
	};

	bool isFooterPresent() const {
		return ((_footerLeft != TEXT("")) || (_footerMiddle != TEXT("")) || (_footerRight != TEXT("")));
	};

	bool isUserMargePresent() const {
		return ((_marge.left != 0) || (_marge.top != 0) || (_marge.right != 0) || (_marge.bottom != 0));
	};
};

struct NppGUI
{
	NppGUI() :
		_toolBarStatus(TB_LARGE), _toolbarShow(true), _statusBarShow(true), _menuBarShow(true),
		_tabStatus(TAB_DRAWTOPBAR | TAB_DRAWINACTIVETAB | TAB_DRAGNDROP), _splitterPos(POS_HORIZOTAL),
		_userDefineDlgStatus(UDD_DOCKED), _tabSize(8), _tabReplacedBySpace(false), _fileAutoDetection(cdEnabled),
		_fileAutoDetectionOriginalValue(_fileAutoDetection), _checkHistoryFiles(true), _isMaximized(false), _isMinimizedToTray(false),
		_rememberLastSession(true), _enableMouseWheelZoom(true),  _doTaskList(true), _maitainIndent(true), _enableSmartHilite(true),
		_disableSmartHiliteTmp(false),_enableTagsMatchHilite(true), _enableTagAttrsHilite(true), _enableHiliteNonHTMLZone(false), 
		_styleMRU(true), _styleURL(0), _isLangMenuCompact(false), _backup(bak_none), _useDir(false), _autocStatus(autoc_none), 
		_autocFromLen(1), _funcParams(false), _doesExistUpdater(false), _caretBlinkRate(250), _caretWidth(1),
		_enableMultiSelection(false), _shortTitlebar(false), _openSaveDir(dir_followCurrent)
	{
		_appPos.left = 0;
		_appPos.top = 0;
		_appPos.right = 700;
		_appPos.bottom = 500;

		_backupDir[0] = '\0';
		_defaultDir[0] = '\0';
		_defaultDirExp[0] = '\0';
	}

	toolBarStatusType _toolBarStatus;		// small, large or standard
	bool _toolbarShow;
	bool _statusBarShow;		// show or hide
	bool _menuBarShow;

	// 1st bit : draw top bar;
	// 2nd bit : draw inactive tabs
	// 3rd bit : enable drag & drop
	// 4th bit : reduce the height
	// 5th bit : enable vertical
	// 6th bit : enable multiline

	// 0:don't draw; 1:draw top bar 2:draw inactive tabs 3:draw both 7:draw both+drag&drop
	int _tabStatus;

	bool _splitterPos;			// horizontal or vertical
	int _userDefineDlgStatus;	// (hide||show) && (docked||undocked)

	int _tabSize;
	bool _tabReplacedBySpace;

	ChangeDetect _fileAutoDetection;
	ChangeDetect _fileAutoDetectionOriginalValue;
	bool _checkHistoryFiles;

	RECT _appPos;

	bool _isMaximized;
	bool _isMinimizedToTray;
	bool _rememberLastSession;
	bool _enableMouseWheelZoom;
	bool _doTaskList;
	bool _maitainIndent;
	bool _enableSmartHilite;
	bool _disableSmartHiliteTmp;
	bool _enableTagsMatchHilite;
	bool _enableTagAttrsHilite;
	bool _enableHiliteNonHTMLZone;
	bool _styleMRU;

	// 0 : do nothing
	// 1 : don't draw underline
	// 2 : draw underline
	int _styleURL;

	NewDocDefaultSettings _newDocDefaultSettings;
	void setTabReplacedBySpace(bool b) {_tabReplacedBySpace = b;};
	const NewDocDefaultSettings & getNewDocDefaultSettings() const {return _newDocDefaultSettings;};
	std::vector<LangMenuItem> _excludedLangList;
	bool _isLangMenuCompact;

	PrintSettings _printSettings;
	BackupFeature _backup;
	bool _useDir;
	generic_string _backupDir;
	DockingManagerData _dockingData;
	GlobalOverride _globalOverride;
	enum AutocStatus{autoc_none, autoc_func, autoc_word};
	AutocStatus _autocStatus;
	size_t  _autocFromLen;
	bool _funcParams;

	generic_string _definedSessionExt;

    struct AutoUpdateOptions {
        bool _doAutoUpdate;
        int _intervalDays;
        Date _nextUpdateDate;
        AutoUpdateOptions(): _doAutoUpdate(true), _intervalDays(15), _nextUpdateDate(Date()) {};
    } _autoUpdateOpt;

	bool _doesExistUpdater;
	int _caretBlinkRate;
	int _caretWidth;
    bool _enableMultiSelection;

	bool _shortTitlebar;

	OpenSaveDirSetting _openSaveDir;
	TCHAR _defaultDir[MAX_PATH];
	TCHAR _defaultDirExp[MAX_PATH];	//expanded environment variables
	generic_string _themeName;
};

struct ScintillaViewParams
{
	ScintillaViewParams() :
		_lineNumberMarginShow(true), _bookMarkMarginShow(true),
		_folderStyle(FOLDER_STYLE_BOX), _indentGuideLineShow(true),
	    _currentLineHilitingShow(true), _wrapSymbolShow(false), _doWrap(false),
	    _edgeMode(EDGE_NONE), _edgeNbColumn(0),
		_zoom(0), _whiteSpaceShow(false), _eolShow(false)
	{}

	bool _lineNumberMarginShow;
	bool _bookMarkMarginShow;
	//bool _docChangeStateMarginShow;
	folderStyle  _folderStyle; //"simple", TEXT("arrow"), TEXT("circle") and "box"
	bool _indentGuideLineShow;
	bool _currentLineHilitingShow;
	bool _wrapSymbolShow;
	bool _doWrap;
	int _edgeMode;
	int _edgeNbColumn;
	int _zoom;
	bool _whiteSpaceShow;
	bool _eolShow;

};

#define NB_LIST 20
// JOCE: There's no longer a reason to keep around the NB_MAX_* below now that we use std::vectors.
#define NB_MAX_LRF_FILE 30
#define NB_MAX_USER_LANG 30
#define NB_MAX_EXTERNAL_LANG 30
#define LANG_NAME_LEN 32

#define NB_MAX_FINDHISTORY_FIND    30
#define NB_MAX_FINDHISTORY_REPLACE 30
#define NB_MAX_FINDHISTORY_PATH    30
#define NB_MAX_FINDHISTORY_FILTER  20


#define MASK_ReplaceBySpc 0x80
#define MASK_TabSize 0x7F

struct Lang
{
	LangType _langID;
	generic_string _langName;
	const TCHAR *_defaultExtList;
	const TCHAR *_langKeyWordList[NB_LIST];
	const TCHAR *_pCommentLineSymbol;
	const TCHAR *_pCommentStart;
	const TCHAR *_pCommentEnd;
	bool _isTabReplacedBySpace;
	int _tabSize;

	Lang():
		_langID(L_TXT),
		_defaultExtList(NULL),
		_pCommentLineSymbol(NULL),
		_pCommentStart(NULL),
		_pCommentEnd(NULL),
		_isTabReplacedBySpace(false),
		_tabSize(-1)
	{
		for (int i = 0 ; i < NB_LIST ; i++)
		{
			_langKeyWordList[i] = NULL;
		}
	}

	Lang(LangType langID, const TCHAR *name) :
		_langID(langID),
		_defaultExtList(NULL),
		_pCommentLineSymbol(NULL),
		_pCommentStart(NULL),
		_pCommentEnd(NULL),
		_isTabReplacedBySpace(false),
		_tabSize(-1)
	{
		if (name)
			_langName = name;
		for (int i = 0 ; i < NB_LIST ; i++)
		{
			_langKeyWordList[i] = NULL;
		}
	};
	~Lang() {}
	void setDefaultExtList(const TCHAR *extLst){
		_defaultExtList = extLst;
	}

	void setCommentLineSymbol(const TCHAR *commentLine){
		_pCommentLineSymbol = commentLine;
	}

	void setCommentStart(const TCHAR *commentStart){
		_pCommentStart = commentStart;
	}

	void setCommentEnd(const TCHAR *commentEnd){
		_pCommentEnd = commentEnd;
	}

    void setTabInfo(int tabInfo) {
        if (tabInfo != -1 && tabInfo & MASK_TabSize)
        {
            _isTabReplacedBySpace = (tabInfo & MASK_ReplaceBySpc) != 0;
            _tabSize = tabInfo & MASK_TabSize;
        }
    };

	const TCHAR * getDefaultExtList() const {
		return _defaultExtList;
	}

	void setWords(const TCHAR *words, int index) {
		_langKeyWordList[index] = words;
	}

	const TCHAR * getWords(int index) const {
		return _langKeyWordList[index];
	}

	LangType getLangID() const {return _langID;}
	const TCHAR * getLangName() const {return _langName.c_str();}
    int getTabInfo() const {
        if (_tabSize == -1) return -1;
        return (_isTabReplacedBySpace?0x80:0x00) | _tabSize;
    }
};

class UserLangContainer
{
public :
	UserLangContainer();
	UserLangContainer(const TCHAR *name, const TCHAR *ext);

	const UserLangContainer & operator=(const UserLangContainer & ulc);

	int getNbKeywordList() const {return KWL_NB_KEYWORD_LISTS;}

	generic_string& getKeywordList(int idx){ assert(idx < KWL_NB_KEYWORD_LISTS); return _keywordLists[idx]; }
	const generic_string& getName() const { return _name; }
	const generic_string& getExtension() const { return _ext; }
	bool isCaseIgnored() const { return _isCaseIgnored; }
	bool isCommentLineSymbol() const { return _isCommentLineSymbol; }
	bool isCommentSymbol() const { return _isCommentSymbol; }
	bool isPrefix(int idx) const { assert(idx < nbPrefixListAllowed); return _isPrefix[idx]; }

	void setKeywordList(int idx, const generic_string& str){ assert(idx < KWL_NB_KEYWORD_LISTS); _keywordLists[idx] = str; }
	void setName(const generic_string& inName) { _name = inName; }
	void setExtension(const generic_string& inExt) { _ext = inExt; }
	void setIsCaseIgnored(bool caseIgnored) { _isCaseIgnored = caseIgnored; }
	void setIsCommentLineSymbol(bool commentLineSymbol) { _isCommentLineSymbol = commentLineSymbol; }
	void setIsCommentSymbol(bool commentSymbol) { _isCommentSymbol = commentSymbol; }
	void setIsPrefix(int idx, bool prefix ) { assert(idx < nbPrefixListAllowed); _isPrefix[idx] = prefix; }

	bool feedUserSettings(TiXmlNode *settingsRoot);
	bool feedUserKeywordList(TiXmlNode *node);
	bool feedUserStyles(TiXmlNode *node);

	// JOCE: get rid of this from the public interface.
	// Right now, it's just a pain in the but to do, but that should be done sooner or later.
	StyleArray _styleArray;

	// JOCE: Jsut for now.
	TCHAR _escapeChar[2];

private:
	generic_string _name;
	generic_string _ext;

	generic_string _keywordLists[KWL_NB_KEYWORD_LISTS];

	bool _isCaseIgnored;
	bool _isCommentLineSymbol;
	bool _isCommentSymbol;
	bool _isPrefix[nbPrefixListAllowed];
};

#define MAX_EXTERNAL_LEXER_NAME_LEN 16
#define MAX_EXTERNAL_LEXER_DESC_LEN 32

class ExternalLangContainer
{
public:
	TCHAR _name[MAX_EXTERNAL_LEXER_NAME_LEN];
	TCHAR _desc[MAX_EXTERNAL_LEXER_DESC_LEN];

	ExternalLangContainer(const TCHAR *name, const TCHAR *desc) {
		generic_strncpy(_name, name, MAX_EXTERNAL_LEXER_NAME_LEN);
		generic_strncpy(_desc, desc, MAX_EXTERNAL_LEXER_DESC_LEN);
	};
};

struct FindHistory {
	enum searchMode{normal, extended, regExpr};
	enum transparencyMode{none, onLossingFocus, persistant};

	FindHistory() : _nbMaxFindHistoryPath(10), _nbMaxFindHistoryFilter(10), _nbMaxFindHistoryFind(10), _nbMaxFindHistoryReplace(10),
					_isMatchWord(false), _isMatchCase(false),_isWrap(true),_isDirectionDown(true),
					_isFifRecuisive(true), _isFifInHiddenFolder(false), _searchMode(normal), _transparencyMode(onLossingFocus),
					_transparency(150), _isDlgAlwaysVisible(false), _isFilterFollowDoc(false), _isFolderFollowDoc(false)
	{
	}

	int _nbMaxFindHistoryPath;
	int _nbMaxFindHistoryFilter;
	int _nbMaxFindHistoryFind;
	int _nbMaxFindHistoryReplace;

    std::vector<generic_string> _findHistoryPaths;
	std::vector<generic_string> _findHistoryFilters;
	std::vector<generic_string> _findHistoryFinds;
	std::vector<generic_string> _findHistoryReplaces;

	bool _isMatchWord;
	bool _isMatchCase;
	bool _isWrap;
	bool _isDirectionDown;

	bool _isFifRecuisive;
	bool _isFifInHiddenFolder;

	searchMode _searchMode;
	transparencyMode _transparencyMode;
	int _transparency;

	bool _isDlgAlwaysVisible;
	bool _isFilterFollowDoc;
	bool _isFolderFollowDoc;
};


#ifdef UNICODE

class LocalizationSwitcher {
friend class NppParameters;
public :
    LocalizationSwitcher() {}

	struct LocalizationDefinition {
		wchar_t *_langName;
		wchar_t *_xmlFileName;
	};

	bool addLanguageFromXml(std::wstring xmlFullPath);
	std::wstring getLangFromXmlFileName(const wchar_t *fn) const;

	std::wstring getXmlFilePathFromLangName(const wchar_t *langName) const;
	bool switchToLang(wchar_t *lang2switch) const;

	size_t size() const {
		return _localizationList.size();
	};

	std::pair<std::wstring, std::wstring> getElementFromIndex(size_t index) {
		if (index >= _localizationList.size())
			return std::pair<std::wstring, std::wstring>(TEXT(""), TEXT(""));
		return _localizationList[index];
	};

    void setFileName(const char *fn) {
        if (fn)
            _fileName = fn;
    };

    string getFileName() const {
        return _fileName;
    };

private :
	std::vector< std::pair< std::wstring, std::wstring > > _localizationList;
	std::wstring _nativeLangPath;
    std::string _fileName;
};
#endif

class ThemeSwitcher {
friend class NppParameters;

public :
	ThemeSwitcher(){};

	void addThemeFromXml(generic_string xmlFullPath) {
		_themeList.push_back(std::pair<generic_string, generic_string>(getThemeFromXmlFileName(xmlFullPath.c_str()), xmlFullPath));
	};

	void addDefaultThemeFromXml(generic_string xmlFullPath) {
		_themeList.push_back(std::pair<generic_string, generic_string>(TEXT("Default (styles.xml)"), xmlFullPath));
	};

	generic_string getThemeFromXmlFileName(const TCHAR *fn) const;

	generic_string getXmlFilePathFromThemeName(const TCHAR *themeName) const {
		if (!themeName || themeName[0])
			return TEXT("");
		generic_string themePath = _stylesXmlPath;
		return themePath;
	};

	bool themeNameExists(const TCHAR *themeName) {
		for (size_t i = 0; i < _themeList.size(); i++ )
		{
			if (! (getElementFromIndex(i)).first.compare(themeName) ) return true;
		}
		return false;
	}

	size_t size() const {
		return _themeList.size();
	};


	std::pair<generic_string, generic_string> & getElementFromIndex(size_t index) {
		//if (index >= _themeList.size())
			//return std::pair<generic_string, generic_string>(TEXT(""), TEXT(""));
		return _themeList[index];
	};

private :
	std::vector< std::pair< generic_string, generic_string > > _themeList;
	generic_string _stylesXmlPath;
};

class PluginList {
public :
    void add(generic_string fn, bool isInBL){
        _list.push_back(std::pair<generic_string, bool>(fn, isInBL));
    };
private :
    std::vector<std::pair<generic_string, bool>>_list;
};

#define NB_LANG 80
#define DUP true
#define FREE false

class NppParameters
{
public:
    static NppParameters * getInstance() {return _pSelf;};
	static LangType getLangIDFromStr(const TCHAR *langName);
	bool load();
	bool reloadLang();
	bool reloadStylers(TCHAR *stylePath = NULL);
    void destroyInstance();

	bool _isTaskListRBUTTONUP_Active;
	int L_END;

	const NppGUI & getNppGUI() const {
        return _nppGUI;
    };

    const TCHAR * getWordList(LangType langID, int typeIndex) const {
	Lang *pLang = getLangFromID(langID);
	    if (!pLang) return NULL;

        return pLang->getWords(typeIndex);
    };

	Lang * getLangFromID(LangType langID) const {
		for( std::vector<Lang *>::const_iterator it = _langList.begin(), end = _langList.end();
			 it != end;
			 ++it)
		{
			if ((*it)->_langID == langID)
			{
				return *it;
			}
		}
		return NULL;
	}

	Lang * getLangFromIndex(int i) const {
		// JOCE check if we can easily make i a size_t
		if (i >= (int)_langList.size())
		{
			return NULL;
		}
		return _langList[i];
	}

	int getNbLang() const {return _langList.size();}

	LangType getLangFromExt(const TCHAR *ext);

	const TCHAR * getLangExtFromName(const TCHAR *langName) const {
		for( std::vector<Lang *>::const_iterator it = _langList.begin(), end = _langList.end();
			it != end;
			++it)
		{
			if ((*it)->_langName == langName)
				return (*it)->_defaultExtList;
		}
		return NULL;
	};

	const TCHAR * getLangExtFromLangType(LangType langType) const {
		for( std::vector<Lang *>::const_iterator it = _langList.begin(), end = _langList.end();
			it != end;
			++it)
		{
			if ((*it)->_langID == langType)
				return (*it)->_defaultExtList;
		}
		return NULL;
	};

	int getNbLRFile() const {return _LRFileList.size();}

	const generic_string& getLRFile(int index) const {
		// JOCE check if we can easily make index a size_t
		assert(index < (int)_LRFileList.size());
		return _LRFileList[index];
	}

	void setNbMaxFile(int nb) {
		_nbMaxFile = nb;
	};

	int getNbMaxFile() const {return _nbMaxFile;};

    const ScintillaViewParams & getSVP(bool whichOne) const {
        return _svp[whichOne];
    };

	bool writeNbHistoryFile(int nb);

	bool writeHistory(const TCHAR *fullpath);

	TiXmlNode * getChildElementByAttribut(TiXmlNode *pere, const TCHAR *childName,\
										  const TCHAR *attributName, const TCHAR *attributVal) const;

	bool writeScintillaParams(const ScintillaViewParams & svp, bool whichOne);

	bool writeGUIParams();

	void writeStyles(LexerStylerArray & lexersStylers, StyleArray & globalStylers);
    bool insertTabInfo(const TCHAR *langName, int tabInfo);

    LexerStylerArray & getLStylerArray() {return _lexerStylerArray;};
    StyleArray & getGlobalStylers() {return _widgetStyleArray;};

    StyleArray & getMiscStylerArray() {return _widgetStyleArray;};
	GlobalOverride & getGlobalOverrideStyle() {return _nppGUI._globalOverride;};

    COLORREF getCurLineHilitingColour() {
		int i = _widgetStyleArray.getStylerIndexByName(TEXT("Current line background colour"));
        if (i == -1) return i;
        Style & style = _widgetStyleArray.getStyler(i);
        return style._bgColor;
    };
    void setCurLineHilitingColour(COLORREF colour2Set) {
        int i = _widgetStyleArray.getStylerIndexByName(TEXT("Current line background colour"));
        if (i == -1) return;
        Style & style = _widgetStyleArray.getStyler(i);
        style._bgColor = colour2Set;
    };

	void setFontList(HWND hWnd);
	const std::vector<generic_string> & getFontList() const {return _fontlist;};

	int getNbUserLang() const {return _userLangArray.size();}
	UserLangContainer & getULCFromIndex(int i) {return *_userLangArray[i];}
	UserLangContainer * getULCFromName(const TCHAR *userLangName) {
		std::vector<UserLangContainer *>::iterator it = _userLangArray.begin(), end = _userLangArray.end();
		for (; it != end; ++it )
		{
			if ((*it)->getName() == userLangName)
			{
				return (*it);
			}
		}
		// Should never pass this point.
		assert(it != end);
		return NULL;
	}

	int getNbExternalLang() const {return _nbExternalLang;};
	int getExternalLangIndexFromName(const TCHAR *externalLangName) const {
		for (int i = 0 ; i < _nbExternalLang ; i++)
		{
			if (!lstrcmp(externalLangName, _externalLangArray[i]->_name))
				return i;
		}
		return -1;
	};
	ExternalLangContainer & getELCFromIndex(int i) {return *_externalLangArray[i];};

	bool ExternalLangHasRoom() const {return _nbExternalLang < NB_MAX_EXTERNAL_LANG;};

	void getExternalLexerFromXmlTree(TiXmlDocument *doc);
	std::vector<TiXmlDocument *> * getExternalLexerDoc() { return &_pXmlExternalLexerDoc;};

	void writeUserDefinedLang();
	void writeShortcuts();
	void writeSession(const Session * session, const TCHAR *fileName = NULL);
	bool writeFindHistory();


	bool isExistingUserLangName(const TCHAR *newName) const {
		if ((!newName) || (!newName[0]))
		{
			return true;
		}

		for (std::vector<UserLangContainer *>::const_iterator it = _userLangArray.begin(), end = _userLangArray.end();
		     it != end;
		     ++it )
		{
			if ((*it)->getName() == newName)
			{
				return true;
			}
		}

		return false;
	}

	bool getUserDefinedLangNameFromExt(TCHAR *ext, generic_string& outLangName) {
		if ((!ext) || (!ext[0]))
			return false;

		for (std::vector<UserLangContainer *>::iterator it = _userLangArray.begin(), end = _userLangArray.end();
			it != end;
			++it )
		{
			std::vector<generic_string> extVect;
			cutString((*it)->getExtension(), extVect);
			for (size_t j = 0 ; j < extVect.size() ; j++)
			{
				if (extVect[j] == ext)
				{
					outLangName = (*it)->getName();
					return true;
				}
			}
		}
		return false;
	}

	int addUserLangToEnd(const UserLangContainer & userLang, const TCHAR *newName);
	void removeUserLang(int index);

	bool isExistingExternalLangName(const TCHAR *newName) const {
		if ((!newName) || (!newName[0]))
			return true;

		for (int i = 0 ; i < _nbExternalLang ; i++)
		{
			if (!lstrcmp(_externalLangArray[i]->_name, newName))
				return true;
		}
		return false;
	};

	int addExternalLangToEnd(ExternalLangContainer * externalLang);

	//TiXmlDocument * getNativeLang() const {return _pXmlNativeLangDoc;};

	TiXmlDocumentA * getNativeLangA() const {return _pXmlNativeLangDocA;};

	TiXmlDocument * getToolIcons() const {return _pXmlToolIconsDoc;};

	bool isTransparentAvailable() const {
		return (_transparentFuncAddr != NULL);
	};

	void SetTransparent(HWND hwnd, int percent) {
		if (!_transparentFuncAddr) return;
		::SetWindowLongPtr(hwnd, GWL_EXSTYLE, ::GetWindowLongPtrW(hwnd, GWL_EXSTYLE) | 0x00080000);
		_transparentFuncAddr(hwnd, 0, percent, 0x00000002);
	};

	void removeTransparent(HWND hwnd) {
		::SetWindowLongPtr(hwnd, GWL_EXSTYLE,  ::GetWindowLongPtr(hwnd, GWL_EXSTYLE) & ~0x00080000);
	};

	void setCmdlineParam(const CmdLineParams & cmdLineParams) {
		_cmdLineParams = cmdLineParams;
	};
	CmdLineParams & getCmdLineParams() {return _cmdLineParams;};

	void setFileSaveDlgFilterIndex(int ln) {_fileSaveDlgFilterIndex = ln;};
	int getFileSaveDlgFilterIndex() const {return _fileSaveDlgFilterIndex;};

	bool isRemappingShortcut() const {return _shortcuts.size() != 0;};

	std::vector<CommandShortcut> & getUserShortcuts() {return _shortcuts;};
	std::vector<int> & getUserModifiedShortcuts() {return _customizedShortcuts;};
	void addUserModifiedIndex(int index);

	std::vector<MacroShortcut> & getMacroList() {return _macros;};
	std::vector<UserCommand> & getUserCommandList() {return _userCommands;};
	std::vector<PluginCmdShortcut> & getPluginCommandList() {return _pluginCommands;};
	std::vector<int> & getPluginModifiedKeyIndices() {return _pluginCustomizedCmds;};
	void addPluginModifiedIndex(int index);

	std::vector<ScintillaKeyMap> & getScintillaKeyList() {return _scintillaKeyCommands;};
	std::vector<int> & getScintillaModifiedKeyIndices() {return _scintillaModifiedKeyIndices;};
	void addScintillaModifiedIndex(int index);

	std::vector<MenuItemUnit> & getContextMenuItems() {return _contextMenuItems;};
	Session* getSession() const {return _session;};
	bool hasCustomContextMenu() const {return !_contextMenuItems.empty();};

	void setAccelerator(Accelerator *pAccel) {_pAccelerator = pAccel;};
	Accelerator * getAccelerator() {return _pAccelerator;};
	void setScintillaAccelerator(ScintillaAccelerator *pScintAccel) {_pScintAccelerator = pScintAccel;};
	ScintillaAccelerator * getScintillaAccelerator() {return _pScintAccelerator;};

	generic_string getNppPath() const {return _nppPath;};
	const TCHAR * getAppDataNppDir() const {return _appdataNppDir.c_str();};
	const TCHAR * getWorkingDir() const {return _currentDirectory.c_str();};
	void setWorkingDir(const TCHAR * newPath);

	bool loadSession(Session* session, const TCHAR *sessionFileName);
	int langTypeToCommandID(LangType lt) const;
	WNDPROC getEnableThemeDlgTexture() const {return _enableThemeDialogTextureFuncAddr;};

	struct FindDlgTabTitiles {
		generic_string _find;
		generic_string _replace;
		generic_string _findInFiles;
		FindDlgTabTitiles() : _find(TEXT("")), _replace(TEXT("")), _findInFiles(TEXT("")) {};
		bool isWellFilled() {
			return (lstrcmp(_find.c_str(), TEXT("")) != 0 && lstrcmp(_replace.c_str(), TEXT("")) && lstrcmp(_findInFiles.c_str(), TEXT("")));
		};
	};

	FindDlgTabTitiles & getFindDlgTabTitiles() { return _findDlgTabTitiles;};

	const char * getNativeLangMenuStringA(int itemID);

	bool asNotepadStyle() const {return _asNotepadStyle;};

	bool reloadPluginCmds() {
		return getPluginCmdsFromXmlTree();
	}

	bool getContextMenuFromXmlTree(HMENU mainMenuHadle);
	bool reloadContextMenuFromXmlTree(HMENU mainMenuHadle);
	FindHistory & getFindHistory() {return _findHistory;};
	bool _isFindReplacing; // an on the fly variable for find/replace functions

#ifdef UNICODE
	LocalizationSwitcher & getLocalizationSwitcher() {
		return _localizationSwitcher;
	};
#endif
	ThemeSwitcher & getThemeSwitcher() {
		return _themeSwitcher;
	};

	int getIndexFromKeywordListName(const TCHAR *name);

    std::vector<generic_string> & getBlackList() {return _blacklist;};
    bool isInBlackList(TCHAR *fn) {
        for (size_t i = 0 ; i < _blacklist.size() ; i++)
            if (_blacklist[i] == fn)
                return true;
        return false;
    };

    PluginList & getPluginList() {return _pluginList;};

private:
    NppParameters();
	~NppParameters();

    static NppParameters *_pSelf;

	TiXmlDocument* _pXmlDoc;
	TiXmlDocument* _pXmlUserDoc;
	TiXmlDocument* _pXmlUserStylerDoc;
	TiXmlDocument* _pXmlUserLangDoc;
	TiXmlDocument* _pXmlToolIconsDoc;
	TiXmlDocument* _pXmlShortcutDoc;
	TiXmlDocument* _pXmlContextMenuDoc;
	TiXmlDocument* _pXmlSessionDoc;
	TiXmlDocument* _pXmlBlacklistDoc;

	TiXmlDocumentA *_pXmlNativeLangDocA;

	std::vector<TiXmlDocument *> _pXmlExternalLexerDoc;

	NppGUI _nppGUI;
	ScintillaViewParams _svp[2];

	std::vector<Lang *> _langList;
	std::vector<generic_string> _LRFileList;
	int _nbMaxFile;

	FindHistory _findHistory;

	std::vector<UserLangContainer *> _userLangArray;

	generic_string _userDefineLangPath;
	// JOCE use a std::vector instead!
	ExternalLangContainer *_externalLangArray[NB_MAX_EXTERNAL_LANG];
	int _nbExternalLang;

	CmdLineParams _cmdLineParams;

	int _fileSaveDlgFilterIndex;

    // All Styles (colours & fonts)
	LexerStylerArray _lexerStylerArray;
    StyleArray _widgetStyleArray;

	std::vector<generic_string> _fontlist;
    std::vector<generic_string> _blacklist;
    PluginList _pluginList;

	HMODULE _hUser32;
	HMODULE _hUXTheme;

	WNDPROC _transparentFuncAddr;
	WNDPROC _enableThemeDialogTextureFuncAddr;


	std::vector<CommandShortcut> _shortcuts;		//main menu shortcuts. Static size
	std::vector<int> _customizedShortcuts;			//altered main menu shortcuts. Indices static. Needed when saving alterations
	std::vector<MacroShortcut> _macros;				//macro shortcuts, dynamic size, defined on loading macros and adding/deleting them
	std::vector<UserCommand> _userCommands;			//run shortcuts, dynamic size, defined on loading run commands and adding/deleting them
	std::vector<PluginCmdShortcut> _pluginCommands;	//plugin commands, dynamic size, defined on loading plugins
	std::vector<int> _pluginCustomizedCmds;			//plugincommands that have been altered. Indices determined after loading ALL plugins. Needed when saving alterations

	std::vector<ScintillaKeyMap> _scintillaKeyCommands;	//scintilla keycommands. Static size
	std::vector<int> _scintillaModifiedKeyIndices;		//modified scintilla keys. Indices static, determined by searching for commandId. Needed when saving alterations
#ifdef UNICODE
	LocalizationSwitcher _localizationSwitcher;
#endif
	ThemeSwitcher _themeSwitcher;

	//std::vector<generic_string> _noMenuCmdNames;
	std::vector<MenuItemUnit> _contextMenuItems;
	Session* _session;

	generic_string _shortcutsPath;
	generic_string _contextMenuPath;
	generic_string _sessionPath;
    generic_string _blacklistPath;
	generic_string _nppPath;
	generic_string _userPath;
	generic_string _stylerPath;
	generic_string _appdataNppDir; // sentinel of the absence of "doLocalConf.xml" : (_appdataNppDir == TEXT(""))?"doLocalConf.xml present":"doLocalConf.xml absent"
	generic_string _currentDirectory;

	Accelerator *_pAccelerator;
	ScintillaAccelerator * _pScintAccelerator;

	FindDlgTabTitiles _findDlgTabTitiles;
	bool _asNotepadStyle;

	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX * /*lpntme*/, int /*FontType*/, LPARAM lParam)
	{
		std::vector<generic_string> *pStrVect = (std::vector<generic_string> *)lParam;
        size_t vectSize = pStrVect->size();

		//Search through all the fonts, EnumFontFamiliesEx never states anything about order
		//Start at the end though, that's the most likely place to find a duplicate
		for(int i = vectSize - 1 ; i >= 0 ; i--) {
			if ( !lstrcmp((*pStrVect)[i].c_str(), (const TCHAR *)lpelfe->elfLogFont.lfFaceName) )
				return 1;	//we already have seen this typeface, ignore it
		}
		//We can add the font
		//Add the face name and not the full name, we do not care about any styles
		pStrVect->push_back((TCHAR *)lpelfe->elfLogFont.lfFaceName);
		return 1; // I want to get all fonts
	};

	void getLangKeywordsFromXmlTree();
	bool getUserParametersFromXmlTree();
	bool getUserStylersFromXmlTree();
	bool getUserDefineLangsFromXmlTree();
	bool getShortcutsFromXmlTree();

	bool getMacrosFromXmlTree();
	bool getUserCmdsFromXmlTree();
	bool getPluginCmdsFromXmlTree();
	bool getScintKeysFromXmlTree();
	bool getSessionFromXmlTree(TiXmlDocument *pSessionDoc = NULL, Session *session = NULL);
    bool getBlackListFromXmlTree();

	void feedGUIParameters(TiXmlNode *node);
	void feedKeyWordsParameters(TiXmlNode *node);
	void feedFileListParameters(TiXmlNode *node);
    void feedScintillaParam(bool whichOne, TiXmlNode *node);
	void feedDockingManager(TiXmlNode *node);
	void feedFindHistoryParameters(TiXmlNode *node);

	bool feedStylerArray(TiXmlNode *node);
    void getAllWordStyles(TCHAR *lexerName, TiXmlNode *lexerNode);

	void feedUserLang(TiXmlNode *node);

	void feedShortcut(TiXmlNode *node);
	void feedMacros(TiXmlNode *node);
	void feedUserCmds(TiXmlNode *node);
	void feedPluginCustomizedCmds(TiXmlNode *node);
	void feedScintKeys(TiXmlNode *node);
    bool feedBlacklist(TiXmlNode *node);

	void getActions(TiXmlNode *node, Macro & macro);
	bool getShortcuts(TiXmlNode *node, Shortcut & sc);

    void writeStyle2Element(Style & style2Wite, Style & style2Sync, TiXmlElement *element);
	void insertUserLang2Tree(TiXmlNode *node, UserLangContainer *userLang);
	void insertCmd(TiXmlNode *cmdRoot, const CommandShortcut & cmd);
	void insertMacro(TiXmlNode *macrosRoot, const MacroShortcut & macro);
	void insertUserCmd(TiXmlNode *userCmdRoot, const UserCommand & userCmd);
	void insertScintKey(TiXmlNode *scintKeyRoot, const ScintillaKeyMap & scintKeyMap);
	void insertPluginCmd(TiXmlNode *pluginCmdRoot, const PluginCmdShortcut & pluginCmd);
	TiXmlElement * insertGUIConfigBoolNode(TiXmlNode *r2w, const TCHAR *name, bool bVal);
	void insertDockingParamNode(TiXmlNode *GUIRoot);
	void writeExcludedLangList(TiXmlElement *element);
	void writePrintSetting(TiXmlElement *element);
	void initMenuKeys();		//initialise menu keys and scintilla keys. Other keys are initialized on their own
	void initScintillaKeys();	//these functions have to be called first before any modifications are loaded
};

#endif //PARAMETERS_H
