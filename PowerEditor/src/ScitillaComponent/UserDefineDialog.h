/*
this file is part of Notepad++
Copyright (C)2003 Don HO <donho@altern.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef USER_DEFINE_H
#define USER_DEFINE_H

#ifndef STATIC_DIALOG_H
#include "StaticDialog.h"
#endif

#ifndef CONTROLS_TAB_H
#include "ControlsTab.h"
#endif

class ScintillaEditView;
class UserLangContainer;
class ColourPicker;
struct Style;

#define DOCK true
#define UNDOCK false

#define maxNbGroup 10

class SharedParametersDialog : public StaticDialog
{
public:
	SharedParametersDialog() {};
	SharedParametersDialog(int nbGroup) : _nbGroup(nbGroup) {};
	virtual void updateDlg() = 0;


protected :
	//Shared data
	static UserLangContainer *_pUserLang;
	static ScintillaEditView *_pScintilla;

	//data for per object
	int _nbGroup;
	ColourPicker *_pFgColour[maxNbGroup];
    ColourPicker *_pBgColour[maxNbGroup];
	int _fgStatic[maxNbGroup];
	int _bgStatic[maxNbGroup];
	int _fontSizeCombo[maxNbGroup];
	int _fontNameCombo[maxNbGroup];

    BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    void initControls();
	void styleUpdate(const Style & style, ColourPicker *pFgColourPicker, ColourPicker *pBgColourPicker,
					 int fontComboId, int fontSizeComboId, int boldCheckId, int italicCheckId, int underlineCheckId);

	bool setPropertyByCheck(HWND hwnd, WPARAM id, bool & bool2set);
	virtual void setKeywords2List(int ctrlID) = 0;
	virtual int getGroupIndexFromCombo(int ctrlID, bool & isFontSize) const = 0;
	virtual int getStylerIndexFromCP(HWND hWnd, bool & isFG, ColourPicker **ppCP) const = 0;
	virtual int getGroupeIndexFromCheck(int ctrlID, int & fontStyleMask) const = 0;
};

class FolderStyleDialog : public SharedParametersDialog
{
public:
	FolderStyleDialog();
	void updateDlg();
protected :
	void setKeywords2List(int ctrlID);

	int getGroupIndexFromCombo(int ctrlID, bool & isFontSize) const;
	int getStylerIndexFromCP(HWND hWnd, bool & isFG, ColourPicker **ppCP) const;
	int getGroupeIndexFromCheck(int ctrlID, int & fontStyleMask) const;
};

class KeyWordsStyleDialog : public SharedParametersDialog
{
public:
	KeyWordsStyleDialog() ;
	void updateDlg();

protected :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
	void setKeywords2List(int id);

    // SEE @REF #01
    int getGroupIndexFromCombo(int ctrlID, bool & isFontSize)  const;
    int getStylerIndexFromCP(HWND hWnd, bool & isFG, ColourPicker **ppCP) const;
    int getGroupeIndexFromCheck(int ctrlID, int & fontStyleMask) const;
};

class CommentStyleDialog : public SharedParametersDialog
{
public :
    CommentStyleDialog();
    void updateDlg();
protected :

	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);

    void setKeywords2List(int id);
    int getGroupIndexFromCombo(int ctrlID, bool & isFontSize) const;

    int getStylerIndexFromCP(HWND hWnd, bool & isFG, ColourPicker **ppCP) const;
    int getGroupeIndexFromCheck(int ctrlID, int & fontStyleMask) const;

private :
    void convertTo(TCHAR *dest, const TCHAR *toConvert, TCHAR prefix) const;
	void retrieve(TCHAR *dest, const TCHAR *toRetrieve, TCHAR prefix) const;
};

class SymbolsStyleDialog : public SharedParametersDialog
{
public :
	static const bool ADD;
	static const bool REMOVE;
	SymbolsStyleDialog();
	void updateDlg();
protected :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
    void setKeywords2List(int /*ctrlID*/) {}
    int getGroupIndexFromCombo(int ctrlID, bool & isFontSize) const;
    int getStylerIndexFromCP(HWND hWnd, bool & isFG, ColourPicker **ppCP) const;
    int getGroupeIndexFromCheck(int ctrlID, int & fontStyleMask) const;

private :
	// 2 static const TCHAR * to have the compatibility with the old xml
	static const TCHAR *_delimTag1;
	static const TCHAR *_delimTag2;

	void symbolAction(bool action);
	void listboxsRemoveAll();
	void listboxsInit();
	void listboxsReInit() {
		listboxsRemoveAll();
		listboxsInit();
	};
};

class UserDefineDialog : public SharedParametersDialog
{
friend class ScintillaEditView;
public :
	UserDefineDialog();
	~UserDefineDialog();

	void init(HINSTANCE hInst, HWND hPere, ScintillaEditView *pSev) {
		if (!_pScintilla)
		{
			Window::init(hInst, hPere);
			_pScintilla = pSev;
		}
	};

	void setScintilla(ScintillaEditView *pScinView) {
		_pScintilla = pScinView;
	};

	virtual void create(int dialogID, bool isRTL = false) {
		StaticDialog::create(dialogID, isRTL);
	}

	void destroy() {
		// JOCE destroy all children...
	};

	int getWidth() const {
		return _dlgPos.right;
	};

	int getHeight() const {
		return _dlgPos.bottom;
	};
	void doDialog(bool willBeShown = true, bool isRTL = false);

	virtual void reSizeTo(RECT & rc) // should NEVER be const !!!
	{
		Window::reSizeTo(rc);
		display(false);
		display();
	};

	void changeStyle();
    bool isDocked() const {return _status == DOCK;};
	void setDockStatus(bool isDocked) {_status = isDocked;};

	int getNbKeywordList();
	bool isDirty() const {return _isDirty;};
	HWND getFolderHandle() const {
		return _folderStyleDlg.getHSelf();
	};

	HWND getKeywordsHandle() const {
		return _keyWordsStyleDlg.getHSelf();
	};

	HWND getCommentHandle() const {
		return _commentStyleDlg.getHSelf();
	};

	HWND getSymbolHandle() const {
		return _symbolsStyleDlg.getHSelf();
	};

	void setTabName(int index, const TCHAR *name2set) {
		_ctrlTab.renameTab(index, name2set);
	};
protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
	ControlsTab _ctrlTab;
	WindowVector _wVector;
	UserLangContainer *_pCurrentUserLang;

	FolderStyleDialog		_folderStyleDlg;
	KeyWordsStyleDialog		_keyWordsStyleDlg;
	CommentStyleDialog		_commentStyleDlg;
	SymbolsStyleDialog		_symbolsStyleDlg;

	bool _status;
    RECT _dlgPos;
	int _currentHight;
	int _yScrollPos;
    int _prevHightVal;

	bool _isDirty;
	void getActualPosSize() {
        ::GetWindowRect(_hSelf, &_dlgPos);
        _dlgPos.right -= _dlgPos.left;
        _dlgPos.bottom -= _dlgPos.top;
    };
    void restorePosSize(){reSizeTo(_dlgPos);};
	void enableLangAndControlsBy(int index);

protected :
	void setKeywords2List(int /*ctrlID*/){}
    int getGroupIndexFromCombo(int /*ctrlID*/, bool & /*isFontSize*/) const {return -1;};
    int getStylerIndexFromCP(HWND /*hWnd*/, bool & /*isFG*/, ColourPicker ** /*ppCP*/) const {return -1;};
    int getGroupeIndexFromCheck(int /*ctrlID*/, int & /*fontStyleMask*/) const {return -1;};
	void updateDlg();
};

#endif //USER_DEFINE_H
