// Scintilla source code edit control
/** @file LexRebol.cxx
 ** Lexer for REBOL.
 ** Written by Pascal Hurni, inspired from LexLua by Paul Winwood & Marcos E. Wurzius & Philippe Lhoste
 ** Rewritten by Oldes Huhuman, it started just as a quick fix, but I almost rewritten it completely finally.
 **     It's still possible that it's missing some obscure cases, but it's usable for my purposes now.
 **
 ** History:
 **		2005-04-07	First release.
 **		2005-04-10	Closing parens and brackets go now in default style
 **					String and comment nesting should be more safe
 **     2011-04-19  Rewritten by Oldes to support a lot of missing or buggy functionality
 **/
// NoCopyright 2011 by Oldes <oldes.huhuman@gmail.com>
// Copyright 2005 by Pascal Hurni <pascal_hurni@fastmail.fm>
// The License.txt file describes the conditions under which this software may be distributed.


// NPPSTART Joce 10/11/10 Scintilla_precomp_headers
#include "precompiled_headers.h"
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include <assert.h>
//#include <ctype.h>
//
//#include "ILexer.h"
//#include "Scintilla.h"
//#include "SciLexer.h"
//
//#include "PropSetSimple.h"
//#include "WordList.h"
//#include "LexAccessor.h"
//#include "Accessor.h"
//#include "StyleContext.h"
//#include "CharacterSet.h"
//#include "LexerModule.h"
// NPPEND

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

static inline bool IsAnOperator(const int ch, const int ch2, const int ch3) {
	// One char operators
	if (IsASpaceOrTab(ch2)) {
		return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '>' || ch == '=' || ch == '?';
	}

	// Two char operators
	if (IsASpaceOrTab(ch3)) {
		return (ch == '*' && ch2 == '*') ||
			   (ch == '/' && ch2 == '/') ||
			   (ch == '<' && (ch2 == '=' || ch2 == '>')) ||
			   (ch == '>' && ch2 == '=') ||
			   (ch == '=' && (ch2 == '=' || ch2 == '?')) ||
			   (ch == '?' && ch2 == '?');
	}

	return false;
}

static inline bool IsBinaryStart(const int ch, const int ch2, const int ch3, const int ch4) {
	return (ch == '#' && ch2 == '{') ||
		   (ch == '2' && ch2 == '#' && ch3 == '{' ) ||
		   (ch == '1' && ch2 == '6' && ch3 == '#' && ch4 == '{' ) ||
		   (ch == '6' && ch2 == '4' && ch3 == '#' && ch4 == '{' );
}

static inline bool IsHexNumberChar(int ch) {
	return IsADigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

static inline bool IsAlpha(int ch) {
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}
static inline bool IsDelimiterChar(int ch) {
	return (
		(ch >= 0 && ch <= 32) ||
		ch == '(' || ch == '[' ||
		ch == ')' || ch == ']' ||
		ch == '{' || ch == '}' ||
		ch == '"' || ch == ';' || ch == 127);
}
static inline bool IsFollowSlashChar(int ch) {
	return (
		IsDelimiterChar(ch) ||
		ch == '+' || ch == '-' || ch == '.' || ch == '#' ||
		ch == '$' || ch == '%' || ch == ',' || ch == '<' ||
		ch == '>' || ch == '\\');
}

static inline bool IsAWordChar(const int ch) {
	return !(IsDelimiterChar(ch) || ch == '@');
}

static inline bool IsAWordStart(const int ch, const int ch2) {
	/* word-char-first: charset ["!&*=?^^_`|~" #"A" - #"Z" #"a" - #"z"] */
	return (
		 isalpha(ch) ||
		 ch == '?' || ch == '!' || ch == '`' || ch == '*' || ch == '&' ||
		 ch == '|' || ch == '=' || ch == '_' || ch == '~' || ch == '^' ||
		 (( ch == '+' || ch == '-' || ch == '.') && !isdigit(ch2) ) ||
		 ch > 127);
}
static inline bool IsAWordRest(const int ch) {
	return (
		 isalpha(ch) || isdigit(ch) ||
		 ch == '?' || ch == '!' || ch == '`' || ch == '*' || ch == '&' ||
		 ch == '|' || ch == '=' || ch == '_' || ch == '~' || ch == '^' ||
		 ch == '\'' || ch == '+' || ch == '-' || ch == '.'  ||
		 ch > 127);
}

static inline bool IsNewlineChar(int ch) {
	return (ch == '\r' || ch == '\n');
}

static inline bool IsDecimalPointChar(int ch) {
	return (ch == '.' || ch == ',');
}

static inline void InvalidValue(StyleContext& sc) {
	sc.ChangeState(SCE_REBOL_INVALID);
	while( sc.More() && !IsDelimiterChar(sc.ch)){sc.Forward();}
	sc.SetState(SCE_REBOL_DEFAULT);
}

static int isReversedTupleValue(StyleContext& sc, int offset){
	char str[4]="000";
	int c = 0, ch;
	while ( IsADigit(ch = sc.GetRelative(offset-c)) ){
		if (c == 3) return 0;
		str[2-c] = ch;
		c++;
	}
	if ( atoi( str ) > 255 ) return 0;
	if ( ch == '.' && IsADigit(sc.GetRelative(offset-c-1)) ) return -c-1;
	if ( IsDelimiterChar(ch) ) return -c;
	return 0;
}

static int isTupleValue(StyleContext& sc, int offset){
	char str[4];
	int c = 0;
	int ch = sc.GetRelative(offset);
	if (!IsADigit(ch)) return 0;
	while ( IsADigit(ch) ){
		if (c == 3) return 0;
		str[c] = ch;
		c++;
		ch = sc.GetRelative(offset+c);
	}
	if ( atoi( str ) > 255 ) return 0;
	if ( ch == '.' && IsADigit(sc.GetRelative(offset+c+1))) return c+1;
	if ( IsDelimiterChar(ch) ) return c;
	return 0; 
}

static int isReversedTimeValue(StyleContext& sc, int offset){
	int c = 0, ch;
	while ( IsADigit(ch = sc.GetRelative(offset+c)) || ch == ':' ){
		if ( ch == ':' && !IsADigit(sc.GetRelative(offset+c+1)) ) return 0;
		c--;
	}
	if ( ( IsDelimiterChar(ch) ||
		  ((ch == '+' || ch == '-') && IsDelimiterChar(sc.GetRelative(offset+c-1))) 

		 ) //&& sc.GetRelative(offset+c+1) != ':'
	) return c;
	return 0;
}

static int isNumber(StyleContext& sc, int ofs){
	int ch = sc.GetRelative(ofs);
	if (ch == '+' || ch == '-') {
		ch = sc.GetRelative(++ofs);
	}
	if (IsADigit(ch)) {
		ofs++;
		while (IsADigit(sc.GetRelative(ofs))) { ofs++; }
		if (IsDecimalPointChar(sc.GetRelative(ofs))) ofs++;
		while (IsADigit(sc.GetRelative(ofs))) { ofs++; }
	} else if ( IsDecimalPointChar(ch) && IsADigit(sc.GetRelative(ofs+1)) ){
		ofs++;
		while (IsADigit(sc.GetRelative(ofs))) { ofs++; }
	} else {
		return 0;
	}
	ch = sc.GetRelative(ofs);
	if (ch == 'e' || ch == 'E') {
		ch = sc.GetRelative(++ofs);
		if (ch == '+' || ch == '-') {
			ch = sc.GetRelative(++ofs);
		}
		if (IsADigit(ch)) {
			ofs++;
			while (IsADigit(sc.GetRelative(ofs))) { ofs++; }
		} else {
			return -ofs;
		}
	}
	return ofs;
}

static bool IsValidDate(StyleContext& sc){
	int delimiter = sc.ch;
	char num[3] = "00";
	char numyear[6] = "\0\0\0\0\0";
	int day, i;
	int month = 0;
	int year;
	int ch1 = sc.chPrev;
	int ch2 = sc.GetRelative(-2);
	int ch3 = sc.GetRelative(-3);

	if ( IsADigit(ch1) && IsADigit(ch2) && IsDelimiterChar(ch3) ){
		num[0]=ch2;
		num[1]=ch1;

	} else if ( IsADigit(ch1) && IsDelimiterChar(ch2) ){
		num[1]=ch1;
	}
	day = atoi ( num );
	if ( day == 0 ) return false;

	sc.Forward();
	num[0] = 0;
	num[1] = 0;
	ch1 = sc.ch;
	ch2 = sc.chNext;
	ch3 = sc.GetRelative(2);
	
	if ( IsADigit(ch1) && IsADigit(ch2) && ch3 == delimiter ){
		num[0]=ch1;
		num[1]=ch2;
		month = atoi ( num );
		sc.Forward( 3 );
	} else if ( IsADigit(ch1) && ch2 == delimiter ) {
		num[0]=ch1;
		month = atoi ( num );
		sc.Forward( 2 );
	} else if (sc.GetRelative(3) == delimiter) {
		if (sc.MatchIgnoreCase("jan")) {
			month = 1;
		} else if (sc.MatchIgnoreCase("feb")) {
			month = 2;
		} else if (sc.MatchIgnoreCase("mar")) {
			month = 3;
		} else if (sc.MatchIgnoreCase("apr")) {
			month = 4;
		} else if (sc.MatchIgnoreCase("may")) {
			month = 5;
		} else if (sc.MatchIgnoreCase("jun")) {
			month = 6;
		} else if (sc.MatchIgnoreCase("jul")) {
			month = 7;
		} else if (sc.MatchIgnoreCase("aug")) {
			month = 8;
		} else if (sc.MatchIgnoreCase("sep")) {
			month = 9;
		} else if (sc.MatchIgnoreCase("oct")) {
			month = 10;
		} else if (sc.MatchIgnoreCase("nov")) {
			month = 11;
		} else if (sc.MatchIgnoreCase("dec")) {
			month = 12;
		} else {
			return false;
		}
		sc.Forward(4);
	} else {
		if (sc.MatchIgnoreCase("january")) {
			month = 1;
			sc.Forward(7);
		} else if (sc.MatchIgnoreCase("february")) {
			month = 2;
			sc.Forward(8);
		} else if (sc.MatchIgnoreCase("march")) {
			month = 3;
			sc.Forward(5);
		} else if (sc.MatchIgnoreCase("april")) {
			month = 4;
			sc.Forward(5);
		} else if (sc.MatchIgnoreCase("may")) {
			month = 5;
			sc.Forward(3);
		} else if (sc.MatchIgnoreCase("june")) {
			month = 6;
			sc.Forward(4);
		} else if (sc.MatchIgnoreCase("july")) {
			month = 7;
			sc.Forward(4);
		} else if (sc.MatchIgnoreCase("august")) {
			month = 8;
			sc.Forward(6);
		} else if (sc.MatchIgnoreCase("september")) {
			month = 9;
			sc.Forward(9);
		} else if (sc.MatchIgnoreCase("october")) {
			month = 10;
			sc.Forward(7);
		} else if (sc.MatchIgnoreCase("november")) {
			month = 11;
			sc.Forward(8);
		} else if (sc.MatchIgnoreCase("december")) {
			month = 12;
			sc.Forward(8);
		} else {
			return false;
		}

		if (sc.ch == delimiter) {
			sc.Forward();
		} else {
			return false;
		}
	}

	if (!IsADigit(sc.ch)) return false;
	//year:
	i = 0;
	while( sc.More() && IsADigit(sc.ch) ) {
		numyear[i] = sc.ch;
		if ( ++i>5 ) return false;
		sc.Forward();
	}
	year = atoi(numyear);
	if (year <= 60) {
		year += 2000;
	} else if (year <= 99) {
		year += 1900;
	}
	bool isLeapYear = year%400 ==0 || (year%100 != 0 && year%4 == 0);
	if ( month == 0 || month > 12 || year > 16383 ||
		(month == 1 && day > 31) ||
		(month == 2 && ((isLeapYear && day > 29) || (!isLeapYear && day > 28))) ||
		(month == 3 && day > 31) ||
		(month == 4 && day > 30) ||
		(month == 5 && day > 31) ||
		(month == 6 && day > 30) ||
		(month == 7 && day > 31) ||
		(month == 8 && day > 31) ||
		(month == 9 && day > 30) ||
		(month == 10 && day > 31) ||
		(month == 11 && day > 30) ||
		(month == 12 && day > 31)
		) return false;

	if (sc.ch == '/') { //time
		sc.Forward();
		if((sc.ch == '+' /*|| sc.ch == '-'*/) && IsADigit(sc.chNext)) {
			sc.Forward();
		}
		if (IsADigit(sc.ch)) {
			while ( sc.More() && IsADigit(sc.ch) ) sc.Forward();
			if (sc.ch == ':' && IsADigit(sc.chNext)) {
				sc.Forward();
				while ( sc.More() && IsADigit(sc.ch) ) sc.Forward(); //1-1-2000/00:00
				if (IsDelimiterChar(sc.ch)) return true;
				if (sc.ch == ':' && IsADigit(sc.chNext)) {
					sc.Forward();
					while ( sc.More() && IsADigit(sc.ch) ) sc.Forward(); //1-1-2000/00:00:00
				}
				if (IsDecimalPointChar(sc.ch)) {
					sc.Forward();
					if (IsADigit(sc.ch)) {
						while ( sc.More() && IsADigit(sc.ch) ) sc.Forward(); 
					} else {
						return false; //invalid like: 1-1-2000/00:00:00.x
					}
				}
				if (sc.ch == '+' || sc.ch == '-') {//timezone
					sc.Forward();
					if (IsADigit(sc.ch)) {
						while ( sc.More() && IsADigit(sc.ch) ) sc.Forward();
						if (sc.ch == ':' && IsADigit(sc.chNext)) {
							sc.Forward();
							while ( sc.More() && IsADigit(sc.ch) ) sc.Forward();
						} else {
							return false;
						}
					} else {
						return false;
					}
				}
			} else {
				return false; //invalid like: 1-1-2000/0
			}
		} else {
			return false; //invalid datetime like 1-1-2000/
		}
	}
	return (IsDelimiterChar(sc.ch));
}

static void ColouriseMoney(StyleContext& sc){
	if (IsADigit(sc.chNext) || IsDecimalPointChar(sc.chNext)){
		int ch1 = sc.chPrev;
		int ch2 = sc.GetRelative(-2);
		int ch3 = sc.GetRelative(-3);
		int ch4 = sc.GetRelative(-4);
		if(
			  IsDelimiterChar(ch1) ||
			( (ch1 == '+' || ch1 == '-') && IsDelimiterChar(ch2) ) ||
			( IsAlpha(ch1) && (	IsDelimiterChar(ch2) ||	( (ch2 == '+' || ch2 == '-') && IsDelimiterChar(ch3) )) ) ||
			( IsAlpha(ch1) && IsAlpha(ch2) && (	IsDelimiterChar(ch3) ||	( (ch3 == '+' || ch3 == '-') && IsDelimiterChar(ch4) )) ) ||
			( IsAlpha(ch1) && IsAlpha(ch2) && IsAlpha(ch3) && (	IsDelimiterChar(ch4) ||	( (ch4 == '+' || ch4 == '-') && IsDelimiterChar(sc.GetRelative(-5)) )) )
			) {
			sc.ChangeState(SCE_REBOL_MONEY);
			sc.Forward();
			if(IsADigit(sc.ch)) {
				while(sc.More() && IsADigit(sc.ch)) sc.Forward();
			}
			if(IsDecimalPointChar(sc.ch)) {
				if(IsADigit(sc.chNext)){
					sc.Forward();
					while(sc.More() && IsADigit(sc.ch)) sc.Forward();
				} else if (!IsADigit(sc.chPrev)){
					InvalidValue(sc); // $.
				} else {
					sc.Forward();
				}
			}
			if(!IsDelimiterChar(sc.ch)){
				InvalidValue(sc);
			}
		} else {
			InvalidValue(sc);
		}
	} else {
		InvalidValue(sc);
	}
	sc.SetState(SCE_REBOL_DEFAULT);
}
static void ColouriseTime(StyleContext& sc) {
	while (
		sc.More() &&
		(IsADigit(sc.ch) || (sc.ch == ':' && IsADigit(sc.chNext)))
	){
		sc.Forward();
	}
	if ( IsADigit(sc.chPrev) && IsDelimiterChar(sc.ch) ) {
		sc.SetState(SCE_REBOL_DEFAULT);
	} else if ( IsDecimalPointChar(sc.ch) && IsADigit(sc.chPrev) ){
		if ( IsADigit(sc.chNext) || IsDelimiterChar(sc.chNext) ) {
			sc.Forward();
			while( sc.More() && IsADigit(sc.ch) ) { sc.Forward();}
			if (IsDelimiterChar(sc.ch)){
				sc.SetState(SCE_REBOL_DEFAULT);
			} else {
				InvalidValue(sc);
			}
		} else {
			InvalidValue(sc);
		}
		
	} else {
		InvalidValue(sc);
	}
}
static void ColouriseEscapedChar(StyleContext& sc) {
	int prevState = sc.state;
	if(prevState == SCE_REBOL_COMMENT) {
		sc.Forward(2);
		return;
	}
	sc.SetState(SCE_REBOL_CHARACTER);
	sc.Forward();
	if (sc.ch == '(') {
		sc.Forward();

		int ch0 = sc.ch;
		int ch1 = sc.chNext;
		int ch2 = sc.GetRelative(2);
		int ch3 = sc.GetRelative(3);
		int ch4 = sc.GetRelative(4);

		if (
			sc.MatchIgnoreCase("tab)") || 
			sc.MatchIgnoreCase("esc)") || 
			sc.MatchIgnoreCase("del)")
		){
			sc.Forward(4);
			sc.SetState(prevState);
		} else if(
			sc.MatchIgnoreCase("line)") ||
			sc.MatchIgnoreCase("null)") ||
			sc.MatchIgnoreCase("back)") ||
			sc.MatchIgnoreCase("page)")
		) {
			sc.Forward(5);
			sc.SetState(prevState);
		} else if ( ch1 == ')' && IsHexNumberChar(ch0)) {
			//^(0), ^(a), ^(F)
			sc.Forward();
			sc.ForwardSetState(prevState);
		} else if (ch2 == ')' && IsHexNumberChar(ch0) && IsHexNumberChar(ch1)) {
			//^(00), ^(a0), ^(F0)
			sc.Forward(3);
			sc.SetState(prevState);
		} else if (ch3 == ')' && IsHexNumberChar(ch0) && IsHexNumberChar(ch1) && IsHexNumberChar(ch2)) {
			//^(120), ^(0A0)
			sc.Forward(4);
			sc.SetState(prevState);
		} else if (ch4 == ')' && IsHexNumberChar(ch0) && IsHexNumberChar(ch1) && IsHexNumberChar(ch2) && IsHexNumberChar(ch3)) {
			//^(0120), ^(0A0F)
			sc.Forward(5);
			sc.SetState(prevState);
		} else {
			sc.SetState(prevState);
		}
	}  else {
		sc.ForwardSetState(prevState);
	}
}


static void ColouriseRebolDoc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler) {

	StyleContext sc(startPos, length, initStyle, styler);
	
	WordList &keywords1 = *keywordlists[0];
	WordList &keywords2 = *keywordlists[1];
	WordList &keywords3 = *keywordlists[2];
	WordList &keywords4 = *keywordlists[3];
	WordList &keywords5 = *keywordlists[4];
	WordList &keywords6 = *keywordlists[5];

	int ofs, skip;

	int currentLine = styler.GetLine(startPos);
	// Initialize the braced string {.. { ... } ..} nesting level, if we are inside such a string.
	int nestingLevel = 0;


	if (initStyle == SCE_REBOL_STRING || initStyle == SCE_REBOL_COMMENT || initStyle == SCE_REBOL_SERIALIZED) {
		nestingLevel = styler.GetLineState(currentLine - 1);
	}

	bool blockComment = initStyle == SCE_REBOL_COMMENT;

	if (startPos == 0) {
		sc.SetState(SCE_REBOL_PREFACE);
	}
	
	while ( sc.More() ) {
	
		//--- What to do at line end ?
		if (sc.atLineEnd) {
			// Can be either inside a {} string or simply at eol
			if (sc.state != SCE_REBOL_STRING && sc.state != SCE_REBOL_COMMENT &&
				sc.state != SCE_REBOL_BINARY && sc.state != SCE_REBOL_PREFACE &&
				sc.state != SCE_REBOL_SERIALIZED && sc.state != SCE_REBOL_TAG &&
				sc.state != SCE_REBOL_TAGSTRING)
				sc.SetState(SCE_REBOL_DEFAULT);

			// Update the line state, so it can be seen by next line
			currentLine = styler.GetLine(sc.currentPos);
			switch (sc.state) {
			case SCE_REBOL_STRING:
			case SCE_REBOL_COMMENT:
			case SCE_REBOL_BINARY:
			case SCE_REBOL_SERIALIZED:
			case SCE_REBOL_TAG:
			case SCE_REBOL_TAGSTRING:
				// Inside a braced string, we set the line state
				styler.SetLineState(currentLine, nestingLevel);
				break;
			default:
				// Reset the line state
				styler.SetLineState(currentLine, 0);
				break;
			}

			// continue with next char
			//
			sc.Forward();
			continue;
		}
		
		//--- What to do on white-space ?
		if (IsASpaceOrTab(sc.ch))
		{
			// Return to default if any of these states
			if (sc.state == SCE_REBOL_OPERATOR || sc.state == SCE_REBOL_CHARACTER ||
				(sc.state >= SCE_REBOL_NUMBER && sc.state < SCE_REBOL_QUESTION &&
				sc.state != SCE_REBOL_BINARY && sc.state != SCE_REBOL_TAG && sc.state != SCE_REBOL_TAGSTRING)
			) {
				sc.SetState(SCE_REBOL_DEFAULT);
			}
		}
		
		//--- Specialize state ?
		// URL, Email look like identifier
		if (sc.ch == '@' && (sc.state == SCE_REBOL_IDENTIFIER || sc.state == SCE_REBOL_LITWORD || sc.state == SCE_REBOL_SETWORD)) {
			while(sc.More() && !(IsDelimiterChar(sc.chNext) || sc.chNext == '@')) sc.Forward();
			if (sc.chNext == '@') {
				InvalidValue(sc);
			} else {
				sc.ChangeState(SCE_REBOL_URL);
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
			}
			continue;
		}
		if (sc.state == SCE_REBOL_IDENTIFIER)
		{
			if (sc.ch == ':'){
				if(!IsAWordChar(sc.chNext)) {
					sc.ChangeState(SCE_REBOL_SETWORD);
				} else if (sc.chPrev != '/') {
					sc.ChangeState(SCE_REBOL_URL);
				}
			} else if (sc.ch == '$') {
				ColouriseMoney(sc);
				continue;
			} else if (sc.ch == '!') {
				if(!IsAWordChar(sc.chNext) && sc.chNext!=':') {
					sc.ChangeState(SCE_REBOL_DATATYPE);
				}
			} else if (sc.ch == '?') {
				if(!IsAWordChar(sc.chNext) && sc.chNext!=':') {
					sc.ChangeState(SCE_REBOL_QUESTION);
				}
			}
		}

		// Words look like identifiers

		if (sc.state == SCE_REBOL_IDENTIFIER || sc.state == SCE_REBOL_QUESTION || (sc.state >= SCE_REBOL_WORD && sc.state <= SCE_REBOL_WORD6)) {
			// Keywords ?
			if (!IsAWordChar(sc.ch) || sc.Match('/') || sc.Match(':')) {
				char s[100];
				sc.GetCurrentLowered(s, sizeof(s));
				blockComment = strcmp(s, "comment") == 0;
				if (keywords1.InList(s)) {
					sc.ChangeState(SCE_REBOL_WORD);
				} else if (keywords2.InList(s)) {
					sc.ChangeState(SCE_REBOL_WORD2);
				} else if (keywords3.InList(s)) {
					sc.ChangeState(SCE_REBOL_WORD3);
				} else if (keywords4.InList(s)) {
					sc.ChangeState(SCE_REBOL_WORD4);
				} else if (keywords5.InList(s)) {
					sc.ChangeState(SCE_REBOL_WORD5);
				} else if (keywords6.InList(s)) {
					sc.ChangeState(SCE_REBOL_WORD6);
				} else if (sc.Match('/')) {
					sc.ChangeState(SCE_REBOL_REFINEMENT);
				}
				// Keep same style if there are refinements
				if (!sc.Match('/') && !sc.Match(':')) {
					if(sc.state==SCE_REBOL_IDENTIFIER){
						sc.ChangeState(SCE_REBOL_DEFAULT);
					} else {
						sc.SetState(SCE_REBOL_DEFAULT);
					}
				}
			}
		}



		//--- Determine if the current state should terminate
		if (sc.state == SCE_REBOL_STRING && nestingLevel == 0) {
			if (sc.ch == '^') {
				ColouriseEscapedChar(sc);
				continue;
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
			}
		} else if ((sc.state == SCE_REBOL_STRING && nestingLevel > 0)|| sc.state == SCE_REBOL_COMMENT) {
			if (sc.ch == '^') {
				ColouriseEscapedChar(sc);
				continue;
			} else if (sc.ch == '}') {
				if (--nestingLevel == 0) {
					sc.ForwardSetState(SCE_REBOL_DEFAULT);
					if (blockComment) blockComment = 0;
				}
			} else if (sc.ch == '{') {
				nestingLevel++;
			}
		} else if (sc.state == SCE_REBOL_BINARY) {
			if (sc.ch == '}') {
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
			}
		} else if (sc.state == SCE_REBOL_PREFACE) {
			if (sc.MatchIgnoreCase("rebol")) {
				int i;
				for (i=5; IsASpaceOrTab(styler.SafeGetCharAt(sc.currentPos+i, 0)); i++);
				if (sc.GetRelative(i) == '[')
					sc.SetState(SCE_REBOL_DEFAULT);
			} else if (sc.MatchIgnoreCase("red/system")) {
				int i;
				for (i=10; IsASpaceOrTab(styler.SafeGetCharAt(sc.currentPos+i, 0)); i++);
				if (sc.GetRelative(i) == '[')
					sc.SetState(SCE_REBOL_DEFAULT);
			}
		} else if (sc.state == SCE_REBOL_TAG) {
			if (sc.ch == '>') {
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
				continue;
			} else if (sc.ch == '"') {
				sc.SetState(SCE_REBOL_TAGSTRING);
			}
			sc.Forward();
			continue;
		} else if (sc.state == SCE_REBOL_TAGSTRING) {
			if (sc.ch == '"') {
				sc.ForwardSetState(SCE_REBOL_TAG);
				continue;
			}
			sc.Forward();
			continue;
		}

		//--- Parens and bracket changes to default style when the current is a number
		if (sc.state == SCE_REBOL_OPERATOR || sc.state == SCE_REBOL_CHARACTER ||
			(sc.state >= SCE_REBOL_NUMBER && sc.state <= SCE_REBOL_QUESTION)
		) {
			if (sc.ch == '(' || sc.ch == '[' || sc.ch == ')' || sc.ch == ']' || sc.ch == ';') {
				sc.SetState(SCE_REBOL_DEFAULT);
			}
		} else if (sc.state == SCE_REBOL_SERIALIZED) {
			if (sc.ch == '[') {
				nestingLevel++;
			} else if (sc.ch == ']') {
				nestingLevel--;
				if (nestingLevel == 0) {
					sc.ForwardSetState(SCE_REBOL_DEFAULT);
					continue;
				}
			}
		}

		//--- Determine if a new state should be entered.
		
		if (sc.state == SCE_REBOL_DEFAULT) {
			
			int ch1 = sc.ch;
			int ch2 = sc.chNext;
			int ch3 = sc.GetRelative(2);
			int ch4 = sc.GetRelative(3);


			if (IsASpaceOrTab(ch1)) {
				while (sc.More() && IsASpaceOrTab(sc.ch)) sc.Forward();
				continue;
			} else if (IsAnOperator(ch1, ch2, ch3)) {
				sc.SetState(SCE_REBOL_OPERATOR);
			} else if (ch1 == '\"') {
				sc.SetState(SCE_REBOL_STRING);
			} else if (ch1 == '{') {
				sc.SetState(blockComment ? SCE_REBOL_COMMENT : SCE_REBOL_STRING);
				++nestingLevel;
			} else if (ch1 == '}') {
				sc.SetState(SCE_REBOL_INVALID);
			} else if (ch1 == '2' && ch2 == '#' && ch3 == '{' ) {
				//base-2 binary like 2#{0100 0111}
				sc.SetState(SCE_REBOL_BINARY);
				sc.Forward(3);
				unsigned int n = 0; //bits counter
				while (sc.More()) {
					if (sc.ch == '1' || sc.ch == '0') {
						n++;
					} else if (sc.ch == '}') {
						if (n % 8 != 0) sc.ChangeState(SCE_REBOL_INVALID);
						break;
					} else if (!(sc.ch == '\r' || sc.ch == '\n' || IsASpaceOrTab(sc.ch)) || !sc.More()){
						sc.ChangeState(SCE_REBOL_INVALID);
						break;
					}
					sc.Forward();
				}
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
			} else if (
				(ch1 == '#' && ch2 == '{') ||
				(ch1 == '1' && ch2 == '6' && ch3 == '#' && ch4 == '{')
				) {
				//base-16 binary like 16#{73} #{7400}
				sc.SetState(SCE_REBOL_BINARY);

				if (ch1 == '#') {
					sc.Forward(2);
				} else {
					sc.Forward(4);
				}
				unsigned int n = 0; //bytes counter
				while(sc.More()) {
					if (sc.ch == '}') {
						if (n % 2 != 0) sc.ChangeState(SCE_REBOL_INVALID);
						break;
					} else if (IsHexNumberChar(sc.ch)) {
						n++;
					} else if (!(sc.ch == '\r' || sc.ch == '\n' || IsASpaceOrTab(sc.ch)) || !sc.More()){
						sc.ChangeState(SCE_REBOL_INVALID);
						break;
					}
					sc.Forward();
				}
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
			} else if (ch1 == '6' && ch2 == '4' && ch3 == '#' && ch4 == '{') {
				//base-64 binary like 64#{LmNvbSA8yw9CB0aGvXmgUkVCu2Uz934b}
				sc.SetState(SCE_REBOL_BINARY);
				sc.Forward(4);
				while(sc.More()) {
					if (sc.ch == '}') {
						break;
					} else if (
						!sc.More() ||
						!(
							(sc.ch >= 'a'  && sc.ch <= 'z') ||
							(sc.ch >= 'A'  && sc.ch <= 'Z') ||
							(sc.ch >= '0'  && sc.ch <= '9') ||
							 sc.ch == '='  || sc.ch == '+'  || sc.ch == '/' ||
							 sc.ch == '\r' || sc.ch == '\n' || IsASpaceOrTab(sc.ch)
						)
					) {
						sc.ChangeState(SCE_REBOL_INVALID);
						break;
					}
					sc.Forward();
				}
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
			} else if( (ofs = isNumber(sc,0)) != 0) {
				if (ofs > 0) {
						sc.SetState(SCE_REBOL_NUMBER);
						sc.Forward(ofs);
						if (IsDelimiterChar(sc.ch)){
							sc.SetState(SCE_REBOL_DEFAULT);
						} else if (sc.ch == '.' && IsADigit(sc.chNext)) {
							//test if it's a valid tuple:
							ofs = -1;
							do {
								skip = isReversedTupleValue(sc, ofs);
								ofs += skip;
							} while (skip < 0);
							if (IsDelimiterChar(sc.GetRelative(ofs))){
								sc.ChangeState(SCE_REBOL_TUPLE);
								ofs = 1;
								do {
									skip = isTupleValue(sc, ofs);
									ofs += skip;
								} while (skip > 0);
								if (!IsDelimiterChar(sc.GetRelative(ofs))){
									sc.Forward(ofs);
									InvalidValue(sc);
								}
							} else {
								//sc.ChangeState(SCE_REBOL_INVALID);
								InvalidValue(sc);
							}
						} else if (sc.ch == 'x') {
							//test if it's valid pair:
							sc.ChangeState(SCE_REBOL_PAIR);
							sc.Forward();
							if ((ofs = isNumber(sc,0)) > 0) {
								sc.Forward(ofs);
								if (IsDelimiterChar(sc.ch)){
									sc.SetState(SCE_REBOL_DEFAULT);
									continue;
								} else {
									InvalidValue(sc);
								}
							} else {
								InvalidValue(sc);
							}
							
						} else if (sc.ch == '%') {
							if (IsDelimiterChar(sc.chNext)){
								sc.ForwardSetState(SCE_REBOL_DEFAULT);
								continue;
							} else {
								InvalidValue(sc);
							}
						} else if (sc.ch == ':') {
							//test if it's valid time value:
							ofs = isReversedTimeValue(sc, -1);
							if ( ofs == 0 ) {
								InvalidValue(sc);
							} else {
								sc.ChangeState(SCE_REBOL_DATETIME);
								sc.Forward();
								ColouriseTime(sc);
							}
						} else if (sc.ch == '-' || sc.ch == '/') {
							//test if it's valid date:
							if ( !IsValidDate(sc) ) {
								InvalidValue(sc);
							} else {
								sc.ChangeState(SCE_REBOL_DATETIME);
							}
						} else {
							InvalidValue(sc);
						}
						continue;
				} else if (ofs < 0) {
					sc.SetState(SCE_REBOL_INVALID);
					InvalidValue(sc);
				}
			} else if (ch1 == ':' && IsADigit(ch2)) { // :0:0
				sc.SetState(SCE_REBOL_DATETIME);
				sc.Forward();
				ColouriseTime(sc);
			} else if ( (ch1 == '+' || ch1 == '-') && ch2 == ':' && IsADigit(ch3) ){ // +:0:0
				sc.SetState(SCE_REBOL_DATETIME);
				sc.Forward(2);
				ColouriseTime(sc);
			} else if (ch1 == '\'') {
				if (IsAWordStart(ch2, ch3)) {
					sc.SetState(SCE_REBOL_LITWORD);
				} else {
					sc.SetState(SCE_REBOL_INVALID);
					InvalidValue(sc);
				}
			}  else if (ch1 == ':') {
				if (IsAWordStart(ch2, ch3)) {
					sc.SetState(SCE_REBOL_SETWORD);
				} else if (sc.chPrev==')') {
					sc.SetState(SCE_REBOL_SETWORD);
					sc.ForwardSetState(SCE_REBOL_DEFAULT);
					continue;
				} else {
					sc.SetState(SCE_REBOL_INVALID);
					InvalidValue(sc);
				}
			} else if (ch1 == '/') {
				if(IsFollowSlashChar(sc.chNext)){
					sc.SetState(SCE_REBOL_DEFAULT);
					sc.Forward();
					continue;
				} else {
					sc.SetState(SCE_REBOL_REFINEMENT);
				}
			} else if (IsAWordStart(ch1, ch2)) {
				sc.SetState(SCE_REBOL_IDENTIFIER);
			} else if (ch1 == ';') {
				sc.SetState(SCE_REBOL_COMMENT);
				sc.Forward();
				while( sc.More() && !(sc.chNext == '\r' || sc.chNext == '\n')) {
					sc.Forward();
				}
				sc.ForwardSetState(SCE_REBOL_DEFAULT);
				continue;
			} else if (ch1 == '$') {
				sc.SetState(SCE_REBOL_MONEY);
				ColouriseMoney(sc);
				continue;
			} else if (ch1 == '%') {
				sc.SetState(SCE_REBOL_FILE);
				if (ch2 == '\"') {
					sc.Forward();
					while (!(sc.chNext == '\r' || sc.chNext == '\n' || sc.chNext=='\"')) {
						sc.Forward();
					}
					if (sc.chNext=='\"') {
						sc.Forward();
						sc.ForwardSetState(SCE_REBOL_DEFAULT);
					} else {
						sc.ChangeState(SCE_REBOL_INVALID);
					}
				} else if (IsDelimiterChar(sc.chNext)) {
					sc.ChangeState(SCE_REBOL_INVALID);
					sc.ForwardSetState(SCE_REBOL_DEFAULT);
				} else {
					while (!IsDelimiterChar(sc.chNext)) {
						sc.Forward();
					}
					sc.ForwardSetState(SCE_REBOL_DEFAULT);
				}
				continue;
			} else if (ch1 == '<') {
				sc.SetState(SCE_REBOL_TAG);
				/*
				while (sc.More()) {
					if( sc.ch == '"') {
						if(!ColouriseTagString(sc)){
							InvalidValue(sc);
							break;
						} else {
							continue;
						}
					} else if (sc.ch == '>') {
						sc.ForwardSetState(SCE_REBOL_DEFAULT);
						break;
					}
					sc.Forward();
				}*/
				
			} else if (ch1 == '#') {
				if (ch2 == '"') {
					sc.SetState(SCE_REBOL_CHARACTER);
					sc.Forward(2);
					if (sc.ch == '^') {
						ColouriseEscapedChar(sc);
						if (sc.ch == '\"') {
							sc.ForwardSetState(SCE_REBOL_DEFAULT);
						} else {
							sc.ChangeState(SCE_REBOL_INVALID);
						}
						continue;
					} else if (sc.ch == '\"') { // #""
						sc.ForwardSetState(SCE_REBOL_DEFAULT); 
					} else if (sc.chNext == '\"') {
						sc.Forward();
						sc.ForwardSetState(SCE_REBOL_DEFAULT); 
					} else {
						sc.ChangeState(SCE_REBOL_INVALID);
					}
					continue;

				} else if (sc.chNext == '[') {
					//SERIALIZED VALUE like #[none] (can be also multiline!)
					sc.SetState(SCE_REBOL_SERIALIZED);
					sc.Forward();
					nestingLevel++;
				} else if (sc.chNext != '"' && sc.chNext != '{' ) {
					sc.SetState(SCE_REBOL_ISSUE);
				}
			} else { //if (ch1 == '@' || ch1 == '>') {
				sc.SetState(SCE_REBOL_INVALID);
				InvalidValue(sc);
			} 
		}
		sc.Forward();
	}
	sc.Complete();
}


static void FoldRebolDoc(unsigned int startPos, int length, int /* initStyle */, WordList *[],
                            Accessor &styler) {
	unsigned int lengthDoc = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int levelCurrent = levelPrev;
	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	for (unsigned int i = startPos; i < lengthDoc; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		int style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (style == SCE_REBOL_DEFAULT) {
			if (ch == '[') {
				levelCurrent++;
			} else if (ch == ']') {
				levelCurrent--;
			}
		}
		if (atEOL) {
			int lev = levelPrev;
			if (visibleChars == 0)
				lev |= SC_FOLDLEVELWHITEFLAG;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
		if (!isspacechar(ch))
			visibleChars++;
	}
	// Fill in the real level of the next line, keeping the current flags as they will be filled in later
	int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel(lineCurrent, levelPrev | flagsNext);
}

static const char * const rebolWordListDesc[] = {
	"Function definitions",
	"IO functions",
	"Control structures",
	"words 3",
	"words 4",
	"words 5",
	"words 6",
	0
};

LexerModule lmREBOL(SCLEX_REBOL, ColouriseRebolDoc, "rebol", FoldRebolDoc, rebolWordListDesc);

