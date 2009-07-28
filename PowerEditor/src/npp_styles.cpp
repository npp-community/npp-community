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

#include "precompiled_headers.h"

#include "npp_styles.h"
#include "TinyXml.h"
#include "Parameters_def.h"
#include "colors.h"

// ***********************************
//
// Style
//
// ***********************************

Style::Style() :
	_styleID(-1),
	_styleDesc(NULL),
	_fgColor(COLORREF(-1)),
	_bgColor(COLORREF(-1)),
	_colorStyle(COLORSTYLE_ALL),
	_fontName(NULL),
	_fontStyle(-1),
	_fontSize(-1),
	_keywordClass(-1),
	_keywords(NULL)
{

}

Style::Style( const Style & style )
{
	_styleID = style._styleID;
	_styleDesc = style._styleDesc;
	_fgColor = style._fgColor;
	_bgColor = style._bgColor;
	_colorStyle = style._colorStyle;
	_fontName = style._fontName;
	_fontSize = style._fontSize;
	_fontStyle = style._fontStyle;
	_keywordClass = style._keywordClass;
	if (style._keywords)
		_keywords = new std::generic_string(*(style._keywords));
	else
		_keywords = NULL;
}

Style::~Style()
{
	if (_keywords)
	{
		delete _keywords;
	}
}

const Style & Style::operator=( const Style & style )
{
	if (this != &style)
	{
		this->_styleID = style._styleID;
		this->_styleDesc = style._styleDesc;
		this->_fgColor = style._fgColor;
		this->_bgColor = style._bgColor;
		this->_colorStyle = style._colorStyle;
		this->_fontName = style._fontName;
		this->_fontSize = style._fontSize;
		this->_fontStyle = style._fontStyle;
		this->_keywordClass = style._keywordClass;

		if (!(this->_keywords) && style._keywords)
			this->_keywords = new std::generic_string(*(style._keywords));
		else if (this->_keywords && style._keywords)
			this->_keywords->assign(*(style._keywords));
		else if (this->_keywords && !(style._keywords))
		{
			delete (this->_keywords);
			this->_keywords = NULL;
		}
	}
	return *this;
}

void Style::setKeywords( const TCHAR *str )
{
	if (!_keywords)
	{
		_keywords = new std::generic_string(str);
	}
	else
	{
		*_keywords = str;
	}
}

// ***********************************
//
// StyleArray
//
// ***********************************


// JOCE: The following static funcs will need to live somewhere else since they're
// accessed by other files...  Common.* maybe?
static int strVal(const TCHAR *str, int base) {
	if (!str) return -1;
	if (!str[0]) return 0;

	TCHAR *finStr;
	int result = generic_strtol(str, &finStr, base);
	if (*finStr != '\0')
		return -1;
	return result;
};

static int hexStrVal(const TCHAR *str) {
	return strVal(str, 16);
};

static int decStrVal(const TCHAR *str) {
	return strVal(str, 10);
};

static int getKwClassFromName(const TCHAR *str) {
	if (!lstrcmp(TEXT("instre1"), str)) return LANG_INDEX_INSTR;
	if (!lstrcmp(TEXT("instre2"), str)) return LANG_INDEX_INSTR2;
	if (!lstrcmp(TEXT("type1"), str)) return LANG_INDEX_TYPE;
	if (!lstrcmp(TEXT("type2"), str)) return LANG_INDEX_TYPE2;
	if (!lstrcmp(TEXT("type3"), str)) return LANG_INDEX_TYPE3;
	if (!lstrcmp(TEXT("type4"), str)) return LANG_INDEX_TYPE4;
	if (!lstrcmp(TEXT("type5"), str)) return LANG_INDEX_TYPE5;

	if ((str[1] == '\0') && (str[0] >= '0') && (str[0] <= '8')) // up to KEYWORDSET_MAX
		return str[0] - '0';

	return -1;
};



const StyleArray & StyleArray::operator=( const StyleArray & sa )
{
	if (this != &sa)
	{
		this->_nbStyler = sa._nbStyler;
		for (int i = 0 ; i < _nbStyler ; i++)
		{
			this->_styleArray[i] = sa._styleArray[i];
		}
	}
	return *this;
}

void StyleArray::addStyler(int styleID, TiXmlNode *styleNode)
{
	assert( hasEnoughSpace() );

	_styleArray[_nbStyler]._styleID = styleID;

	if (styleNode)
	{
		TiXmlElement *element = styleNode->ToElement();

		// For _fgColor, _bgColor :
		// RGB() | (result & 0xFF000000) It's the case for -1 (0xFFFFFFFF)
		// returned by hexStrVal(str)
		const TCHAR *str = element->Attribute(TEXT("name"));
		if (str)
		{
			_styleArray[_nbStyler]._styleDesc = str;
		}

		str = element->Attribute(TEXT("fgColor"));
		if (str)
		{
			unsigned long result = hexStrVal(str);
			_styleArray[_nbStyler]._fgColor = (RGB((result >> 16) & 0xFF, (result >> 8) & 0xFF, result & 0xFF)) | (result & 0xFF000000);

		}

		str = element->Attribute(TEXT("bgColor"));
		if (str)
		{
			unsigned long result = hexStrVal(str);
			_styleArray[_nbStyler]._bgColor = (RGB((result >> 16) & 0xFF, (result >> 8) & 0xFF, result & 0xFF)) | (result & 0xFF000000);
		}

		str = element->Attribute(TEXT("colorStyle"));
		if (str)
		{
			_styleArray[_nbStyler]._colorStyle = decStrVal(str);
		}

		str = element->Attribute(TEXT("fontName"));
		_styleArray[_nbStyler]._fontName = str;

		str = element->Attribute(TEXT("fontStyle"));
		if (str)
		{
			_styleArray[_nbStyler]._fontStyle = decStrVal(str);
		}

		str = element->Attribute(TEXT("fontSize"));
		if (str)
		{
			_styleArray[_nbStyler]._fontSize = decStrVal(str);
		}

		str = element->Attribute(TEXT("keywordClass"));
		if (str)
		{
			_styleArray[_nbStyler]._keywordClass = getKwClassFromName(str);
		}

		TiXmlNode *v = styleNode->FirstChild();
		if (v)
		{
			_styleArray[_nbStyler]._keywords = new std::generic_string(v->Value());
		}
	}
	_nbStyler++;
}

void StyleArray::addStyler( int styleID, TCHAR *styleName )
{
	assert( hasEnoughSpace() );

	_styleArray[_nbStyler]._styleID = styleID;
	_styleArray[_nbStyler]._styleDesc = styleName;
	_styleArray[_nbStyler]._fgColor = black;
	_styleArray[_nbStyler]._bgColor = white;
	_nbStyler++;
}

int StyleArray::getStylerIndexByID( int id )
{
	for (int i = 0 ; i < _nbStyler ; i++)
		if (_styleArray[i]._styleID == id)
			return i;
	return -1;
}

int StyleArray::getStylerIndexByName( const TCHAR *name ) const
{
	if (!name)
	{
		return -1;
	}

	for (int i = 0 ; i < _nbStyler ; i++)
	{
		if (!lstrcmp(_styleArray[i]._styleDesc, name))
		{
			return i;
		}
	}
	return -1;
}

// ***********************************
//
// LexerStyler
//
// ***********************************


void LexerStyler::setLexerName( const TCHAR *lexerName )
{
	_lexerName = lexerName;
}

void LexerStyler::setLexerDesc( const TCHAR *lexerDesc )
{
	_lexerDesc = lexerDesc;
}

void LexerStyler::setLexerUserExt( const TCHAR *lexerUserExt )
{
	_lexerUserExt = lexerUserExt;
}


// ***********************************
//
// LexerStyler
//
// ***********************************

const LexerStylerArray & LexerStylerArray::operator=( const LexerStylerArray & lsa )
{
	if (this != &lsa)
	{
		this->_nbLexerStyler = lsa._nbLexerStyler;
		for (int i = 0 ; i < this->_nbLexerStyler ; i++)
		{
			this->_lexerStylerArray[i] = lsa._lexerStylerArray[i];
		}
	}
	return *this;
}

LexerStyler * LexerStylerArray::getLexerStylerByName( const TCHAR *lexerName )
{
	if (!lexerName) return NULL;
	for (int i = 0 ; i < _nbLexerStyler ; i++)
	{
		if (!lstrcmp(_lexerStylerArray[i].getLexerName(), lexerName))
			return &(_lexerStylerArray[i]);
	}
	return NULL;
}
