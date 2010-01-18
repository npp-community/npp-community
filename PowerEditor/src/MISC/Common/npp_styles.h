// This file is part of notepad++
// Copyright (C)2009 The Notepad++ Team
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

#ifndef MISC_COMMON_NPPSTYLES_H
#define MISC_COMMON_NPPSTYLES_H

enum FontStyle
{
	FONTSTYLE_BOLD = 1,
	FONTSTYLE_ITALIC = 2,
	FONTSTYLE_UNDERLINE = 4
};

enum ColorStyle
{
	COLORSTYLE_FOREGROUND = 0x01,
	COLORSTYLE_BACKGROUND = 0x02,
	COLORSTYLE_ALL = COLORSTYLE_FOREGROUND|COLORSTYLE_BACKGROUND
};

// forward declarations
class TiXmlNode;

struct Style
{
	int _styleID;
	generic_string _styleDesc;

	COLORREF _fgColor;
	COLORREF _bgColor;
	int _colorStyle;
	generic_string _fontName;
	int _fontStyle;
	int _fontSize;

	int _keywordClass;
	generic_string *_keywords;

	Style();
	Style(const Style & style);

	~Style();

	const Style & operator=(const Style & style);

	void setKeywords(const TCHAR *str);
};

struct GlobalOverride
{
	bool isEnable() const {return (enableFg || enableBg || enableFont || enableFontSize || enableBold || enableItalic || enableUnderLine);};
	bool enableFg;
	bool enableBg;
	bool enableFont;
	bool enableFontSize;
	bool enableBold;
	bool enableItalic;
	bool enableUnderLine;
	GlobalOverride():enableFg(false), enableBg(false), enableFont(false), enableFontSize(false), enableBold(false), enableItalic(false), enableUnderLine(false) {};
};

#define MAX_STYLE 30

// JOCE: This looks like some kind of map right here.  Probably should be one even.
struct StyleArray
{
public:
	StyleArray() : _nbStyler(0){};

	const StyleArray & operator=(const StyleArray & sa);

	int getNbStyler() const {return _nbStyler;}
	void setNbStyler(int nb) {_nbStyler = nb;}

	Style & getStyler(int index) {return _styleArray[index];}

	void addStyler(int styleID, TiXmlNode *styleNode);
	void addStyler(int styleID, TCHAR *styleName);

	int getStylerIndexByID(int id);

	int getStylerIndexByName(const TCHAR *name) const;

	bool hasEnoughSpace() {return (_nbStyler < MAX_STYLE);} // to be moved to protected...

protected:

	// JOCE: Risk right here...  Should be a vector probably.
	Style _styleArray[MAX_STYLE]; // JOCE: WOW!  That looks evil.  No checks nothing.
	int _nbStyler;
};


struct LexerStyler : public StyleArray
{
public :
	void setLexerName(const TCHAR *lexerName);
	void setLexerDesc(const TCHAR *lexerDesc);
	void setLexerUserExt(const TCHAR *lexerUserExt);

	const TCHAR * getLexerName() const {return _lexerName.c_str();}
	const TCHAR * getLexerDesc() const {return _lexerDesc.c_str();}
	const TCHAR * getLexerUserExt() const {return _lexerUserExt.c_str();}

private :
	generic_string _lexerName;
	generic_string _lexerDesc;
	generic_string _lexerUserExt;
};

#define  MAX_LEXER_STYLE 80


// JOCE: This looks like some kind of map right here.  Probably should be one even.
struct LexerStylerArray
{
public :
	LexerStylerArray() : _nbLexerStyler(0){};

	const LexerStylerArray & operator=(const LexerStylerArray & lsa);

	int getNbLexer() const {return _nbLexerStyler;};

	LexerStyler & getLexerFromIndex(int index)
	{
		return _lexerStylerArray[index];
	};

	const TCHAR * getLexerNameFromIndex(int index) const {return _lexerStylerArray[index].getLexerName();}
	const TCHAR * getLexerDescFromIndex(int index) const {return _lexerStylerArray[index].getLexerDesc();}

	LexerStyler * getLexerStylerByName(const TCHAR *lexerName);
	bool hasEnoughSpace() {return (_nbLexerStyler < MAX_LEXER_STYLE);};
	void addLexerStyler(const TCHAR *lexerName, const TCHAR *lexerDesc, const TCHAR *lexerUserExt, TiXmlNode *lexerNode);
	void eraseAll();
private :

	// JOCE: Risk right here...  Should be a vector probably.
	LexerStyler _lexerStylerArray[MAX_LEXER_STYLE];
	int _nbLexerStyler;
};

#endif // MISC_COMMON_NPPSTYLES_H
