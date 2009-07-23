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
#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include "StaticDialog.h"

const size_t nameLenMax = 64;

class NppParameters;

void getKeyStrFromVal(UCHAR keyVal, std::generic_string & str);
void getNameStrFromCmd(INT cmd, std::generic_string & str);

struct KeyCombo {
	bool _isCtrl;
	bool _isAlt;
	bool _isShift;
	UCHAR _key;
};

class Shortcut : public StaticDialog
{
public:
	Shortcut();

	Shortcut(const TCHAR *name, bool isCtrl, bool isAlt, bool isShift, UCHAR key);

	Shortcut(const Shortcut & sc);

	BYTE getAcceleratorModifiers();

	Shortcut & operator=(const Shortcut & sc);
	friend const bool operator==(const Shortcut & a, const Shortcut & b);

	friend inline const bool operator!=(const Shortcut & a, const Shortcut & b) {
		return !(a == b);
	}

	virtual int doDialog();

	virtual bool isValid() const;
	virtual bool isEnabled() const {	//true if _keyCombo != 0, false if _keyCombo == 0, in which case no accelerator should be made
		return (_keyCombo._key != 0);
	}

	virtual std::generic_string toString() const;					//the hotkey part
	std::generic_string toMenuItemString() const;
	const KeyCombo & getKeyCombo() const {
		return _keyCombo;
	}

	const TCHAR * getName() const {
		return _name;
	}

	const TCHAR * getMenuName() const {
		return _menuName;
	}

	void setName(const TCHAR * name);

protected :
	KeyCombo _keyCombo;
	virtual BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
	bool _canModifyName;
	TCHAR _name[nameLenMax];		//normal name is plain text (for display purposes)
	TCHAR _menuName[nameLenMax];	//menu name has ampersands for quick keys
};

class CommandShortcut : public Shortcut {
public:
	CommandShortcut(Shortcut sc, long id) :	Shortcut(sc), _id(id) {};
	long getID() const {return _id;};
	void setID(long id) { _id = id;};

private :
	long _id;
};


class ScintillaKeyMap : public Shortcut {
public:
	ScintillaKeyMap(Shortcut sc, long scintillaKeyID, unsigned long id);
	long getScintillaKeyID() const {return _scintillaKeyID;}
	int getMenuCmdID() const {return _menuCmdID;};
	int toKeyDef(int index) const;

	KeyCombo getKeyComboByIndex(int index) const;
	void ScintillaKeyMap::setKeyComboByIndex(int index, KeyCombo combo);
	void removeKeyComboByIndex(int index);
	void clearDups();
	int addKeyCombo(KeyCombo combo);
	bool isEnabled() const;
	size_t getSize() const;

	std::generic_string toString() const;
	std::generic_string toString(int index) const;

	int doDialog();

	//only compares the internal KeyCombos, nothing else
	friend const bool operator==(const ScintillaKeyMap & a, const ScintillaKeyMap & b);

	friend inline const bool operator!=(const ScintillaKeyMap & a, const ScintillaKeyMap & b) {
		return !(a == b);
	};

private:
	long _scintillaKeyID;
	int _menuCmdID;
	std::vector<KeyCombo> _keyCombos;
	int size;
	void applyToCurrentIndex();
	void validateDialog();
	void showCurrentSettings();
	void updateListItem(int index);
protected :
	BOOL CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
};


class Window;
class ScintillaEditView;

struct recordedMacroStep {
	enum MacroTypeIndex {mtUseLParameter, mtUseSParameter, mtMenuCommand};

	int message;
	long wParameter;
	long lParameter;
	std::generic_string sParameter;
	MacroTypeIndex MacroType;

	recordedMacroStep(int iMessage, long wParam, long lParam);
	recordedMacroStep(int iCommandID) : message(0), wParameter(iCommandID), lParameter(0), MacroType(mtMenuCommand) {};

	recordedMacroStep(int type, int iMessage, long wParam, long lParam, const TCHAR *sParam)
		: message(iMessage), wParameter(wParam), lParameter(lParam), MacroType(MacroTypeIndex(type)){
		sParameter = *reinterpret_cast<const TCHAR *>(sParam);
	};
	bool isValid() const {
		return true;
	};

	void PlayBack(Window* pNotepad, ScintillaEditView *pEditView);
};

typedef std::vector<recordedMacroStep> Macro;

class MacroShortcut : public CommandShortcut {
friend class NppParameters;
public:
	MacroShortcut(Shortcut sc, Macro macro, int id) : CommandShortcut(sc, id), _macro(macro) {_canModifyName = true;};
	Macro & getMacro() {return _macro;};
private:
	Macro _macro;
};


class UserCommand : public CommandShortcut {
friend class NppParameters;
public:
	UserCommand(Shortcut sc, const TCHAR *cmd, int id) : CommandShortcut(sc, id), _cmd(cmd) {_canModifyName = true;};
	const TCHAR* getCmd() const {return _cmd.c_str();};
private:
	std::generic_string _cmd;
};

class PluginCmdShortcut : public CommandShortcut {
//friend class NppParameters;
public:
	PluginCmdShortcut(Shortcut sc, int id, const TCHAR *moduleName, int internalID) :
		CommandShortcut(sc, id), _id(id), _internalID(internalID) {
		lstrcpy(_moduleName, moduleName);
	};
	bool isValid() const;
	const TCHAR * getModuleName() const {return _moduleName;};
	int getInternalID() const {return _internalID;};
	long getID() const {return _id;};

private :
	long _id;
	TCHAR _moduleName[nameLenMax];
	int _internalID;
};

class Accelerator { //Handles accelerator keys for Notepad++ menu, including custom commands
friend class ShortcutMapper;
public:
	Accelerator():_hAccelMenu(NULL), _hMenuParent(NULL), _hAccTable(NULL), _pAccelArray(NULL), _nbAccelItems(0){};
	~Accelerator();
	void init(HMENU hMenu, HWND menuParent);
	HACCEL getAccTable() const {return _hAccTable;};

	void updateShortcuts();
	void updateFullMenu();

private:
	HMENU _hAccelMenu;
	HWND _hMenuParent;
	HACCEL _hAccTable;
	ACCEL *_pAccelArray;
	int _nbAccelItems;

	void reNew();
	void updateMenuItemByCommand(CommandShortcut csc);
};

class ScintillaAccelerator {	//Handles accelerator keys for scintilla
public:
	ScintillaAccelerator() : _nrScintillas(0) {};
	void init(std::vector<HWND> * vScintillas, HMENU hMenu, HWND menuParent);
	void updateKeys();
	void updateKey(ScintillaKeyMap skmOld, ScintillaKeyMap skm);
private:
	HMENU _hAccelMenu;
	HWND _hMenuParent;
	std::vector<HWND> _vScintillas;
	int _nrScintillas;

	void updateMenuItemByID(ScintillaKeyMap skm, int id);
};

#endif //SHORTCUTS_H
