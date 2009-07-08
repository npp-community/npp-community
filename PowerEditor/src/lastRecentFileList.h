#ifndef LASTRECENTFILELIST_H
#define LASTRECENTFILELIST_H

#include "Common.h"

struct RecentItem {
	int _id;
	std::generic_string _name;
	RecentItem(const TCHAR * name) : _name(name) {};
};

typedef std::deque<RecentItem> recentList;

class LastRecentFileList
{
public :
	LastRecentFileList();
	virtual ~LastRecentFileList();

	void initMenu(HMENU hMenu, int idBase, int posBase);

	void updateMenu();

	void add(const TCHAR *fn);
	void remove(const TCHAR *fn);
	void remove(int index);
	void clear();

	int getSize() {
		return _size;
	};

	int getMaxNbLRF() const;

	int getUserMaxNbLRF() const {
		return _userMax;
	};

	std::generic_string & getItem(int id);	//use menu id
	std::generic_string & getIndex(int index);	//use menu id

	void setUserMaxNbLRF(int size);

	void saveLRFL();

	void setLock(bool lock) {
		_locked = lock;
	};

	void setLangEncoding(int nativeLangEncoding) {
		_nativeLangEncoding = nativeLangEncoding;
	};

private:
	recentList _lrfl;
	int _userMax;
	int _size;
	int _nativeLangEncoding;

	// For the menu
	HMENU _hMenu;
	int _posBase;
	int _idBase;
	bool* _idFreeArray;
	bool _hasSeparators;
	bool _locked;

	int find(const TCHAR *fn);

	int popFirstAvailableID();
	void setAvailable(int id);
};

#endif //LASTRECENTFILELIST_H
