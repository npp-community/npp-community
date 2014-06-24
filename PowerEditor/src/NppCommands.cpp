// This file is part of notepad++
// Copyright (C)2009-2010 The Notepad++ Team
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

#include "MISC/PluginsManager/PluginsManager.h"
#include "MISC/Process/Process.h"

#include "WinControls/AboutDlg/AboutDlg.h"
#include "WinControls/ColourPicker/WordStyleDlg.h"
#include "WinControls/Grid/ShortcutMapper.h"
#include "WinControls/ImageListSet/ImageListSet.h"
#include "WinControls/Preference/preferenceDlg.h"
#include "WinControls/shortcut/RunMacroDlg.h"
#include "WinControls/SplitterContainer/SplitterContainer.h"
#include "WinControls/StaticDialog/RunDlg/RunDlg.h"
#include "WinControls/TaskList/TaskListDlg.h"
#include "WinControls/ToolBar/Toolbar.h"
#include "WinControls/TrayIcon/trayIconControler.h"
#include "WinControls/WindowsDlg/WindowsDlg.h"
#include "WinControls/WindowsDlg/WindowsDlgRc.h"

#include "ScintillaComponent/Buffer.h"
#include "ScintillaComponent/columnEditor.h"
#include "ScintillaComponent/DocTabView.h"
#include "ScintillaComponent/FindReplaceDlg.h"
#include "ScintillaComponent/GoToLineDlg.h"
#include "ScintillaComponent/UserDefineDialog.h"
#include "ScintillaComponent/ScintillaEditView.h"
#include "ScintillaComponent/UserDefineResource.h"

#include "EncodingMapper.h"
#include "lastRecentFileList.h"
#include "localization.h"
#include "MenuCmdID.h"
#include "Parameters.h"
#include "ValueDlg.h"

#include "Notepad_plus_Window.h"
#include "Notepad_plus.h"

void Notepad_plus::macroPlayback(Macro macro)
{
	_pEditView->execute(SCI_BEGINUNDOACTION);

	for (Macro::iterator step = macro.begin(); step != macro.end(); step++)
	{
		if (step->isPlayable())
		{
			step->PlayBack(this->_pPublicInterface, _pEditView);
		}
		else
		{
			_findReplaceDlg->execSavedCommand(step->message, step->lParameter, step->sParameter);
		}
	}

	_pEditView->execute(SCI_ENDUNDOACTION);
}
void Notepad_plus::command(int id)
{
	switch (id)
	{
		case IDM_FILE_NEW:
			fileNew();
			break;

		case IDM_FILE_OPEN:
			fileOpen();
			break;

		case IDM_FILE_RELOAD:
			fileReload();
			break;

		case IDM_FILE_CLOSE:
			if (fileClose())
                checkDocState();
			break;

		case IDM_FILE_DELETE:
			if (fileDelete())
                checkDocState();
			break;

		case IDM_FILE_RENAME:
			fileRename();
			break;

		case IDM_FILE_CLOSEALL:
			fileCloseAll();
            checkDocState();
			break;

		case IDM_FILE_CLOSEALL_BUT_CURRENT :
			fileCloseAllButCurrent();
            checkDocState();
			break;

		case IDM_FILE_SAVE :
			fileSave();
			break;

		case IDM_FILE_SAVEALL :
			fileSaveAll();
			break;

		case IDM_FILE_SAVEAS :
			fileSaveAs();
			break;

		case IDM_FILE_SAVECOPYAS :
			fileSaveAs(BUFFER_INVALID, true);
			break;

		case IDM_FILE_LOADSESSION:
			fileLoadSession();
			break;

		case IDM_FILE_SAVESESSION:
			fileSaveSession();
			break;

		case IDM_FILE_PRINTNOW :
			filePrint(false);
			break;

		case IDM_FILE_PRINT :
			filePrint(true);
			break;

		case IDM_FILE_EXIT:
			::SendMessage(_pPublicInterface->getHSelf(), WM_CLOSE, 0, 0);
			break;

		case IDM_EDIT_UNDO:
			_pEditView->execute(WM_UNDO);
			checkClipboard();
			checkUndoState();
			break;

		case IDM_EDIT_REDO:
			_pEditView->execute(SCI_REDO);
			checkClipboard();
			checkUndoState();
			break;

		case IDM_EDIT_CUT:
			_pEditView->execute(WM_CUT);
			checkClipboard();
			break;

		case IDM_EDIT_COPY:
			_pEditView->execute(WM_COPY);
			checkClipboard();
			break;

		case IDM_EDIT_PASTE:
		{
			int eolMode = int(_pEditView->execute(SCI_GETEOLMODE));
			_pEditView->execute(SCI_PASTE);
			_pEditView->execute(SCI_CONVERTEOLS, eolMode);
		}
		break;

		case IDM_EDIT_DELETE:
			_pEditView->execute(WM_CLEAR);
			break;

		case IDM_MACRO_STARTRECORDINGMACRO:
		case IDM_MACRO_STOPRECORDINGMACRO:
		case IDC_EDIT_TOGGLEMACRORECORDING:
		{
			if (_recordingMacro)
			{
				assert(_runMacroDlg);
				// STOP !!!
				_mainEditView->execute(SCI_STOPRECORD);
				_subEditView->execute(SCI_STOPRECORD);

				_mainEditView->execute(SCI_SETCURSOR, (WPARAM)SC_CURSORNORMAL);
				_subEditView->execute(SCI_SETCURSOR, (WPARAM)SC_CURSORNORMAL);

				_recordingMacro = false;
				_runMacroDlg->initMacroList();
			}
			else
			{
				_mainEditView->execute(SCI_SETCURSOR, 9);
				_subEditView->execute(SCI_SETCURSOR, 9);
				_macro.clear();

				// START !!!
				_mainEditView->execute(SCI_STARTRECORD);
				_subEditView->execute(SCI_STARTRECORD);
				_recordingMacro = true;
			}
			checkMacroState();
			break;
		}

		case IDM_MACRO_PLAYBACKRECORDEDMACRO:
			if (!_recordingMacro) // if we're not currently recording, then playback the recorded keystrokes
			{
				macroPlayback(_macro);
			}
			break;

		case IDM_MACRO_RUNMULTIMACRODLG :
		{
			if (!_recordingMacro) // if we're not currently recording, then playback the recorded keystrokes
			{
				assert(_runMacroDlg);
				bool isFirstTime = !_runMacroDlg->isCreated();
				_runMacroDlg->doDialog(_nativeLangSpeaker->isRTL());

				if (isFirstTime)
				{
					_nativeLangSpeaker->changeDlgLang(_runMacroDlg->getHSelf(), "MultiMacro");
				}
				break;

			}
		}
		break;

		case IDM_MACRO_SAVECURRENTMACRO :
		{
			if (addCurrentMacro())
			{
				assert(_runMacroDlg);
				_runMacroDlg->initMacroList();
			}
			break;
		}
		case IDM_EDIT_FULLPATHTOCLIP :
		case IDM_EDIT_CURRENTDIRTOCLIP :
		case IDM_EDIT_FILENAMETOCLIP :
		{
			Buffer * buf = _pEditView->getCurrentBuffer();
			if (id == IDM_EDIT_FULLPATHTOCLIP)
			{
				str2Cliboard(buf->getFullPathName());
			}
			else if (id == IDM_EDIT_CURRENTDIRTOCLIP)
			{
				generic_string dir(buf->getFullPathName());
				PathRemoveFileSpec(dir);
				str2Cliboard(dir.c_str());
			}
			else if (id == IDM_EDIT_FILENAMETOCLIP)
			{
				str2Cliboard(buf->getFileName());
			}
		}
		break;

		case IDM_SEARCH_FIND :
		case IDM_SEARCH_REPLACE :
		{
			assert(_findReplaceDlg);

			const int strSize = FINDREPLACE_MAXLENGTH;
			TCHAR str[strSize];

			bool isFirstTime = !_findReplaceDlg->isCreated();

			_findReplaceDlg->doDialog((id == IDM_SEARCH_FIND)?FIND_DLG:REPLACE_DLG, _nativeLangSpeaker->isRTL());

			_pEditView->getGenericSelectedText(str, strSize);
			_findReplaceDlg->setSearchText(str);
			setFindReplaceFolderFilter(NULL, NULL);

			if (isFirstTime)
				_nativeLangSpeaker->changeFindReplaceDlgLang(_findReplaceDlg);
			break;
		}

		case IDM_SEARCH_FINDINFILES :
		{
			::SendMessage(_pPublicInterface->getHSelf(), NPPM_LAUNCHFINDINFILESDLG, 0, 0);
			break;
		}
		case IDM_SEARCH_FINDINCREMENT :
		{
			assert(_incrementFindDlg);
			const int strSize = FINDREPLACE_MAXLENGTH;
			TCHAR str[strSize];

			_pEditView->getGenericSelectedText(str, strSize, false);
			if (0!=str[0])         // the selected text is not empty, then use it
				_incrementFindDlg->setSearchText(str, _pEditView->getCurrentBuffer()->getUnicodeMode() != uni8Bit);

			_incrementFindDlg->display();
		}
		break;

		case IDM_SEARCH_FINDNEXT :
		case IDM_SEARCH_FINDPREV :
		{
			assert(_findReplaceDlg);
			if (!_findReplaceDlg->isCreated())
				return;

			FindOption op = _findReplaceDlg->getCurrentOptions();
			op._whichDirection = (id == IDM_SEARCH_FINDNEXT?DIR_DOWN:DIR_UP);
			generic_string s = _findReplaceDlg->getText2search();

			_findReplaceDlg->processFindNext(s.c_str(), &op);
			break;
		}
		break;

        case IDM_SEARCH_SETANDFINDNEXT :
		case IDM_SEARCH_SETANDFINDPREV :
        {
			assert(_findReplaceDlg);
            bool isFirstTime = !_findReplaceDlg->isCreated();
			if (isFirstTime)
				_findReplaceDlg->doDialog(FIND_DLG, _nativeLangSpeaker->isRTL(), false);

			const int strSize = FINDREPLACE_MAXLENGTH;
			TCHAR str[strSize];
			_pEditView->getGenericSelectedText(str, strSize);
			_findReplaceDlg->setSearchText(str);
			FindReplaceDlg::_env->_str2Search = str;
			setFindReplaceFolderFilter(NULL, NULL);
			if (isFirstTime)
				_nativeLangSpeaker->changeFindReplaceDlgLang(_findReplaceDlg);

			FindOption op = _findReplaceDlg->getCurrentOptions();
			op._whichDirection = (id == IDM_SEARCH_SETANDFINDNEXT?DIR_DOWN:DIR_UP);

			_findReplaceDlg->processFindNext(str, &op);
			break;
        }

		case IDM_SEARCH_GOTONEXTFOUND:
		{
			assert(_findReplaceDlg);
			_findReplaceDlg->gotoNextFoundResult();
			break;
		}
		case IDM_SEARCH_GOTOPREVFOUND:
		{
			assert(_findReplaceDlg);
			_findReplaceDlg->gotoNextFoundResult(-1);
			break;
		}
		case IDM_FOCUS_ON_FOUND_RESULTS:
		{
			assert(_findReplaceDlg);
			if (GetFocus() == _findReplaceDlg->getHFindResults())
				// focus already on found results, switch to current edit view
				switchEditViewTo(currentView());
			else
				_findReplaceDlg->focusOnFinder();
			break;
		}

		case IDM_SEARCH_VOLATILE_FINDNEXT :
		case IDM_SEARCH_VOLATILE_FINDPREV :
		{
			assert(_findReplaceDlg);
			TCHAR text2Find[MAX_PATH];
			_pEditView->getGenericSelectedText(text2Find, MAX_PATH);

			FindOption op;
			op._isWholeWord = false;
			op._whichDirection = (id == IDM_SEARCH_VOLATILE_FINDNEXT?DIR_DOWN:DIR_UP);
			_findReplaceDlg->processFindNext(text2Find, &op);
			break;
		}

		case IDM_SEARCH_MARKALLEXT1 :
		case IDM_SEARCH_MARKALLEXT2 :
		case IDM_SEARCH_MARKALLEXT3 :
		case IDM_SEARCH_MARKALLEXT4 :
		case IDM_SEARCH_MARKALLEXT5 :
		{
			assert(_findReplaceDlg);
			int styleID;
			if (id == IDM_SEARCH_MARKALLEXT1)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT1;
			else if (id == IDM_SEARCH_MARKALLEXT2)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT2;
			else if (id == IDM_SEARCH_MARKALLEXT3)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT3;
			else if (id == IDM_SEARCH_MARKALLEXT4)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT4;
			else // (id == IDM_SEARCH_MARKALLEXT5)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT5;

			const int strSize = FINDREPLACE_MAXLENGTH;
			TCHAR text2Find[strSize];
			_pEditView->getGenericSelectedText(text2Find, strSize, false);
            if (text2Find[0] == '\0')
            {
                _pEditView->getGenericWordOnCaretPos(text2Find, strSize);
            }
			_findReplaceDlg->markAll(text2Find, styleID);

			break;
		}
		case IDM_SEARCH_UNMARKALLEXT1 :
		case IDM_SEARCH_UNMARKALLEXT2 :
		case IDM_SEARCH_UNMARKALLEXT3 :
		case IDM_SEARCH_UNMARKALLEXT4 :
		case IDM_SEARCH_UNMARKALLEXT5 :
		{
			int styleID;
			if (id == IDM_SEARCH_UNMARKALLEXT1)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT1;
			else if (id == IDM_SEARCH_UNMARKALLEXT2)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT2;
			else if (id == IDM_SEARCH_UNMARKALLEXT3)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT3;
			else if (id == IDM_SEARCH_UNMARKALLEXT4)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT4;
			else // (id == IDM_SEARCH_UNMARKALLEXT5)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT5;

			_pEditView->clearIndicator(styleID);
			break;
		}

		case IDM_SEARCH_GONEXTMARKER1 :
		case IDM_SEARCH_GONEXTMARKER2 :
		case IDM_SEARCH_GONEXTMARKER3 :
		case IDM_SEARCH_GONEXTMARKER4 :
		case IDM_SEARCH_GONEXTMARKER5 :
		case IDM_SEARCH_GONEXTMARKER_DEF :
		{
			int styleID;
			if (id == IDM_SEARCH_GONEXTMARKER1)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT1;
			else if (id == IDM_SEARCH_GONEXTMARKER2)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT2;
			else if (id == IDM_SEARCH_GONEXTMARKER3)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT3;
			else if (id == IDM_SEARCH_GONEXTMARKER4)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT4;
			else if (id == IDM_SEARCH_GONEXTMARKER5)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT5;
			else // (id == IDM_SEARCH_GONEXTMARKER_DEF)
				styleID = SCE_UNIVERSAL_FOUND_STYLE;

			goToNextIndicator(styleID);

			break;
		}

		case IDM_SEARCH_GOPREVMARKER1 :
		case IDM_SEARCH_GOPREVMARKER2 :
		case IDM_SEARCH_GOPREVMARKER3 :
		case IDM_SEARCH_GOPREVMARKER4 :
		case IDM_SEARCH_GOPREVMARKER5 :
		case IDM_SEARCH_GOPREVMARKER_DEF :
		{
			int styleID;
			if (id == IDM_SEARCH_GOPREVMARKER1)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT1;
			else if (id == IDM_SEARCH_GOPREVMARKER2)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT2;
			else if (id == IDM_SEARCH_GOPREVMARKER3)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT3;
			else if (id == IDM_SEARCH_GOPREVMARKER4)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT4;
			else if (id == IDM_SEARCH_GOPREVMARKER5)
				styleID = SCE_UNIVERSAL_FOUND_STYLE_EXT5;
			else // (id == IDM_SEARCH_GOPREVMARKER_DEF)
				styleID = SCE_UNIVERSAL_FOUND_STYLE;

			goToPreviousIndicator(styleID);

			break;
		}

		case IDM_SEARCH_CLEARALLMARKS :
		{
			_pEditView->clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_EXT1);
			_pEditView->clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_EXT2);
			_pEditView->clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_EXT3);
			_pEditView->clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_EXT4);
			_pEditView->clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_EXT5);
			break;
		}

        case IDM_SEARCH_GOTOLINE :
		{
			assert(_goToLineDlg);
			bool isFirstTime = !_goToLineDlg->isCreated();
			_goToLineDlg->doDialog(_nativeLangSpeaker->isRTL());
			if (isFirstTime)
			{
				_nativeLangSpeaker->changeDlgLang(_goToLineDlg->getHSelf(), "GoToLine");
			}
			break;
		}

        case IDM_EDIT_COLUMNMODE :
		{
			assert(_colEditorDlg);
			bool isFirstTime = !_colEditorDlg->isCreated();
			_colEditorDlg->doDialog(_nativeLangSpeaker->isRTL());
			if (isFirstTime)
			{
				_nativeLangSpeaker->changeDlgLang(_colEditorDlg->getHSelf(), "ColumnEditor");
			}
			break;
		}

		case IDM_SEARCH_GOTOMATCHINGBRACE :
		{
			int braceAtCaret = -1;
			int braceOpposite = -1;
			findMatchingBracePos(braceAtCaret, braceOpposite);

			if (braceOpposite != -1)
				_pEditView->execute(SCI_GOTOPOS, braceOpposite);
			break;
		}

        case IDM_SEARCH_TOGGLE_BOOKMARK :
	        bookmarkToggle(-1);
            break;

	    case IDM_SEARCH_NEXT_BOOKMARK:
		    bookmarkNext(true);
		    break;

	    case IDM_SEARCH_PREV_BOOKMARK:
		    bookmarkNext(false);
		    break;

	    case IDM_SEARCH_CLEAR_BOOKMARKS:
			bookmarkClearAll();
		    break;

        case IDM_VIEW_USER_DLG :
        {
			assert(_toolBar);
		    bool isUDDlgVisible = false;

		    UserDefineDialog *udd = ScintillaEditView::getUserDefineDlg();

		    if (!udd->isCreated())
		    {
			    _pEditView->doUserDefineDlg(true, _nativeLangSpeaker->isRTL());
				_nativeLangSpeaker->changeUserDefineLang(udd);
				if (_isUDDocked)
					::SendMessage(udd->getHSelf(), WM_COMMAND, IDC_DOCK_BUTTON, 0);

		    }
			else
			{
				isUDDlgVisible = udd->isVisible();
				bool isUDDlgDocked = udd->isDocked();

				if ((isUDDlgDocked)&&(isUDDlgVisible))
				{
					assert(_subSplitter);
					::ShowWindow(_pMainSplitter->getHSelf(), SW_HIDE);

					if (bothActive())
						_pMainWindow = _subSplitter;
					else
						_pMainWindow = _pDocTab;

					::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);

					udd->display(false);
					_mainWindowStatus &= ~WindowUserActive;
				}
				else if ((isUDDlgDocked)&&(!isUDDlgVisible))
				{
					assert(_subSplitter);
                    if (!_pMainSplitter)
                    {
                        _pMainSplitter = new SplitterContainer;
                        _pMainSplitter->init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf());

                        Window *pWindow;
                        if (bothActive())
                            pWindow = _subSplitter;
                        else
                            pWindow = _pDocTab;

                        _pMainSplitter->create(pWindow, ScintillaEditView::getUserDefineDlg(), 8, RIGHT_FIX, 45);
                    }

					_pMainWindow = _pMainSplitter;

					_pMainSplitter->setWin0((bothActive())?(Window *)&_subSplitter:(Window *)_pDocTab);

					::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
					_pMainWindow->display();

					_mainWindowStatus |= WindowUserActive;
				}
				else if ((!isUDDlgDocked)&&(isUDDlgVisible))
				{
					udd->display(false);
				}
				else //((!isUDDlgDocked)&&(!isUDDlgVisible))
					udd->display();
			}
			checkMenuItem(IDM_VIEW_USER_DLG, !isUDDlgVisible);
			_toolBar->setCheck(IDM_VIEW_USER_DLG, !isUDDlgVisible);

            break;
        }

		case IDM_EDIT_SELECTALL:
			_pEditView->execute(SCI_SELECTALL);
			checkClipboard();
			break;

		case IDM_EDIT_INS_TAB:
			_pEditView->execute(SCI_TAB);
			break;

		case IDM_EDIT_RMV_TAB:
			_pEditView->execute(SCI_BACKTAB);
			break;

		case IDM_EDIT_DUP_LINE:
			_pEditView->execute(SCI_LINEDUPLICATE);
			break;

		case IDM_EDIT_SPLIT_LINES:
			_pEditView->execute(SCI_TARGETFROMSELECTION);
			_pEditView->execute(SCI_LINESSPLIT);
			break;

		case IDM_EDIT_JOIN_LINES:
			_pEditView->execute(SCI_TARGETFROMSELECTION);
			_pEditView->execute(SCI_LINESJOIN);
			break;

		case IDM_EDIT_LINE_UP:
			_pEditView->currentLinesUp();
			break;

		case IDM_EDIT_LINE_DOWN:
			_pEditView->currentLinesDown();
			break;

		case IDM_EDIT_UPPERCASE:
            _pEditView->convertSelectedTextToUpperCase();
			break;

		case IDM_EDIT_LOWERCASE:
            _pEditView->convertSelectedTextToLowerCase();
			break;

		case IDM_EDIT_BLOCK_COMMENT:
			doBlockComment(cm_toggle);
			break;

		case IDM_EDIT_BLOCK_COMMENT_SET:
			doBlockComment(cm_comment);
			break;

		case IDM_EDIT_BLOCK_UNCOMMENT:
			doBlockComment(cm_uncomment);
			break;

		case IDM_EDIT_STREAM_COMMENT:
			doStreamComment();
			break;

		case IDM_EDIT_TRIMTRAILING:
			doTrimTrailing();
			break;

		case IDM_EDIT_SETREADONLY:
		{
			Buffer * buf = _pEditView->getCurrentBuffer();
			buf->setUserReadOnly(!buf->getUserReadOnly());
		}
		break;

		case IDM_EDIT_CLEARREADONLY:
		{
			Buffer * buf = _pEditView->getCurrentBuffer();

			DWORD dwFileAttribs = ::GetFileAttributes(buf->getFullPathName());
			dwFileAttribs ^= FILE_ATTRIBUTE_READONLY;

			::SetFileAttributes(buf->getFullPathName(), dwFileAttribs);

			buf->setFileReadOnly(false);
		}
		break;

		case IDM_SEARCH_CUTMARKEDLINES :
			cutMarkedLines();
			break;

		case IDM_SEARCH_COPYMARKEDLINES :
			copyMarkedLines();
			break;

		case IDM_SEARCH_PASTEMARKEDLINES :
			pasteToMarkedLines();
			break;

		case IDM_SEARCH_DELETEMARKEDLINES :
			deleteMarkedLines();
			break;

		case IDM_SEARCH_INVERSEMARKS :
			inverseMarks();
			break;

		case IDM_VIEW_FULLSCREENTOGGLE :
			fullScreenToggle();
			break;

	    case IDM_VIEW_ALWAYSONTOP:
		{
			int check = (::GetMenuState(_mainMenuHandle, id, MF_BYCOMMAND) == MF_CHECKED)?MF_UNCHECKED:MF_CHECKED;
			::CheckMenuItem(_mainMenuHandle, id, MF_BYCOMMAND | check);
			SetWindowPos(_pPublicInterface->getHSelf(), check == MF_CHECKED?HWND_TOPMOST:HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}
		break;


		case IDM_VIEW_FOLD_CURRENT :
		case IDM_VIEW_UNFOLD_CURRENT :
			_pEditView->foldCurrentPos((id==IDM_VIEW_FOLD_CURRENT)?fold_collapse:fold_uncollapse);
			break;

		case IDM_VIEW_TOGGLE_FOLDALL:
		case IDM_VIEW_TOGGLE_UNFOLDALL:
		{
			_pEditView->foldAll((id==IDM_VIEW_TOGGLE_FOLDALL)?fold_collapse:fold_uncollapse);
		}
		break;

		case IDM_VIEW_FOLD_1:
		case IDM_VIEW_FOLD_2:
		case IDM_VIEW_FOLD_3:
		case IDM_VIEW_FOLD_4:
		case IDM_VIEW_FOLD_5:
		case IDM_VIEW_FOLD_6:
		case IDM_VIEW_FOLD_7:
		case IDM_VIEW_FOLD_8:
			_pEditView->collapse(id - IDM_VIEW_FOLD - 1, fold_collapse);
			break;

		case IDM_VIEW_UNFOLD_1:
		case IDM_VIEW_UNFOLD_2:
		case IDM_VIEW_UNFOLD_3:
		case IDM_VIEW_UNFOLD_4:
		case IDM_VIEW_UNFOLD_5:
		case IDM_VIEW_UNFOLD_6:
		case IDM_VIEW_UNFOLD_7:
		case IDM_VIEW_UNFOLD_8:
			_pEditView->collapse(id - IDM_VIEW_UNFOLD - 1, fold_uncollapse);
			break;


		case IDM_VIEW_TOOLBAR_REDUCE:
		{
			assert(_toolBar);
            toolBarStatusType state = _toolBar->getState();

            if (state != TB_SMALL)
            {
			    _toolBar->reduce();
			    changeToolBarIcons();
            }
		}
		break;

		case IDM_VIEW_TOOLBAR_ENLARGE:
		{
			assert(_toolBar);
            toolBarStatusType state = _toolBar->getState();

            if (state != TB_LARGE)
            {
			    _toolBar->enlarge();
			    changeToolBarIcons();
            }
		}
		break;

		case IDM_VIEW_TOOLBAR_STANDARD:
		{
			assert(_toolBar);
			toolBarStatusType state = _toolBar->getState();

            if (state != TB_STANDARD)
            {
				_toolBar->setToUglyIcons();
			}
		}
		break;

		case IDM_VIEW_REDUCETABBAR :
		{
			assert(_docTabIconList);
			assert(_mainDocTab);
			assert(_subDocTab);

			_toReduceTabBar = !_toReduceTabBar;

			//Resize the  icon
			int iconSize = _toReduceTabBar?12:18;

			//Resize the tab height
			int tabHeight = _toReduceTabBar?20:25;
			TabCtrl_SetItemSize(_mainDocTab->getHSelf(), 45, tabHeight);
			TabCtrl_SetItemSize(_subDocTab->getHSelf(), 45, tabHeight);
			_docTabIconList->setIconSize(iconSize);

			//change the font
			int stockedFont = _toReduceTabBar?DEFAULT_GUI_FONT:SYSTEM_FONT;
			HFONT hf = (HFONT)::GetStockObject(stockedFont);

			if (hf)
			{
				::SendMessage(_mainDocTab->getHSelf(), WM_SETFONT, (WPARAM)hf, MAKELPARAM(TRUE, 0));
				::SendMessage(_subDocTab->getHSelf(), WM_SETFONT, (WPARAM)hf, MAKELPARAM(TRUE, 0));
			}

			::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
			break;
		}

		case IDM_VIEW_REFRESHTABAR :
		{
			::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
			break;
		}
        case IDM_VIEW_LOCKTABBAR:
		{
			bool isDrag = TabBarPlus::doDragNDropOrNot();
            TabBarPlus::doDragNDrop(!isDrag);
			//checkMenuItem(IDM_VIEW_LOCKTABBAR, isDrag);
            break;
		}


		case IDM_VIEW_DRAWTABBAR_INACIVETAB:
		{
			TabBarPlus::setDrawInactiveTab(!TabBarPlus::drawInactiveTab());
			break;
		}
		case IDM_VIEW_DRAWTABBAR_TOPBAR:
		{
			TabBarPlus::setDrawTopBar(!TabBarPlus::drawTopBar());
			break;
		}

		case IDM_VIEW_DRAWTABBAR_CLOSEBOTTUN :
		{
			assert(_mainDocTab);
			assert(_subDocTab);
			TabBarPlus::setDrawTabCloseButton(!TabBarPlus::drawTabCloseButton());

			// This part is just for updating (redraw) the tabs
			{
				int tabHeight = TabBarPlus::drawTabCloseButton()?21:20;
				TabCtrl_SetItemSize(_mainDocTab->getHSelf(), 45, tabHeight);
				TabCtrl_SetItemSize(_subDocTab->getHSelf(), 45, tabHeight);
			}
			::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
			break;
		}

		case IDM_VIEW_DRAWTABBAR_DBCLK2CLOSE :
		{
			TabBarPlus::setDbClk2Close(!TabBarPlus::isDbClk2Close());
			break;
		}

		case IDM_VIEW_DRAWTABBAR_VERTICAL :
		{
			TabBarPlus::setVertical(!TabBarPlus::isVertical());
			::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
			break;
		}

		case IDM_VIEW_DRAWTABBAR_MULTILINE :
		{
			TabBarPlus::setMultiLine(!TabBarPlus::isMultiLine());
			::SendMessage(_pPublicInterface->getHSelf(), WM_SIZE, 0, 0);
			break;
		}

		case IDM_VIEW_POSTIT :
		{
			postItToggle();
		}
		break;

		case IDM_VIEW_TAB_SPACE:
		{
			assert(_toolBar);
			assert(_mainEditView);
			assert(_subEditView);
			bool isChecked = !(::GetMenuState(_mainMenuHandle, IDM_VIEW_TAB_SPACE, MF_BYCOMMAND) == MF_CHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_EOL, MF_BYCOMMAND | MF_UNCHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_ALL_CHARACTERS, MF_BYCOMMAND | MF_UNCHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_TAB_SPACE, MF_BYCOMMAND | (isChecked?MF_CHECKED:MF_UNCHECKED));
			_toolBar->setCheck(IDM_VIEW_ALL_CHARACTERS, false);
			_mainEditView->showEOL(false);
			_mainEditView->showWSAndTab(isChecked);
			_subEditView->showEOL(false);
			_subEditView->showWSAndTab(isChecked);

            ScintillaViewParams & svp = (ScintillaViewParams &)(NppParameters::getInstance())->getSVP();
            svp._whiteSpaceShow = isChecked;
            svp._eolShow = false;
			break;
		}
		case IDM_VIEW_EOL:
		{
			assert(_toolBar);
			assert(_mainEditView);
			assert(_subEditView);
			bool isChecked = !(::GetMenuState(_mainMenuHandle, IDM_VIEW_EOL, MF_BYCOMMAND) == MF_CHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_TAB_SPACE, MF_BYCOMMAND | MF_UNCHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_EOL, MF_BYCOMMAND | (isChecked?MF_CHECKED:MF_UNCHECKED));
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_ALL_CHARACTERS, MF_BYCOMMAND | MF_UNCHECKED);
			_toolBar->setCheck(IDM_VIEW_ALL_CHARACTERS, false);
			_mainEditView->showEOL(isChecked);
			_subEditView->showEOL(isChecked);
			_mainEditView->showWSAndTab(false);
			_subEditView->showWSAndTab(false);

            ScintillaViewParams & svp = (ScintillaViewParams &)(NppParameters::getInstance())->getSVP();
            svp._whiteSpaceShow = false;
            svp._eolShow = isChecked;
			break;
		}
		case IDM_VIEW_ALL_CHARACTERS:
		{
			assert(_toolBar);
			assert(_mainEditView);
			assert(_subEditView);
			bool isChecked = !(::GetMenuState(_mainMenuHandle, id, MF_BYCOMMAND) == MF_CHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_EOL, MF_BYCOMMAND | MF_UNCHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_TAB_SPACE, MF_BYCOMMAND | MF_UNCHECKED);
			::CheckMenuItem(_mainMenuHandle, IDM_VIEW_ALL_CHARACTERS, MF_BYCOMMAND | (isChecked?MF_CHECKED:MF_UNCHECKED));
			_mainEditView->showInvisibleChars(isChecked);
			_subEditView->showInvisibleChars(isChecked);
			_toolBar->setCheck(IDM_VIEW_ALL_CHARACTERS, isChecked);

            ScintillaViewParams & svp = (ScintillaViewParams &)(NppParameters::getInstance())->getSVP();
            svp._whiteSpaceShow = isChecked;
            svp._eolShow = isChecked;
			break;
		}

		case IDM_VIEW_INDENT_GUIDE:
		{
			assert(_toolBar);
			assert(_mainEditView);
			assert(_subEditView);
			_mainEditView->showIndentGuideLine(!_pEditView->isShownIndentGuide());
			_subEditView->showIndentGuideLine(!_pEditView->isShownIndentGuide());
            _toolBar->setCheck(IDM_VIEW_INDENT_GUIDE, _pEditView->isShownIndentGuide());
			checkMenuItem(IDM_VIEW_INDENT_GUIDE, _pEditView->isShownIndentGuide());

            ScintillaViewParams & svp = (ScintillaViewParams &)(NppParameters::getInstance())->getSVP();
            svp._indentGuideLineShow = _pEditView->isShownIndentGuide();
			break;
		}

		case IDM_VIEW_WRAP:
		{
			assert(_toolBar);
			assert(_mainEditView);
			assert(_subEditView);
			bool isWraped = !_pEditView->isWrap();
			_mainEditView->wrap(isWraped);
			_subEditView->wrap(isWraped);
            _toolBar->setCheck(IDM_VIEW_WRAP, isWraped);
			checkMenuItem(IDM_VIEW_WRAP, isWraped);

            ScintillaViewParams & svp = (ScintillaViewParams &)(NppParameters::getInstance())->getSVP();
            svp._doWrap = isWraped;
			break;
		}
		case IDM_VIEW_WRAP_SYMBOL:
		{
			assert(_mainEditView);
			assert(_subEditView);
			_mainEditView->showWrapSymbol(!_pEditView->isWrapSymbolVisible());
			_subEditView->showWrapSymbol(!_pEditView->isWrapSymbolVisible());
			checkMenuItem(IDM_VIEW_WRAP_SYMBOL, _pEditView->isWrapSymbolVisible());

            ScintillaViewParams & svp = (ScintillaViewParams &)(NppParameters::getInstance())->getSVP();
            svp._wrapSymbolShow = _pEditView->isWrapSymbolVisible();
			break;
		}

		case IDM_VIEW_HIDELINES:
		{
			_pEditView->hideLines();
			break;
		}

		case IDM_VIEW_ZOOMIN:
		{
			_pEditView->execute(SCI_ZOOMIN);
			break;
		}
		case IDM_VIEW_ZOOMOUT:
			_pEditView->execute(SCI_ZOOMOUT);
			break;

		case IDM_VIEW_ZOOMRESTORE:
			//Zoom factor of 0 points means default view
			_pEditView->execute(SCI_SETZOOM, 0);//_zoomOriginalValue);
			break;

		case IDM_VIEW_SYNSCROLLV:
		{
			assert(_toolBar);
			_syncInfo._isSynScollV = !_syncInfo._isSynScollV;
			checkMenuItem(IDM_VIEW_SYNSCROLLV, _syncInfo._isSynScollV);
			_toolBar->setCheck(IDM_VIEW_SYNSCROLLV, _syncInfo._isSynScollV);

			if (_syncInfo._isSynScollV)
			{
				int mainCurrentLine = _mainEditView->execute(SCI_GETFIRSTVISIBLELINE);
				int subCurrentLine = _subEditView->execute(SCI_GETFIRSTVISIBLELINE);
				_syncInfo._line = mainCurrentLine - subCurrentLine;
			}

		}
		break;

		case IDM_VIEW_SYNSCROLLH:
		{
			assert(_toolBar);
			_syncInfo._isSynScollH = !_syncInfo._isSynScollH;
			checkMenuItem(IDM_VIEW_SYNSCROLLH, _syncInfo._isSynScollH);
			_toolBar->setCheck(IDM_VIEW_SYNSCROLLH, _syncInfo._isSynScollH);

			if (_syncInfo._isSynScollH)
			{
				int mxoffset = _mainEditView->execute(SCI_GETXOFFSET);
				int pixel = int(_mainEditView->execute(SCI_TEXTWIDTH, STYLE_DEFAULT, (LPARAM)"P"));
				int mainColumn = mxoffset/pixel;

				int sxoffset = _subEditView->execute(SCI_GETXOFFSET);
				pixel = int(_subEditView->execute(SCI_TEXTWIDTH, STYLE_DEFAULT, (LPARAM)"P"));
				int subColumn = sxoffset/pixel;
				_syncInfo._column = mainColumn - subColumn;
			}
		}
		break;

		case IDM_VIEW_SUMMARY:
		{
			generic_stringstream strStream;

			Buffer * curBuf = _pEditView->getCurrentBuffer();
			int fileLen = curBuf->getFileLength();

			if (fileLen != -1)
			{
				strStream << TEXT("Full file path: ") << curBuf->getFullPathName() << std::endl
						  << TEXT("Created: ") << curBuf->getFileTime(Buffer::ft_created) << std::endl
						  << TEXT("Modified: ") << curBuf->getFileTime(Buffer::ft_modified) << std::endl
						  << TEXT("File length (in byte): ") << fileLen << std::endl
						  << std::endl;
			}

			UniMode um = _pEditView->getCurrentBuffer()->getUnicodeMode();

			strStream << TEXT("Characters (without blanks): ") << getCurrentDocCharCount(um) << std::endl
					  << TEXT("Words: ") << wordCount() << std::endl
					  << TEXT("Lines: ") << _pEditView->execute(SCI_GETLINECOUNT) << std::endl
					  << TEXT("Current document length: ") << _pEditView->execute(SCI_GETLENGTH) << std::endl
					  << getSelectedCharNumber(um) << TEXT(" selected characters (")
					  << getSelectedBytes() << TEXT(" bytes) in ")
					  << getSelectedAreas() <<TEXT(" ranges") << std::endl;

			::MessageBox(_pPublicInterface->getHSelf(), strStream.str().c_str(), TEXT("Summary"), MB_OK|MB_APPLMODAL);
		}
		break;

		case IDM_EXECUTE:
		{
			assert(_runDlg);
			bool isFirstTime = !_runDlg->isCreated();
			_runDlg->doDialog(_nativeLangSpeaker->isRTL());
			if (isFirstTime)
			{
				_nativeLangSpeaker->changeDlgLang(_runDlg->getHSelf(), "Run");
			}

			break;
		}

		case IDM_FORMAT_TODOS :
		case IDM_FORMAT_TOUNIX :
		case IDM_FORMAT_TOMAC :
		{
			Buffer * buf = _pEditView->getCurrentBuffer();

			int f = int((id == IDM_FORMAT_TODOS)?SC_EOL_CRLF:(id == IDM_FORMAT_TOUNIX)?SC_EOL_LF:SC_EOL_CR);

			buf->setFormat((formatType)f);
			_pEditView->execute(SCI_CONVERTEOLS, buf->getFormat());
			break;
		}

		case IDM_FORMAT_ANSI :
		case IDM_FORMAT_UTF_8 :
		case IDM_FORMAT_UCS_2BE :
		case IDM_FORMAT_UCS_2LE :
		case IDM_FORMAT_AS_UTF_8 :
		{
			Buffer * buf = _pEditView->getCurrentBuffer();

			UniMode um;
			bool shoulBeDirty = true;
			switch (id)
			{
				case IDM_FORMAT_AS_UTF_8:
					shoulBeDirty = buf->getUnicodeMode() != uni8Bit;
					um = uniCookie;
					break;

				case IDM_FORMAT_UTF_8:
					um = uniUTF8;
					break;

				case IDM_FORMAT_UCS_2BE:
					um = uni16BE;
					break;

				case IDM_FORMAT_UCS_2LE:
					um = uni16LE;
					break;

				default : // IDM_FORMAT_ANSI
					shoulBeDirty = buf->getUnicodeMode() != uniCookie;
					um = uni8Bit;
			}

			if (buf->getEncoding() != -1)
			{
				if (buf->isDirty())
				{
					int answer = ::MessageBox(NULL, TEXT("You should save the current modification.\rAll the saved modifications can not be undone.\r\rContinue?"), TEXT("Save Current Modification"), MB_YESNO);
					if (answer == IDYES)
					{
						fileSave();
						_pEditView->execute(SCI_EMPTYUNDOBUFFER);
					}
					else
						return;
				}

				if (_pEditView->execute(SCI_CANUNDO) == TRUE)
				{
					int answer = ::MessageBox(NULL, TEXT("All the saved modifications can not be undone.\r\rContinue?"), TEXT("Lose Undo Ability Waning"), MB_YESNO);
					if (answer == IDYES)
					{
						// Do nothing
					}
					else
						return;
				}

				buf->setEncoding(-1);

				if (um == uni8Bit)
					_pEditView->execute(SCI_SETCODEPAGE, CP_ACP);
				else
					buf->setUnicodeMode(um);
				fileReload();
			}
			else
			{
				if (buf->getUnicodeMode() != um)
				{
					buf->setUnicodeMode(um);
					if (shoulBeDirty)
						buf->setDirty(true);
				}
			}
			break;
		}

        case IDM_FORMAT_WIN_1250 :
        case IDM_FORMAT_WIN_1251 :
        case IDM_FORMAT_WIN_1252 :
        case IDM_FORMAT_WIN_1253 :
        case IDM_FORMAT_WIN_1254 :
        case IDM_FORMAT_WIN_1255 :
        case IDM_FORMAT_WIN_1256 :
        case IDM_FORMAT_WIN_1257 :
        case IDM_FORMAT_WIN_1258 :
        case IDM_FORMAT_ISO_8859_1  :
        case IDM_FORMAT_ISO_8859_2  :
        case IDM_FORMAT_ISO_8859_3  :
        case IDM_FORMAT_ISO_8859_4  :
        case IDM_FORMAT_ISO_8859_5  :
        case IDM_FORMAT_ISO_8859_6  :
        case IDM_FORMAT_ISO_8859_7  :
        case IDM_FORMAT_ISO_8859_8  :
        case IDM_FORMAT_ISO_8859_9  :
        case IDM_FORMAT_ISO_8859_10 :
        case IDM_FORMAT_ISO_8859_11 :
        case IDM_FORMAT_ISO_8859_13 :
        case IDM_FORMAT_ISO_8859_14 :
        case IDM_FORMAT_ISO_8859_15 :
        case IDM_FORMAT_ISO_8859_16 :
        case IDM_FORMAT_DOS_437 :
        case IDM_FORMAT_DOS_720 :
        case IDM_FORMAT_DOS_737 :
        case IDM_FORMAT_DOS_775 :
        case IDM_FORMAT_DOS_850 :
        case IDM_FORMAT_DOS_852 :
        case IDM_FORMAT_DOS_855 :
        case IDM_FORMAT_DOS_857 :
        case IDM_FORMAT_DOS_858 :
        case IDM_FORMAT_DOS_860 :
        case IDM_FORMAT_DOS_861 :
        case IDM_FORMAT_DOS_862 :
        case IDM_FORMAT_DOS_863 :
        case IDM_FORMAT_DOS_865 :
        case IDM_FORMAT_DOS_866 :
        case IDM_FORMAT_DOS_869 :
        case IDM_FORMAT_BIG5 :
        case IDM_FORMAT_GB2312 :
        case IDM_FORMAT_SHIFT_JIS :
        case IDM_FORMAT_KOREAN_WIN :
        case IDM_FORMAT_EUC_KR :
        case IDM_FORMAT_TIS_620 :
        case IDM_FORMAT_MAC_CYRILLIC :
        case IDM_FORMAT_KOI8U_CYRILLIC :
        case IDM_FORMAT_KOI8R_CYRILLIC :
        {
			int index = id - IDM_FORMAT_ENCODE;

			EncodingMapper *em = EncodingMapper::getInstance();
			int encoding = em->getEncodingFromIndex(index);
			if (encoding == -1)
			{
				//printStr(TEXT("Encoding problem. Command is not added in encoding_table?"));
				return;
			}

            Buffer * buf = _pEditView->getCurrentBuffer();
            if (buf->isDirty())
            {
                int answer = ::MessageBox(NULL, TEXT("You should save the current modification.\rAll the saved modifications can not be undone.\r\rContinue?"), TEXT("Save Current Modification"), MB_YESNO);
                if (answer == IDYES)
                {
                    fileSave();
					_pEditView->execute(SCI_EMPTYUNDOBUFFER);
                }
                else
                    return;
            }

            if (_pEditView->execute(SCI_CANUNDO) == TRUE)
            {
                int answer = ::MessageBox(NULL, TEXT("All the saved modifications can not be undone.\r\rContinue?"), TEXT("Lose Undo Ability Waning"), MB_YESNO);
                if (answer == IDYES)
                {
                    // Do nothing
                }
                else
                    return;
            }

            if (!buf->isDirty())
            {
				Buffer *buf = _pEditView->getCurrentBuffer();
				buf->setEncoding(encoding);
				buf->setUnicodeMode(uniCookie);
				fileReload();
            }
			break;
		}


		case IDM_FORMAT_CONV2_ANSI:
		case IDM_FORMAT_CONV2_AS_UTF_8:
		case IDM_FORMAT_CONV2_UTF_8:
		case IDM_FORMAT_CONV2_UCS_2BE:
		case IDM_FORMAT_CONV2_UCS_2LE:
		{
			int idEncoding = -1;
			Buffer *buf = _pEditView->getCurrentBuffer();
            UniMode um = buf->getUnicodeMode();
            int encoding = buf->getEncoding();

			switch(id)
			{
				case IDM_FORMAT_CONV2_ANSI:
				{
                    if (encoding != -1)
                    {
                        // do nothing
                        return;
                    }
                    else
                    {
					    if (um == uni8Bit)
						    return;

                        // set scintilla to ANSI
					    idEncoding = IDM_FORMAT_ANSI;
                    }
					break;
				}
				case IDM_FORMAT_CONV2_AS_UTF_8:
				{
                    if (encoding != -1)
                    {
                        buf->setDirty(true);
                        buf->setUnicodeMode(uniCookie);
                        buf->setEncoding(-1);
                        return;
                    }

					idEncoding = IDM_FORMAT_AS_UTF_8;
					if (um == uniCookie)
						return;

					if (um != uni8Bit)
					{
						::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, idEncoding, 0);
						_pEditView->execute(SCI_EMPTYUNDOBUFFER);
						return;
					}

					break;
				}
				case IDM_FORMAT_CONV2_UTF_8:
				{
                    if (encoding != -1)
                    {
                        buf->setDirty(true);
                        buf->setUnicodeMode(uniUTF8);
                        buf->setEncoding(-1);
                        return;
                    }

					idEncoding = IDM_FORMAT_UTF_8;
					if (um == uniUTF8)
						return;

					if (um != uni8Bit)
					{
						::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, idEncoding, 0);
						_pEditView->execute(SCI_EMPTYUNDOBUFFER);
						return;
					}
					break;
				}

				case IDM_FORMAT_CONV2_UCS_2BE:
				{
                    if (encoding != -1)
                    {
                        buf->setDirty(true);
                        buf->setUnicodeMode(uni16BE);
                        buf->setEncoding(-1);
                        return;
                    }

					idEncoding = IDM_FORMAT_UCS_2BE;
					if (um == uni16BE)
						return;

					if (um != uni8Bit)
					{
						::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, idEncoding, 0);
						_pEditView->execute(SCI_EMPTYUNDOBUFFER);
						return;
					}
					break;
				}

				case IDM_FORMAT_CONV2_UCS_2LE:
				{
                    if (encoding != -1)
                    {
                        buf->setDirty(true);
                        buf->setUnicodeMode(uni16LE);
                        buf->setEncoding(-1);
                        return;
                    }

					idEncoding = IDM_FORMAT_UCS_2LE;
					if (um == uni16LE)
						return;
					if (um != uni8Bit)
					{
						::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, idEncoding, 0);
						_pEditView->execute(SCI_EMPTYUNDOBUFFER);
						return;
					}
					break;
				}

				NO_DEFAULT_CASE;
			}

			if (idEncoding != -1)
			{
				// Save the current clipboard content
				::OpenClipboard(_pPublicInterface->getHSelf());
				HANDLE clipboardData = ::GetClipboardData(CF_TEXT);
				int len = ::GlobalSize(clipboardData);
				LPVOID clipboardDataPtr = ::GlobalLock(clipboardData);

				HANDLE allocClipboardData = ::GlobalAlloc(GMEM_MOVEABLE, len);
				LPVOID clipboardData2 = ::GlobalLock(allocClipboardData);

				::memcpy(clipboardData2, clipboardDataPtr, len);
				::GlobalUnlock(clipboardData);
				::GlobalUnlock(allocClipboardData);
				::CloseClipboard();

				_pEditView->saveCurrentPos();

				// Cut all text
				int docLen = _pEditView->getCurrentDocLen();
				_pEditView->execute(SCI_COPYRANGE, 0, docLen);
				_pEditView->execute(SCI_CLEARALL);

				// Change to the proper buffer, save buffer status

				::SendMessage(_pPublicInterface->getHSelf(), WM_COMMAND, idEncoding, 0);

				// Paste the texte, restore buffer status
				_pEditView->execute(SCI_PASTE);
				_pEditView->restoreCurrentPos();

				// Restore the previous clipboard data
				::OpenClipboard(_pPublicInterface->getHSelf());
				::EmptyClipboard();
				::SetClipboardData(CF_TEXT, clipboardData2);
				::CloseClipboard();

				//Do not free anything, EmptyClipboard does that
				_pEditView->execute(SCI_EMPTYUNDOBUFFER);
			}
			break;
		}
        /*
        case (IDM_FORMAT_WIN_1250  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1251  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1252  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1253  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1254  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1255  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1256  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1257  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_WIN_1258  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_1   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_2   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_3   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_4   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_5   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_6   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_7   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_8   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_9   + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_10  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_11  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_13  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_14  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_15  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_ISO_8859_16  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_437  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_720  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_737  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_775  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_850  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_852  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_855  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_857  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_858  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_860  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_861  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_862  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_863  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_865  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_866  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_DOS_869  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_BIG5  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_GB2312  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_SHIFT_JIS  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_KOREAN_WIN  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_EUC_KR  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_TIS_620  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_MAC_CYRILLIC  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_KOI8U_CYRILLIC  + IDM_FORMAT_CONVERT):
        case (IDM_FORMAT_KOI8R_CYRILLIC  + IDM_FORMAT_CONVERT):
        {
            int index = id - IDM_FORMAT_CONVERT - IDM_FORMAT_ENCODE;

			EncodingMapper *em = EncodingMapper::getInstance();
			int newEncoding = em->getEncodingFromIndex(index);
			if (newEncoding == -1)
			{
				return;
			}

            Buffer *buf = _pEditView->getCurrentBuffer();
            UniMode um = buf->getUnicodeMode();
            int oldEncoding = buf->getEncoding();

            if (oldEncoding == newEncoding)
                return;

            if (oldEncoding != -1)
            {
                //do waring
            }
            buf->setEncoding(newEncoding);

            break;
        }
*/
		case IDM_SETTING_TAB_REPLCESPACE:
		case IDM_SETTING_TAB_SIZE:
		{
            _pEditView->setTabSettings(_pEditView->getCurrentBuffer()->getCurrentLang());
			break;
		}

		case IDM_SETTING_AUTOCNBCHAR:
		{
			assert(_preferenceDlg);
			const int NB_MIN_CHAR = 1;
			const int NB_MAX_CHAR = 9;

			ValueDlg valDlg;
			NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
			valDlg.init(_pPublicInterface->getHinst(), _preferenceDlg->getHSelf(), nppGUI._autocFromLen, TEXT("Nb char : "));
			POINT p;
			::GetCursorPos(&p);
			::ScreenToClient(_pPublicInterface->getHParent(), &p);
			int size = valDlg.doDialog(p, _nativeLangSpeaker->isRTL());

			if (size != -1)
			{
				if (size > NB_MAX_CHAR)
					size = NB_MAX_CHAR;
				else if (size < NB_MIN_CHAR)
					size = NB_MIN_CHAR;

				nppGUI._autocFromLen = size;
			}
			break;
		}

		case IDM_SETTING_HISTORY_SIZE :
		{
			assert(_preferenceDlg);
			ValueDlg nbHistoryDlg;
			NppParameters *pNppParam = NppParameters::getInstance();
			nbHistoryDlg.init(_pPublicInterface->getHinst(), _preferenceDlg->getHSelf(), pNppParam->getNbMaxFile(), TEXT("Max File : "));
			POINT p;
			::GetCursorPos(&p);
			::ScreenToClient(_pPublicInterface->getHParent(), &p);
			int size = nbHistoryDlg.doDialog(p, _nativeLangSpeaker->isRTL());

			if (size != -1)
			{
				if (size > NB_MAX_LRF_FILE)
					size = NB_MAX_LRF_FILE;
				pNppParam->setNbMaxFile(size);
				_lastRecentFileList->setUserMaxNbLRF(size);
			}
			break;
		}

		case IDM_SETTING_IMPORTPLUGIN :
        {
            // get plugin source path
            const TCHAR *extFilterName = TEXT("Notepad++ pligin");
            const TCHAR *extFilter = TEXT(".dll");
            const TCHAR *destDir = TEXT("plugins");

            std::vector<generic_string> copiedFiles = addNppComponents(destDir, extFilterName, extFilter);

            // load plugin
            std::vector<generic_string> dll2Remove;
            for (size_t i = 0 ; i < copiedFiles.size() ; i++)
            {
                int index = _pluginsManager->loadPlugin(copiedFiles[i].c_str(), dll2Remove);
                if (_pluginsManager->getMenuHandle())
                    _pluginsManager->addInMenuFromPMIndex(index);
            }
            if (!_pluginsManager->getMenuHandle())
                _pluginsManager->setMenu(_mainMenuHandle, NULL);
            ::DrawMenuBar(_pPublicInterface->getHSelf());
            break;
        }

        case IDM_SETTING_IMPORTSTYLETHEMS :
        {
            // get plugin source path
            const TCHAR *extFilterName = TEXT("Notepad++ style theme");
            const TCHAR *extFilter = TEXT(".xml");
            const TCHAR *destDir = TEXT("themes");

            // load styler
            NppParameters *pNppParams = NppParameters::getInstance();
            ThemeSwitcher & themeSwitcher = pNppParams->getThemeSwitcher();

            std::vector<generic_string> copiedFiles = addNppComponents(destDir, extFilterName, extFilter);
            for (size_t i = 0 ; i < copiedFiles.size() ; i++)
            {
                generic_string themeName(themeSwitcher.getThemeFromXmlFileName(copiedFiles[i].c_str()));
		        if (!themeSwitcher.themeNameExists(themeName.c_str()))
		        {
			        themeSwitcher.addThemeFromXml(copiedFiles[i].c_str());
                    if (_configStyleDlg->isCreated())
                    {
                        _configStyleDlg->addLastThemeEntry();
                    }
		        }
            }
            break;
        }

		case IDM_SETTING_SHORTCUT_MAPPER :
		case IDM_SETTING_SHORTCUT_MAPPER_MACRO :
        case IDM_SETTING_SHORTCUT_MAPPER_RUN :
		{
            GridState st = id==IDM_SETTING_SHORTCUT_MAPPER_MACRO?STATE_MACRO:id==IDM_SETTING_SHORTCUT_MAPPER_RUN?STATE_USER:STATE_MENU;
			ShortcutMapper shortcutMapper;
			shortcutMapper.init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf(), st);
            _nativeLangSpeaker->changeShortcutmapperLang(&shortcutMapper);
			shortcutMapper.doDialog(_nativeLangSpeaker->isRTL());
			shortcutMapper.destroy();
			break;
		}

		case IDM_SETTING_PREFERENCE :
		{
			assert(_preferenceDlg);
			bool isFirstTime = !_preferenceDlg->isCreated();
			_preferenceDlg->doDialog(_nativeLangSpeaker->isRTL());

			if (isFirstTime)
			{
				_nativeLangSpeaker->changePrefereceDlgLang(_preferenceDlg);
			}
			break;
		}

        case IDM_VIEW_GOTO_ANOTHER_VIEW:
            docGotoAnotherEditView(TransferMove);
			checkSyncState();
            break;

        case IDM_VIEW_CLONE_TO_ANOTHER_VIEW:
            docGotoAnotherEditView(TransferClone);
			checkSyncState();
            break;

        case IDM_VIEW_GOTO_NEW_INSTANCE :
            docOpenInNewInstance(TransferMove);
            break;

        case IDM_VIEW_LOAD_IN_NEW_INSTANCE:
            docOpenInNewInstance(TransferClone);
            break;

		case IDM_VIEW_SWITCHTO_OTHER_VIEW:
		{
			int view_to_focus;
			HWND wnd = GetFocus();
			if (_pEditView->getHSelf() == wnd)
			{
				view_to_focus = otherView();
				if (!viewVisible(view_to_focus)) view_to_focus = _activeView;
			}
			else
			{
				view_to_focus = currentView();
			}
			switchEditViewTo(view_to_focus);
			break;
		}

        case IDM_ABOUT:
		{
			assert(_aboutDlg);
			bool isFirstTime = !_aboutDlg->isCreated();
			_aboutDlg->doDialog();
			if (isFirstTime && _nativeLangSpeaker->getNativeLangA())
			{
                if (_nativeLangSpeaker->getLangEncoding() == NPP_CP_BIG5)
				{
					const char *authorName = "�J���^";
					HWND hItem = ::GetDlgItem(_aboutDlg->getHSelf(), IDC_AUTHOR_NAME);
#ifdef UNICODE
					WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
					const wchar_t *authorNameW = wmc->char2wchar(authorName, NPP_CP_BIG5);
					::SetWindowText(hItem, authorNameW);
#else
					::SetWindowText(hItem, authorName);
#endif
				}
			}
			break;
		}

		case IDM_HELP :
		{
			generic_string nppHelpPath((NppParameters::getInstance())->getNppPath());

			nppHelpPath += TEXT("\\NppHelp.chm");
			if (::PathFileExists(nppHelpPath.c_str()))
				::ShellExecute(NULL, TEXT("open"), nppHelpPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
			else
			{
				generic_string msg = nppHelpPath;
				msg += TEXT("\rdoesn't exist. Please download it on Notepad++ site.");
				::MessageBox(_pPublicInterface->getHSelf(), msg.c_str(), TEXT("File does not exist"), MB_OK);
			}
		}
		break;

		case IDM_HOMESWEETHOME :
		{
			::ShellExecute(NULL, TEXT("open"), TEXT("http://notepad-plus-plus.org/"), NULL, NULL, SW_SHOWNORMAL);
			break;
		}
		case IDM_PROJECTPAGE :
		{
			::ShellExecute(NULL, TEXT("open"), TEXT("http://sourceforge.net/projects/notepad-plus/"), NULL, NULL, SW_SHOWNORMAL);
			break;
		}

		case IDM_ONLINEHELP:
		{
			::ShellExecute(NULL, TEXT("open"), TEXT("http://sourceforge.net/apps/mediawiki/notepad-plus/index.php?title=Main_Page"), NULL, NULL, SW_SHOWNORMAL);
			break;
		}

		case IDM_FORUM:
		{
			::ShellExecute(NULL, TEXT("open"), TEXT("http://sourceforge.net/forum/?group_id=95717"), NULL, NULL, SW_SHOWNORMAL);
			break;
		}

		case IDM_PLUGINSHOME:
		{
			::ShellExecute(NULL, TEXT("open"), TEXT("http://sourceforge.net/apps/mediawiki/notepad-plus/index.php?title=Plugin_Central"), NULL, NULL, SW_SHOWNORMAL);
			break;
		}

		case IDM_UPDATE_NPP :
		{
			generic_string updaterDir = (NppParameters::getInstance())->getNppPath();
			PathAppend(updaterDir ,TEXT("updater"));

			generic_string updaterFullPath = updaterDir;
			PathAppend(updaterFullPath, TEXT("gup.exe"));

			generic_string param = TEXT("-verbose -v");
			// nul char is expected in version strings.
			//lint -e840 (Info -- Use of nul character in a string literal)
			param += VERSION_VALUE;
			//lint +e840
			Process updater(updaterFullPath.c_str(), param.c_str(), updaterDir.c_str());

			updater.run();
			break;
		}

		case IDM_EDIT_AUTOCOMPLETE :
			showAutoComp();
			break;

		case IDM_EDIT_AUTOCOMPLETE_CURRENTFILE :
			autoCompFromCurrentFile();
			break;

		case IDM_EDIT_FUNCCALLTIP :
			showFunctionComp();
			break;

        case IDM_LANGSTYLE_CONFIG_DLG :
		{
			assert (_configStyleDlg);
			bool isFirstTime = !_configStyleDlg->isCreated();
			_configStyleDlg->doDialog(_nativeLangSpeaker->isRTL());
			if (isFirstTime)
			{
				_nativeLangSpeaker->changeConfigLang(_configStyleDlg->getHSelf());
			}
			break;
		}

        case IDM_LANG_C	:
        case IDM_LANG_CPP :
        case IDM_LANG_JAVA :
        case IDM_LANG_CS :
        case IDM_LANG_HTML :
        case IDM_LANG_XML :
        case IDM_LANG_JS :
        case IDM_LANG_PHP :
        case IDM_LANG_ASP :
        case IDM_LANG_CSS :
        case IDM_LANG_LUA :
        case IDM_LANG_PERL :
        case IDM_LANG_PYTHON :
        case IDM_LANG_PASCAL :
        case IDM_LANG_BATCH :
        case IDM_LANG_OBJC :
        case IDM_LANG_VB :
        case IDM_LANG_SQL :
        case IDM_LANG_ASCII :
        case IDM_LANG_TEXT :
        case IDM_LANG_RC :
        case IDM_LANG_MAKEFILE :
        case IDM_LANG_INI :
        case IDM_LANG_TEX :
        case IDM_LANG_FORTRAN :
        case IDM_LANG_BASH :
        case IDM_LANG_FLASH :
		case IDM_LANG_NSIS :
		case IDM_LANG_TCL :
		case IDM_LANG_LISP :
		case IDM_LANG_SCHEME :
		case IDM_LANG_ASM :
		case IDM_LANG_DIFF :
		case IDM_LANG_PROPS :
		case IDM_LANG_PS:
		case IDM_LANG_RUBY:
		case IDM_LANG_SMALLTALK:
		case IDM_LANG_VHDL :
        case IDM_LANG_KIX :
        case IDM_LANG_CAML :
        case IDM_LANG_ADA :
        case IDM_LANG_VERILOG :
		case IDM_LANG_MATLAB :
		case IDM_LANG_HASKELL :
        case IDM_LANG_AU3 :
		case IDM_LANG_INNO :
		case IDM_LANG_CMAKE :
		case IDM_LANG_YAML :
        case IDM_LANG_COBOL :
        case IDM_LANG_D :
        case IDM_LANG_GUI4CLI :
        case IDM_LANG_POWERSHELL :
        case IDM_LANG_R :
		case IDM_LANG_REBOL :
        case IDM_LANG_JSP :
		case IDM_LANG_RSP :
		case IDM_LANG_USER :
		{
            setLanguage(menuID2LangType(id));
		}
        break;

        case IDC_PREV_DOC :
        case IDC_NEXT_DOC :
        {
			assert(_mainDocTab);
			assert(_subDocTab);
			int nbDoc = viewVisible(MAIN_VIEW)?_mainDocTab->nbItem():0;
			nbDoc += viewVisible(SUB_VIEW)?_subDocTab->nbItem():0;

			bool doTaskList = ((NppParameters::getInstance())->getNppGUI())._doTaskList;
			if (nbDoc > 1)
			{
				bool direction = (id == IDC_NEXT_DOC)?dirDown:dirUp;

				if (!doTaskList)
				{
					activateNextDoc(direction);
				}
				else
				{
					assert(_docTabIconList);
					TaskListDlg tld;
					HIMAGELIST hImgLst = _docTabIconList->getHandle();
					tld.init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf(), hImgLst, direction);
					tld.doDialog();
				}
			}
			_linkTriggered = true;
		}
        break;

		case IDM_OPEN_ALL_RECENT_FILE : {
			BufferID lastOne = BUFFER_INVALID;
			int size = _lastRecentFileList->getSize();
			for (int i = size - 1; i >= 0; i--)
			{
				BufferID test = doOpen(_lastRecentFileList->getIndex(i).c_str());
				if (test != BUFFER_INVALID)
					lastOne = test;
			}
			if (lastOne != BUFFER_INVALID) {
				switchToFile(lastOne);
			}
			break; }

		case IDM_CLEAN_RECENT_FILE_LIST :
			_lastRecentFileList->clear();
			break;

		case IDM_EDIT_RTL :
		case IDM_EDIT_LTR :
		{
			long exStyle = ::GetWindowLongPtr(_pEditView->getHSelf(), GWL_EXSTYLE);
			exStyle = (id == IDM_EDIT_RTL)?exStyle|WS_EX_LAYOUTRTL:exStyle&(~WS_EX_LAYOUTRTL);
			::SetWindowLongPtr(_pEditView->getHSelf(), GWL_EXSTYLE, exStyle);
			_pEditView->redraw();
		}
		break;

		case IDM_WINDOW_WINDOWS :
		{
			WindowsDlg _windowsDlg;
			_windowsDlg.init(_pPublicInterface->getHinst(), _pPublicInterface->getHSelf(), _pDocTab);

            const TiXmlNodeA *nativeLangA = _nativeLangSpeaker->getNativeLangA();
			TiXmlNodeA *dlgNode = NULL;
			if (nativeLangA)
			{
				dlgNode = nativeLangA->FirstChild("Dialog");
				if (dlgNode)
					dlgNode = _nativeLangSpeaker->searchDlgNode(dlgNode, "Window");
			}
			_windowsDlg.doDialog(dlgNode);
		}
		break;

		case IDM_SETTING_MENU_WHEEL:
		{
			EnableMouseWheelZoom(NppParameters::getInstance()->getNppGUI()._enableMouseWheelZoom);
		}
		break;

		case IDM_SYSTRAYPOPUP_NEWDOC:
		{
			NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
			::ShowWindow(_pPublicInterface->getHSelf(), nppGUI._isMaximized?SW_MAXIMIZE:SW_SHOW);
			fileNew();
		}
		break;

		case IDM_SYSTRAYPOPUP_ACTIVATE :
		{
			NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
			::ShowWindow(_pPublicInterface->getHSelf(), nppGUI._isMaximized?SW_MAXIMIZE:SW_SHOW);
		}
		break;

		case IDM_SYSTRAYPOPUP_NEW_AND_PASTE:
		{
			NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
			::ShowWindow(_pPublicInterface->getHSelf(), nppGUI._isMaximized?SW_MAXIMIZE:SW_SHOW);
			BufferID bufferID = _pEditView->getCurrentBufferID();
			Buffer * buf = MainFileManager->getBufferByID(bufferID);
			if (!buf->isUntitled() || buf->docLength() != 0)
			{
				fileNew();
			}
			command(IDM_EDIT_PASTE);
		}
		break;

		case IDM_SYSTRAYPOPUP_OPENFILE:
		{
			NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
			::ShowWindow(_pPublicInterface->getHSelf(), nppGUI._isMaximized?SW_MAXIMIZE:SW_SHOW);
			fileOpen();
		}
		break;

		case IDM_SYSTRAYPOPUP_CLOSE:
		{
			_pPublicInterface->setIsPrelaunch(false);
			_pTrayIco->doTrayIcon(TRAYICON_REMOVE);
			if (!::IsWindowVisible(_pPublicInterface->getHSelf()))
				::SendMessage(_pPublicInterface->getHSelf(), WM_CLOSE, 0,0);
		}
		break;

		default :
			if (id > IDM_FILEMENU_LASTONE && id < (IDM_FILEMENU_LASTONE + _lastRecentFileList->getMaxNbLRF() + 1))
			{
				BufferID lastOpened = doOpen(_lastRecentFileList->getItem(id).c_str());
				if (lastOpened != BUFFER_INVALID) {
					switchToFile(lastOpened);
				}
			}
			else if ((id > IDM_LANG_USER) && (id < IDM_LANG_USER_LIMIT))
			{
				TCHAR langName[langNameLenMax];
				::GetMenuString(_mainMenuHandle, id, langName, langNameLenMax, MF_BYCOMMAND);
				_pEditView->getCurrentBuffer()->setLangType(L_USER, langName);
			}
			else if ((id >= IDM_LANG_EXTERNAL) && (id <= IDM_LANG_EXTERNAL_LIMIT))
			{
				setLanguage((LangType)(id - IDM_LANG_EXTERNAL + L_EXTERNAL));
			}
			else if ((id >= ID_MACRO) && (id < ID_MACRO_LIMIT))
			{
				int i = id - ID_MACRO;
				std::vector<MacroShortcut> & theMacros = (NppParameters::getInstance())->getMacroList();
				macroPlayback(theMacros[i].getMacro());
			}
			else if ((id >= ID_USER_CMD) && (id < ID_USER_CMD_LIMIT))
			{
				int i = id - ID_USER_CMD;
				std::vector<UserCommand> & theUserCommands = (NppParameters::getInstance())->getUserCommandList();
				UserCommand ucmd = theUserCommands[i];

				Command cmd(ucmd.getCmd());
				cmd.run(_pPublicInterface->getHSelf());
			}
			else if ((id >= ID_PLUGINS_CMD) && (id < ID_PLUGINS_CMD_LIMIT))
			{
				int i = id - ID_PLUGINS_CMD;
				_pluginsManager->runPluginCommand(i);
			}
			else if (_pluginsManager->inDynamicRange(id)) // in the dynamic range allocated with NPPM_ALLOCATECMDID
			{
				_pluginsManager->relayNppMessages(WM_COMMAND, id, 0);
			}
/*UNLOAD
			else if ((id >= ID_PLUGINS_REMOVING) && (id < ID_PLUGINS_REMOVING_END))
			{
				int i = id - ID_PLUGINS_REMOVING;
				_pluginsManager->unloadPlugin(i, _pPublicInterface->getHSelf());
			}
*/
			else if ((id >= IDM_WINDOW_MRU_FIRST) && (id <= IDM_WINDOW_MRU_LIMIT))
			{
				activateDoc(id-IDM_WINDOW_MRU_FIRST);
			}
	}

	if (_recordingMacro)
		switch (id)
		{
			case IDM_FILE_NEW :
			case IDM_FILE_CLOSE :
			case IDM_FILE_CLOSEALL :
			case IDM_FILE_CLOSEALL_BUT_CURRENT :
			case IDM_FILE_SAVE :
			case IDM_FILE_SAVEALL :
			case IDM_FILE_RELOAD:
			case IDM_EDIT_UNDO:
			case IDM_EDIT_REDO:
			case IDM_EDIT_CUT:
			case IDM_EDIT_COPY:
			//case IDM_EDIT_PASTE:
			case IDM_EDIT_DELETE:
			case IDM_SEARCH_FINDNEXT :
			case IDM_SEARCH_FINDPREV :
            case IDM_SEARCH_SETANDFINDNEXT :
			case IDM_SEARCH_SETANDFINDPREV :
			case IDM_SEARCH_GOTOMATCHINGBRACE :
			case IDM_SEARCH_TOGGLE_BOOKMARK :
			case IDM_SEARCH_NEXT_BOOKMARK:
			case IDM_SEARCH_PREV_BOOKMARK:
			case IDM_SEARCH_CLEAR_BOOKMARKS:
			case IDM_EDIT_SELECTALL:
			case IDM_EDIT_INS_TAB:
			case IDM_EDIT_RMV_TAB:
			case IDM_EDIT_DUP_LINE:
			case IDM_EDIT_TRANSPOSE_LINE:
			case IDM_EDIT_SPLIT_LINES:
			case IDM_EDIT_JOIN_LINES:
			case IDM_EDIT_LINE_UP:
			case IDM_EDIT_LINE_DOWN:
			case IDM_EDIT_UPPERCASE:
			case IDM_EDIT_LOWERCASE:
			case IDM_EDIT_BLOCK_COMMENT:
			case IDM_EDIT_BLOCK_COMMENT_SET:
			case IDM_EDIT_BLOCK_UNCOMMENT:
			case IDM_EDIT_STREAM_COMMENT:
			case IDM_EDIT_TRIMTRAILING:
			case IDM_EDIT_SETREADONLY :
			case IDM_EDIT_FULLPATHTOCLIP :
			case IDM_EDIT_FILENAMETOCLIP :
			case IDM_EDIT_CURRENTDIRTOCLIP :
			case IDM_EDIT_CLEARREADONLY :
			case IDM_EDIT_RTL :
			case IDM_EDIT_LTR :
			case IDM_VIEW_FULLSCREENTOGGLE :
			case IDM_VIEW_ALWAYSONTOP :
			case IDM_VIEW_WRAP :
			case IDM_VIEW_FOLD_CURRENT :
			case IDM_VIEW_UNFOLD_CURRENT :
			case IDM_VIEW_TOGGLE_FOLDALL:
			case IDM_VIEW_TOGGLE_UNFOLDALL:
			case IDM_VIEW_FOLD_1:
			case IDM_VIEW_FOLD_2:
			case IDM_VIEW_FOLD_3:
			case IDM_VIEW_FOLD_4:
			case IDM_VIEW_FOLD_5:
			case IDM_VIEW_FOLD_6:
			case IDM_VIEW_FOLD_7:
			case IDM_VIEW_FOLD_8:
			case IDM_VIEW_UNFOLD_1:
			case IDM_VIEW_UNFOLD_2:
			case IDM_VIEW_UNFOLD_3:
			case IDM_VIEW_UNFOLD_4:
			case IDM_VIEW_UNFOLD_5:
			case IDM_VIEW_UNFOLD_6:
			case IDM_VIEW_UNFOLD_7:
			case IDM_VIEW_UNFOLD_8:
			case IDM_VIEW_GOTO_ANOTHER_VIEW:
			case IDM_VIEW_SYNSCROLLV:
			case IDM_VIEW_SYNSCROLLH:
			case IDC_PREV_DOC :
			case IDC_NEXT_DOC :
			case IDM_SEARCH_GOPREVMARKER1   :
			case IDM_SEARCH_GOPREVMARKER2   :
			case IDM_SEARCH_GOPREVMARKER3   :
			case IDM_SEARCH_GOPREVMARKER4   :
			case IDM_SEARCH_GOPREVMARKER5   :
			case IDM_SEARCH_GOPREVMARKER_DEF:
			case IDM_SEARCH_GONEXTMARKER1   :
			case IDM_SEARCH_GONEXTMARKER2   :
			case IDM_SEARCH_GONEXTMARKER3   :
			case IDM_SEARCH_GONEXTMARKER4   :
			case IDM_SEARCH_GONEXTMARKER5   :
			case IDM_SEARCH_GONEXTMARKER_DEF:
			case IDM_SEARCH_VOLATILE_FINDNEXT:
			case IDM_SEARCH_VOLATILE_FINDPREV:
			case IDM_SEARCH_CUTMARKEDLINES   :
			case IDM_SEARCH_COPYMARKEDLINES  :
			case IDM_SEARCH_PASTEMARKEDLINES :
			case IDM_SEARCH_DELETEMARKEDLINES:
			case IDM_SEARCH_MARKALLEXT1      :
			case IDM_SEARCH_UNMARKALLEXT1    :
			case IDM_SEARCH_MARKALLEXT2      :
			case IDM_SEARCH_UNMARKALLEXT2    :
			case IDM_SEARCH_MARKALLEXT3      :
			case IDM_SEARCH_UNMARKALLEXT3    :
			case IDM_SEARCH_MARKALLEXT4      :
			case IDM_SEARCH_UNMARKALLEXT4    :
			case IDM_SEARCH_MARKALLEXT5      :
			case IDM_SEARCH_UNMARKALLEXT5    :
			case IDM_SEARCH_CLEARALLMARKS    :
				_macro.push_back(recordedMacroStep(id));
				break;

			NO_DEFAULT_CASE;

		}

}
