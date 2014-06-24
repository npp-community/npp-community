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

#include "precompiled_headers.h"

#include "Notepad_plus.h"
#include "WinControls/OpenSaveFileDialog/FileDialog.h"
#include "ScintillaComponent/printer.h"
#include "MISC/FileNameStringSplitter.h"
#include "ValueDlg.h"
#include "Utf8_16.h"
#include "WinControls/Grid/ShortcutMapper.h"
#include "WinControls/TaskList/TaskListDlg.h"
#include "ScintillaComponent/xmlMatchedTagsHighlighter.h"
#include "MISC/Process/Process.h"

#include "WinControls/AboutDlg/AboutDlg.h"
#include "WinControls/StaticDialog/RunDlg/RunDlg.h"
#include "ScintillaComponent/GoToLineDlg.h"
#include "ScintillaComponent/columnEditor.h"
#include "WinControls/Preference/preferenceDlg.h"
#include "WinControls/ColourPicker/WordStyleDlg.h"
#include "WinControls/WindowsDlg/WindowsDlg.h"
#include "WinControls/WindowsDlg/WindowsDlgRc.h"
#include "WinControls/shortcut/RunMacroDlg.h"
#include "ScintillaComponent/FindReplaceDlg.h"
#include "ScintillaComponent/UserDefineDialog.h"

#include "ScintillaComponent/UserDefineResource.h"

#include "WinControls/ImageListSet/ImageListSet.h"
#include "lastRecentFileList.h"
#include "ScintillaComponent/SmartHighlighter.h"
#include "ScintillaComponent/AutoCompletion.h"
#include "MISC/PluginsManager/PluginsManager.h"

#include "WinControls/DockingWnd/DockingManager.h"
#include "WinControls/DockingWnd/Docking.h"
#include "WinControls/DockingWnd/DockingCont.h"
#include "ScintillaComponent/DocTabView.h"
#include "WinControls/SplitterContainer/SplitterContainer.h"

#include "WinControls/ContextMenu/ContextMenu.h"
#include "ScintillaComponent/ScintillaEditView.h"
#include "WinControls/ToolBar/Toolbar.h"
#include "WinControls/StatusBar/StatusBar.h"

#include "WinControls/TrayIcon/trayIconControler.h"

#include "MenuCmdID.h"

#include "ScintillaComponent/Buffer.h"

#include "Parameters.h"

#include "MISC/Common/npp_session.h"
#include "EncodingMapper.h"

#include "localization.h"

#include "Notepad_plus_Window.h"

enum tb_stat {tb_saved, tb_unsaved, tb_ro};

int docTabIconIDs[] = {IDI_SAVED_ICON, IDI_UNSAVED_ICON, IDI_READONLY_ICON};

ToolBarButtonUnit toolBarIcons[] = {
	{IDM_FILE_NEW,		IDI_NEW_OFF_ICON,		IDI_NEW_ON_ICON,		IDI_NEW_OFF_ICON, IDR_FILENEW},
	{IDM_FILE_OPEN,		IDI_OPEN_OFF_ICON,		IDI_OPEN_ON_ICON,		IDI_OPEN_OFF_ICON, IDR_FILEOPEN},
	{IDM_FILE_SAVE,		IDI_SAVE_OFF_ICON,		IDI_SAVE_ON_ICON,		IDI_SAVE_DISABLE_ICON, IDR_FILESAVE},
	{IDM_FILE_SAVEALL,	IDI_SAVEALL_OFF_ICON,	IDI_SAVEALL_ON_ICON,	IDI_SAVEALL_DISABLE_ICON, IDR_SAVEALL},
	{IDM_FILE_CLOSE,	IDI_CLOSE_OFF_ICON,		IDI_CLOSE_ON_ICON,		IDI_CLOSE_OFF_ICON, IDR_CLOSEFILE},
	{IDM_FILE_CLOSEALL,	IDI_CLOSEALL_OFF_ICON,	IDI_CLOSEALL_ON_ICON,	IDI_CLOSEALL_OFF_ICON, IDR_CLOSEALL},
	{IDM_FILE_PRINTNOW,	IDI_PRINT_OFF_ICON,		IDI_PRINT_ON_ICON,		IDI_PRINT_OFF_ICON, IDR_PRINT},

	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//

	{IDM_EDIT_CUT,		IDI_CUT_OFF_ICON,		IDI_CUT_ON_ICON,		IDI_CUT_DISABLE_ICON, IDR_CUT},
	{IDM_EDIT_COPY,		IDI_COPY_OFF_ICON,		IDI_COPY_ON_ICON,		IDI_COPY_DISABLE_ICON, IDR_COPY},
	{IDM_EDIT_PASTE,	IDI_PASTE_OFF_ICON,		IDI_PASTE_ON_ICON,		IDI_PASTE_DISABLE_ICON, IDR_PASTE},

	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//

	{IDM_EDIT_UNDO,		IDI_UNDO_OFF_ICON,		IDI_UNDO_ON_ICON,		IDI_UNDO_DISABLE_ICON, IDR_UNDO},
	{IDM_EDIT_REDO,		IDI_REDO_OFF_ICON,		IDI_REDO_ON_ICON,		IDI_REDO_DISABLE_ICON, IDR_REDO},
	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//

	{IDM_SEARCH_FIND,		IDI_FIND_OFF_ICON,		IDI_FIND_ON_ICON,		IDI_FIND_OFF_ICON, IDR_FIND},
	{IDM_SEARCH_REPLACE,  IDI_REPLACE_OFF_ICON,	IDI_REPLACE_ON_ICON,	IDI_REPLACE_OFF_ICON, IDR_REPLACE},

	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//
	{IDM_VIEW_ZOOMIN,	IDI_ZOOMIN_OFF_ICON,	IDI_ZOOMIN_ON_ICON,		IDI_ZOOMIN_OFF_ICON, IDR_ZOOMIN},
	{IDM_VIEW_ZOOMOUT,	IDI_ZOOMOUT_OFF_ICON,	IDI_ZOOMOUT_ON_ICON,	IDI_ZOOMOUT_OFF_ICON, IDR_ZOOMOUT},

	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//
	{IDM_VIEW_SYNSCROLLV,	IDI_SYNCV_OFF_ICON,	IDI_SYNCV_ON_ICON,	IDI_SYNCV_DISABLE_ICON, IDR_SYNCV},
	{IDM_VIEW_SYNSCROLLH,	IDI_SYNCH_OFF_ICON,	IDI_SYNCH_ON_ICON,	IDI_SYNCH_DISABLE_ICON, IDR_SYNCH},

	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//
	{IDM_VIEW_WRAP,  IDI_VIEW_WRAP_OFF_ICON,	IDI_VIEW_WRAP_ON_ICON,	IDI_VIEW_WRAP_OFF_ICON, IDR_WRAP},
	{IDM_VIEW_ALL_CHARACTERS,  IDI_VIEW_ALL_CHAR_OFF_ICON,	IDI_VIEW_ALL_CHAR_ON_ICON,	IDI_VIEW_ALL_CHAR_OFF_ICON, IDR_INVISIBLECHAR},
	{IDM_VIEW_INDENT_GUIDE,  IDI_VIEW_INDENT_OFF_ICON,	IDI_VIEW_INDENT_ON_ICON,	IDI_VIEW_INDENT_OFF_ICON, IDR_INDENTGUIDE},
	{IDM_VIEW_USER_DLG,  IDI_VIEW_UD_DLG_OFF_ICON,	IDI_VIEW_UD_DLG_ON_ICON,	IDI_VIEW_UD_DLG_OFF_ICON, IDR_SHOWPANNEL},

	//-------------------------------------------------------------------------------------//
	{0,					IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON,		IDI_SEPARATOR_ICON, IDI_SEPARATOR_ICON},
	//-------------------------------------------------------------------------------------//

	{IDM_MACRO_STARTRECORDINGMACRO,		IDI_STARTRECORD_OFF_ICON,	IDI_STARTRECORD_ON_ICON,	IDI_STARTRECORD_DISABLE_ICON, IDR_STARTRECORD},
	{IDM_MACRO_STOPRECORDINGMACRO,		IDI_STOPRECORD_OFF_ICON,	IDI_STOPRECORD_ON_ICON,		IDI_STOPRECORD_DISABLE_ICON, IDR_STOPRECORD},
	{IDM_MACRO_PLAYBACKRECORDEDMACRO,	IDI_PLAYRECORD_OFF_ICON,	IDI_PLAYRECORD_ON_ICON,		IDI_PLAYRECORD_DISABLE_ICON, IDR_PLAYRECORD},
	{IDM_MACRO_RUNMULTIMACRODLG,			IDI_MMPLAY_OFF_ICON,		IDI_MMPLAY_ON_ICON,			IDI_MMPLAY_DIS_ICON, IDR_M_PLAYRECORD},
	{IDM_MACRO_SAVECURRENTMACRO,			IDI_SAVERECORD_OFF_ICON,	IDI_SAVERECORD_ON_ICON,		IDI_SAVERECORD_DISABLE_ICON, IDR_SAVERECORD}

};


Notepad_plus::Notepad_plus():
	_pPublicInterface(NULL),
	_pMainWindow(NULL),
	_dockingManager(NULL),
	_smartHighlighter(NULL),
	_nativeLangSpeaker(new NativeLangSpeaker()),
	_mainDocTab(NULL),
	_subDocTab(NULL),
	_pDocTab(NULL),
	_pNonDocTab(NULL),
	_subEditView(new ScintillaEditView()),
	_mainEditView(new ScintillaEditView()),
	_invisibleEditView(new ScintillaEditView()),
	_fileEditView(new ScintillaEditView()),
	_pEditView(NULL),
	_pNonEditView(NULL),
	_pMainSplitter(NULL),
	_subSplitter(NULL),
	_autoCompleteMain(new AutoCompletion(_mainEditView)),
	_autoCompleteSub(new AutoCompletion(_subEditView)),
	_tabPopupMenu(NULL),
	_tabPopupDropMenu(NULL),
	_toolBar(NULL),
	_docTabIconList(NULL),
	_statusBar(NULL),
	_toReduceTabBar(false),
	_rebarTop(NULL),
	_rebarBottom(NULL),
	_findReplaceDlg(NULL),
	_incrementFindDlg(NULL),
	_aboutDlg(NULL),
	_runDlg(NULL),
	_goToLineDlg(NULL),
	_colEditorDlg(NULL),
	_configStyleDlg(NULL),
	_preferenceDlg(NULL),
	_lastRecentFileList(new LastRecentFileList()),
	_windowsMenu(NULL),
	_mainMenuHandle(NULL),
	_sysMenuEntering(false),
	_recordingMacro(false),
	_runMacroDlg(NULL),
	_linkTriggered(true),
	_isDocModifing(false),
	_isHotspotDblClicked(false),
	_isUDDocked(false),
	_pTrayIco(NULL),
	_zoomOriginalValue(0),
	_pluginsManager(new PluginsManager()),
	_restoreButton(NULL),
	_isFileOpening(false),
	_rememberThisSession(true),
	_mainWindowStatus(0),
	_activeView(MAIN_VIEW)
{
	ZeroMemory(&_prevSelectedRange, sizeof(_prevSelectedRange));

	TiXmlDocumentA *nativeLangDocRootA = (NppParameters::getInstance())->getNativeLangA();
    _nativeLangSpeaker->init(nativeLangDocRootA);
#ifdef UNICODE
	LocalizationSwitcher & localizationSwitcher = (NppParameters::getInstance())->getLocalizationSwitcher();
    const char *fn = _nativeLangSpeaker->getFileName();
    if (fn)
    {
        localizationSwitcher.setFileName(fn);
    }
#endif

}

// ATTENTION : the order of the destruction is very important
// because if the parent's window handle is destroyed before
// the destruction of its children windows' handles,
// its children windows' handles will be destroyed automatically!
Notepad_plus::~Notepad_plus()
{
	NppParameters::destroyInstance();
	FileManager::destroyInstance();
	WcharMbcsConvertor::destroyInstance();
	EncodingMapper::destroyInstance();
	if (_pTrayIco)
		delete _pTrayIco;

	delete _lastRecentFileList;
	delete _autoCompleteMain;
	delete _autoCompleteSub;

	delete _subEditView;
	delete _mainEditView;
	delete _invisibleEditView;
	delete _fileEditView;

	delete _pluginsManager;
	delete _nativeLangSpeaker;
}

LRESULT Notepad_plus::init(HWND hwnd)
{
	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = pNppParam->getNppGUI();

	if (!_dockingManager)
	{
		_dockingManager = new DockingManager();
	}

	if (!_toolBar)
	{
		_toolBar = new ToolBar();
		if (TiXmlDocument *toolIconsDocRoot = (NppParameters::getInstance())->getToolIcons())
		{
			_toolBar->initTheme(toolIconsDocRoot);
		}
	}

	if (!_statusBar)
	{
		_statusBar = new StatusBar();
	}

	if (!_rebarTop)
	{
		_rebarTop = new ReBar();
	}

	if (!_rebarBottom)
	{
		_rebarBottom = new ReBar();
	}

	// Init dialogs and windows.
	if(!_findReplaceDlg)
	{
		_findReplaceDlg = new FindReplaceDlg();
	}

	if (!_incrementFindDlg)
	{
		_incrementFindDlg = new FindIncrementDlg();
	}

	if (!_aboutDlg)
	{
		_aboutDlg = new AboutDlg();
	}

	if (!_runDlg)
	{
		_runDlg = new RunDlg();
	}

	if (!_goToLineDlg)
	{
		_goToLineDlg = new GoToLineDlg();
	}

	if (!_colEditorDlg)
	{
		_colEditorDlg = new ColumnEditorDlg();
	}

	if (!_configStyleDlg)
	{
		_configStyleDlg = new WordStyleDlg();
	}

	if (!_preferenceDlg)
	{
		_preferenceDlg = new PreferenceDlg();
	}

	if (!_windowsMenu)
	{
		_windowsMenu = new WindowsMenu();
	}

	if (!_runMacroDlg)
	{
		_runMacroDlg = new RunMacroDlg();
	}

	if (!_smartHighlighter)
	{
		_smartHighlighter = new SmartHighlighter(_findReplaceDlg);
	}

	if (!_docTabIconList)
	{
		_docTabIconList = new IconList();
	}

	if (!_mainDocTab)
	{
		_mainDocTab = new DocTabView();
	}

	if (!_subDocTab)
	{
		_subDocTab = new DocTabView();
	}

	if (!_subSplitter)
	{
		_subSplitter = new SplitterContainer();
	}

	if (!_restoreButton)
	{
		_restoreButton = new ButtonDlg();
	}

	// Menu
	_mainMenuHandle = ::GetMenu(hwnd);
	int langPos2BeRemoved = MENUINDEX_LANGUAGE+1;
	if (nppGUI._isLangMenuCompact)
		langPos2BeRemoved = MENUINDEX_LANGUAGE;
	::RemoveMenu(_mainMenuHandle, langPos2BeRemoved, MF_BYPOSITION);

	//Views
	_pDocTab = _mainDocTab;
	_pEditView = _mainEditView;
	_pNonDocTab = _subDocTab;
	_pNonEditView = _subEditView;

	_mainEditView->init(_pPublicInterface->getHinst(), hwnd);
	_subEditView->init(_pPublicInterface->getHinst(), hwnd);

	_fileEditView->init(_pPublicInterface->getHinst(), hwnd);
	MainFileManager->init(this, _fileEditView);	//get it up and running asap.

	pNppParam->setFontList(hwnd);


	_mainWindowStatus = WindowMainActive;
	_activeView = MAIN_VIEW;

    const ScintillaViewParams & svp = pNppParam->getSVP();

	int tabBarStatus = nppGUI._tabStatus;
	_toReduceTabBar = ((tabBarStatus & TAB_REDUCE) != 0);
	_docTabIconList->create(_toReduceTabBar?13:20, _pPublicInterface->getHinst(), docTabIconIDs, sizeof(docTabIconIDs)/sizeof(int));

	_mainDocTab->init(_pPublicInterface->getHinst(), hwnd, _mainEditView, _docTabIconList);
	_subDocTab->init(_pPublicInterface->getHinst(), hwnd, _subEditView, _docTabIconList);

	_mainEditView->display();

	_invisibleEditView->init(_pPublicInterface->getHinst(), hwnd);
	_invisibleEditView->execute(SCI_SETUNDOCOLLECTION);
	_invisibleEditView->execute(SCI_EMPTYUNDOBUFFER);
	_invisibleEditView->wrap(false); // Make sure no slow down

	// Configuration of 2 scintilla views
	_mainEditView->showMargin(ScintillaEditView::_SC_MARGIN_LINENUMBER, svp._lineNumberMarginShow);
	_subEditView->showMargin(ScintillaEditView::_SC_MARGIN_LINENUMBER, svp._lineNumberMarginShow);
	_mainEditView->showMargin(ScintillaEditView::_SC_MARGIN_SYMBOL, svp._bookMarkMarginShow);
	_subEditView->showMargin(ScintillaEditView::_SC_MARGIN_SYMBOL, svp._bookMarkMarginShow);

	_mainEditView->showIndentGuideLine(svp._indentGuideLineShow);
	_subEditView->showIndentGuideLine(svp._indentGuideLineShow);

	::SendMessage(hwnd, NPPM_INTERNAL_SETCARETWIDTH, 0, 0);
	::SendMessage(hwnd, NPPM_INTERNAL_SETCARETBLINKRATE, 0, 0);

	_configStyleDlg->init(_pPublicInterface->getHinst(), hwnd);
	_preferenceDlg->init(_pPublicInterface->getHinst(), hwnd);

    //Marker Margin config
    _mainEditView->setMakerStyle(svp._folderStyle);
    _subEditView->setMakerStyle(svp._folderStyle);

	//Line wrap method
	_mainEditView->setWrapMode(svp._lineWrapMethod);
    _subEditView->setWrapMode(svp._lineWrapMethod);

	_mainEditView->execute(SCI_SETCARETLINEVISIBLE, svp._currentLineHilitingShow);
	_subEditView->execute(SCI_SETCARETLINEVISIBLE, svp._currentLineHilitingShow);

	_mainEditView->execute(SCI_SETCARETLINEVISIBLEALWAYS, true);
	_subEditView->execute(SCI_SETCARETLINEVISIBLEALWAYS, true);

	_mainEditView->wrap(svp._doWrap);
	_subEditView->wrap(svp._doWrap);

	_mainEditView->execute(SCI_SETEDGECOLUMN, svp._edgeNbColumn);
	_mainEditView->execute(SCI_SETEDGEMODE, svp._edgeMode);
	_subEditView->execute(SCI_SETEDGECOLUMN, svp._edgeNbColumn);
	_subEditView->execute(SCI_SETEDGEMODE, svp._edgeMode);

	_mainEditView->showEOL(svp._eolShow);
	_subEditView->showEOL(svp._eolShow);

	_mainEditView->showWSAndTab(svp._whiteSpaceShow);
	_subEditView->showWSAndTab(svp._whiteSpaceShow);

	_mainEditView->showWrapSymbol(svp._wrapSymbolShow);
	_subEditView->showWrapSymbol(svp._wrapSymbolShow);

	_mainEditView->performGlobalStyles();
	_subEditView->performGlobalStyles();

	_zoomOriginalValue = _pEditView->execute(SCI_GETZOOM);
	_mainEditView->execute(SCI_SETZOOM, svp._zoom);
	_subEditView->execute(SCI_SETZOOM, svp._zoom2);

	::SendMessage(hwnd, NPPM_INTERNAL_SETMULTISELCTION, 0, 0);

	_mainEditView->execute(SCI_SETADDITIONALSELECTIONTYPING, true);
	_subEditView->execute(SCI_SETADDITIONALSELECTIONTYPING, true);

	_mainEditView->execute(SCI_SETVIRTUALSPACEOPTIONS, SCVS_RECTANGULARSELECTION);
	_subEditView->execute(SCI_SETVIRTUALSPACEOPTIONS, SCVS_RECTANGULARSELECTION);

	TabBarPlus::doDragNDrop(true);

	if (_toReduceTabBar)
	{
		HFONT hf = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

		if (hf)
		{
			::SendMessage(_mainDocTab->getHSelf(), WM_SETFONT, (WPARAM)hf, MAKELPARAM(TRUE, 0));
			::SendMessage(_subDocTab->getHSelf(), WM_SETFONT, (WPARAM)hf, MAKELPARAM(TRUE, 0));
		}
		TabCtrl_SetItemSize(_mainDocTab->getHSelf(), 45, 20);
		TabCtrl_SetItemSize(_subDocTab->getHSelf(), 45, 20);
	}
	_mainDocTab->display();


	TabBarPlus::doDragNDrop((tabBarStatus & TAB_DRAGNDROP) != 0);
	TabBarPlus::setDrawTopBar((tabBarStatus & TAB_DRAWTOPBAR) != 0);
	TabBarPlus::setDrawInactiveTab((tabBarStatus & TAB_DRAWINACTIVETAB) != 0);
	TabBarPlus::setDrawTabCloseButton((tabBarStatus & TAB_CLOSEBUTTON) != 0);
	TabBarPlus::setDbClk2Close((tabBarStatus & TAB_DBCLK2CLOSE) != 0);
	TabBarPlus::setVertical((tabBarStatus & TAB_VERTICAL) != 0);
	drawTabbarColoursFromStylerArray();

	//--Splitter Section--//
	bool isVertical = (nppGUI._splitterPos == POS_VERTICAL);

	_subSplitter->init(_pPublicInterface->getHinst(), hwnd);
	_subSplitter->create(_mainDocTab, _subDocTab, 8, DYNAMIC, 50, isVertical);

	//--Status Bar Section--//
	bool willBeShown = nppGUI._statusBarShow;
	_statusBar->init(_pPublicInterface->getHinst(), hwnd, 6);
	_statusBar->setPartWidth(STATUSBAR_DOC_SIZE, 200);
	_statusBar->setPartWidth(STATUSBAR_CUR_POS, 230);
	_statusBar->setPartWidth(STATUSBAR_EOF_FORMAT, 110);
	_statusBar->setPartWidth(STATUSBAR_UNICODE_TYPE, 120);
	_statusBar->setPartWidth(STATUSBAR_TYPING_MODE, 30);
	_statusBar->display(willBeShown);

	_pMainWindow = _mainDocTab;

	_dockingManager->init(_pPublicInterface->getHinst(), hwnd, &_pMainWindow);

	if (nppGUI._isMinimizedToTray && _pTrayIco == NULL)
		_pTrayIco = new trayIconControler(hwnd, IDI_M30ICON, IDC_MINIMIZED_TRAY, ::LoadIcon(_pPublicInterface->getHinst(), MAKEINTRESOURCE(IDI_M30ICON)), TEXT(""));

	checkSyncState();

	// Plugin Manager
	NppData nppData;
	nppData._nppHandle = hwnd;
	nppData._scintillaMainHandle = _mainEditView->getHSelf();
	nppData._scintillaSecondHandle = _subEditView->getHSelf();

	_scintillaCtrls4Plugins.init(_pPublicInterface->getHinst(), hwnd);
	_pluginsManager->init(nppData);
	_pluginsManager->loadPlugins();
	const TCHAR *appDataNpp = pNppParam->getAppDataNppDir();
	if (appDataNpp[0])
		_pluginsManager->loadPlugins(appDataNpp);

	_restoreButton->init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf());

	// ------------ //
	// Menu Section //
	// ------------ //

	// Macro Menu
	std::vector<MacroShortcut> & macros  = pNppParam->getMacroList();
	HMENU hMacroMenu = ::GetSubMenu(_mainMenuHandle, MENUINDEX_MACRO);
	size_t const posBase = 6;
	size_t nbMacro = macros.size();
	if (nbMacro >= 1)
		::InsertMenu(hMacroMenu, posBase - 1, MF_BYPOSITION, (unsigned int)-1, 0);

	for (size_t i = 0 ; i < nbMacro ; i++)
	{
		::InsertMenu(hMacroMenu, posBase + i, MF_BYPOSITION, ID_MACRO + i, macros[i].toMenuItemString().c_str());
	}

    if (nbMacro >= 1)
    {
        ::InsertMenu(hMacroMenu, posBase + nbMacro + 1, MF_BYPOSITION, (unsigned int)-1, 0);
        ::InsertMenu(hMacroMenu, posBase + nbMacro + 2, MF_BYCOMMAND, IDM_SETTING_SHORTCUT_MAPPER_MACRO, TEXT("Modify Shortcut/Delete Macro..."));
    }
	// Run Menu
	std::vector<UserCommand> & userCommands = pNppParam->getUserCommandList();
	HMENU hRunMenu = ::GetSubMenu(_mainMenuHandle, MENUINDEX_RUN);
	int const runPosBase = 2;
	size_t nbUserCommand = userCommands.size();
	if (nbUserCommand >= 1)
		::InsertMenu(hRunMenu, runPosBase - 1, MF_BYPOSITION, (unsigned int)-1, 0);
	for (size_t i = 0 ; i < nbUserCommand ; i++)
	{
		::InsertMenu(hRunMenu, runPosBase + i, MF_BYPOSITION, ID_USER_CMD + i, userCommands[i].toMenuItemString().c_str());
	}

    if (nbUserCommand >= 1)
    {
		::InsertMenu(hRunMenu, runPosBase + nbUserCommand + 1, MF_BYPOSITION, (unsigned int)-1, 0);
        ::InsertMenu(hRunMenu, runPosBase + nbUserCommand + 2, MF_BYCOMMAND, IDM_SETTING_SHORTCUT_MAPPER_RUN, TEXT("Modify Shortcut/Delete Command..."));
    }

	// Updater menu item
	if (!nppGUI._doesExistUpdater)
	{
		//::MessageBox(NULL, TEXT("pas de updater"), TEXT(""), MB_OK);
		::DeleteMenu(_mainMenuHandle, IDM_UPDATE_NPP, MF_BYCOMMAND);
		::DrawMenuBar(hwnd);
	}
	//Languages Menu
	HMENU hLangMenu = ::GetSubMenu(_mainMenuHandle, MENUINDEX_LANGUAGE);

	// Add external languages to menu
	for (int i = 0 ; i < pNppParam->getNbExternalLang() ; i++)
	{
		ExternalLangContainer & externalLangContainer = pNppParam->getELCFromIndex(i);

		int numLangs = ::GetMenuItemCount(hLangMenu);
		const int bufferSize = 100;
		TCHAR buffer[bufferSize];

		int x;
		for(x = 0; (x == 0 || lstrcmp(externalLangContainer._name, buffer) > 0) && x < numLangs; x++)
		{
			::GetMenuString(hLangMenu, x, buffer, bufferSize, MF_BYPOSITION);
		}

		::InsertMenu(hLangMenu, x-1, MF_BYPOSITION, IDM_LANG_EXTERNAL + i, externalLangContainer._name);
	}

	if (nppGUI._excludedLangList.size() > 0)
	{
		for (size_t i = 0 ; i < nppGUI._excludedLangList.size() ; i++)
		{
			int cmdID = pNppParam->langTypeToCommandID(nppGUI._excludedLangList[i]._langType);
			const int itemSize = 256;
			TCHAR itemName[itemSize];
			::GetMenuString(hLangMenu, cmdID, itemName, itemSize, MF_BYCOMMAND);
			nppGUI._excludedLangList[i]._cmdID = cmdID;
			nppGUI._excludedLangList[i]._langName = itemName;
			::DeleteMenu(hLangMenu, cmdID, MF_BYCOMMAND);
			DrawMenuBar(hwnd);
		}
	}

	// Add User Define Languages Entry
	int udlpos = ::GetMenuItemCount(hLangMenu);

	if (pNppParam->getNbUserLang() > 0)
	{
		::InsertMenu(hLangMenu, udlpos++, MF_BYPOSITION|MF_SEPARATOR, 0, TEXT(""));
	}

	for (int i = 0 ; i < pNppParam->getNbUserLang() ; i++)
	{
		UserLangContainer & userLangContainer = pNppParam->getULCFromIndex(i);
		::InsertMenu(hLangMenu, udlpos + i, MF_BYPOSITION, IDM_LANG_USER + i + 1, userLangContainer.getName().c_str());
	}

	//Add recent files
	HMENU hFileMenu = ::GetSubMenu(_mainMenuHandle, MENUINDEX_FILE);
	int nbLRFile = pNppParam->getNbLRFile();
	int pos = IDM_FILEMENU_LASTONE - IDM_FILE + 2;

	_lastRecentFileList->initMenu(hFileMenu, IDM_FILEMENU_LASTONE + 1, pos);
	_lastRecentFileList->setLangEncoding(_nativeLangSpeaker->getLangEncoding());
	for (int i = 0 ; i < nbLRFile ; i++)
	{
		const generic_string& stdStr = pNppParam->getLRFile(i);
		if (!nppGUI._checkHistoryFiles || PathFileExists(stdStr.c_str()))
		{
			_lastRecentFileList->add(stdStr.c_str());
		}
	}

	//Plugin menu
	_pluginsManager->setMenu(_mainMenuHandle, NULL);

	//Main menu is loaded, now load context menu items
	pNppParam->getContextMenuFromXmlTree(_mainMenuHandle);

	if (pNppParam->hasCustomContextMenu())
	{
		_mainEditView->execute(SCI_USEPOPUP, FALSE);
		_subEditView->execute(SCI_USEPOPUP, FALSE);
	}

	generic_string pluginsTrans, windowTrans;
	_nativeLangSpeaker->changeMenuLang(_mainMenuHandle, pluginsTrans, windowTrans);
	::DrawMenuBar(_pPublicInterface->getHSelf());

	if (_pluginsManager->hasPlugins() && pluginsTrans != TEXT(""))
	{
		::ModifyMenu(_mainMenuHandle, MENUINDEX_PLUGINS, MF_BYPOSITION, 0, pluginsTrans.c_str());
	}
	//Windows menu
	_windowsMenu->init(_pPublicInterface->getHinst(), _mainMenuHandle, windowTrans.c_str());

	// Update context menu strings
	std::vector<MenuItemUnit> & tmp = pNppParam->getContextMenuItems();
	size_t len = tmp.size();
	TCHAR menuName[64];
	for (size_t i = 0 ; i < len ; i++)
	{
		if (tmp[i]._itemName == TEXT(""))
		{
			::GetMenuString(_mainMenuHandle, tmp[i]._cmdID, menuName, 64, MF_BYCOMMAND);
			tmp[i]._itemName = purgeMenuItemString(menuName);
		}
	}

	//Input all the menu item names into shortcut list
	//This will automatically do all translations, since menu translation has been done already
	std::vector<CommandShortcut> & shortcuts = pNppParam->getUserShortcuts();
	len = shortcuts.size();

	for(size_t i = 0; i < len; i++)
	{
		CommandShortcut & csc = shortcuts[i];
		if (!csc.getName()[0])
		{	//no predefined name, get name from menu and use that
			::GetMenuString(_mainMenuHandle, csc.getID(), menuName, 64, MF_BYCOMMAND);
			csc.setName(purgeMenuItemString(menuName, true).c_str());
		}
	}
	//Translate non-menu shortcuts
	_nativeLangSpeaker->changeShortcutLang();

	//Update plugin shortcuts, all plugin commands should be available now
	pNppParam->reloadPluginCmds();

	// Shortcut Accelerator : should be the last one since it will capture all the shortcuts
	_accelerator.init(_mainMenuHandle, hwnd);
	pNppParam->setAccelerator(&_accelerator);

	// Scintilla key accelerator
	std::vector<HWND> scints;
	scints.push_back(_mainEditView->getHSelf());
	scints.push_back(_subEditView->getHSelf());
	_scintaccelerator.init(&scints, _mainMenuHandle, hwnd);

	pNppParam->setScintillaAccelerator(&_scintaccelerator);
	_scintaccelerator.updateKeys();

	::DrawMenuBar(hwnd);


	//-- Tool Bar Section --//
	toolBarStatusType tbStatus = nppGUI._toolBarStatus;
	willBeShown = nppGUI._toolbarShow;

	// To notify plugins that toolbar icons can be registered
	SCNotification scnN;
	scnN.nmhdr.code = NPPN_TBMODIFICATION;
	scnN.nmhdr.hwndFrom = hwnd;
	scnN.nmhdr.idFrom = 0;
	_pluginsManager->notify(&scnN);

	_toolBar->init(_pPublicInterface->getHinst(), hwnd, tbStatus, toolBarIcons, sizeof(toolBarIcons)/sizeof(ToolBarButtonUnit));

	changeToolBarIcons();

	_rebarTop->init(_pPublicInterface->getHinst(), hwnd);
	_rebarBottom->init(_pPublicInterface->getHinst(), hwnd);
	_toolBar->addToRebar(_rebarTop);
	_rebarTop->setIDVisible(REBAR_BAR_TOOLBAR, willBeShown);

	//--Init dialogs--//
	_findReplaceDlg->init(_pPublicInterface->getHinst(), hwnd, &_pEditView);
	_incrementFindDlg->init(_pPublicInterface->getHinst(), hwnd, _findReplaceDlg, _nativeLangSpeaker->isRTL());
	_incrementFindDlg->addToRebar(_rebarBottom);
	_goToLineDlg->init(_pPublicInterface->getHinst(), hwnd, &_pEditView);
	_colEditorDlg->init(_pPublicInterface->getHinst(), hwnd, &_pEditView);
	_aboutDlg->init(_pPublicInterface->getHinst(), hwnd);
	_runDlg->init(_pPublicInterface->getHinst(), hwnd);
	_runMacroDlg->init(_pPublicInterface->getHinst(), hwnd);

	//--User Define Dialog Section--//
	int uddStatus = nppGUI._userDefineDlgStatus;
	UserDefineDialog *udd = ScintillaEditView::getUserDefineDlg();

	bool uddShow = false;
	switch (uddStatus)
	{
		case UDD_SHOW :                 // show & undocked
			udd->doDialog(true, _nativeLangSpeaker->isRTL());
			_nativeLangSpeaker->changeUserDefineLang(udd);
			uddShow = true;
			break;
		case UDD_DOCKED : {              // hide & docked
			_isUDDocked = true;
			break;}
		case (UDD_SHOW | UDD_DOCKED) :    // show & docked
			udd->doDialog(true, _nativeLangSpeaker->isRTL());
			_nativeLangSpeaker->changeUserDefineLang(udd);
			::SendMessage(udd->getHSelf(), WM_COMMAND, IDC_DOCK_BUTTON, 0);
			uddShow = true;
			break;

		default :                        // hide & undocked
			break;
	}
	// UserDefine Dialog

	checkMenuItem(IDM_VIEW_USER_DLG, uddShow);
	_toolBar->setCheck(IDM_VIEW_USER_DLG, uddShow);

	//launch the plugin dlg memorized at the last session
	DockingManagerData &dmd = nppGUI._dockingData;

	_dockingManager->setDockedContSize(CONT_LEFT  , nppGUI._dockingData._leftWidth);
	_dockingManager->setDockedContSize(CONT_RIGHT , nppGUI._dockingData._rightWidth);
	_dockingManager->setDockedContSize(CONT_TOP	 , nppGUI._dockingData._topHeight);
	_dockingManager->setDockedContSize(CONT_BOTTOM, nppGUI._dockingData._bottomHight);

	for (size_t i = 0 ; i < dmd._pluginDockInfo.size() ; i++)
	{
		PlugingDlgDockingInfo & pdi = dmd._pluginDockInfo[i];

		if (pdi._isVisible)
			_pluginsManager->runPluginCommand(pdi._name.c_str(), pdi._internalID);
	}

	for (size_t i = 0 ; i < dmd._containerTabInfo.size() ; i++)
	{
		ContainerTabInfo & cti = dmd._containerTabInfo[i];
		_dockingManager->setActiveTab(cti._cont, cti._activeTab);
	}
	//Load initial docs into doctab
	loadBufferIntoView(_mainEditView->getCurrentBufferID(), MAIN_VIEW);
	loadBufferIntoView(_subEditView->getCurrentBufferID(), SUB_VIEW);
	activateBuffer(_mainEditView->getCurrentBufferID(), MAIN_VIEW);
	activateBuffer(_subEditView->getCurrentBufferID(), SUB_VIEW);
	MainFileManager->increaseDocNr();	//so next doc starts at 2

	::SetFocus(_mainEditView->getHSelf());
	return TRUE;
}



void Notepad_plus::killAllChildren()
{
	if (_rebarTop)
	{
		delete _rebarTop;
		_rebarTop = NULL;
	}

	if (_rebarBottom)
	{
		delete _rebarBottom;
		_rebarBottom = NULL;
	}

    if (_pMainSplitter)
    {
        delete _pMainSplitter;
        _pMainSplitter = NULL;
    }

	if (_mainDocTab)
	{
		delete _mainDocTab;
		_mainDocTab = NULL;
	}

	if (_subDocTab)
	{
		delete _subDocTab;
		_subDocTab = NULL;
	}

	_mainEditView->destroy();
    _subEditView->destroy();
	_invisibleEditView->destroy();

	if (_subSplitter)
	{
		delete _subSplitter;
		_subSplitter = NULL;
	}

	_scintillaCtrls4Plugins.destroy();

	if (_dockingManager)
	{
		delete _dockingManager;
		_dockingManager = NULL;
	}

	if (_statusBar)
	{
		delete _statusBar;
		_statusBar = NULL;
	}

	if	(_toolBar)
	{
		delete _toolBar;
		_toolBar = NULL;
	}

	if (_smartHighlighter)
	{
		delete _smartHighlighter;
		_smartHighlighter = NULL;
	}

	if (_findReplaceDlg)
	{
		delete _findReplaceDlg;
		_findReplaceDlg = NULL;
	}

	if (_incrementFindDlg)
	{
		delete _incrementFindDlg;
		_incrementFindDlg = NULL;
	}

	if (_aboutDlg)
	{
		delete _aboutDlg;
		_aboutDlg = NULL;
	}

	if (_runDlg)
	{
		delete _runDlg;
		_runDlg = NULL;
	}

	if (_goToLineDlg)
	{
		delete _goToLineDlg;
		_goToLineDlg = NULL;
	}

	if (_colEditorDlg)
	{
		delete _colEditorDlg;
		_colEditorDlg = NULL;
	}

	if (_configStyleDlg)
	{
		delete _configStyleDlg;
		_configStyleDlg = NULL;
	}

	if (_preferenceDlg)
	{
		delete _preferenceDlg;
		_preferenceDlg = NULL;
	}

	if (_restoreButton)
	{
		delete _restoreButton;
		_restoreButton = NULL;
	}

	if (_windowsMenu)
	{
		delete _windowsMenu;
		_windowsMenu = NULL;
	}

	if (_runMacroDlg)
	{
		delete _runMacroDlg;
		_runMacroDlg = NULL;
	}

	if (_docTabIconList)
	{
		delete _docTabIconList;
		_docTabIconList = NULL;
	}

	if (_tabPopupMenu)
	{
		delete _tabPopupMenu;
		_tabPopupMenu = NULL;
	}

	if (_tabPopupDropMenu)
	{
		delete _tabPopupDropMenu;
		_tabPopupDropMenu = NULL;
	}
}

bool Notepad_plus::saveGUIParams()
{
	assert(_rebarTop);
	assert(_statusBar);
	assert(_toolBar);
	assert(_subSplitter);

	NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
	nppGUI._toolbarShow = _rebarTop->getIDVisible(REBAR_BAR_TOOLBAR);
	nppGUI._toolBarStatus = _toolBar->getState();

	nppGUI._tabStatus = (TabBarPlus::doDragNDropOrNot()?TAB_DRAWTOPBAR:0) | \
						(TabBarPlus::drawTopBar()?TAB_DRAGNDROP:0) | \
						(TabBarPlus::drawInactiveTab()?TAB_DRAWINACTIVETAB:0) | \
						(_toReduceTabBar?TAB_REDUCE:0) | \
						(TabBarPlus::drawTabCloseButton()?TAB_CLOSEBUTTON:0) | \
						(TabBarPlus::isDbClk2Close()?TAB_DBCLK2CLOSE:0) | \
						(TabBarPlus::isVertical() ? TAB_VERTICAL:0) | \
						(TabBarPlus::isMultiLine() ? TAB_MULTILINE:0) |\
						(nppGUI._tabStatus & TAB_HIDE);
	nppGUI._splitterPos = _subSplitter->isVertical()?POS_VERTICAL:POS_HORIZOTAL;
	UserDefineDialog *udd = ScintillaEditView::getUserDefineDlg();
	bool b = udd->isDocked();
	nppGUI._userDefineDlgStatus = (b?UDD_DOCKED:0) | (udd->isVisible()?UDD_SHOW:0);

	// Save the position

	WINDOWPLACEMENT posInfo;

    posInfo.length = sizeof(WINDOWPLACEMENT);
	::GetWindowPlacement(_pPublicInterface->getHSelf(), &posInfo);

	nppGUI._appPos.left = posInfo.rcNormalPosition.left;
	nppGUI._appPos.top = posInfo.rcNormalPosition.top;
	nppGUI._appPos.right = posInfo.rcNormalPosition.right - posInfo.rcNormalPosition.left;
	nppGUI._appPos.bottom = posInfo.rcNormalPosition.bottom - posInfo.rcNormalPosition.top;
	nppGUI._isMaximized = (IsZoomed(_pPublicInterface->getHSelf()) || (posInfo.flags & WPF_RESTORETOMAXIMIZED));

	saveDockingParams();

	return (NppParameters::getInstance())->writeGUIParams();
}

void Notepad_plus::saveDockingParams()
{
	assert(_dockingManager);

	NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();

	// Save the docking information
	nppGUI._dockingData._leftWidth		= _dockingManager->getDockedContSize(CONT_LEFT);
	nppGUI._dockingData._rightWidth		= _dockingManager->getDockedContSize(CONT_RIGHT);
	nppGUI._dockingData._topHeight		= _dockingManager->getDockedContSize(CONT_TOP);
	nppGUI._dockingData._bottomHight	= _dockingManager->getDockedContSize(CONT_BOTTOM);

	// clear the conatainer tab information (active tab)
	nppGUI._dockingData._containerTabInfo.clear();

	// create a vector to save the current information
	std::vector<PlugingDlgDockingInfo>	vPluginDockInfo;
	std::vector<FloatingWindowInfo>		vFloatingWindowInfo;

	// save every container
	std::vector<DockingCont*> vCont = _dockingManager->getContainerInfo();

	for (size_t i = 0 ; i < vCont.size() ; i++)
	{
		// save at first the visible Tb's
		std::vector<tTbData *>	vDataVis	= vCont[i]->getDataOfVisTb();

		for (size_t j = 0 ; j < vDataVis.size() ; j++)
		{
			if (vDataVis[j]->pszName && vDataVis[j]->pszName[0])
			{
				PlugingDlgDockingInfo pddi(vDataVis[j]->pszModuleName, vDataVis[j]->dlgID, i, vDataVis[j]->iPrevCont, true);
				vPluginDockInfo.push_back(pddi);
			}
		}

		// save the hidden Tb's
		std::vector<tTbData *>	vDataAll	= vCont[i]->getDataOfAllTb();

		for (size_t j = 0 ; j < vDataAll.size() ; j++)
		{
			if ((vDataAll[j]->pszName && vDataAll[j]->pszName[0]) && (!vCont[i]->isTbVis(vDataAll[j])))
			{
				PlugingDlgDockingInfo pddi(vDataAll[j]->pszModuleName, vDataAll[j]->dlgID, i, vDataAll[j]->iPrevCont, false);
				vPluginDockInfo.push_back(pddi);
			}
		}

		// save the position, when container is a floated one
		if (i >= DOCKCONT_MAX)
		{
			RECT	rc;
			vCont[i]->getWindowRect(rc);
			FloatingWindowInfo fwi(i, rc.left, rc.top, rc.right, rc.bottom);
			vFloatingWindowInfo.push_back(fwi);
		}

		// save the active tab
		ContainerTabInfo act(i, vCont[i]->getActiveTb());
		nppGUI._dockingData._containerTabInfo.push_back(act);
	}

	// add the missing information and store it in nppGUI
	UCHAR floatContArray[50];
	memset(floatContArray, 0, 50);

	for (size_t i = 0 ; i < nppGUI._dockingData._pluginDockInfo.size() ; i++)
	{
		BOOL	isStored = FALSE;
		for (size_t j = 0; j < vPluginDockInfo.size(); j++)
		{
			if (nppGUI._dockingData._pluginDockInfo[i] == vPluginDockInfo[j])
			{
				isStored = TRUE;
				break;
			}
		}

		if (isStored == FALSE)
		{
			int floatCont	= 0;

			if (nppGUI._dockingData._pluginDockInfo[i]._currContainer >= DOCKCONT_MAX)
				floatCont = nppGUI._dockingData._pluginDockInfo[i]._currContainer;
			else
				floatCont = nppGUI._dockingData._pluginDockInfo[i]._prevContainer;

			if (floatContArray[floatCont] == 0)
			{
				RECT *pRc = nppGUI._dockingData.getFloatingRCFrom(floatCont);
				if (pRc)
					vFloatingWindowInfo.push_back(FloatingWindowInfo(floatCont, pRc->left, pRc->top, pRc->right, pRc->bottom));
				floatContArray[floatCont] = 1;
			}

			vPluginDockInfo.push_back(nppGUI._dockingData._pluginDockInfo[i]);
		}
	}

	nppGUI._dockingData._pluginDockInfo = vPluginDockInfo;
	nppGUI._dockingData._flaotingWindowInfo = vFloatingWindowInfo;
}


int Notepad_plus::getHtmlXmlEncoding(const TCHAR *fileName) const
{
	// Get Language type
	TCHAR *ext = PathFindExtension(fileName);
	if (*ext == '.') //extension found
	{
		ext += 1;
	}
	else
	{
		return -1;
	}
	NppParameters *pNppParamInst = NppParameters::getInstance();
	LangType langT = pNppParamInst->getLangFromExt(ext);
	if (langT != L_XML && langT != L_HTML && langT == L_PHP)
		return -1;

	// Get the begining of file data
	FILE *f = NULL;
	generic_fopen(f, fileName, TEXT("rb"));
	if (!f)
		return -1;
	const int blockSize = 1024; // To ensure that length is long enough to capture the encoding in html
	char data[blockSize];
	int lenFile = fread(data, 1, blockSize, f);
	fclose(f);

	// Put data in _invisibleEditView
	_invisibleEditView->execute(SCI_CLEARALL);
    _invisibleEditView->execute(SCI_APPENDTEXT, lenFile, (LPARAM)data);

	const char *encodingAliasRegExpr = "[a-zA-Z0-9_-]+";

	if (langT == L_XML)
	{
		// find encoding by RegExpr

		const char *xmlHeaderRegExpr = "<?xml[ \\t]+version[ \\t]*=[ \\t]*\"[^\"]+\"[ \\t]+encoding[ \\t]*=[ \\t]*\"[^\"]+\"[ \\t]*.*?>";

        int startPos = 0;
		int endPos = lenFile-1;
		_invisibleEditView->execute(SCI_SETSEARCHFLAGS, SCFIND_REGEXP|SCFIND_POSIX);

		_invisibleEditView->execute(SCI_SETTARGETSTART, startPos);
		_invisibleEditView->execute(SCI_SETTARGETEND, endPos);

		int posFound = _invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(xmlHeaderRegExpr), (LPARAM)xmlHeaderRegExpr);
		if (posFound != -1)
		{
            const char *encodingBlockRegExpr = "encoding[ \\t]*=[ \\t]*\"[^\".]+\"";
            _invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(encodingBlockRegExpr), (LPARAM)encodingBlockRegExpr);

            const char *encodingRegExpr = "\".+\"";
            _invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(encodingRegExpr), (LPARAM)encodingRegExpr);

			_invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(encodingAliasRegExpr), (LPARAM)encodingAliasRegExpr);

            startPos = int(_invisibleEditView->execute(SCI_GETTARGETSTART));
			endPos = int(_invisibleEditView->execute(SCI_GETTARGETEND));

            char encodingStr[128];
            _invisibleEditView->getText(encodingStr, startPos, endPos);

			EncodingMapper *em = EncodingMapper::getInstance();
            int enc = em->getEncodingFromString(encodingStr);
            return (enc==CP_ACP?-1:enc);
		}
        return -1;
	}
	else // if (langT == L_HTML)
	{
		const char *htmlHeaderRegExpr  = "<meta[ \\t]+http-equiv[ \\t]*=[ \\t\"']*Content-Type[ \\t\"']*content[ \\t]*= *[\"']text/html;[ \\t]+charset[ \\t]*=[ \\t]*.+[\"'] */*>";
		const char *htmlHeaderRegExpr2 = "<meta[ \\t]+content[ \\t]*= *[\"']text/html;[ \\t]+charset[ \\t]*=[ \\t]*.+[ \\t\"']http-equiv[ \\t]*=[ \\t\"']*Content-Type[ \\t\"']*/*>";
		const char *charsetBlock = "charset[ \\t]*=[ \\t]*[^\"']+";
		const char *intermediaire = "=[ \\t]*.+";
		const char *encodingStrRE = "[^ \\t=]+";

        int startPos = 0;
		int endPos = lenFile-1;
		_invisibleEditView->execute(SCI_SETSEARCHFLAGS, SCFIND_REGEXP|SCFIND_POSIX);

		_invisibleEditView->execute(SCI_SETTARGETSTART, startPos);
		_invisibleEditView->execute(SCI_SETTARGETEND, endPos);

		int posFound = _invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(htmlHeaderRegExpr), (LPARAM)htmlHeaderRegExpr);

		if (posFound == -1)
		{
			posFound = _invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(htmlHeaderRegExpr2), (LPARAM)htmlHeaderRegExpr2);
			if (posFound == -1)
				return -1;
		}
		_invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(charsetBlock), (LPARAM)charsetBlock);
		_invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(intermediaire), (LPARAM)intermediaire);
		_invisibleEditView->execute(SCI_SEARCHINTARGET, strlen(encodingStrRE), (LPARAM)encodingStrRE);

        startPos = int(_invisibleEditView->execute(SCI_GETTARGETSTART));
		endPos = int(_invisibleEditView->execute(SCI_GETTARGETEND));

        char encodingStr[128];
        _invisibleEditView->getText(encodingStr, startPos, endPos);

		EncodingMapper *em = EncodingMapper::getInstance();
		int enc = em->getEncodingFromString(encodingStr);
        return (enc==CP_ACP?-1:enc);
	}
}


bool Notepad_plus::replaceAllFiles() {

	assert(_findReplaceDlg);

	ScintillaEditView *pOldView = _pEditView;
	_pEditView = _invisibleEditView;
	Document oldDoc = _invisibleEditView->execute(SCI_GETDOCPOINTER);
	Buffer * oldBuf = _invisibleEditView->getCurrentBuffer();	//for manually setting the buffer, so notifications can be handled properly

	Buffer * pBuf = NULL;

	int nbTotal = 0;
	const bool isEntireDoc = true;

    if (_mainWindowStatus & WindowMainActive)
    {
		assert(_mainDocTab);
		for (int i = 0 ; i < _mainDocTab->nbItem() ; i++)
	    {
			pBuf = MainFileManager->getBufferByID(_mainDocTab->getBufferByIndex(i));
			if (pBuf->isReadOnly())
				continue;
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
			int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
			_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);
			_invisibleEditView->_currentBuffer = pBuf;
		    _invisibleEditView->execute(SCI_BEGINUNDOACTION);
			nbTotal += _findReplaceDlg->processAll(ProcessReplaceAll, FindReplaceDlg::_env, isEntireDoc);
			_invisibleEditView->execute(SCI_ENDUNDOACTION);
		}
	}

	if (_mainWindowStatus & WindowSubActive)
    {
		assert(_subDocTab);
		for (int i = 0 ; i < _subDocTab->nbItem() ; i++)
	    {
			pBuf = MainFileManager->getBufferByID(_subDocTab->getBufferByIndex(i));
			if (pBuf->isReadOnly())
				continue;
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
			int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
			_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);
			_invisibleEditView->_currentBuffer = pBuf;
		    _invisibleEditView->execute(SCI_BEGINUNDOACTION);
			nbTotal += _findReplaceDlg->processAll(ProcessReplaceAll, FindReplaceDlg::_env, isEntireDoc);
			_invisibleEditView->execute(SCI_ENDUNDOACTION);
		}
	}

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, oldDoc);
	_invisibleEditView->_currentBuffer = oldBuf;
	_pEditView = pOldView;


	if (nbTotal < 0)
		::printStr(TEXT("The regular expression to search is formed badly"));
	else
	{
		if (nbTotal)
			enableCommand(IDM_FILE_SAVEALL, true, MENU | TOOLBAR);
		TCHAR result[64];
		wsprintf(result, TEXT("%d occurrences replaced."), nbTotal);
		::printStr(result);
	}


	return true;
}

bool Notepad_plus::matchInList(const TCHAR *fileName, const std::vector<generic_string> & patterns)
{
	for (size_t i = 0 ; i < patterns.size() ; i++)
	{
		if (PathMatchSpec(fileName, patterns[i].c_str()))
			return true;
	}
	return false;
}

void Notepad_plus::saveFindHistory()
{
	assert(_findReplaceDlg);
	_findReplaceDlg->saveFindHistory();
	(NppParameters::getInstance())->writeFindHistory();
}

void Notepad_plus::saveUserDefineLangs()
{
	if (ScintillaEditView::getUserDefineDlg()->isDirty())
		(NppParameters::getInstance())->writeUserDefinedLang();
}

void Notepad_plus::saveShortcuts()
{
	NppParameters::getInstance()->writeShortcuts();
}

void Notepad_plus::doTrimTrailing()
{
	_pEditView->execute(SCI_BEGINUNDOACTION);
	int nbLines = _pEditView->execute(SCI_GETLINECOUNT);
	for (int line = 0 ; line < nbLines ; line++)
	{
		int lineStart = _pEditView->execute(SCI_POSITIONFROMLINE,line);
		int lineEnd = _pEditView->execute(SCI_GETLINEENDPOSITION,line);
		int i = lineEnd - 1;
		char c = (char)_pEditView->execute(SCI_GETCHARAT,i);

		for ( ; (i >= lineStart) && (c == ' ') || (c == '\t') ; c = (char)_pEditView->execute(SCI_GETCHARAT,i))
			i--;

		if (i < (lineEnd - 1))
			_pEditView->replaceTarget(TEXT(""), i + 1, lineEnd);
	}
	_pEditView->execute(SCI_ENDUNDOACTION);
}

void Notepad_plus::loadLastSession()
{
	Session* lastSession = (NppParameters::getInstance())->getSession();
	loadSession(lastSession);
}

void Notepad_plus::getMatchedFileNames(const TCHAR *dir, const std::vector<generic_string> & patterns, std::vector<generic_string> & fileNames, bool isRecursive, bool isInHiddenDir)
{
	generic_string dirFilter(dir);
	dirFilter += TEXT("*.*");
	WIN32_FIND_DATA foundData;

	HANDLE hFile = ::FindFirstFile(dirFilter.c_str(), &foundData);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		if (foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!isInHiddenDir && (foundData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				// branles rien
			}
			else if (isRecursive)
			{
				if ((lstrcmp(foundData.cFileName, TEXT("."))) && (lstrcmp(foundData.cFileName, TEXT(".."))))
				{
					generic_string pathDir(dir);
					pathDir += foundData.cFileName;
					pathDir += TEXT("\\");
					getMatchedFileNames(pathDir.c_str(), patterns, fileNames, isRecursive, isInHiddenDir);
				}
			}
		}
		else
		{
			if (matchInList(foundData.cFileName, patterns))
			{
				generic_string pathFile(dir);
				pathFile += foundData.cFileName;
				fileNames.push_back(pathFile.c_str());
			}
		}
	}
	while (::FindNextFile(hFile, &foundData))
	{
		if (foundData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!isInHiddenDir && (foundData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				// branles rien
			}
			else if (isRecursive)
			{
				if ((lstrcmp(foundData.cFileName, TEXT("."))) && (lstrcmp(foundData.cFileName, TEXT(".."))))
				{
					generic_string pathDir(dir);
					pathDir += foundData.cFileName;
					pathDir += TEXT("\\");
					getMatchedFileNames(pathDir.c_str(), patterns, fileNames, isRecursive, isInHiddenDir);
				}
			}
		}
		else
		{
			if (matchInList(foundData.cFileName, patterns))
			{
				generic_string pathFile(dir);
				pathFile += foundData.cFileName;
				fileNames.push_back(pathFile.c_str());
			}
		}
	}
	::FindClose(hFile);
}

DWORD WINAPI AsyncCancelFindInFiles(LPVOID NppHWND)
{
	MessageBox((HWND) NULL, TEXT("Searching...\nPress Enter to Cancel"), TEXT("Find In Files"), MB_OK);
	PostMessage((HWND) NppHWND, NPPM_INTERNAL_CANCEL_FIND_IN_FILES, 0, 0);
	return 0;
}

bool Notepad_plus::replaceInFiles()
{
	assert(_findReplaceDlg);
	const TCHAR *dir2Search = _findReplaceDlg->getDir2Search();
	if (!dir2Search[0] || !::PathFileExists(dir2Search))
	{
		return false;
	}

	bool isRecursive = _findReplaceDlg->isRecursive();
	bool isInHiddenDir = _findReplaceDlg->isInHiddenDir();
	int nbTotal = 0;

	ScintillaEditView *pOldView = _pEditView;
	_pEditView = _invisibleEditView;
	Document oldDoc = _invisibleEditView->execute(SCI_GETDOCPOINTER);
	Buffer * oldBuf = _invisibleEditView->getCurrentBuffer();	//for manually setting the buffer, so notifications can be handled properly
	HANDLE CancelThreadHandle = NULL;

	std::vector<generic_string> patterns2Match;
	_findReplaceDlg->getPatterns(patterns2Match);
	if (patterns2Match.size() == 0)
	{
		_findReplaceDlg->setFindInFilesDirFilter(NULL, TEXT("*.*"));
		_findReplaceDlg->getPatterns(patterns2Match);
	}
	std::vector<generic_string> fileNames;

	getMatchedFileNames(dir2Search, patterns2Match, fileNames, isRecursive, isInHiddenDir);

	if (fileNames.size() > 1)
		CancelThreadHandle = ::CreateThread(NULL, 0, AsyncCancelFindInFiles, _pPublicInterface->getHSelf(), 0, NULL);

	bool dontClose = false;
	for (size_t i = 0 ; i < fileNames.size() ; i++)
	{
		MSG msg;
		if (PeekMessage(&msg, _pPublicInterface->getHSelf(), NPPM_INTERNAL_CANCEL_FIND_IN_FILES, NPPM_INTERNAL_CANCEL_FIND_IN_FILES, PM_REMOVE)) break;

		BufferID id = MainFileManager->getBufferFromName(fileNames.at(i).c_str());
		if (id != BUFFER_INVALID)
		{
			dontClose = true;
		}
		else
		{
			id = MainFileManager->loadFile(fileNames.at(i).c_str());
			dontClose = false;
		}

		if (id != BUFFER_INVALID)
		{
			Buffer * pBuf = MainFileManager->getBufferByID(id);
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
			int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
			_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);
			_invisibleEditView->_currentBuffer = pBuf;

			int nbReplaced = _findReplaceDlg->processAll(ProcessReplaceAll, FindReplaceDlg::_env, true, fileNames.at(i).c_str());
			nbTotal += nbReplaced;
			if (nbReplaced)
			{
				MainFileManager->saveBuffer(id, pBuf->getFullPathName());
			}

			if (!dontClose)
				MainFileManager->closeBuffer(id, _pEditView);
		}
	}

	if (CancelThreadHandle)
		TerminateThread(CancelThreadHandle, 0);

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, oldDoc);
	_invisibleEditView->_currentBuffer = oldBuf;
	_pEditView = pOldView;

	TCHAR msg[128];
	wsprintf(msg, TEXT("%d occurences replaced"), nbTotal);
	printStr(msg);

	return true;
}

bool Notepad_plus::findInFiles()
{
	assert (_findReplaceDlg);
	const TCHAR *dir2Search = _findReplaceDlg->getDir2Search();

	if (!dir2Search[0] || !::PathFileExists(dir2Search))
	{
		return false;
	}

	bool isRecursive = _findReplaceDlg->isRecursive();
	bool isInHiddenDir = _findReplaceDlg->isInHiddenDir();
	int nbTotal = 0;
	ScintillaEditView *pOldView = _pEditView;
	_pEditView = _invisibleEditView;
	Document oldDoc = _invisibleEditView->execute(SCI_GETDOCPOINTER);
	HANDLE CancelThreadHandle = NULL;

	std::vector<generic_string> patterns2Match;
	_findReplaceDlg->getPatterns(patterns2Match);
	if (patterns2Match.size() == 0)
	{
		_findReplaceDlg->setFindInFilesDirFilter(NULL, TEXT("*.*"));
		_findReplaceDlg->getPatterns(patterns2Match);
	}
	std::vector<generic_string> fileNames;
	getMatchedFileNames(dir2Search, patterns2Match, fileNames, isRecursive, isInHiddenDir);

	if (fileNames.size() > 1)
		CancelThreadHandle = ::CreateThread(NULL, 0, AsyncCancelFindInFiles, _pPublicInterface->getHSelf(), 0, NULL);

	_findReplaceDlg->beginNewFilesSearch();

	bool dontClose = false;
	for (size_t i = 0 ; i < fileNames.size() ; i++)
	{
		MSG msg;
		if (PeekMessage(&msg, _pPublicInterface->getHSelf(), NPPM_INTERNAL_CANCEL_FIND_IN_FILES, NPPM_INTERNAL_CANCEL_FIND_IN_FILES, PM_REMOVE)) break;

		BufferID id = MainFileManager->getBufferFromName(fileNames.at(i).c_str());
		if (id != BUFFER_INVALID)
		{
			dontClose = true;
		}
		else
		{
			id = MainFileManager->loadFile(fileNames.at(i).c_str());
			dontClose = false;
		}

		if (id != BUFFER_INVALID)
		{
			Buffer * pBuf = MainFileManager->getBufferByID(id);
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
			int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
			_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);

			nbTotal += _findReplaceDlg->processAll(ProcessFindAll, FindReplaceDlg::_env, true, fileNames.at(i).c_str());
			if (!dontClose)
				MainFileManager->closeBuffer(id, _pEditView);
		}
	}

	if (CancelThreadHandle)
		TerminateThread(CancelThreadHandle, 0);

	_findReplaceDlg->finishFilesSearch(nbTotal);

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, oldDoc);
	_pEditView = pOldView;

	_findReplaceDlg->putFindResult(nbTotal);

	FindHistory & findHistory = (NppParameters::getInstance())->getFindHistory();
	if (nbTotal && !findHistory._isDlgAlwaysVisible)
		_findReplaceDlg->display(false);
	return true;
}


bool Notepad_plus::findInOpenedFiles()
{
	assert(_findReplaceDlg);

	int nbTotal = 0;
	ScintillaEditView *pOldView = _pEditView;
	_pEditView = _invisibleEditView;
	Document oldDoc = _invisibleEditView->execute(SCI_GETDOCPOINTER);

	Buffer * pBuf = NULL;

	const bool isEntireDoc = true;

	_findReplaceDlg->beginNewFilesSearch();

    if (_mainWindowStatus & WindowMainActive)
    {
		assert(_mainDocTab);
		for (int i = 0 ; i < _mainDocTab->nbItem() ; i++)
	    {
			pBuf = MainFileManager->getBufferByID(_mainDocTab->getBufferByIndex(i));
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
			int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
			_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);
			nbTotal += _findReplaceDlg->processAll(ProcessFindAll, FindReplaceDlg::_env, isEntireDoc, pBuf->getFullPathName());
	    }
    }

    if (_mainWindowStatus & WindowSubActive)
    {
		assert(_subDocTab);
		for (int i = 0 ; i < _subDocTab->nbItem() ; i++)
	    {
			pBuf = MainFileManager->getBufferByID(_subDocTab->getBufferByIndex(i));
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
			int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
			_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);
			nbTotal += _findReplaceDlg->processAll(ProcessFindAll, FindReplaceDlg::_env, isEntireDoc, pBuf->getFullPathName());
	    }
    }

	_findReplaceDlg->finishFilesSearch(nbTotal);

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, oldDoc);
	_pEditView = pOldView;

	_findReplaceDlg->putFindResult(nbTotal);

	FindHistory & findHistory = (NppParameters::getInstance())->getFindHistory();
	if (nbTotal && !findHistory._isDlgAlwaysVisible)
		_findReplaceDlg->display(false);
	return true;
}


bool Notepad_plus::findInCurrentFile()
{
	assert(_findReplaceDlg);
	int nbTotal = 0;
	Buffer * pBuf = _pEditView->getCurrentBuffer();
	ScintillaEditView *pOldView = _pEditView;
	_pEditView = _invisibleEditView;
	Document oldDoc = _invisibleEditView->execute(SCI_GETDOCPOINTER);

	const bool isEntireDoc = true;

	_findReplaceDlg->beginNewFilesSearch();

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, pBuf->getDocument());
	int cp = _invisibleEditView->execute(SCI_GETCODEPAGE);
	_invisibleEditView->execute(SCI_SETCODEPAGE, pBuf->getUnicodeMode() == uni8Bit ? cp : SC_CP_UTF8);
	nbTotal += _findReplaceDlg->processAll(ProcessFindAll, FindReplaceDlg::_env, isEntireDoc, pBuf->getFullPathName());

	_findReplaceDlg->finishFilesSearch(nbTotal);

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, oldDoc);
	_pEditView = pOldView;

	_findReplaceDlg->putFindResult(nbTotal);

	FindHistory & findHistory = (NppParameters::getInstance())->getFindHistory();
	if (nbTotal && !findHistory._isDlgAlwaysVisible)
		_findReplaceDlg->display(false);
	return true;
}

void Notepad_plus::filePrint(bool showDialog)
{
	Printer printer;

	int startPos = int(_pEditView->execute(SCI_GETSELECTIONSTART));
	int endPos = int(_pEditView->execute(SCI_GETSELECTIONEND));

	printer.init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf(), _pEditView, showDialog, startPos, endPos);
	printer.doPrint();
}

int Notepad_plus::doSaveOrNot(const TCHAR *fn)
{
	const TCHAR pattern[64] = TEXT("Save file \"%s\" ?");
	TCHAR phrase[512];
	wsprintf(phrase, pattern, fn);
	return doActionOrNot(TEXT("Save"), phrase, MB_YESNOCANCEL | MB_ICONQUESTION | MB_APPLMODAL);
}

int Notepad_plus::doReloadOrNot(const TCHAR *fn, bool dirty)
{
	const TCHAR* pattern = TEXT("%s\r\rThis file has been modified by another program.\rDo you want to reload it%s?");
	const TCHAR* lose_info_str = dirty ? TEXT(" and lose the changes made in Notepad++") : TEXT("");
	TCHAR phrase[512];
	wsprintf(phrase, pattern, fn, lose_info_str);
	int icon = dirty ? MB_ICONEXCLAMATION : MB_ICONQUESTION;
	return doActionOrNot(TEXT("Reload"), phrase, MB_YESNO | MB_APPLMODAL | icon);
}

int Notepad_plus::doCloseOrNot(const TCHAR *fn)
{
	const TCHAR pattern[128] = TEXT("The file \"%s\" doesn't exist anymore.\rKeep this file in editor?");
	TCHAR phrase[512];
	wsprintf(phrase, pattern, fn);
	return doActionOrNot(TEXT("Keep non existing file"), phrase, MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
}

int Notepad_plus::doDeleteOrNot(const TCHAR *fn)
{
	const TCHAR pattern[128] = TEXT("The file \"%s\"\rwill be deleted from your disk and this document will be closed.\rContinue?");
	TCHAR phrase[512];
	wsprintf(phrase, pattern, fn);
	return doActionOrNot(TEXT("Delete file"), phrase, MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
}

int Notepad_plus::doActionOrNot(const TCHAR *title, const TCHAR *displayText, int type)
{
	return ::MessageBox(_pPublicInterface->getHSelf(), displayText, title, type);
}

void Notepad_plus::enableMenu(int cmdID, bool doEnable) const
{
	int flag = doEnable?MF_ENABLED | MF_BYCOMMAND:MF_DISABLED | MF_GRAYED | MF_BYCOMMAND;
	::EnableMenuItem(_mainMenuHandle, cmdID, flag);
}

void Notepad_plus::enableCommand(int cmdID, bool doEnable, int which) const
{
	if (which & MENU)
	{
		enableMenu(cmdID, doEnable);
	}
	if (which & TOOLBAR)
	{
		assert(_toolBar);
		_toolBar->enable(cmdID, doEnable);
	}
}

void Notepad_plus::checkClipboard()
{

	bool hasSelection = (_pEditView->execute(SCI_GETSELECTIONSTART) != _pEditView->execute(SCI_GETSELECTIONEND)) || (_pEditView->execute(SCI_GETSELECTIONS) > 0);
	bool canPaste = (_pEditView->execute(SCI_CANPASTE) != 0);
	enableCommand(IDM_EDIT_CUT, hasSelection, MENU | TOOLBAR);
	enableCommand(IDM_EDIT_COPY, hasSelection, MENU | TOOLBAR);
	enableCommand(IDM_EDIT_PASTE, canPaste, MENU | TOOLBAR);
	enableCommand(IDM_EDIT_DELETE, hasSelection, MENU | TOOLBAR);
	enableCommand(IDM_EDIT_UPPERCASE, hasSelection, MENU);
	enableCommand(IDM_EDIT_LOWERCASE, hasSelection, MENU);
	enableCommand(IDM_EDIT_BLOCK_COMMENT, hasSelection, MENU);
	enableCommand(IDM_EDIT_BLOCK_COMMENT_SET, hasSelection, MENU);
	enableCommand(IDM_EDIT_BLOCK_UNCOMMENT, hasSelection, MENU);
	enableCommand(IDM_EDIT_STREAM_COMMENT, hasSelection, MENU);
}

void Notepad_plus::checkDocState()
{
	Buffer * curBuf = _pEditView->getCurrentBuffer();

	bool isCurrentDirty = curBuf->isDirty();
	bool isSeveralDirty = isCurrentDirty;
	bool isFileExisting = PathFileExists(curBuf->getFullPathName()) != FALSE;
	if (!isCurrentDirty)
	{
		for(int i = 0; i < MainFileManager->getNrBuffers(); i++)
		{
			if (MainFileManager->getBufferByIndex(i)->isDirty())
			{
				isSeveralDirty = true;
				break;
			}
		}
	}

	bool isCurrentUntitled = curBuf->isUntitled();
	enableCommand(IDM_FILE_SAVE, isCurrentDirty, MENU | TOOLBAR);
	enableCommand(IDM_FILE_SAVEALL, isSeveralDirty, MENU | TOOLBAR);
	enableCommand(IDM_VIEW_GOTO_NEW_INSTANCE, !(isCurrentDirty || isCurrentUntitled), MENU);
	enableCommand(IDM_VIEW_LOAD_IN_NEW_INSTANCE, !(isCurrentDirty || isCurrentUntitled), MENU);

	bool isSysReadOnly = curBuf->getFileReadOnly();
	if (isSysReadOnly)
	{
		::CheckMenuItem(_mainMenuHandle, IDM_EDIT_SETREADONLY, MF_BYCOMMAND | MF_UNCHECKED);
		enableCommand(IDM_EDIT_SETREADONLY, false, MENU);
		enableCommand(IDM_EDIT_CLEARREADONLY, true, MENU);
	}
	else
	{
		enableCommand(IDM_EDIT_SETREADONLY, true, MENU);
		enableCommand(IDM_EDIT_CLEARREADONLY, false, MENU);
		bool isUserReadOnly = curBuf->getUserReadOnly();
		::CheckMenuItem(_mainMenuHandle, IDM_EDIT_SETREADONLY, MF_BYCOMMAND | (isUserReadOnly?MF_CHECKED:MF_UNCHECKED));
	}
	enableCommand(IDM_FILE_DELETE, isFileExisting, MENU);
	enableCommand(IDM_FILE_RENAME, isFileExisting, MENU);

	enableConvertMenuItems(curBuf->getFormat());
	checkUnicodeMenuItems(/*curBuf->getUnicodeMode()*/);
	checkLangsMenu(-1);
}

void Notepad_plus::checkUndoState()
{
	enableCommand(IDM_EDIT_UNDO, _pEditView->execute(SCI_CANUNDO) != 0, MENU | TOOLBAR);
	enableCommand(IDM_EDIT_REDO, _pEditView->execute(SCI_CANREDO) != 0, MENU | TOOLBAR);
}

void Notepad_plus::checkMacroState()
{
	enableCommand(IDM_MACRO_STARTRECORDINGMACRO, !_recordingMacro, MENU | TOOLBAR);
	enableCommand(IDM_MACRO_STOPRECORDINGMACRO, _recordingMacro, MENU | TOOLBAR);
	enableCommand(IDM_MACRO_PLAYBACKRECORDEDMACRO, !_macro.empty() && !_recordingMacro, MENU | TOOLBAR);
	enableCommand(IDM_MACRO_SAVECURRENTMACRO, !_macro.empty() && !_recordingMacro, MENU | TOOLBAR);

	enableCommand(IDM_MACRO_RUNMULTIMACRODLG, (!_macro.empty() && !_recordingMacro) || !((NppParameters::getInstance())->getMacroList()).empty(), MENU | TOOLBAR);
}

void Notepad_plus::checkSyncState()
{
	bool canDoSync = viewVisible(MAIN_VIEW) && viewVisible(SUB_VIEW);
	if (!canDoSync)
	{
		assert(_toolBar);
		_syncInfo._isSynScollV = false;
		_syncInfo._isSynScollH = false;
		checkMenuItem(IDM_VIEW_SYNSCROLLV, false);
		checkMenuItem(IDM_VIEW_SYNSCROLLH, false);
		_toolBar->setCheck(IDM_VIEW_SYNSCROLLV, false);
		_toolBar->setCheck(IDM_VIEW_SYNSCROLLH, false);
	}
	enableCommand(IDM_VIEW_SYNSCROLLV, canDoSync, MENU | TOOLBAR);
	enableCommand(IDM_VIEW_SYNSCROLLH, canDoSync, MENU | TOOLBAR);
}

void Notepad_plus::checkLangsMenu(int id) const
{
	Buffer * curBuf = _pEditView->getCurrentBuffer();
	if (id == -1)
	{
		id = (NppParameters::getInstance())->langTypeToCommandID(curBuf->getLangType());
		if (id == IDM_LANG_USER)
		{
			if (curBuf->isUserDefineLangExt())
			{
				const TCHAR *userLangName = curBuf->getUserDefineLangName();
				TCHAR menuLangName[16];

				for (int i = IDM_LANG_USER + 1 ; i <= IDM_LANG_USER_LIMIT ; i++)
				{
					if (::GetMenuString(_mainMenuHandle, i, menuLangName, sizeof(menuLangName), MF_BYCOMMAND))
						if (!lstrcmp(userLangName, menuLangName))
						{
							::CheckMenuRadioItem(_mainMenuHandle, IDM_LANG_C, IDM_LANG_USER_LIMIT, i, MF_BYCOMMAND);
							return;
						}
				}
			}
		}
	}
	::CheckMenuRadioItem(_mainMenuHandle, IDM_LANG_C, IDM_LANG_USER_LIMIT, id, MF_BYCOMMAND);
}

generic_string Notepad_plus::getLangDesc(LangType langType, bool shortDesc)
{

	if ((langType >= L_EXTERNAL) && (langType < NppParameters::getInstance()->L_END))
	{
		ExternalLangContainer & elc = NppParameters::getInstance()->getELCFromIndex(langType - L_EXTERNAL);
		if (shortDesc)
			return generic_string(elc._name);
		else
			return generic_string(elc._desc);
	}

	if (langType > L_EXTERNAL)
        langType = L_TEXT;

	generic_string str2Show = ScintillaEditView::langNames[langType].longName;

	if (langType == L_USER)
	{
		Buffer * currentBuf = _pEditView->getCurrentBuffer();
		if (currentBuf->isUserDefineLangExt())
		{
			str2Show += TEXT(" - ");
			str2Show += currentBuf->getUserDefineLangName();
		}
	}
	return str2Show;
}

void Notepad_plus::setLangStatus(LangType langType){
	_statusBar->setText(getLangDesc(langType).c_str(), STATUSBAR_DOC_TYPE);
};


void Notepad_plus::copyMarkedLines()
{
	int lastLine = _pEditView->lastZeroBasedLineNumber();
	generic_string globalStr = TEXT("");
	for (int i = lastLine ; i >= 0 ; i--)
	{
		if (bookmarkPresent(i))
		{
			generic_string currentStr = getMarkedLine(i) + globalStr;
			globalStr = currentStr;
		}
	}
	str2Cliboard(globalStr.c_str());
}

void Notepad_plus::cutMarkedLines()
{
	int lastLine = _pEditView->lastZeroBasedLineNumber();
	generic_string globalStr = TEXT("");

	_pEditView->execute(SCI_BEGINUNDOACTION);
	for (int i = lastLine ; i >= 0 ; i--)
	{
		if (bookmarkPresent(i))
		{
			generic_string currentStr = getMarkedLine(i) + globalStr;
			globalStr = currentStr;

			deleteMarkedline(i);
		}
	}
	_pEditView->execute(SCI_ENDUNDOACTION);
	str2Cliboard(globalStr.c_str());
}

void Notepad_plus::deleteMarkedLines()
{
	int lastLine = _pEditView->lastZeroBasedLineNumber();

	_pEditView->execute(SCI_BEGINUNDOACTION);
	for (int i = lastLine ; i >= 0 ; i--)
	{
		if (bookmarkPresent(i))
			deleteMarkedline(i);
	}
	_pEditView->execute(SCI_ENDUNDOACTION);
}

void Notepad_plus::pasteToMarkedLines()
{
	int clipFormat;
#ifdef UNICODE
	clipFormat = CF_UNICODETEXT;
#else
	clipFormat = CF_TEXT;
#endif
	BOOL canPaste = ::IsClipboardFormatAvailable(clipFormat);
	if (!canPaste)
		return;
	int lastLine = _pEditView->lastZeroBasedLineNumber();

	::OpenClipboard(_pPublicInterface->getHSelf());
	HANDLE clipboardData = ::GetClipboardData(clipFormat);
	::GlobalSize(clipboardData);
	LPVOID clipboardDataPtr = ::GlobalLock(clipboardData);

	generic_string clipboardStr = (const TCHAR *)clipboardDataPtr;

	::GlobalUnlock(clipboardData);
	::CloseClipboard();

	_pEditView->execute(SCI_BEGINUNDOACTION);
	for (int i = lastLine ; i >= 0 ; i--)
	{
		if (bookmarkPresent(i))
		{
			replaceMarkedline(i, clipboardStr.c_str());
		}
	}
	_pEditView->execute(SCI_ENDUNDOACTION);
}

void Notepad_plus::deleteMarkedline(int ln)
{
	int lineLen = _pEditView->execute(SCI_LINELENGTH, ln);
	int lineBegin = _pEditView->execute(SCI_POSITIONFROMLINE, ln);

	bookmarkDelete(ln);
	TCHAR emptyString[2] = TEXT("");
	_pEditView->replaceTarget(emptyString, lineBegin, lineBegin + lineLen);
}

void Notepad_plus::inverseMarks()
{
	int lastLine = _pEditView->lastZeroBasedLineNumber();
	for (int i = 0 ; i <= lastLine  ; i++)
	{
		if (bookmarkPresent(i))
		{
			bookmarkDelete(i);
		}
		else
		{
			bookmarkAdd(i);
		}
	}
}

void Notepad_plus::replaceMarkedline(int ln, const TCHAR *str)
{
	int lineBegin = _pEditView->execute(SCI_POSITIONFROMLINE, ln);
	int lineEnd = _pEditView->execute(SCI_GETLINEENDPOSITION, ln);

	_pEditView->replaceTarget(str, lineBegin, lineEnd);
}

generic_string Notepad_plus::getMarkedLine(int ln)
{
	int lineLen = _pEditView->execute(SCI_LINELENGTH, ln);
	int lineBegin = _pEditView->execute(SCI_POSITIONFROMLINE, ln);

	TCHAR * buf = new TCHAR[lineLen+1];
	_pEditView->getGenericText(buf, lineBegin, lineBegin + lineLen);
	generic_string line = buf;
	delete [] buf;

	return line;
}

void Notepad_plus::findMatchingBracePos(int & braceAtCaret, int & braceOpposite)
{
	int caretPos = int(_pEditView->execute(SCI_GETCURRENTPOS));
	braceAtCaret = -1;
	braceOpposite = -1;
	TCHAR charBefore = '\0';

	int lengthDoc = int(_pEditView->execute(SCI_GETLENGTH));

	if ((lengthDoc > 0) && (caretPos > 0))
    {
		charBefore = TCHAR(_pEditView->execute(SCI_GETCHARAT, caretPos - 1, 0));
	}
	// Priority goes to character before caret
	if (charBefore && generic_strchr(TEXT("[](){}"), charBefore))
    {
		braceAtCaret = caretPos - 1;
	}

	if (lengthDoc > 0  && (braceAtCaret < 0))
    {
		// No brace found so check other side
		TCHAR charAfter = TCHAR(_pEditView->execute(SCI_GETCHARAT, caretPos, 0));
		if (charAfter && generic_strchr(TEXT("[](){}"), charAfter))
        {
			braceAtCaret = caretPos;
		}
	}
	if (braceAtCaret >= 0)
		braceOpposite = int(_pEditView->execute(SCI_BRACEMATCH, braceAtCaret, 0));
}

// return true if 1 or 2 (matched) brace(s) is found
bool Notepad_plus::braceMatch()
{
	int braceAtCaret = -1;
	int braceOpposite = -1;
	findMatchingBracePos(braceAtCaret, braceOpposite);

	if ((braceAtCaret != -1) && (braceOpposite == -1))
    {
		_pEditView->execute(SCI_BRACEBADLIGHT, braceAtCaret);
		_pEditView->execute(SCI_SETHIGHLIGHTGUIDE, 0);
	}
    else
    {
		_pEditView->execute(SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);

		if (_pEditView->isShownIndentGuide())
        {
            int columnAtCaret = int(_pEditView->execute(SCI_GETCOLUMN, braceAtCaret));
		    int columnOpposite = int(_pEditView->execute(SCI_GETCOLUMN, braceOpposite));
			_pEditView->execute(SCI_SETHIGHLIGHTGUIDE, (columnAtCaret < columnOpposite)?columnAtCaret:columnOpposite);
        }
    }

    enableCommand(IDM_SEARCH_GOTOMATCHINGBRACE, (braceAtCaret != -1) && (braceOpposite != -1), MENU | TOOLBAR);
    return (braceAtCaret != -1);
}


void Notepad_plus::setDisplayFormat(formatType f)
{
	generic_string str;
	switch (f)
	{
		case MAC_FORMAT :
			str = TEXT("Macintosh");
			break;
		case UNIX_FORMAT :
			str = TEXT("UNIX");
			break;
		default :
			str = TEXT("Dos\\Windows");
	}
	_statusBar->setText(str.c_str(), STATUSBAR_EOF_FORMAT);
}

void Notepad_plus::setUniModeText()
{
	assert(_statusBar);
	Buffer *buf = _pEditView->getCurrentBuffer();
	int encoding = buf->getEncoding();
	UniMode um = buf->getUnicodeMode();

	generic_string uniModeTextString;

	if (encoding == -1)
	{
		switch (um)
		{
			case uniUTF8:
				uniModeTextString = TEXT("UTF-8"); break;
			case uni16BE:
				uniModeTextString = TEXT("UCS-2 Big Endian"); break;
			case uni16LE:
				uniModeTextString = TEXT("UCS-2 Little Endian"); break;
			case uni16BE_NoBOM:
				uniModeTextString = TEXT("UCS-2 BE w/o BOM"); break;
			case uni16LE_NoBOM:
				uniModeTextString = TEXT("UCS-2 LE w/o BOM"); break;
			case uniCookie:
				uniModeTextString = TEXT("ANSI as UTF-8"); break;
			default :
				uniModeTextString = TEXT("ANSI");
		}
	}
	else
	{
		EncodingMapper *em = EncodingMapper::getInstance();
		int cmdID = em->getIndexFromEncoding(encoding);
		if (cmdID == -1)
		{
			//printStr(TEXT("Encoding problem. Encoding is not added in encoding_table?"));
			return;
		}
		cmdID += IDM_FORMAT_ENCODE;

		const int itemSize = 64;
		TCHAR uniModeText[itemSize];
		::GetMenuString(_mainMenuHandle, cmdID, uniModeText, itemSize, MF_BYCOMMAND);
		uniModeTextString = uniModeText;
	}
	_statusBar->setText(uniModeTextString.c_str(), STATUSBAR_UNICODE_TYPE);
}


void Notepad_plus::charAdded(TCHAR chAdded)
{
	bool indentMaintain = NppParameters::getInstance()->getNppGUI()._maitainIndent;
	if (indentMaintain)
		MaintainIndentation(chAdded);
}

void Notepad_plus::addHotSpot(bool docIsModifing)
{
	//bool docIsModifing = true;
	int posBegin2style = 0;
	if (docIsModifing)
		posBegin2style = _pEditView->execute(SCI_GETCURRENTPOS);

	int endStyle = _pEditView->execute(SCI_GETENDSTYLED);
	if (docIsModifing)
	{

		posBegin2style = _pEditView->execute(SCI_GETCURRENTPOS);
		if (posBegin2style > 0) posBegin2style--;
		UCHAR ch = (UCHAR)_pEditView->execute(SCI_GETCHARAT, posBegin2style);

		// determinating the type of EOF to make sure how many steps should we be back
		if ((ch == 0x0A) || (ch == 0x0D))
		{
			int eolMode = _pEditView->execute(SCI_GETEOLMODE);

			if ((eolMode == SC_EOL_CRLF) && (posBegin2style > 1))
				posBegin2style -= 2;
			else if (posBegin2style > 0)
				posBegin2style -= 1;
		}

		ch = (UCHAR)_pEditView->execute(SCI_GETCHARAT, posBegin2style);
		while ((posBegin2style > 0) && ((ch != 0x0A) && (ch != 0x0D)))
		{
			ch = (UCHAR)_pEditView->execute(SCI_GETCHARAT, posBegin2style--);
		}
	}

	int endStyled = _pEditView->execute(SCI_GETENDSTYLED, 0, 0);
	int lineEndStyled = _pEditView->execute(SCI_LINEFROMPOSITION, endStyled, 0);
	endStyled = _pEditView->execute(SCI_POSITIONFROMLINE, lineEndStyled, 0);

	int startPos = 0;
	int endPos = endStyled;

	_pEditView->execute(SCI_SETSEARCHFLAGS, SCFIND_REGEXP|SCFIND_POSIX);

	_pEditView->execute(SCI_SETTARGETSTART, startPos);
	_pEditView->execute(SCI_SETTARGETEND, endPos);

	std::vector<std::pair<int, int> > hotspotStylers;

	int bitsNeeded = _pEditView->execute(SCI_GETSTYLEBITSNEEDED);
	int styleMask = (1<<bitsNeeded)-1;

	int style_hotspot = styleMask-1;
	int posFound = _pEditView->execute(SCI_SEARCHINTARGET, strlen(URL_REG_EXPR), (LPARAM)URL_REG_EXPR);

	while (posFound != -1)
	{
		int start = int(_pEditView->execute(SCI_GETTARGETSTART));
		int end = int(_pEditView->execute(SCI_GETTARGETEND));
		int foundTextLen = end - start;
		int idStyle = _pEditView->execute(SCI_GETSTYLEAT, posFound);

		if (end < posBegin2style - 1)
		{
			if (style_hotspot > 1)
				style_hotspot--;
		}
		else
		{
			int fs = -1;
			if (_pEditView->IsHotspotStyleID(idStyle))
			{
				fs = idStyle;
			}
			else
			{
				for (size_t i = 0 ; i < hotspotStylers.size() ; i++)
				{
					if (hotspotStylers[i].second == idStyle)
					{
						fs = hotspotStylers[i].first;
						break;
					}
				}
			}

			if (fs != -1)
			{
				_pEditView->execute(SCI_STARTSTYLING, start, 0xFF);
				_pEditView->execute(SCI_SETSTYLING, foundTextLen, fs);

			}
			else
			{
				std::pair<int, int> MyPair(style_hotspot, idStyle);
				hotspotStylers.push_back(MyPair);

				Style hotspotStyle;

				if (_pEditView->getHotSpotFromStyle(hotspotStyle, idStyle) == false)
				{
					hotspotStyle._styleID = style_hotspot;
					_pEditView->createHotSpotFromStyle(hotspotStyle, idStyle, _nativeLangSpeaker->getLangEncoding());
					_pEditView->setHotspotStyle(hotspotStyle, idStyle);
				}

				_pEditView->execute(SCI_STARTSTYLING, start, styleMask);
				_pEditView->execute(SCI_SETSTYLING, foundTextLen, style_hotspot);
				if (style_hotspot > 1)
					style_hotspot--;
			}
		}

		_pEditView->execute(SCI_SETTARGETSTART, posFound + foundTextLen);
		_pEditView->execute(SCI_SETTARGETEND, endPos);

		posFound = _pEditView->execute(SCI_SEARCHINTARGET, strlen(URL_REG_EXPR), (LPARAM)URL_REG_EXPR);
	}

	_pEditView->execute(SCI_STARTSTYLING, endStyle, 0xFF);
	_pEditView->execute(SCI_SETSTYLING, 0, 0);
}



void Notepad_plus::MaintainIndentation(TCHAR ch)
{
	int eolMode = int(_pEditView->execute(SCI_GETEOLMODE));
	int curLine = int(_pEditView->getCurrentLineNumber());
	int lastLine = curLine - 1;
	int indentAmount = 0;

	if (((eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) && ch == '\n') ||
	        (eolMode == SC_EOL_CR && ch == '\r'))
	{
		while (lastLine >= 0 && _pEditView->getLineLength(lastLine) == 0)
			lastLine--;

		if (lastLine >= 0) {
			indentAmount = _pEditView->getLineIndent(lastLine);
		}
		if (indentAmount > 0) {
			_pEditView->setLineIndent(curLine, indentAmount);
		}
	}
}
void Notepad_plus::specialCmd(int id/*, int param*/)
{
	//if ((param != 1) && (param != 2)) return;

	NppParameters *pNppParam = NppParameters::getInstance();
	//ScintillaEditView *pEditView = (param == 1)?_mainEditView:_subEditView;

	switch (id)
	{
        case IDM_VIEW_LINENUMBER:
        case IDM_VIEW_SYMBOLMARGIN:
		case IDM_VIEW_DOCCHANGEMARGIN:
        {
            int margin;
            if (id == IDM_VIEW_LINENUMBER)
                margin = ScintillaEditView::_SC_MARGIN_LINENUMBER;
            else //if (id == IDM_VIEW_SYMBOLMARGIN)
                margin = ScintillaEditView::_SC_MARGIN_SYMBOL;
			/*
            else if (id == IDM_VIEW_DOCCHANGEMARGIN)
			{
				margin = ScintillaEditView::_SC_MARGIN_MODIFMARKER;
			}

			else
				margin = ScintillaEditView::_SC_MARGE_FOLDER;
*/
			if (_mainEditView->hasMarginShowed(margin))
			{
                _mainEditView->showMargin(margin, false);
				_subEditView->showMargin(margin, false);
			}
            else
			{
				_mainEditView->showMargin(margin);
                _subEditView->showMargin(margin);
			}
			break;
        }

        case IDM_VIEW_FOLDERMAGIN_SIMPLE:
        case IDM_VIEW_FOLDERMAGIN_ARROW:
        case IDM_VIEW_FOLDERMAGIN_CIRCLE:
        case IDM_VIEW_FOLDERMAGIN_BOX:
		case IDM_VIEW_FOLDERMAGIN:
        {
            folderStyle fStyle = (id == IDM_VIEW_FOLDERMAGIN_SIMPLE)?FOLDER_STYLE_SIMPLE:\
								 (id == IDM_VIEW_FOLDERMAGIN_ARROW)?FOLDER_STYLE_ARROW:\
								 (id == IDM_VIEW_FOLDERMAGIN_CIRCLE)?FOLDER_STYLE_CIRCLE:\
								 (id == IDM_VIEW_FOLDERMAGIN)?FOLDER_STYLE_NONE:FOLDER_STYLE_BOX;

            _mainEditView->setMakerStyle(fStyle);
			_subEditView->setMakerStyle(fStyle);
            break;
        }

		case IDM_VIEW_CURLINE_HILITING:
		{
            COLORREF colour = pNppParam->getCurLineHilitingColour();
			_mainEditView->setCurrentLineHiLiting(!_pEditView->isCurrentLineHiLiting(), colour);
			_subEditView->setCurrentLineHiLiting(!_pEditView->isCurrentLineHiLiting(), colour);
			break;
		}

		case IDM_VIEW_EDGEBACKGROUND:
		case IDM_VIEW_EDGELINE:
		case IDM_VIEW_EDGENONE:
		{
			int mode;
			switch (id)
			{
				case IDM_VIEW_EDGELINE:
				{
					mode = EDGE_LINE;
					break;
				}
				case IDM_VIEW_EDGEBACKGROUND:
				{
					mode = EDGE_BACKGROUND;
					break;
				}
				default :
					mode = EDGE_NONE;
			}
			_mainEditView->execute(SCI_SETEDGEMODE, mode);
			_subEditView->execute(SCI_SETEDGEMODE, mode);
			break;
		}

		case IDM_SETTING_EDGE_SIZE :
		{
			assert(_preferenceDlg);
			ValueDlg nbColumnEdgeDlg;
			ScintillaViewParams & svp = (ScintillaViewParams &)pNppParam->getSVP();
			nbColumnEdgeDlg.init(_pPublicInterface->getHinst(), _preferenceDlg->getHSelf(), svp._edgeNbColumn, TEXT("Nb of column:"));
			nbColumnEdgeDlg.setNBNumber(3);

			POINT p;
			::GetCursorPos(&p);
			::ScreenToClient(_pPublicInterface->getHParent(), &p);
			int size = nbColumnEdgeDlg.doDialog(p, _nativeLangSpeaker->isRTL());

			if (size != -1)
			{
				svp._edgeNbColumn = size;
				_mainEditView->execute(SCI_SETEDGECOLUMN, size);
				_subEditView->execute(SCI_SETEDGECOLUMN, size);
			}
			break;
		}

		case IDM_VIEW_LWDEF:
		case IDM_VIEW_LWALIGN:
		case IDM_VIEW_LWINDENT:
		{
			int mode = SC_WRAPINDENT_FIXED;

			switch (id)
			{
				case IDM_VIEW_LWALIGN:
				{
					mode = SC_WRAPINDENT_SAME;
				} break;

				case IDM_VIEW_LWINDENT:
				{
					mode = SC_WRAPINDENT_INDENT;
				} break;

				default: // case IDM_VIEW_LWALIGN:
				{
					mode = SC_WRAPINDENT_FIXED;
				} break;
			}

			_mainEditView->execute(SCI_SETWRAPINDENTMODE, mode);
			_subEditView->execute(SCI_SETWRAPINDENTMODE, mode);
			break;
		}

		NO_DEFAULT_CASE;
	}
}

void Notepad_plus::setLanguage(LangType langType) {
	//Add logic to prevent changing a language when a document is shared between two views
	//If so, release one document
	bool reset = false;
	Document prev = 0;
	if (bothActive()) {
		if (_mainEditView->getCurrentBufferID() == _subEditView->getCurrentBufferID()) {
			reset = true;
			_subEditView->saveCurrentPos();
			prev = _subEditView->execute(SCI_GETDOCPOINTER);
			_subEditView->execute(SCI_SETDOCPOINTER, 0, 0);
		}
	}
	if (reset) {
		_mainEditView->getCurrentBuffer()->setLangType(langType);
	} else {
		_pEditView->getCurrentBuffer()->setLangType(langType);
	}

	addHotSpot(false);

	if (reset) {
		_subEditView->execute(SCI_SETDOCPOINTER, 0, prev);
		_subEditView->restoreCurrentPos();
	}
};

enum LangType Notepad_plus::menuID2LangType(int cmdID)
{
	switch (cmdID)
	{
        case IDM_LANG_C	:
            return L_C;
        case IDM_LANG_CPP :
            return L_CPP;
        case IDM_LANG_JAVA :
            return L_JAVA;
        case IDM_LANG_CS :
            return L_CS;
        case IDM_LANG_HTML :
            return L_HTML;
        case IDM_LANG_XML :
            return L_XML;
        case IDM_LANG_JS :
            return L_JS;
        case IDM_LANG_PHP :
            return L_PHP;
        case IDM_LANG_ASP :
            return L_ASP;
        case IDM_LANG_JSP :
            return L_JSP;
        case IDM_LANG_CSS :
            return L_CSS;
        case IDM_LANG_LUA :
            return L_LUA;
        case IDM_LANG_PERL :
            return L_PERL;
        case IDM_LANG_PYTHON :
            return L_PYTHON;
        case IDM_LANG_PASCAL :
            return L_PASCAL;
        case IDM_LANG_BATCH :
            return L_BATCH;
        case IDM_LANG_OBJC :
            return L_OBJC;
        case IDM_LANG_VB :
            return L_VB;
        case IDM_LANG_SQL :
            return L_SQL;
        case IDM_LANG_ASCII :
            return L_ASCII;
        case IDM_LANG_TEXT :
            return L_TEXT;
        case IDM_LANG_RC :
            return L_RC;
        case IDM_LANG_MAKEFILE :
            return L_MAKEFILE;
        case IDM_LANG_INI :
            return L_INI;
        case IDM_LANG_TEX :
            return L_TEX;
        case IDM_LANG_FORTRAN :
            return L_FORTRAN;
        case IDM_LANG_BASH :
            return L_BASH;
        case IDM_LANG_FLASH :
            return L_FLASH;
		case IDM_LANG_NSIS :
            return L_NSIS;
		case IDM_LANG_TCL :
            return L_TCL;
		case IDM_LANG_LISP :
			return L_LISP;
		case IDM_LANG_SCHEME :
			return L_SCHEME;
		case IDM_LANG_ASM :
            return L_ASM;
		case IDM_LANG_DIFF :
            return L_DIFF;
		case IDM_LANG_PROPS :
            return L_PROPS;
		case IDM_LANG_PS:
            return L_PS;
		case IDM_LANG_RUBY:
            return L_RUBY;
		case IDM_LANG_SMALLTALK:
            return L_SMALLTALK;
		case IDM_LANG_VHDL :
            return L_VHDL;
        case IDM_LANG_KIX :
            return L_KIX;
        case IDM_LANG_CAML :
            return L_CAML;
        case IDM_LANG_ADA :
            return L_ADA;
        case IDM_LANG_VERILOG :
            return L_VERILOG;
		case IDM_LANG_MATLAB :
            return L_MATLAB;
		case IDM_LANG_HASKELL :
            return L_HASKELL;
        case IDM_LANG_AU3 :
            return L_AU3;
		case IDM_LANG_INNO :
            return L_INNO;
		case IDM_LANG_CMAKE :
            return L_CMAKE;
		case IDM_LANG_YAML :
			return L_YAML;
        case IDM_LANG_COBOL :
            return L_COBOL;
        case IDM_LANG_D :
            return L_D;
        case IDM_LANG_GUI4CLI :
            return L_GUI4CLI;
        case IDM_LANG_POWERSHELL :
            return L_POWERSHELL;
        case IDM_LANG_R :
            return L_R;
		case IDM_LANG_REBOL:
            return L_REBOL;
		case IDM_LANG_RSP:
            return L_RSP;
		
		case IDM_LANG_USER :
            return L_USER;
		default: {
			if (cmdID >= IDM_LANG_USER && cmdID <= IDM_LANG_USER_LIMIT) {
				return L_USER;
			}
			break; }
	}
	return L_EXTERNAL;
}


void Notepad_plus::setTitle()
{
	const NppGUI & nppGUI = NppParameters::getInstance()->getNppGUI();
	//Get the buffer
	Buffer * buf = _pEditView->getCurrentBuffer();

	generic_string result = TEXT("");
	if (buf->isDirty())
	{
		result += TEXT("*");
	}

	if (nppGUI._shortTitlebar)
	{
		result += buf->getFileName();
	}
	else
	{
		result += buf->getFullPathName();
	}
	result += TEXT(" - ");
	result += TEXT("Notepad++ CR");
	//::SetWindowText(_hSelf, title);
	::SendMessage(_pPublicInterface->getHSelf(), WM_SETTEXT, 0, (LPARAM)result.c_str());

	::SendMessage(_pPublicInterface->getHSelf(), WM_SETTEXT, 0, (LPARAM)result.c_str());
}

void Notepad_plus::activateNextDoc(bool direction)
{
	int nbDoc = _pDocTab->nbItem();

    int curIndex = _pDocTab->getCurrentTabIndex();
    curIndex += (direction == dirUp)?-1:1;

	if (curIndex >= nbDoc)
	{
		if (viewVisible(otherView()))
			switchEditViewTo(otherView());
		curIndex = 0;
	}
	else if (curIndex < 0)
	{
		if (viewVisible(otherView()))
		{
			switchEditViewTo(otherView());
			nbDoc = _pDocTab->nbItem();
		}
		curIndex = nbDoc - 1;
	}

	BufferID id = _pDocTab->getBufferByIndex(curIndex);
	activateBuffer(id, currentView());
}

void Notepad_plus::activateDoc(int pos)
{
	int nbDoc = _pDocTab->nbItem();
	if (pos == _pDocTab->getCurrentTabIndex())
	{
		Buffer * buf = _pEditView->getCurrentBuffer();
		buf->increaseRecentTag();
		return;
	}

	if (pos >= 0 && pos < nbDoc)
	{
		BufferID id = _pDocTab->getBufferByIndex(pos);
		activateBuffer(id, currentView());
	}
}


static const char utflen[] = {1,1,2,3};

size_t Notepad_plus::getSelectedCharNumber(UniMode u)
{
	size_t result = 0;
	int numSel = _pEditView->execute(SCI_GETSELECTIONS);
	if (u == uniUTF8 || u == uniCookie)
	{
		// Strange things are happening to the loop index variable, but I'm not touching this parsing code with a 10 foot pole.
		// for loop index variable �Symbol� whose type category is �String�modified in body of the for loop
		//lint -e850
		for (int i=0; i < numSel; i++)
		{
			size_t line1 = _pEditView->execute(SCI_LINEFROMPOSITION, _pEditView->execute(SCI_GETSELECTIONNSTART, i));
			size_t line2 = _pEditView->execute(SCI_LINEFROMPOSITION, _pEditView->execute(SCI_GETSELECTIONNEND, i));
			for (size_t j = line1; j <= line2; j++)
			{
				size_t stpos = _pEditView->execute(SCI_GETLINESELSTARTPOSITION, j);
				if (stpos != INVALID_POSITION)
				{
					size_t endpos = _pEditView->execute(SCI_GETLINESELENDPOSITION, j);
					for (size_t pos = stpos; pos < endpos; pos++)
					{
						unsigned char c = 0xf0 & (unsigned char)_pEditView->execute(SCI_GETCHARAT, pos);
						if (c >= 0xc0)
							pos += utflen[(c & 0x30) >>  4];
						result++;
					}
				}
			}
		}
		//lint +e850
	}
	else
	{
		for (int i=0; i < numSel; i++)
		{
			size_t stpos = _pEditView->execute(SCI_GETSELECTIONNSTART, i);
			size_t endpos = _pEditView->execute(SCI_GETSELECTIONNEND, i);
			result += (endpos - stpos);
			size_t line1 = _pEditView->execute(SCI_LINEFROMPOSITION, stpos);
			size_t line2 = _pEditView->execute(SCI_LINEFROMPOSITION, endpos);
			line2 -= line1;
			if (_pEditView->execute(SCI_GETEOLMODE) == SC_EOL_CRLF) line2 *= 2;
			result -= line2;
		}
		if (u != uni8Bit && u != uni7Bit) result *= 2;
	}
	return result;
}


#ifdef _OPENMP
#include <omp.h>
#endif
static inline size_t countUtf8Characters(unsigned char *buf, int pos, int endpos)
{
	size_t result = 0;
	while(pos < endpos)
	{
		unsigned char c = buf[pos++];
		if ((c&0xc0) == 0x80 // do not count unexpected continuation bytes (this handles the case where an UTF-8 character is split in the middle)
			|| c == '\n' || c == '\r') continue; // do not count end of lines
		if (c >= 0xc0) pos += utflen[(c & 0x30) >>  4];
		result++;
	}
	return result;
}


size_t Notepad_plus::getCurrentDocCharCount(UniMode u)
{
	if (u != uniUTF8 && u != uniCookie)
	{
		size_t numLines = _pEditView->execute(SCI_GETLINECOUNT);
		int result = _pEditView->execute(SCI_GETLENGTH);
		size_t lines = numLines==0?0:numLines-1;
		if (_pEditView->execute(SCI_GETEOLMODE) == SC_EOL_CRLF) lines *= 2;
		result -= lines;
		return ((int)result < 0)?0:result;
	}
	else
	{
		// Note that counting is not well defined for invalid UTF-8 characters.
		// This method is O(filelength) regardless of the number of characters we count (due to SCI_GETCHARACTERPOINTER);
		// it would not be appropriate for counting characters in a small selection.
		size_t result;

		size_t endpos = _pEditView->execute(SCI_GETLENGTH);
		unsigned char* buf = (unsigned char*)_pEditView->execute(SCI_GETCHARACTERPOINTER); // Scintilla doc sais the pointer can be invalidated by any other "execute"

#ifdef _OPENMP // parallel counting of characters with OpenMP
		if(endpos > 50000) // starting threads takes time; for small files it is better to simply count in one thread
		{
			#pragma omp parallel reduction(+: result)
			{
				// split in chunks of same size (except last chunk if it's not evenly divisible)
				unsigned int num_threads = omp_get_num_threads();
				unsigned int thread_num = omp_get_thread_num();
				size_t chunk_size = endpos/num_threads;
				size_t pos = chunk_size*thread_num;
				size_t endpos_local = (thread_num == num_threads-1) ? endpos : pos+chunk_size;
				result = countUtf8Characters(buf, pos, endpos_local);
			}
		}
		else
#endif
		{
			result = countUtf8Characters(buf, 0, endpos);
		}
		return result;
	}
}


bool Notepad_plus::isFormatUnicode(UniMode u)
{
	return (u != uni8Bit && u != uni7Bit && u != uniUTF8 && u != uniCookie);
}

int Notepad_plus::getBOMSize(UniMode u)
{
	switch(u)
	{
		case uni16LE:
		case uni16BE:
			return 2;
		case uniUTF8:
			return 3;
		default:
			return 0;
	}
}

int Notepad_plus::getSelectedAreas()
{
	int numSel = _pEditView->execute(SCI_GETSELECTIONS);
	if (numSel == 1) // either 0 or 1 selection
		return (_pEditView->execute(SCI_GETSELECTIONNSTART, 0) == _pEditView->execute(SCI_GETSELECTIONNEND, 0)) ? 0 : 1;
	return (_pEditView->execute(SCI_SELECTIONISRECTANGLE)) ? 1 : numSel;
}

size_t Notepad_plus::getSelectedBytes()
{
	int numSel = _pEditView->execute(SCI_GETSELECTIONS);
	size_t result = 0;
	for (int i = 0; i < numSel; i++)
		result += (_pEditView->execute(SCI_GETSELECTIONNEND, i) - _pEditView->execute(SCI_GETSELECTIONNSTART, i));
	return result;
}

int Notepad_plus::wordCount()
{
	assert(_findReplaceDlg);
    FindOption env;
    env._str2Search = TEXT("[^ 	\\\\.,;:!?()+\\-\\*/=\\]\\[{}&~\"'`|@$%�<>\\^]+");
	//printStr(env._str2Search.c_str());
    env._searchType = FindRegex;
    return _findReplaceDlg->processAll(ProcessCountAll, &env, true);
}


void Notepad_plus::updateStatusBar()
{
	assert(_statusBar);
    TCHAR strLnCol[128];
	TCHAR strSel[64];

	long nbByte = _pEditView->getSelectedByteNumber();
	if (nbByte != -1)
		wsprintf(strSel, TEXT("Sel : %d"), nbByte);
	else
		wsprintf(strSel, TEXT("Sel : %s"), TEXT("N/A"));

    wsprintf(strLnCol, TEXT("Ln : %d    Col : %d    %s"),\
        (_pEditView->getCurrentLineNumber() + 1), \
        (_pEditView->getCurrentColumnNumber() + 1),\
        strSel);

    _statusBar->setText(strLnCol, STATUSBAR_CUR_POS);

    TCHAR strDocLen[256];
	wsprintf(strDocLen, TEXT("length : %d    lines : %d"), _pEditView->getCurrentDocLen(), _pEditView->execute(SCI_GETLINECOUNT));
    _statusBar->setText(strDocLen, STATUSBAR_DOC_SIZE);
    _statusBar->setText(_pEditView->execute(SCI_GETOVERTYPE) ? TEXT("OVR") : TEXT("INS"), STATUSBAR_TYPING_MODE);
}

void Notepad_plus::dropFiles(HDROP hdrop)
{
	if (hdrop)
	{
		assert(_mainDocTab);
		assert(_subDocTab);
		// Determinate in which view the file(s) is (are) dropped
		POINT p;
		::DragQueryPoint(hdrop, &p);
		HWND hWin = ::RealChildWindowFromPoint(_pPublicInterface->getHSelf(), p);
		if (!hWin) return;

		if ((_mainEditView->getHSelf() == hWin) || (_mainDocTab->getHSelf() == hWin))
			switchEditViewTo(MAIN_VIEW);
		else if ((_subEditView->getHSelf() == hWin) || (_subDocTab->getHSelf() == hWin))
			switchEditViewTo(SUB_VIEW);
		else
		{
			::SendMessage(hWin, WM_DROPFILES, (WPARAM)hdrop, 0);
			return;
		}

		int filesDropped = ::DragQueryFile(hdrop, 0xffffffff, NULL, 0);
		BufferID lastOpened = BUFFER_INVALID;
		for (int i = 0 ; i < filesDropped ; ++i)
		{
			TCHAR pathDropped[MAX_PATH];
			::DragQueryFile(hdrop, i, pathDropped, MAX_PATH);
			BufferID test = doOpen(pathDropped);
			if (test != BUFFER_INVALID)
				lastOpened = test;
            //setLangStatus(_pEditView->getCurrentDocType());
		}
		if (lastOpened != BUFFER_INVALID) {
			switchToFile(lastOpened);
		}
		::DragFinish(hdrop);
		// Put Notepad_plus to forefront
		// May not work for Win2k, but OK for lower versions
		// Note: how to drop a file to an iconic window?
		// Actually, it is the Send To command that generates a drop.
		if (::IsIconic(_pPublicInterface->getHSelf()))
		{
			::ShowWindow(_pPublicInterface->getHSelf(), SW_RESTORE);
		}
		::SetForegroundWindow(_pPublicInterface->getHSelf());
	}
}

void Notepad_plus::checkModifiedDocument()
{
	//this will trigger buffer updates. If the status changes, Notepad++ will be informed and can do its magic
	MainFileManager->checkFilesystemChanges();
}

void Notepad_plus::getMainClientRect(RECT &rc) const
{
	assert(_rebarTop);
	assert(_rebarBottom);
	assert(_statusBar);
    _pPublicInterface->getClientRect(rc);
	rc.top += _rebarTop->getHeight();
	rc.bottom -= rc.top + _rebarBottom->getHeight() + _statusBar->getHeight();
}

void Notepad_plus::showView(int whichOne) {
	if (viewVisible(whichOne))	//no use making visible view visible
		return;

	assert(_subSplitter);

	if (_mainWindowStatus & WindowUserActive) {
		 _pMainSplitter->setWin0(_subSplitter);
		 _pMainWindow = _pMainSplitter;
	} else {
		_pMainWindow = _subSplitter;
	}

	if (whichOne == MAIN_VIEW) {
		assert(_mainDocTab);
		_mainEditView->display(true);
		_mainDocTab->display(true);
	} else if (whichOne == SUB_VIEW) {
		assert(_subDocTab);
		_subEditView->display(true);
		_subDocTab->display(true);
	}
	_pMainWindow->display(true);

	_mainWindowStatus |= (whichOne==MAIN_VIEW)?WindowMainActive:WindowSubActive;

	//Send sizing info to make windows fit
	::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
}

bool Notepad_plus::viewVisible(int whichOne) {
	int viewToCheck = (whichOne == SUB_VIEW?WindowSubActive:WindowMainActive);
	return (_mainWindowStatus & viewToCheck) != 0;
}

void Notepad_plus::hideCurrentView()
{
	hideView(currentView());
}

void Notepad_plus::hideView(int whichOne)
{
	if (!(bothActive()))	//cannot close if not both views visible
		return;

	assert(_mainDocTab);
	assert(_subDocTab);
	assert(_subSplitter);

	Window * windowToSet = (whichOne == MAIN_VIEW)?_subDocTab:_mainDocTab;
	if (_mainWindowStatus & WindowUserActive)
	{
		_pMainSplitter->setWin0(windowToSet);
	}
	else // otherwise the main window is the spltter container that we just created
		_pMainWindow = windowToSet;

	_subSplitter->display(false);	//hide splitter
	//hide scintilla and doctab
	if (whichOne == MAIN_VIEW) {
		_mainEditView->display(false);
		_mainDocTab->display(false);
	} else if (whichOne == SUB_VIEW) {
		_subEditView->display(false);
		_subDocTab->display(false);
	}

	// resize the main window
	::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);

	switchEditViewTo(otherFromView(whichOne));
	int viewToDisable = (whichOne == SUB_VIEW?WindowSubActive:WindowMainActive);
	_mainWindowStatus &= ~viewToDisable;
}

bool Notepad_plus::loadStyles()
{
	NppParameters *pNppParam = NppParameters::getInstance();
	return pNppParam->reloadStylers();
}



bool Notepad_plus::canHideView(int whichOne)
{
	if (!viewVisible(whichOne))
		return false;	//cannot hide hidden view
	if (!bothActive())
		return false;	//cannot hide only window

	assert(_mainDocTab);
	assert(_subDocTab);

	DocTabView * tabToCheck = (whichOne == MAIN_VIEW)?_mainDocTab:_subDocTab;
	Buffer * buf = MainFileManager->getBufferByID(tabToCheck->getBufferByIndex(0));
	bool canHide = ((tabToCheck->nbItem() == 1) && !buf->isDirty() && buf->isUntitled());
	return canHide;
}

void Notepad_plus::loadBufferIntoView(BufferID id, int whichOne, bool dontClose)
{
	assert(_mainDocTab);
	assert(_subDocTab);

	DocTabView * tabToOpen = (whichOne == MAIN_VIEW)?_mainDocTab:_subDocTab;
	ScintillaEditView * viewToOpen = (whichOne == MAIN_VIEW)?_mainEditView:_subEditView;

	//check if buffer exists
	int index = tabToOpen->getIndexByBuffer(id);
	if (index != -1)	//already open, done
		return;

	BufferID idToClose = BUFFER_INVALID;
	//Check if the tab has a single clean buffer. Close it if so
	if (!dontClose && tabToOpen->nbItem() == 1) {
		idToClose = tabToOpen->getBufferByIndex(0);
		Buffer * buf = MainFileManager->getBufferByID(idToClose);
		if (buf->isDirty() || !buf->isUntitled()) {
			idToClose = BUFFER_INVALID;
		}
	}

	MainFileManager->addBufferReference(id, viewToOpen);

	if (idToClose != BUFFER_INVALID) {	//close clean doc. Use special logic to prevent flicker of tab showing then hiding
		tabToOpen->setBuffer(0, id);	//index 0 since only one open
		activateBuffer(id, whichOne);	//activate. DocTab already activated but not a problem
		MainFileManager->closeBuffer(idToClose, viewToOpen);	//delete the buffer
	} else {
		tabToOpen->addBuffer(id);
	}
}

void Notepad_plus::removeBufferFromView(BufferID id, int whichOne)
{
	assert(_mainDocTab);
	assert(_subDocTab);

	DocTabView * tabToClose = (whichOne == MAIN_VIEW)?_mainDocTab:_subDocTab;
	ScintillaEditView * viewToClose = (whichOne == MAIN_VIEW)?_mainEditView:_subEditView;

	//check if buffer exists
	int index = tabToClose->getIndexByBuffer(id);
	if (index == -1)	//doesn't exist, done
		return;

	Buffer * buf = MainFileManager->getBufferByID(id);

	//Cannot close doc if last and clean
	if (tabToClose->nbItem() == 1) {
		if (!buf->isDirty() && buf->isUntitled()) {
			return;	//done
		}
	}

	int active = tabToClose->getCurrentTabIndex();
	if (active == index) {	//need an alternative (close real doc, put empty one back
		if (tabToClose->nbItem() == 1) {	//need alternative doc, add new one. Use special logic to prevent flicker of adding new tab then closing other
			BufferID newID = MainFileManager->newEmptyDocument();
			MainFileManager->addBufferReference(newID, viewToClose);
			tabToClose->setBuffer(0, newID);	//can safely use id 0, last (only) tab open
			activateBuffer(newID, whichOne);	//activate. DocTab already activated but not a problem
		} else {
			int toActivate = 0;
			//activate next doc, otherwise prev if not possible
			if (active == tabToClose->nbItem() - 1) {	//prev
				toActivate = active - 1;
			} else {
				toActivate = active;	//activate the 'active' index. Since we remove the tab first, the indices shift (on the right side)
			}
			tabToClose->deletItemAt(index);	//delete first
			activateBuffer(tabToClose->getBufferByIndex(toActivate), whichOne);	//then activate. The prevent jumpy tab behaviour
		}
	} else {
		tabToClose->deletItemAt(index);
	}

	MainFileManager->closeBuffer(id, viewToClose);
}

int Notepad_plus::switchEditViewTo(int gid)
{
	if (currentView() == gid) {	//make sure focus is ok, then leave
		_pEditView->getFocus();	//set the focus
		return gid;
	}
	if (!viewVisible(gid))
		return currentView();	//cannot activate invisible view
	int oldView = currentView();
	int newView = otherView();

	_activeView = newView;
	//Good old switcheroo
	DocTabView * tempTab = _pDocTab;
	_pDocTab = _pNonDocTab;
	_pNonDocTab = tempTab;
	ScintillaEditView * tempView = _pEditView;
	_pEditView = _pNonEditView;
	_pNonEditView = tempView;

	_pEditView->beSwitched();
    _pEditView->getFocus();	//set the focus

	notifyBufferActivated(_pEditView->getCurrentBufferID(), currentView());
	return oldView;
}

void Notepad_plus::dockUserDlg()
{
	assert(_subSplitter);
    if (!_pMainSplitter)
    {
        _pMainSplitter = new SplitterContainer;
		_pMainSplitter->init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf());

        Window *pWindow;
		if (_mainWindowStatus & (WindowMainActive | WindowSubActive))
            pWindow = _subSplitter;
        else
            pWindow = _pDocTab;

        _pMainSplitter->create(pWindow, ScintillaEditView::getUserDefineDlg(), 8, RIGHT_FIX, 45);
    }

    if (bothActive())
        _pMainSplitter->setWin0(_subSplitter);
    else
        _pMainSplitter->setWin0(_pDocTab);

    _pMainSplitter->display();

    _mainWindowStatus |= WindowUserActive;
    _pMainWindow = _pMainSplitter;

	::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
}

void Notepad_plus::undockUserDlg()
{
    // a cause de surchargement de "display"
    ::ShowWindow(_pMainSplitter->getHSelf(), SW_HIDE);

    if (bothActive())
    {
		assert(_subSplitter);
        _pMainWindow = _subSplitter;
    }
    else
    {
        _pMainWindow = _pDocTab;
    }

    ::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);

    _mainWindowStatus &= ~WindowUserActive;
    (ScintillaEditView::getUserDefineDlg())->display();
}

void Notepad_plus::docOpenInNewInstance(FileTransferMode mode, int x, int y)
{
	BufferID bufferID = _pEditView->getCurrentBufferID();
	Buffer * buf = MainFileManager->getBufferByID(bufferID);
	if (buf->isUntitled() || buf->isDirty())
		return;

	TCHAR nppName[MAX_PATH];
	::GetModuleFileName(NULL, nppName, MAX_PATH);
	generic_string command = TEXT("\"");
	command += nppName;
	command += TEXT("\"");

	command += TEXT(" \"$(FULL_CURRENT_PATH)\" -multiInst -nosession -x");
	TCHAR pX[10], pY[10];
	generic_itoa(x, pX, 10, 10);
	generic_itoa(y, pY, 10, 10);

	command += pX;
	command += TEXT(" -y");
	command += pY;

	Command cmd(command);
	cmd.run(_pPublicInterface->getHSelf());
	if (mode == TransferMove)
	{
		doClose(bufferID, currentView());
		if (noOpenedDoc())
			::SendMessage(_pPublicInterface->getHSelf(), WM_CLOSE, 0, 0);
	}
}

void Notepad_plus::docGotoAnotherEditView(FileTransferMode mode)
{
	// Test if it's only doc to transfer on the hidden view
	// If so then do nothing
	if (mode == TransferMove)
	{
		if (_pDocTab->nbItem() == 1)
		{
			ScintillaEditView *pOtherView = NULL;
			if (_pEditView == _mainEditView)
			{
				pOtherView = _subEditView;
			}
			else if (_pEditView == _subEditView)
			{
				pOtherView = _mainEditView;
			}
			else
				return;

			if (!pOtherView->isVisible())
				return;
		}
	}

	//First put the doc in the other view if not present (if it is, activate it).
	//Then if needed close in the original tab
	BufferID current = _pEditView->getCurrentBufferID();
	int viewToGo = otherView();
	int indexFound = _pNonDocTab->getIndexByBuffer(current);
	if (indexFound != -1)	//activate it
	{
		activateBuffer(current, otherView());
	}
	else	//open the document, also copying the position
	{
		loadBufferIntoView(current, viewToGo);
		Buffer * buf = MainFileManager->getBufferByID(current);
		_pEditView->saveCurrentPos();	//allow copying of position
		buf->setPosition(buf->getPosition(_pEditView), _pNonEditView);
		_pNonEditView->restoreCurrentPos();	//set position
		activateBuffer(current, otherView());
	}

	//Open the view if it was hidden
	int viewToOpen = (viewToGo == SUB_VIEW?WindowSubActive:WindowMainActive);
	if (!(_mainWindowStatus & viewToOpen)) {
		showView(viewToGo);
	}

	//Close the document if we transfered the document instead of cloning it
	if (mode == TransferMove)
	{
		//just close the activate document, since thats the one we moved (no search)
		doClose(_pEditView->getCurrentBufferID(), currentView());
		if (noOpenedDoc())
			::SendMessage(_pPublicInterface->getHSelf(), WM_CLOSE, 0, 0);
	} // else it was cone, so leave it

	//Activate the other view since thats where the document went
	switchEditViewTo(viewToGo);

	//_linkTriggered = true;
}

bool Notepad_plus::activateBuffer(BufferID id, int whichOne)
{
	//scnN.nmhdr.code = NPPN_DOCSWITCHINGOFF;		//superseeded by NPPN_BUFFERACTIVATED

	Buffer * pBuf = MainFileManager->getBufferByID(id);
	bool reload = pBuf->getNeedReload();
	if (reload)
	{
		MainFileManager->reloadBuffer(id);
		pBuf->setNeedReload(false);
	}
	if (whichOne == MAIN_VIEW)
	{
		assert(_mainDocTab);
		if (_mainDocTab->activateBuffer(id))	//only activate if possible
			_mainEditView->activateBuffer(id);
		else
			return false;
	}
	else
	{
		assert(_subDocTab);
		if (_subDocTab->activateBuffer(id))
			_subEditView->activateBuffer(id);
		else
			return false;
	}

	if (reload)
	{
		performPostReload(whichOne);
	}
	notifyBufferActivated(id, whichOne);

	//scnN.nmhdr.code = NPPN_DOCSWITCHINGIN;		//superseeded by NPPN_BUFFERACTIVATED
	return true;
}

void Notepad_plus::performPostReload(int whichOne) {
	NppParameters *pNppParam = NppParameters::getInstance();
	const NppGUI & nppGUI = pNppParam->getNppGUI();
	bool toEnd = (nppGUI._fileAutoDetection == cdAutoUpdateGo2end) || (nppGUI._fileAutoDetection == cdGo2end);
	if (!toEnd)
		return;
	if (whichOne == MAIN_VIEW) {
		_mainEditView->execute(SCI_GOTOLINE, _mainEditView->execute(SCI_GETLINECOUNT) -1);
	} else {
		_subEditView->execute(SCI_GOTOLINE, _subEditView->execute(SCI_GETLINECOUNT) -1);
	}
}

void Notepad_plus::bookmarkNext(bool forwardScan)
{
	int lineno = _pEditView->getCurrentLineNumber();
	int sci_marker = SCI_MARKERNEXT;
	int lineStart = lineno + 1;	//Scan starting from next line
	int lineRetry = 0;				//If not found, try from the beginning
	if (!forwardScan)
    {
		lineStart = lineno - 1;		//Scan starting from previous line
		lineRetry = int(_pEditView->execute(SCI_GETLINECOUNT));	//If not found, try from the end
		sci_marker = SCI_MARKERPREVIOUS;
	}
	int nextLine = int(_pEditView->execute(sci_marker, lineStart, 1 << MARK_BOOKMARK));
	if (nextLine < 0)
		nextLine = int(_pEditView->execute(sci_marker, lineRetry, 1 << MARK_BOOKMARK));

	if (nextLine < 0)
		return;

    _pEditView->execute(SCI_ENSUREVISIBLEENFORCEPOLICY, nextLine);
	_pEditView->execute(SCI_GOTOLINE, nextLine);
}

void Notepad_plus::bookmarkClearAll() const
{
	_pEditView->execute(SCI_MARKERDELETEALL, MARK_BOOKMARK);
}

void Notepad_plus::staticCheckMenuAndTB() const
{
	// Visibility of invisible characters
	bool wsTabShow = _pEditView->isInvisibleCharsShown();
	bool eolShow = _pEditView->isEolVisible();

	bool onlyWS = false;
	bool onlyEOL = false;
	bool bothWSEOL = false;
	if (wsTabShow)
	{
		if (eolShow)
		{
			bothWSEOL = true;
		}
		else
		{
			onlyWS = true;
		}
	}
	else if (eolShow)
	{
		onlyEOL = true;
	}

	checkMenuItem(IDM_VIEW_TAB_SPACE, onlyWS);
	checkMenuItem(IDM_VIEW_EOL, onlyEOL);
	checkMenuItem(IDM_VIEW_ALL_CHARACTERS, bothWSEOL);
	_toolBar->setCheck(IDM_VIEW_ALL_CHARACTERS, bothWSEOL);

	// Visibility of the indentation guide line
	bool b = _pEditView->isShownIndentGuide();
	checkMenuItem(IDM_VIEW_INDENT_GUIDE, b);
	_toolBar->setCheck(IDM_VIEW_INDENT_GUIDE, b);


	// Wrap
	b = _pEditView->isWrap();
	checkMenuItem(IDM_VIEW_WRAP, b);
	_toolBar->setCheck(IDM_VIEW_WRAP, b);
	checkMenuItem(IDM_VIEW_WRAP_SYMBOL, _pEditView->isWrapSymbolVisible());
}

void Notepad_plus::dynamicCheckMenuAndTB() const
{
	//Format conversion
	enableConvertMenuItems(_pEditView->getCurrentBuffer()->getFormat());
	checkUnicodeMenuItems();
}

void Notepad_plus::enableConvertMenuItems(formatType f) const
{
	enableCommand(IDM_FORMAT_TODOS, (f != WIN_FORMAT), MENU);
	enableCommand(IDM_FORMAT_TOUNIX, (f != UNIX_FORMAT), MENU);
	enableCommand(IDM_FORMAT_TOMAC, (f != MAC_FORMAT), MENU);
}

void Notepad_plus::checkUnicodeMenuItems() const
{
	Buffer *buf = _pEditView->getCurrentBuffer();
	UniMode um = buf->getUnicodeMode();
	int encoding = buf->getEncoding();

	int id = -1;
	switch (um)
	{
		case uniUTF8   : id = IDM_FORMAT_UTF_8; break;
		case uni16BE   : id = IDM_FORMAT_UCS_2BE; break;
		case uni16LE   : id = IDM_FORMAT_UCS_2LE; break;
		case uniCookie : id = IDM_FORMAT_AS_UTF_8; break;
		case uni8Bit   : id = IDM_FORMAT_ANSI; break;
	}

	if (encoding == -1)
	{
		// Uncheck all in the sub encoding menu
		::CheckMenuRadioItem(_mainMenuHandle, IDM_FORMAT_ENCODE, IDM_FORMAT_ENCODE_END, IDM_FORMAT_ENCODE, MF_BYCOMMAND);
		::CheckMenuItem(_mainMenuHandle, IDM_FORMAT_ENCODE, MF_UNCHECKED | MF_BYCOMMAND);

		if (id == -1) //um == uni16BE_NoBOM || um == uni16LE_NoBOM
		{
			// Uncheck all in the main encoding menu
			::CheckMenuRadioItem(_mainMenuHandle, IDM_FORMAT_ANSI, IDM_FORMAT_AS_UTF_8, IDM_FORMAT_ANSI, MF_BYCOMMAND);
			::CheckMenuItem(_mainMenuHandle, IDM_FORMAT_ANSI, MF_UNCHECKED | MF_BYCOMMAND);
		}
		else
		{
			::CheckMenuRadioItem(_mainMenuHandle, IDM_FORMAT_ANSI, IDM_FORMAT_AS_UTF_8, id, MF_BYCOMMAND);
		}
	}
	else
	{
		EncodingMapper *em = EncodingMapper::getInstance();
		int cmdID = em->getIndexFromEncoding(encoding);
		if (cmdID == -1)
		{
			//printStr(TEXT("Encoding problem. Encoding is not added in encoding_table?"));
			return;
		}
		cmdID += IDM_FORMAT_ENCODE;

		// Uncheck all in the main encoding menu
		::CheckMenuRadioItem(_mainMenuHandle, IDM_FORMAT_ANSI, IDM_FORMAT_AS_UTF_8, IDM_FORMAT_ANSI, MF_BYCOMMAND);
		::CheckMenuItem(_mainMenuHandle, IDM_FORMAT_ANSI, MF_UNCHECKED | MF_BYCOMMAND);

		// Check the encoding item
		::CheckMenuRadioItem(_mainMenuHandle, IDM_FORMAT_ENCODE, IDM_FORMAT_ENCODE_END, cmdID, MF_BYCOMMAND);
	}
}

void Notepad_plus::showAutoComp() {
	assert(_autoCompleteMain);
	assert(_autoCompleteSub);
	bool isFromPrimary = _pEditView == _mainEditView;
	AutoCompletion * autoC = isFromPrimary?_autoCompleteMain:_autoCompleteSub;
	autoC->showAutoComplete();
}

void Notepad_plus::autoCompFromCurrentFile(bool autoInsert) {
	assert(_autoCompleteMain);
	assert(_autoCompleteSub);
	bool isFromPrimary = _pEditView == _mainEditView;
	AutoCompletion * autoC = isFromPrimary?_autoCompleteMain:_autoCompleteSub;
	autoC->showWordComplete(autoInsert);
}

void Notepad_plus::showFunctionComp() {
	assert(_autoCompleteMain);
	assert(_autoCompleteSub);
	bool isFromPrimary = _pEditView == _mainEditView;
	AutoCompletion * autoC = isFromPrimary?_autoCompleteMain:_autoCompleteSub;
	autoC->showFunctionComplete();
}



















static generic_string extractSymbol(TCHAR prefix, const generic_string& str2extract)
{
	bool found = false;
	TCHAR extracted[128] = TEXT("");

	for (size_t i = 0, j = 0 ; i < str2extract.length() && i < 128 ; i++)
	{
		if (found)
		{
			if (!str2extract[i] || str2extract[i] == ' ')
			{
				extracted[j] = '\0';
				return generic_string(extracted);
			}
			extracted[j++] = str2extract[i];

		}
		else
		{
			if (!str2extract[i])
				return TEXT("");

			if (str2extract[i] == prefix)
				found = true;
		}
	}
	return  generic_string(extracted);
};

bool Notepad_plus::doBlockComment(comment_mode currCommentMode)
{
	const TCHAR *commentLineSybol;
	generic_string symbol;

	Buffer * buf = _pEditView->getCurrentBuffer();
	if (buf->getLangType() == L_USER)
	{
		UserLangContainer * userLangContainer = NppParameters::getInstance()->getULCFromName(buf->getUserDefineLangName());
		if (!userLangContainer)
			return false;

		symbol = extractSymbol('0', userLangContainer->getKeywordList(KWL_COMMENT_INDEX));
		commentLineSybol = symbol.c_str();
	}
	else
		commentLineSybol = buf->getCommentLineSymbol();


	if ((!commentLineSybol) || (!commentLineSybol[0]))
		return false;

    generic_string comment(commentLineSybol);
    comment += TEXT(" ");

	const int linebufferSize = 1024;
    TCHAR linebuf[linebufferSize];
    size_t comment_length = comment.length();
    size_t selectionStart = _pEditView->execute(SCI_GETSELECTIONSTART);
    size_t selectionEnd = _pEditView->execute(SCI_GETSELECTIONEND);
    size_t caretPosition = _pEditView->execute(SCI_GETCURRENTPOS);
    // checking if caret is located in _beginning_ of selected block
    bool move_caret = caretPosition < selectionEnd;
    int selStartLine = _pEditView->execute(SCI_LINEFROMPOSITION, selectionStart);
    int selEndLine = _pEditView->execute(SCI_LINEFROMPOSITION, selectionEnd);
    int lines = selEndLine - selStartLine;
    size_t firstSelLineStart = _pEditView->execute(SCI_POSITIONFROMLINE, selStartLine);
    // "caret return" is part of the last selected line
    if ((lines > 0) && (selectionEnd == static_cast<size_t>(_pEditView->execute(SCI_POSITIONFROMLINE, selEndLine))))
		selEndLine--;
    _pEditView->execute(SCI_BEGINUNDOACTION);

    for (int i = selStartLine; i <= selEndLine; i++)
	{
		int lineStart = _pEditView->execute(SCI_POSITIONFROMLINE, i);
        int lineIndent = lineStart;
        int lineEnd = _pEditView->execute(SCI_GETLINEENDPOSITION, i);
        if ((lineEnd - lineIndent) >= linebufferSize)        // Avoid buffer size problems
                continue;

        lineIndent = _pEditView->execute(SCI_GETLINEINDENTPOSITION, i);
		_pEditView->getGenericText(linebuf, lineIndent, lineEnd);

        generic_string linebufStr = linebuf;

        // empty lines are not commented
        if (lstrlen(linebuf) < 1)
			continue;
		if (currCommentMode != cm_comment)
		{
            if (linebufStr.substr(0, comment_length - 1) == comment.substr(0, comment_length - 1))
				{
                int len = (linebufStr.substr(0, comment_length) == comment)?comment_length:comment_length - 1;

                _pEditView->execute(SCI_SETSEL, lineIndent, lineIndent + len);
					_pEditView->replaceSelWith("");

					if (i == selStartLine) // is this the first selected line?
					selectionStart -= len;
				selectionEnd -= len; // every iteration
					continue;
				}
			}
		if ((currCommentMode == cm_toggle) || (currCommentMode == cm_comment))
		{
			if (i == selStartLine) // is this the first selected line?
				selectionStart += comment_length;
			selectionEnd += comment_length; // every iteration
			_pEditView->insertGenericTextFrom(lineIndent, comment.c_str());
		}
     }
    // after uncommenting selection may promote itself to the lines
    // before the first initially selected line;
    // another problem - if only comment symbol was selected;
    if (selectionStart < firstSelLineStart)
	{
        if (selectionStart >= selectionEnd - (comment_length - 1))
			selectionEnd = firstSelLineStart;
        selectionStart = firstSelLineStart;
    }
    if (move_caret)
	{
        // moving caret to the beginning of selected block
        _pEditView->execute(SCI_GOTOPOS, selectionEnd);
        _pEditView->execute(SCI_SETCURRENTPOS, selectionStart);
    }
	else
	{
        _pEditView->execute(SCI_SETSEL, selectionStart, selectionEnd);
    }
    _pEditView->execute(SCI_ENDUNDOACTION);
    return true;
}

bool Notepad_plus::doStreamComment()
{
	const TCHAR *commentStart;
	const TCHAR *commentEnd;

	generic_string symbolStart;
	generic_string symbolEnd;

	Buffer * buf = _pEditView->getCurrentBuffer();
	if (buf->getLangType() == L_USER)
	{
		UserLangContainer * userLangContainer = NppParameters::getInstance()->getULCFromName(buf->getUserDefineLangName());

		if (!userLangContainer)
			return false;

		symbolStart = extractSymbol('1', userLangContainer->getKeywordList(KWL_COMMENT_INDEX));
		commentStart = symbolStart.c_str();
		symbolEnd = extractSymbol('2', userLangContainer->getKeywordList(KWL_COMMENT_INDEX));
		commentEnd = symbolEnd.c_str();
	}
	else
	{
		commentStart = buf->getCommentStart();
		commentEnd = buf->getCommentEnd();
	}

	if ((!commentStart) || (!commentStart[0]))
		return false;
	if ((!commentEnd) || (!commentEnd[0]))
		return false;

	generic_string start_comment(commentStart);
	generic_string end_comment(commentEnd);
	generic_string white_space(TEXT(" "));

	start_comment += white_space;
	white_space += end_comment;
	end_comment = white_space;
	size_t start_comment_length = start_comment.length();
	size_t selectionStart = _pEditView->execute(SCI_GETSELECTIONSTART);
	size_t selectionEnd = _pEditView->execute(SCI_GETSELECTIONEND);
	size_t caretPosition = _pEditView->execute(SCI_GETCURRENTPOS);
	// checking if caret is located in _beginning_ of selected block
	bool move_caret = caretPosition < selectionEnd;

	// if there is no selection?
	if (selectionEnd - selectionStart <= 0)
	{
		int selLine = _pEditView->execute(SCI_LINEFROMPOSITION, selectionStart);
		selectionStart = _pEditView->execute(SCI_GETLINEINDENTPOSITION, selLine);
		selectionEnd = _pEditView->execute(SCI_GETLINEENDPOSITION, selLine);
	}
	_pEditView->execute(SCI_BEGINUNDOACTION);
	_pEditView->insertGenericTextFrom(selectionStart, start_comment.c_str());
	selectionEnd += start_comment_length;
	selectionStart += start_comment_length;
	_pEditView->insertGenericTextFrom(selectionEnd, end_comment.c_str());
	if (move_caret)
	{
		// moving caret to the beginning of selected block
		_pEditView->execute(SCI_GOTOPOS, selectionEnd);
		_pEditView->execute(SCI_SETCURRENTPOS, selectionStart);
	}
	else
	{
		_pEditView->execute(SCI_SETSEL, selectionStart, selectionEnd);
	}
	_pEditView->execute(SCI_ENDUNDOACTION);
	return true;
}

bool Notepad_plus::saveScintillaParams()
{
	NppParameters * pNppParam = NppParameters::getInstance();
	ScintillaViewParams & svp = (ScintillaViewParams &)pNppParam->getSVP();
	svp._zoom = int(_mainEditView->execute(SCI_GETZOOM));
	svp._zoom2 = int(_subEditView->execute(SCI_GETZOOM));
	return pNppParam->writeScintillaParams(svp);
}

HACCEL Notepad_plus::getAccTable() const
{
	return _accelerator.getAccTable();
}

bool Notepad_plus::addCurrentMacro()
{
	std::vector<MacroShortcut> & theMacros = (NppParameters::getInstance())->getMacroList();

	int nbMacro = theMacros.size();

	int cmdID = ID_MACRO + nbMacro;
	MacroShortcut ms(Shortcut(), _macro, cmdID);
	ms.init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf());

	if (ms.doDialog() != -1)
	{
		HMENU hMacroMenu = ::GetSubMenu(_mainMenuHandle, MENUINDEX_MACRO);
		int const posBase = 6;	//separator at index 5
		if (nbMacro == 0)
		{
			::InsertMenu(hMacroMenu, posBase-1, MF_BYPOSITION, (unsigned int)-1, 0);	//no separator yet, add one

            // Insert the separator and modify/delete command
			::InsertMenu(hMacroMenu, posBase + nbMacro + 1, MF_BYPOSITION, (unsigned int)-1, 0);
            const char * nativeLangShortcutMapperMacro = (NppParameters::getInstance())->getNativeLangMenuStringA(IDM_SETTING_SHORTCUT_MAPPER_MACRO);
            const char * shortcutMapperMacroStr = nativeLangShortcutMapperMacro?nativeLangShortcutMapperMacro:"Modify Shortcut/Delete Macro...";
#ifdef UNICODE
		    WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
		    const wchar_t * shortcutMapperMacroStrW = wmc->char2wchar(shortcutMapperMacroStr, _nativeLangSpeaker->getLangEncoding());
		    ::InsertMenu(hMacroMenu, posBase + nbMacro + 2, MF_BYCOMMAND, IDM_SETTING_SHORTCUT_MAPPER_MACRO, shortcutMapperMacroStrW);
#else
		    ::InsertMenu(hMacroMenu, posBase + nbMacro + 2, MF_BYCOMMAND, IDM_SETTING_SHORTCUT_MAPPER_MACRO, shortcutMapperMacroStr);
#endif
        }
		theMacros.push_back(ms);
		::InsertMenu(hMacroMenu, posBase + nbMacro, MF_BYPOSITION, cmdID, ms.toMenuItemString().c_str());
		_accelerator.updateShortcuts();
		return true;
	}
	return false;
}

void Notepad_plus::changeToolBarIcons()
{
	assert(_toolBar);
	_toolBar->changeIcons();
}

bool Notepad_plus::switchToFile(BufferID id)
{
	if (id == BUFFER_INVALID)
		return false;

	int i = _pDocTab->getIndexByBuffer(id);
	int iView = currentView();

	if (i != -1)
	{
		iView = currentView();
	}
	else
	{
		i = _pNonDocTab->getIndexByBuffer(id);
		if ( i != -1)
		{
			iView = otherView();
		}
	}

	if (i != -1)
	{
		switchEditViewTo(iView);
		activateBuffer(id, currentView());
		return true;
	}
	return false;
}

void Notepad_plus::getTaskListInfo(TaskListInfo *tli)
{
	size_t currentNbDoc = _pDocTab->nbItem();
	size_t nonCurrentNbDoc = _pNonDocTab->nbItem();

	tli->_currentIndex = 0;

	if (!viewVisible(otherView()))
		nonCurrentNbDoc = 0;

	for (size_t i = 0 ; i < currentNbDoc ; i++)
	{
		BufferID bufID = _pDocTab->getBufferByIndex(i);
		Buffer * b = MainFileManager->getBufferByID(bufID);
		int status = b->isReadOnly()?tb_ro:(b->isDirty()?tb_unsaved:tb_saved);
		tli->_tlfsLst.push_back(TaskLstFnStatus(currentView(), i, b->getFullPathName(), status));
	}
	for (size_t i = 0 ; i < nonCurrentNbDoc ; i++)
	{
		BufferID bufID = _pNonDocTab->getBufferByIndex(i);
		Buffer * b = MainFileManager->getBufferByID(bufID);
		int status = b->isReadOnly()?tb_ro:(b->isDirty()?tb_unsaved:tb_saved);
		tli->_tlfsLst.push_back(TaskLstFnStatus(otherView(), i, b->getFullPathName(), status));
	}
}


bool Notepad_plus::goToPreviousIndicator(int indicID2Search, bool isWrap) const
{
    int position = _pEditView->execute(SCI_GETCURRENTPOS);
	int docLen = _pEditView->getCurrentDocLen();

    BOOL isInIndicator = _pEditView->execute(SCI_INDICATORVALUEAT, indicID2Search,  position);
    int posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search,  position);
    int posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search,  position);

	// pre-condition
	if ((posStart == 0) && (posEnd == docLen - 1))
		return false;

    if (posStart <= 0)
	{
		if (!isWrap)
			return false;

		isInIndicator = _pEditView->execute(SCI_INDICATORVALUEAT, indicID2Search,  docLen - 1);
		posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search,  docLen - 1);
	}

    if (isInIndicator) // try to get out of indicator
    {
        posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search, posStart - 1);
        if (posStart <= 0)
		{
			if (!isWrap)
				return false;
			posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search,  docLen - 1);
		}
	}

    int newPos = posStart - 1;
    posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search, newPos);
    posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search, newPos);

	// found
	if (_pEditView->execute(SCI_INDICATORVALUEAT, indicID2Search, posStart))
	{
		NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
		nppGUI._disableSmartHiliteTmp = true;

        int currentline = _pEditView->execute(SCI_LINEFROMPOSITION, posEnd);
	    _pEditView->execute(SCI_ENSUREVISIBLE, currentline);	// make sure target line is unfolded

		_pEditView->execute(SCI_SETSEL, posEnd, posStart);
		_pEditView->execute(SCI_SCROLLCARET);
		return true;
	}
	return false;
}

bool Notepad_plus::goToNextIndicator(int indicID2Search, bool isWrap) const
{
    int position = _pEditView->execute(SCI_GETCURRENTPOS);
	int docLen = _pEditView->getCurrentDocLen();

    BOOL isInIndicator = _pEditView->execute(SCI_INDICATORVALUEAT, indicID2Search,  position);
    int posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search,  position);
    int posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search,  position);

	// pre-condition
	if ((posStart == 0) && (posEnd == docLen - 1))
		return false;

    if (posEnd >= docLen)
	{
		if (!isWrap)
			return false;

		isInIndicator = _pEditView->execute(SCI_INDICATORVALUEAT, indicID2Search,  0);
		posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search, 0);
	}

    if (isInIndicator) // try to get out of indicator
    {
        posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search, posEnd);

        if (posEnd >= docLen)
		{
			if (!isWrap)
				return false;
			posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search, 0);
		}
    }
    int newPos = posEnd + 1;
    posStart = _pEditView->execute(SCI_INDICATORSTART, indicID2Search, newPos);
    posEnd = _pEditView->execute(SCI_INDICATOREND, indicID2Search, newPos);

	// found
	if (_pEditView->execute(SCI_INDICATORVALUEAT, indicID2Search, posStart))
	{
		NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
		nppGUI._disableSmartHiliteTmp = true;

        int currentline = _pEditView->execute(SCI_LINEFROMPOSITION, posEnd);
	    _pEditView->execute(SCI_ENSUREVISIBLE, currentline);	// make sure target line is unfolded

		_pEditView->execute(SCI_SETSEL, posStart, posEnd);
		_pEditView->execute(SCI_SCROLLCARET);
		return true;
	}
	return false;
}

void Notepad_plus::fullScreenToggle()
{
	assert(_rebarTop);
	assert(_rebarBottom);
	if (!_beforeSpecialView.isFullScreen)	//toggle fullscreen on
	{
		_beforeSpecialView._winPlace.length = sizeof(_beforeSpecialView._winPlace);
		::GetWindowPlacement(_pPublicInterface->getHSelf(), &_beforeSpecialView._winPlace);

		RECT fullscreenArea;		//RECT used to calculate window fullscreen size
		//Preset view area, in case something fails, primary monitor values
		fullscreenArea.top = 0;
		fullscreenArea.left = 0;
		fullscreenArea.right = GetSystemMetrics(SM_CXSCREEN);
		fullscreenArea.bottom = GetSystemMetrics(SM_CYSCREEN);

		//if (_winVersion != WV_NT)
		{
			HMONITOR currentMonitor;	//Handle to monitor where fullscreen should go
			MONITORINFO mi;				//Info of that monitor
			//Caution, this will not work on windows 95, so probably add some checking of some sorts like Unicode checks, IF 95 were to be supported
			currentMonitor = ::MonitorFromWindow(_pPublicInterface->getHSelf(), MONITOR_DEFAULTTONEAREST);	//should always be valid monitor handle
			mi.cbSize = sizeof(MONITORINFO);
			if (::GetMonitorInfo(currentMonitor, &mi) != FALSE)
			{
				fullscreenArea = mi.rcMonitor;
				fullscreenArea.right -= fullscreenArea.left;
				fullscreenArea.bottom -= fullscreenArea.top;
			}
		}

		//Setup GUI
        int bs = buttonStatus_fullscreen;
		if (_beforeSpecialView.isPostIt)
        {
            bs |= buttonStatus_postit;
        }
        else
		{
			//only change the GUI if not already done by postit
			_beforeSpecialView.isMenuShown = ::SendMessage(_pPublicInterface->getHSelf(), NPPM_ISMENUHIDDEN, 0, 0) != TRUE;
			if (_beforeSpecialView.isMenuShown)
				::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDEMENU, 0, TRUE);

			//Hide rebar
			_rebarTop->display(false);
			_rebarBottom->display(false);
		}
        _restoreButton->setButtonStatus(bs);

		//Hide window so windows can properly update it
		::ShowWindow(_pPublicInterface->getHSelf(), SW_HIDE);

		//Set popup style for fullscreen window and store the old style
		if (!_beforeSpecialView.isPostIt)
		{
			_beforeSpecialView.preStyle = ::SetWindowLongPtr(_pPublicInterface->getHSelf(), GWL_STYLE, WS_POPUP);
			if (!_beforeSpecialView.preStyle) {	//something went wrong, use default settings
				_beforeSpecialView.preStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
			}
		}

		//Set fullscreen window, highest non-top z-order, show the window and redraw it (refreshing the windowmanager cache aswell)
		::ShowWindow(_pPublicInterface->getHSelf(), SW_SHOW);
		::SetWindowPos(_pPublicInterface->getHSelf(), HWND_TOP, fullscreenArea.left, fullscreenArea.top, fullscreenArea.right, fullscreenArea.bottom, SWP_NOZORDER|SWP_DRAWFRAME|SWP_FRAMECHANGED);
		::SetForegroundWindow(_pPublicInterface->getHSelf());

        // show restore button
        _restoreButton->doDialog(_nativeLangSpeaker->isRTL());

        RECT rect;
        GetWindowRect(_restoreButton->getHSelf(), &rect);
	    int w = rect.right - rect.left;
	    int h = rect.bottom - rect.top;

        RECT nppRect;
        GetWindowRect(_pPublicInterface->getHSelf(), &nppRect);
        int x = nppRect.right - w;
        int y = nppRect.top;
        ::MoveWindow(_restoreButton->getHSelf(), x, y, w, h, FALSE);

        _pEditView->getFocus();
	}
	else	//toggle fullscreen off
	{
		//Hide window for updating, restore style and menu then restore position and Z-Order
		::ShowWindow(_pPublicInterface->getHSelf(), SW_HIDE);

        _restoreButton->setButtonStatus(buttonStatus_fullscreen ^ _restoreButton->getButtonStatus());
        _restoreButton->display(false);

		//Setup GUI
		if (!_beforeSpecialView.isPostIt)
		{
			//only change the GUI if postit isnt active
			if (_beforeSpecialView.isMenuShown)
				::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDEMENU, 0, FALSE);

			//Show rebar
			_rebarTop->display(true);
			_rebarBottom->display(true);
		}

		//Set old style if not fullscreen
		if (!_beforeSpecialView.isPostIt)
		{
			::SetWindowLongPtr( _pPublicInterface->getHSelf(), GWL_STYLE, _beforeSpecialView.preStyle);
			//Redraw the window and refresh windowmanager cache, dont do anything else, sizing is done later on
			::SetWindowPos(_pPublicInterface->getHSelf(), HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_DRAWFRAME|SWP_FRAMECHANGED);
			::ShowWindow(_pPublicInterface->getHSelf(), SW_SHOW);
		}

		if (_beforeSpecialView._winPlace.length)
		{
			if (_beforeSpecialView._winPlace.showCmd == SW_SHOWMAXIMIZED)
			{
				::ShowWindow(_pPublicInterface->getHSelf(), SW_SHOWMAXIMIZED);
			}
			else
			{
				::SetWindowPlacement(_pPublicInterface->getHSelf(), &_beforeSpecialView._winPlace);
			}
		}
		else	//fallback
		{
			::ShowWindow(_pPublicInterface->getHSelf(), SW_SHOW);
		}
	}
	//::SetForegroundWindow(_pPublicInterface->getHSelf());
	_beforeSpecialView.isFullScreen = !_beforeSpecialView.isFullScreen;
	::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
    if (_beforeSpecialView.isPostIt)
    {
        // show restore button on the right position
        RECT rect;
        GetWindowRect(_restoreButton->getHSelf(), &rect);
        int w = rect.right - rect.left;
        int h = rect.bottom - rect.top;

        RECT nppRect;
        GetWindowRect(_pPublicInterface->getHSelf(), &nppRect);
        int x = nppRect.right - w - w;
        int y = nppRect.top + 1;
        ::MoveWindow(_restoreButton->getHSelf(), x, y, w, h, FALSE);
    }
}

void Notepad_plus::postItToggle()
{
	assert(_rebarTop);
	assert(_rebarBottom);
	NppParameters * pNppParam = NppParameters::getInstance();
	if (!_beforeSpecialView.isPostIt)	// PostIt disabled, enable it
	{
		NppGUI & nppGUI = pNppParam->getNppGUI();
		// get current status before switch to postIt
		//check these always
		{
			_beforeSpecialView.isAlwaysOnTop = ::GetMenuState(_mainMenuHandle, IDM_VIEW_ALWAYSONTOP, MF_BYCOMMAND) == MF_CHECKED;
			_beforeSpecialView.isTabbarShown = ::SendMessage(_pPublicInterface->getHSelf(), NPPM_ISTABBARHIDDEN, 0, 0) != TRUE;
			_beforeSpecialView.isStatusbarShown = nppGUI._statusBarShow;
			if (nppGUI._statusBarShow)
				::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDESTATUSBAR, 0, TRUE);
			if (_beforeSpecialView.isTabbarShown)
				::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDETABBAR, 0, TRUE);
			if (!_beforeSpecialView.isAlwaysOnTop)
				::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, IDM_VIEW_ALWAYSONTOP, 0);
		}
		//Only check these if not fullscreen
        int bs = buttonStatus_postit;
		if (_beforeSpecialView.isFullScreen)
        {
            bs |= buttonStatus_fullscreen;
        }
        else
		{
			_beforeSpecialView.isMenuShown = ::SendMessage(_pPublicInterface->getHSelf(), NPPM_ISMENUHIDDEN, 0, 0) != TRUE;
			if (_beforeSpecialView.isMenuShown)
				::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDEMENU, 0, TRUE);

			//Hide rebar
			_rebarTop->display(false);
			_rebarBottom->display(false);
		}
        _restoreButton->setButtonStatus(bs);

		// PostIt!

		//Set popup style for fullscreen window and store the old style
		if (!_beforeSpecialView.isFullScreen)
		{
			//Hide window so windows can properly update it
			::ShowWindow(_pPublicInterface->getHSelf(), SW_HIDE);
			_beforeSpecialView.preStyle = ::SetWindowLongPtr( _pPublicInterface->getHSelf(), GWL_STYLE, WS_POPUP );
			if (!_beforeSpecialView.preStyle) {	//something went wrong, use default settings
				_beforeSpecialView.preStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
			}
			//Redraw the window and refresh windowmanager cache, dont do anything else, sizing is done later on
			::SetWindowPos(_pPublicInterface->getHSelf(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_DRAWFRAME|SWP_FRAMECHANGED);
			::ShowWindow(_pPublicInterface->getHSelf(), SW_SHOW);
		}

        // show restore button
        _restoreButton->doDialog(_nativeLangSpeaker->isRTL());

        RECT rect;
        GetWindowRect(_restoreButton->getHSelf(), &rect);
	    int w = rect.right - rect.left;
	    int h = rect.bottom - rect.top;

        RECT nppRect;
        GetWindowRect(_pPublicInterface->getHSelf(), &nppRect);
        int x = nppRect.right - w - w;
        int y = nppRect.top + 1;
        ::MoveWindow(_restoreButton->getHSelf(), x, y, w, h, FALSE);

        _pEditView->getFocus();
	}
	else	//PostIt enabled, disable it
	{
        _restoreButton->setButtonStatus(buttonStatus_postit ^ _restoreButton->getButtonStatus());
        _restoreButton->display(false);

		//Setup GUI
		if (!_beforeSpecialView.isFullScreen)
		{
			//only change the these parts of GUI if not already done by fullscreen
			if (_beforeSpecialView.isMenuShown)
				::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDEMENU, 0, FALSE);

			//Show rebar
			_rebarTop->display(true);
			_rebarBottom->display(true);
		}
		//Do this GUI config always
		if (_beforeSpecialView.isStatusbarShown)
			::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDESTATUSBAR, 0, FALSE);
		if (_beforeSpecialView.isTabbarShown)
			::SendMessage(_pPublicInterface->getHSelf(), NPPM_HIDETABBAR, 0, FALSE);
		if (!_beforeSpecialView.isAlwaysOnTop)
			::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, IDM_VIEW_ALWAYSONTOP, 0);

		//restore window style if not fullscreen
		if (!_beforeSpecialView.isFullScreen)
		{
			//dwStyle |= (WS_CAPTION | WS_SIZEBOX);
			::ShowWindow(_pPublicInterface->getHSelf(), SW_HIDE);
			::SetWindowLongPtr(_pPublicInterface->getHSelf(), GWL_STYLE, _beforeSpecialView.preStyle);

			//Redraw the window and refresh windowmanager cache, dont do anything else, sizing is done later on
			::SetWindowPos(_pPublicInterface->getHSelf(), HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_DRAWFRAME|SWP_FRAMECHANGED);
			::ShowWindow(_pPublicInterface->getHSelf(), SW_SHOW);
		}
	}

	_beforeSpecialView.isPostIt = !_beforeSpecialView.isPostIt;
	::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
}

void Notepad_plus::doSynScorll(HWND whichView)
{
	int column = 0;
	int line = 0;
	ScintillaEditView *pView;

	// var for Line
	int mainCurrentLine, subCurrentLine;

	// var for Column
	int mxoffset, sxoffset;
	int pixel;
	int mainColumn, subColumn;

    if (whichView == _mainEditView->getHSelf())
	{
		if (_syncInfo._isSynScollV)
		{
			// Compute for Line
			mainCurrentLine = _mainEditView->execute(SCI_GETFIRSTVISIBLELINE);
			subCurrentLine = _subEditView->execute(SCI_GETFIRSTVISIBLELINE);
			line = mainCurrentLine - _syncInfo._line - subCurrentLine;
		}
		if (_syncInfo._isSynScollH)
		{
			// Compute for Column
			mxoffset = _mainEditView->execute(SCI_GETXOFFSET);
			pixel = int(_mainEditView->execute(SCI_TEXTWIDTH, STYLE_DEFAULT, (LPARAM)"P"));
			mainColumn = mxoffset/pixel;

			sxoffset = _subEditView->execute(SCI_GETXOFFSET);
			pixel = int(_subEditView->execute(SCI_TEXTWIDTH, STYLE_DEFAULT, (LPARAM)"P"));
			subColumn = sxoffset/pixel;
			column = mainColumn - _syncInfo._column - subColumn;
		}
		pView = _subEditView;
    }
    else if (whichView == _subEditView->getHSelf())
    {
		if (_syncInfo._isSynScollV)
		{
			// Compute for Line
			mainCurrentLine = _mainEditView->execute(SCI_GETFIRSTVISIBLELINE);
			subCurrentLine = _subEditView->execute(SCI_GETFIRSTVISIBLELINE);
			line = subCurrentLine + _syncInfo._line - mainCurrentLine;
		}
		if (_syncInfo._isSynScollH)
		{
			// Compute for Column
			mxoffset = _mainEditView->execute(SCI_GETXOFFSET);
			pixel = int(_mainEditView->execute(SCI_TEXTWIDTH, STYLE_DEFAULT, (LPARAM)"P"));
			mainColumn = mxoffset/pixel;

			sxoffset = _subEditView->execute(SCI_GETXOFFSET);
			pixel = int(_subEditView->execute(SCI_TEXTWIDTH, STYLE_DEFAULT, (LPARAM)"P"));
			subColumn = sxoffset/pixel;
			column = subColumn + _syncInfo._column - mainColumn;
		}
		pView = _mainEditView;
    }
    else
        return;

	pView->scroll(column, line);
}

bool Notepad_plus::getIntegralDockingData(tTbData & dockData, int & iCont, bool & isVisible)
{
	DockingManagerData & dockingData = (DockingManagerData &)(NppParameters::getInstance())->getNppGUI()._dockingData;

	for (size_t i = 0 ; i < dockingData._pluginDockInfo.size() ; i++)
	{
		const PlugingDlgDockingInfo & pddi = dockingData._pluginDockInfo[i];

		if (!generic_stricmp(pddi._name.c_str(), dockData.pszModuleName) && (pddi._internalID == dockData.dlgID))
		{
			iCont				= pddi._currContainer;
			isVisible			= pddi._isVisible;
			dockData.iPrevCont	= pddi._prevContainer;

			if (dockData.iPrevCont != -1)
			{
				int cont = (pddi._currContainer < DOCKCONT_MAX ? pddi._prevContainer : pddi._currContainer);
				RECT *pRc = dockingData.getFloatingRCFrom(cont);
				if (pRc)
					dockData.rcFloat	= *pRc;
			}
			return true;
		}
	}
	return false;
}


void Notepad_plus::getCurrentOpenedFiles(Session & in_session)
{
	assert(_mainDocTab);
	assert(_subDocTab);
	_mainEditView->saveCurrentPos();	//save position so itll be correct in the session
	_subEditView->saveCurrentPos();	//both views
	in_session._activeView = currentView();
	in_session._activeMainIndex = _mainDocTab->getCurrentTabIndex();
	in_session._activeSubIndex = _subDocTab->getCurrentTabIndex();

	//Use _invisibleEditView to temporarily open documents to retrieve markers
	//Buffer * mainBuf = _mainEditView->getCurrentBuffer();
	//Buffer * subBuf = _subEditView->getCurrentBuffer();
	Document oldDoc = _invisibleEditView->execute(SCI_GETDOCPOINTER);

	for (int i = 0 ; i < _mainDocTab->nbItem() ; i++)
	{
		BufferID bufID = _mainDocTab->getBufferByIndex(i);
		Buffer * buf = MainFileManager->getBufferByID(bufID);
		if (!buf->isUntitled() && PathFileExists(buf->getFullPathName()))
		{
			generic_string	languageName	= getLangFromMenu( buf );
			const TCHAR *langName	= languageName.c_str();

			sessionFileInfo sfi(buf->getFullPathName(), langName, buf->getEncoding(), buf->getPosition(_mainEditView));

			//_mainEditView->activateBuffer(buf->getID());
			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, buf->getDocument());
			int maxLine = _invisibleEditView->execute(SCI_GETLINECOUNT);

			for (int j = 0 ; j < maxLine ; j++)
			{
				if ((_invisibleEditView->execute(SCI_MARKERGET, j)&(1 << MARK_BOOKMARK)) != 0)
				{
					sfi.marks.push_back(j);
				}
			}
			in_session._mainViewFiles.push_back(sfi);
		}
	}

	for (int i = 0 ; i < _subDocTab->nbItem() ; i++)
	{
		BufferID bufID = _subDocTab->getBufferByIndex(i);
		Buffer * buf = MainFileManager->getBufferByID(bufID);
		if (!buf->isUntitled() && PathFileExists(buf->getFullPathName()))
		{
			generic_string	languageName	= getLangFromMenu( buf );
			const TCHAR *langName	= languageName.c_str();

			sessionFileInfo sfi(buf->getFullPathName(), langName, buf->getEncoding(), buf->getPosition(_subEditView));

			_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, buf->getDocument());
			int maxLine = _invisibleEditView->execute(SCI_GETLINECOUNT);
			for (int j = 0 ; j < maxLine ; j++)
			{
				if ((_invisibleEditView->execute(SCI_MARKERGET, j)&(1 << MARK_BOOKMARK)) != 0)
				{
					sfi.marks.push_back(j);
				}
			}
			in_session._subViewFiles.push_back(sfi);
		}
	}

	_invisibleEditView->execute(SCI_SETDOCPOINTER, 0, oldDoc);
}

bool Notepad_plus::str2Cliboard(const TCHAR *str2cpy)
{
	if (!str2cpy)
		return false;

	int len2Allocate = lstrlen(str2cpy) + 1;
	len2Allocate *= sizeof(TCHAR);
	unsigned int cilpboardFormat = CF_UNICODETEXT;

	HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, len2Allocate);
	if (hglbCopy == NULL)
	{
		return false;
	}

	if (!::OpenClipboard(_pPublicInterface->getHSelf()))
		return false;

	::EmptyClipboard();

	// Lock the handle and copy the text to the buffer.
	TCHAR *pStr = (TCHAR *)::GlobalLock(hglbCopy);
	lstrcpy(pStr, str2cpy);
	::GlobalUnlock(hglbCopy);

	// Place the handle on the clipboard.
	::SetClipboardData(cilpboardFormat, hglbCopy);
	::CloseClipboard();
	return true;
}

//ONLY CALL IN CASE OF EMERGENCY: EXCEPTION
//This function is destructive
bool Notepad_plus::emergency(generic_string emergencySavedDir)
{
	::CreateDirectory(emergencySavedDir.c_str(), NULL);
	return dumpFiles(emergencySavedDir.c_str(), TEXT("File"));
}

bool Notepad_plus::dumpFiles(const TCHAR * outdir, const TCHAR * fileprefix) {
	//start dumping unsaved files to recovery directory
	bool somethingsaved = false;
	bool somedirty = false;
	TCHAR savePath[MAX_PATH] = {0};

	//rescue primary
	for(int i = 0; i < MainFileManager->getNrBuffers(); i++) {
		Buffer * docbuf = MainFileManager->getBufferByIndex(i);
		if (!docbuf->isDirty())	//skip saved documents
			continue;
		else
			somedirty = true;

		const TCHAR * unitext = (docbuf->getUnicodeMode() != uni8Bit)?TEXT("_utf8"):TEXT("");
		wsprintf(savePath, TEXT("%s\\%s%03d%s.dump"), outdir, fileprefix, i, unitext);

		bool res = MainFileManager->saveBuffer(docbuf->getID(), savePath);

		somethingsaved |= res;
	}

	return somethingsaved || !somedirty;
}

void Notepad_plus::drawTabbarColoursFromStylerArray()
{
	Style *stActText = getStyleFromName(TABBAR_ACTIVETEXT);
	if (stActText && stActText->_fgColor != -1)
		TabBarPlus::setColour(stActText->_fgColor, TabBarPlus::activeText);

	Style *stActfocusTop = getStyleFromName(TABBAR_ACTIVEFOCUSEDINDCATOR);
	if (stActfocusTop && stActfocusTop->_fgColor != -1)
		TabBarPlus::setColour(stActfocusTop->_fgColor, TabBarPlus::activeFocusedTop);

	Style *stActunfocusTop = getStyleFromName(TABBAR_ACTIVEUNFOCUSEDINDCATOR);
	if (stActunfocusTop && stActunfocusTop->_fgColor != -1)
		TabBarPlus::setColour(stActunfocusTop->_fgColor, TabBarPlus::activeUnfocusedTop);

	Style *stInact = getStyleFromName(TABBAR_INACTIVETEXT);
	if (stInact && stInact->_fgColor != -1)
		TabBarPlus::setColour(stInact->_fgColor, TabBarPlus::inactiveText);
	if (stInact && stInact->_bgColor != -1)
		TabBarPlus::setColour(stInact->_bgColor, TabBarPlus::inactiveBg);
}

void Notepad_plus::notifyBufferChanged(Buffer * buffer, int mask)
{
	assert(_mainDocTab);
	assert(_subDocTab);

	NppParameters *pNppParam = NppParameters::getInstance();
	const NppGUI & nppGUI = pNppParam->getNppGUI();

	_mainEditView->bufferUpdated(buffer, mask);
	_subEditView->bufferUpdated(buffer, mask);
	_mainDocTab->bufferUpdated(buffer, mask);
	_subDocTab->bufferUpdated(buffer, mask);

	bool mainActive = (_mainEditView->getCurrentBuffer() == buffer);
	bool subActive = (_subEditView->getCurrentBuffer() == buffer);

	//Only event that applies to non-active Buffers
	if (mask & BufferChangeStatus) {	//reload etc
		bool didDialog = false;
		switch(buffer->getStatus()) {
			case DOC_UNNAMED: 	//nothing todo
			{
				break;
			}
			case DOC_REGULAR: 	//nothing todo
			{
				break;
			}
			case DOC_MODIFIED:	//ask for reloading
			{
				bool autoUpdate = (nppGUI._fileAutoDetection == cdAutoUpdate) || (nppGUI._fileAutoDetection == cdAutoUpdateGo2end);
				if (!autoUpdate || buffer->isDirty())
				{
                    // if file updating is not silently, we switch to the file to update.
                    int index = _pDocTab->getIndexByBuffer(buffer->getID());
				    int iView = currentView();
				    if (index == -1)
					    iView = otherView();
				    activateBuffer(buffer->getID(), iView);	//activate the buffer in the first view possible

                    // Then we ask user to update
					didDialog = true;
					if (doReloadOrNot(buffer->getFullPathName(), buffer->isDirty()) != IDYES)
						break;	//abort
				}

				doReload(buffer->getID(), false);
				if (mainActive || subActive)
				{
					performPostReload(mainActive?MAIN_VIEW:SUB_VIEW);
				}
				break;
			}
			case DOC_DELETED: 	//ask for keep
			{
				int index = _pDocTab->getIndexByBuffer(buffer->getID());
				int iView = currentView();
				if (index == -1)
					iView = otherView();

				activateBuffer(buffer->getID(), iView);	//activate the buffer in the first view possible
				didDialog = true;
				if (doCloseOrNot(buffer->getFullPathName()) == IDNO)
				{
					//close in both views, doing current view last since that has to remain opened
					doClose(buffer->getID(), otherView());
					doClose(buffer->getID(), currentView());
				}
				break;
			}
		}

		if (didDialog)
		{
			int curPos = _pEditView->execute(SCI_GETCURRENTPOS);
			::PostMessage(_pEditView->getHSelf(), WM_LBUTTONUP, 0, 0);
			::PostMessage(_pEditView->getHSelf(), SCI_SETSEL, curPos, curPos);
			if (::IsIconic(_pPublicInterface->getHSelf()))
				::ShowWindow(_pPublicInterface->getHSelf(), SW_RESTORE);
		}
	}

    if (mask & (BufferChangeReadonly))
	{
		checkDocState();

		bool isSysReadOnly = buffer->getFileReadOnly();
		bool isUserReadOnly = buffer->getUserReadOnly();
		bool isDirty = buffer->isDirty();

		// To notify plugins ro status is changed
		SCNotification scnN;
		scnN.nmhdr.hwndFrom = (void *)buffer->getID();
		scnN.nmhdr.idFrom = (uptr_t)  ((isSysReadOnly || isUserReadOnly? DOCSTAUS_READONLY : 0) | (isDirty ? DOCSTAUS_BUFFERDIRTY : 0));
		scnN.nmhdr.code = NPPN_READONLYCHANGED;
		_pluginsManager->notify(&scnN);

	}

	if (!mainActive && !subActive)
	{
		return;
	}

	if (mask & (BufferChangeLanguage))
	{
		assert(_autoCompleteMain);
		assert(_autoCompleteSub);
		if (mainActive)
			_autoCompleteMain->setLanguage(buffer->getLangType());
		if (subActive)
			_autoCompleteSub->setLanguage(buffer->getLangType());
	}

	if ((currentView() == MAIN_VIEW) && !mainActive)
		return;

	if ((currentView() == SUB_VIEW) && !subActive)
		return;

	if (mask & (BufferChangeDirty|BufferChangeFilename))
	{
		checkDocState();
		setTitle();
		generic_string dir(buffer->getFullPathName());
		PathRemoveFileSpec(dir);
		setWorkingDir(dir.c_str());
	}

	if (mask & (BufferChangeLanguage))
	{
		assert(_autoCompleteMain);
		assert(_autoCompleteSub);
		checkLangsMenu(-1);	//let N++ do search for the item
		setLangStatus(buffer->getLangType());
		if (_mainEditView->getCurrentBuffer() == buffer)
			_autoCompleteMain->setLanguage(buffer->getLangType());
		else if (_subEditView->getCurrentBuffer() == buffer)
			_autoCompleteSub->setLanguage(buffer->getLangType());

		SCNotification scnN;
		scnN.nmhdr.code = NPPN_LANGCHANGED;
		scnN.nmhdr.hwndFrom = _pPublicInterface->getHSelf();
		scnN.nmhdr.idFrom = (uptr_t)_pEditView->getCurrentBufferID();
		_pluginsManager->notify(&scnN);
	}

	if (mask & (BufferChangeFormat|BufferChangeLanguage|BufferChangeUnicode))
	{
		updateStatusBar();
		checkUnicodeMenuItems(/*buffer->getUnicodeMode()*/);
		setUniModeText();
		setDisplayFormat(buffer->getFormat());
		enableConvertMenuItems(buffer->getFormat());
	}
}

void Notepad_plus::notifyBufferActivated(BufferID bufid, int view)
{
	assert(_autoCompleteMain);
	assert(_autoCompleteSub);
	assert(_mainDocTab);
	assert(_subDocTab);

	Buffer * buf = MainFileManager->getBufferByID(bufid);
	buf->increaseRecentTag();

	if (view == MAIN_VIEW) {
		_autoCompleteMain->setLanguage(buf->getLangType());
	} else if (view == SUB_VIEW) {
		_autoCompleteSub->setLanguage(buf->getLangType());
	}

	if (view != currentView()) {
		return;	//dont care if another view did something
	}

	checkDocState();
	dynamicCheckMenuAndTB();
	setLangStatus(buf->getLangType());
	updateStatusBar();
	checkUnicodeMenuItems(/*buf->getUnicodeMode()*/);
	setUniModeText();
	setDisplayFormat(buf->getFormat());
	enableConvertMenuItems(buf->getFormat());
	generic_string dir(buf->getFullPathName());
	PathRemoveFileSpec(dir);
	setWorkingDir(dir.c_str());
	setTitle();
	//Make sure the colors of the tab controls match
	::InvalidateRect(_mainDocTab->getHSelf(), NULL, FALSE);
	::InvalidateRect(_subDocTab->getHSelf(), NULL, FALSE);

	SCNotification scnN;
	scnN.nmhdr.code = NPPN_BUFFERACTIVATED;
	scnN.nmhdr.hwndFrom = _pPublicInterface->getHSelf();
	scnN.nmhdr.idFrom = (uptr_t)bufid;
	_pluginsManager->notify(&scnN);

	_linkTriggered = true;
}

void Notepad_plus::loadCommandlineParams(const TCHAR * commandLine, CmdLineParams * pCmdParams) {
	if (!commandLine || ! pCmdParams)
		return;

	FileNameStringSplitter fnss(commandLine);
	const TCHAR *pFn = NULL;

	LangType lt = pCmdParams->_langType;//LangType(pCopyData->dwData & LASTBYTEMASK);
	int ln =  pCmdParams->_line2go;
    int cn = pCmdParams->_column2go;

	bool readOnly = pCmdParams->_isReadOnly;

	BufferID lastOpened = BUFFER_INVALID;
	for (int i = 0 ; i < fnss.size() ; i++)
	{
		pFn = fnss.getFileName(i);
		BufferID bufID = doOpen(pFn, readOnly);
		if (bufID == BUFFER_INVALID)	//cannot open file
			continue;

		lastOpened = bufID;

		if (lt != L_EXTERNAL && lt < NppParameters::getInstance()->L_END)
		{
			Buffer * pBuf = MainFileManager->getBufferByID(bufID);
			pBuf->setLangType(lt);
		}

		if (ln != -1)
		{	//we have to move the cursor manually
			int iView = currentView();	//store view since fileswitch can cause it to change
			switchToFile(bufID);	//switch to the file. No deferred loading, but this way we can easily move the cursor to the right position

            if (cn == -1)
			_pEditView->execute(SCI_GOTOLINE, ln-1);
            else
            {
                int pos = _pEditView->execute(SCI_FINDCOLUMN, ln-1, cn-1);
                _pEditView->execute(SCI_GOTOPOS, pos);
            }
			switchEditViewTo(iView);	//restore view
		}
	}
	if (lastOpened != BUFFER_INVALID)
    {
		switchToFile(lastOpened);
	}
}


void Notepad_plus::setFindReplaceFolderFilter(const TCHAR *dir, const TCHAR *filter)
{
	assert(_findReplaceDlg);

	generic_string fltr;
	NppParameters *pNppParam = NppParameters::getInstance();
	FindHistory & findHistory = pNppParam->getFindHistory();

	// get current directory in case it's not provided.
	if (!dir && findHistory._isFolderFollowDoc)
	{
		dir = pNppParam->getWorkingDir();
	}

	// get current language file extensions in case it's not provided.
	if (!filter && findHistory._isFilterFollowDoc)
	{
		// Get current language file extensions
		generic_string ext;
		LangType lt = _pEditView->getCurrentBuffer()->getLangType();

		if (lt == L_USER)
		{
			Buffer * buf = _pEditView->getCurrentBuffer();
			UserLangContainer * userLangContainer = pNppParam->getULCFromName(buf->getUserDefineLangName());
			if (userLangContainer)
				ext = userLangContainer->getExtension();
		}
		else
		{
			ext = NppParameters::getInstance()->getLangExtFromLangType(lt);
		}

		if (ext.length() > 0)
		{
			fltr = TEXT("");
			std::vector<generic_string> vStr;
			cutString(ext, vStr);
			for (size_t i = 0; i < vStr.size(); i++)
			{
				fltr += TEXT("*.");
				fltr += vStr[i] + TEXT(" ");
			}
		}
		else
		{
			fltr = TEXT("*.*");
		}
		filter = fltr.c_str();
	}
	_findReplaceDlg->setFindInFilesDirFilter(dir, filter);
}

std::vector<generic_string> Notepad_plus::addNppComponents(const TCHAR *destDir, const TCHAR *extFilterName, const TCHAR *extFilter)
{
    FileDialog fDlg(_pPublicInterface->getHSelf(), _pPublicInterface->getHinst());
    fDlg.setExtFilter(extFilterName, extFilter, NULL);

    std::vector<generic_string> copiedFiles;

    if (stringVector *pfns = fDlg.doOpenMultiFilesDlg())
    {
        // Get plugins dir
		generic_string destDirName = (NppParameters::getInstance())->getNppPath();
        PathAppend(destDirName, destDir);

        if (!::PathFileExists(destDirName.c_str()))
        {
            ::CreateDirectory(destDirName.c_str(), NULL);
        }

        destDirName += TEXT("\\");

        size_t sz = pfns->size();
        for (size_t i = 0 ; i < sz ; i++)
        {
            if (::PathFileExists(pfns->at(i).c_str()))
            {
                // copy to plugins directory
                generic_string destName = destDirName;
                destName += ::PathFindFileName(pfns->at(i).c_str());
                //printStr(destName.c_str());
                if (::CopyFile(pfns->at(i).c_str(), destName.c_str(), FALSE))
                    copiedFiles.push_back(destName.c_str());
            }
        }
    }
    return copiedFiles;
}

void Notepad_plus::setWorkingDir(const TCHAR *dir)
{
	NppParameters * params = NppParameters::getInstance();
	if (params->getNppGUI()._openSaveDir == dir_last)
		return;
	if (params->getNppGUI()._openSaveDir == dir_userDef)
	{
		params->setWorkingDir(NULL);
	}
	else if (dir && PathIsDirectory(dir))
	{
		params->setWorkingDir(dir);
	}
}

void Notepad_plus::bookmarkAdd(int lineno) const
{
	if (lineno == -1)
		lineno = _pEditView->getCurrentLineNumber();
	if (!bookmarkPresent(lineno))
		_pEditView->execute(SCI_MARKERADD, lineno, MARK_BOOKMARK);
}

void Notepad_plus::bookmarkDelete(int lineno) const
{
	if (lineno == -1)
		lineno = _pEditView->getCurrentLineNumber();
	if ( bookmarkPresent(lineno))
		_pEditView->execute(SCI_MARKERDELETE, lineno, MARK_BOOKMARK);
}

bool Notepad_plus::bookmarkPresent(int lineno) const
{
	if (lineno == -1)
		lineno = _pEditView->getCurrentLineNumber();
	LRESULT state = _pEditView->execute(SCI_MARKERGET, lineno);
	return ((state & (1 << MARK_BOOKMARK)) != 0);
}

void Notepad_plus::bookmarkToggle(int lineno) const
{
	if (lineno == -1)
		lineno = _pEditView->getCurrentLineNumber();

	if (bookmarkPresent(lineno))
		bookmarkDelete(lineno);
	else
		bookmarkAdd(lineno);
}

int Notepad_plus::getLangFromMenuName(const TCHAR * langName)
{
	int	id	= 0;
	const int menuSize = 64;
	TCHAR menuLangName[menuSize];

	for ( int i = IDM_LANG_C; i <= IDM_LANG_USER; i++ )
	{
		if ( ::GetMenuString( _mainMenuHandle, i, menuLangName, menuSize, MF_BYCOMMAND ) )
		{
			if ( !lstrcmp( langName, menuLangName ) )
			{
				id	= i;
				break;
			}
		}
	}

	if ( id == 0 )
	{
		for ( int i = IDM_LANG_USER + 1; i <= IDM_LANG_USER_LIMIT; i++ )
		{
			if ( ::GetMenuString( _mainMenuHandle, i, menuLangName, menuSize, MF_BYCOMMAND ) )
			{
				if ( !lstrcmp( langName, menuLangName ) )
				{
					id	= i;
					break;
				}
			}
		}
	}

	return id;
}

generic_string Notepad_plus::getLangFromMenu(const Buffer * buf)
{
	int	id;
	generic_string userLangName;
	TCHAR	menuLangName[32];

	id = (NppParameters::getInstance())->langTypeToCommandID( buf->getLangType() );

	if ( ( id != IDM_LANG_USER ) || !( buf->isUserDefineLangExt() ) )
	{
		::GetMenuString( _mainMenuHandle, id, menuLangName, sizeof( menuLangName ), MF_BYCOMMAND );
		userLangName = (TCHAR *)menuLangName;
	}
	else
	{
		userLangName = buf->getUserDefineLangName();
	}
	return	userLangName;
}

Style * Notepad_plus::getStyleFromName(const TCHAR *styleName)
{
	StyleArray & stylers = (NppParameters::getInstance())->getMiscStylerArray();

	int i = stylers.getStylerIndexByName(styleName);
	Style * st = NULL;
	if (i != -1)
	{
		Style & style = stylers.getStyler(i);
		st = &style;
	}
	return st;
}

bool Notepad_plus::noOpenedDoc() const
{
	assert(_mainDocTab);
	assert(_subDocTab);
	if (_mainDocTab->isVisible() && _subDocTab->isVisible())
		return false;
	if (_pDocTab->nbItem() == 1)
	{
		BufferID buffer = _pDocTab->getBufferByIndex(0);
		Buffer * buf = MainFileManager->getBufferByID(buffer);
		if (!buf->isDirty() && buf->isUntitled())
			return true;
	}
	return false;
}

void Notepad_plus::EnableMouseWheelZoom(bool enable)
{
	_subEditView->execute(SCI_SETWHEELZOOMING, enable);
	_mainEditView->execute(SCI_SETWHEELZOOMING, enable);
	_invisibleEditView->execute(SCI_SETWHEELZOOMING, enable);
	_fileEditView->execute(SCI_SETWHEELZOOMING, enable);
}

void Notepad_plus::checkMenuItem(int itemID, bool willBeChecked) const
{
	::CheckMenuItem(_mainMenuHandle, itemID, MF_BYCOMMAND | (willBeChecked?MF_CHECKED:MF_UNCHECKED));
}

bool Notepad_plus::reloadLang()
{
	NppParameters *pNppParam = NppParameters::getInstance();

	if (!pNppParam->reloadLang())
	{
		return false;
	}

	TiXmlDocumentA *nativeLangDocRootA = pNppParam->getNativeLangA();
	if (!nativeLangDocRootA)
	{
		return false;
	}

    _nativeLangSpeaker->init(nativeLangDocRootA, true);

    pNppParam->reloadContextMenuFromXmlTree(_mainMenuHandle);

	generic_string pluginsTrans, windowTrans;
	_nativeLangSpeaker->changeMenuLang(_mainMenuHandle, pluginsTrans, windowTrans);
    ::DrawMenuBar(_pPublicInterface->getHSelf());

	int indexWindow = ::GetMenuItemCount(_mainMenuHandle) - 3;

	if (_pluginsManager->hasPlugins() && pluginsTrans != TEXT(""))
	{
		::ModifyMenu(_mainMenuHandle, indexWindow - 1, MF_BYPOSITION, 0, pluginsTrans.c_str());
	}

	if (windowTrans != TEXT(""))
	{
		::ModifyMenu(_mainMenuHandle, indexWindow, MF_BYPOSITION, 0, windowTrans.c_str());
		windowTrans += TEXT("...");
		::ModifyMenu(_mainMenuHandle, IDM_WINDOW_WINDOWS, MF_BYCOMMAND, IDM_WINDOW_WINDOWS, windowTrans.c_str());
	}
	// Update scintilla context menu strings
	std::vector<MenuItemUnit> & tmp = pNppParam->getContextMenuItems();
	size_t len = tmp.size();
	TCHAR menuName[64];
	for (size_t i = 0 ; i < len ; i++)
	{
		if (tmp[i]._itemName == TEXT(""))
		{
			::GetMenuString(_mainMenuHandle, tmp[i]._cmdID, menuName, 64, MF_BYCOMMAND);
			tmp[i]._itemName = purgeMenuItemString(menuName);
		}
	}

	std::vector<CommandShortcut> & shortcuts = pNppParam->getUserShortcuts();
	len = shortcuts.size();

	for(size_t i = 0; i < len; i++)
	{
		CommandShortcut & csc = shortcuts[i];
		::GetMenuString(_mainMenuHandle, csc.getID(), menuName, 64, MF_BYCOMMAND);
		csc.setName(purgeMenuItemString(menuName, true).c_str());
	}
	_accelerator.updateFullMenu();

	_scintaccelerator.updateKeys();


	if (_tabPopupMenu && _tabPopupMenu->isCreated())
	{
		_nativeLangSpeaker->changeLangTabContextMenu(_tabPopupMenu->getMenuHandle());
	}
	if (_tabPopupDropMenu && _tabPopupDropMenu->isCreated())
	{
		_nativeLangSpeaker->changeLangTabDrapContextMenu(_tabPopupDropMenu->getMenuHandle());
	}

	if (_preferenceDlg && _preferenceDlg->isCreated())
	{
		_nativeLangSpeaker->changePrefereceDlgLang(_preferenceDlg);
	}

	if (_configStyleDlg && _configStyleDlg->isCreated())
	{
        _nativeLangSpeaker->changeConfigLang(_configStyleDlg->getHSelf());
	}

	if (_findReplaceDlg && _findReplaceDlg->isCreated())
	{
		_nativeLangSpeaker->changeFindReplaceDlgLang(_findReplaceDlg);
	}

	if (_goToLineDlg && _goToLineDlg->isCreated())
	{
		_nativeLangSpeaker->changeDlgLang(_goToLineDlg->getHSelf(), "GoToLine");
	}

	if (_runDlg && _runDlg->isCreated())
	{
		_nativeLangSpeaker->changeDlgLang(_runDlg->getHSelf(), "Run");
	}

	if (_runMacroDlg && _runMacroDlg->isCreated())
	{
		_nativeLangSpeaker->changeDlgLang(_runMacroDlg->getHSelf(), "MultiMacro");
	}

	if (_colEditorDlg && _colEditorDlg->isCreated())
	{
        _nativeLangSpeaker->changeDlgLang(_colEditorDlg->getHSelf(), "ColumnEditor");
	}

	UserDefineDialog *udd = ScintillaEditView::getUserDefineDlg();
	if (udd && udd->isCreated())
	{
		_nativeLangSpeaker->changeUserDefineLang(udd);
	}

	if (_lastRecentFileList)
	{
		_lastRecentFileList->setLangEncoding(_nativeLangSpeaker->getLangEncoding());
	}
	return true;
}
