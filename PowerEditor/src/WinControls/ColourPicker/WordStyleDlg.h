/*
this file is part of notepad++
Copyright (C)2003 Don HO < donho@altern.org >

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

#ifndef WORD_STYLE_H
#define WORD_STYLE_H

#ifndef STATIC_DIALOG_H
#include "StaticDialog.h"
#endif

#ifndef NPP_STYLES
#include "npp_styles.h"
#endif

#ifndef RESOURCE_H
#include "resource.h"
#endif

#define WM_UPDATESCINTILLAS			(WORDSTYLE_USER + 1) //GlobalStyleDlg's msg 2 send 2 its parent

enum fontStyleType {BOLD_STATUS, ITALIC_STATUS, UNDERLINE_STATUS};

#define C_FOREGROUND false
#define C_BACKGROUND true

// Forward declarations
class ColourStaticTextHooker;
class ColourPicker;

class WordStyleDlg : public StaticDialog
{
public :
	WordStyleDlg();
	virtual ~WordStyleDlg();

    void init(HINSTANCE hInst, HWND parent)	{
        Window::init(hInst, parent);
	};

	virtual void create(int dialogID, bool isRTL = false);

    void doDialog(bool isRTL = false);
	virtual void redraw() const;
	void prepare2Cancel();
	void restoreGlobalOverrideValues();

	void apply();

private :
    ColourPicker *_pFgColour;
    ColourPicker *_pBgColour;

    int _currentLexerIndex;
	int _currentThemeIndex;

    HWND _hCheckBold;
    HWND _hCheckItalic;
	HWND _hCheckUnderline;
    HWND _hFontNameCombo;
    HWND _hFontSizeCombo;
	HWND _hSwitch2ThemeCombo;

	HWND _hFgColourStaticText;
	HWND _hBgColourStaticText;
	HWND _hFontNameStaticText;
	HWND _hFontSizeStaticText;
	HWND _hStyleInfoStaticText;
	//TCHAR _originalWarning[256];

	LexerStylerArray _lsArray;
    StyleArray _globalStyles;
	std::generic_string _themeName;

	LexerStylerArray _styles2restored;
	StyleArray _gstyles2restored;
	GlobalOverride _gOverride2restored;
	bool _restoreInvalid;

	ColourStaticTextHooker* colourHooker;

	bool _isDirty;
	bool _isThemeDirty;
    //bool _isSync;
	bool _isShownGOCtrls;

	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);


	Style & getCurrentStyler();
	int whichTabColourIndex();

	void updateColour(bool which);
	void updateFontStyleStatus(fontStyleType whitchStyle);
	void updateExtension();
	void updateFontName();
	void updateFontSize();
	void updateUserKeywords();
	void switchToTheme();
	void updateThemeName(std::generic_string themeName);

	void loadLangListFromNppParam();

	void enableFg(bool isEnable);
	void enableBg(bool isEnable);
	void enableFontName(bool isEnable);
	void enableFontSize(bool isEnable);
	void enableFontStyle(bool isEnable);
    long notifyDataModified();
	void setStyleListFromLexer(int index);
    void setVisualFromStyleList();

	void updateGlobalOverrideCtrls();
	void showGlobalOverrideCtrls(bool show);
};

#endif //WORD_STYLE_H
