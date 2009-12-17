//this file is part of Notepad++
//Copyright (C)2008 Harry Bruin <harrybharry@users.sourceforge.net>
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
#include "FunctionCallTip.h"
#include "ScintillaEditView.h"

#include "tinyxml.h"

struct Token {
	// JOCE: move to generic_string
	TCHAR * token;
	int length;
	bool isIdentifier;
	Token(TCHAR * tok, int len, bool isID) : token(tok), length(len), isIdentifier(isID) {};
};

struct FunctionValues {
	int lastIdentifier;
	int lastFunctionIdentifier;
	int param;
	int scopeLevel;
	FunctionValues() : lastIdentifier(-1), lastFunctionIdentifier(-1), param(0), scopeLevel(-1) {};
};

void FunctionCallTip::setLanguageXML(TiXmlElement * pXmlKeyword) {
	if (isVisible())
		close();
	_pXmlKeyword = pXmlKeyword;
}

bool FunctionCallTip::updateCalltip(int ch, bool needShown) {
	if (!needShown && ch != _start && !isVisible())		//must be already visible
		return false;

	_curPos = _pEditView->execute(SCI_GETCURRENTPOS);

	//recalculate everything
	if (!getCursorFunction()) {	//cannot display calltip (anymore)
		close();
		return false;
	}
	showCalltip();
	return true;
}

void FunctionCallTip::showNextOverload() {
	if (!isVisible())
		return;
	_currentOverload = (_currentOverload+1) % _currentNrOverloads;
	showCalltip();
}

void FunctionCallTip::showPrevOverload() {
	if (!isVisible())
		return;
	_currentOverload = _currentOverload > 0?(_currentOverload-1) : _currentNrOverloads-1;
	showCalltip();
}

void FunctionCallTip::close() {
	if (!isVisible()) {
		return;
	}

	_pEditView->execute(SCI_CALLTIPCANCEL);
	_currentOverload = 0;
}

// Strange things are happening to the loop index variable, but I'm not touching this parsing code with a 10 foot pole.
// for loop index variable ’Symbol’ whose type category is ’String’modified in body of the for loop
//lint -e850
bool FunctionCallTip::getCursorFunction()
{
	int line = _pEditView->execute(SCI_LINEFROMPOSITION, _curPos);
	int startpos = _pEditView->execute(SCI_POSITIONFROMLINE, line);
	int endpos = _pEditView->execute(SCI_GETLINEENDPOSITION, line);
	int len = endpos - startpos + 3;	//also take CRLF in account, even if not there
	int offset = _curPos - startpos;	//offset is cursor location, only stuff before cursor has influence
	const int maxLen = 128;

	if ((offset < 2) || (len >= maxLen))
	{
		reset();
		return false;	//cannot be a func, need name and separator
	}

	TCHAR lineData[maxLen] = TEXT("");

	_pEditView->getLine(line, lineData, len);

	//line aquired, find the functionname
	//first split line into tokens to parse
	//token is identifier or some expression, whitespace is ignored
	std::vector< Token > tokenVector;
	int tokenLen = 0;
	TCHAR ch;
	for (int i = 0; i < offset; i++) 	//we dont care about stuff after the offset
    {
		//tokenVector.push_back(pair(lineData+i, len));
		ch = lineData[i];
		if (isBasicWordChar(ch) || isAdditionalWordChar(ch))	//part of identifier
        {
			tokenLen = 0;
			TCHAR * begin = lineData+i;
            while ((isBasicWordChar(ch) || isAdditionalWordChar(ch)) && i < offset) {
				tokenLen++;
				i++;
				ch = lineData[i];
			}
			tokenVector.push_back(Token(begin, tokenLen, true));
			i--;	//correct overshooting of while loop
		}
        else
        {
			if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') 	//whitespace
            {
				//do nothing
			}
            else
            {
				tokenLen = 1;
				tokenVector.push_back(Token(lineData+i, tokenLen, false));
			}
		}
	}

	int vsize = (int)tokenVector.size();
	//mind nested funcs, like |blblb a (x, b(), c);|
	//therefore, use stack
	std::vector<FunctionValues> valueVec;

	FunctionValues curValue, newValue;
	int scopeLevel = 0;
	for (int i = 0; i < vsize; i++) {
		Token & curToken = tokenVector.at(i);
		if (curToken.isIdentifier) {
			curValue.lastIdentifier = i;
		} else {
			if (curToken.token[0] == _start) {
				scopeLevel++;
				newValue = curValue;
				valueVec.push_back(newValue);	//store the current settings, so when this new function doesnt happen to be the 'real' one, we can restore everything

				curValue.scopeLevel = scopeLevel;
				if (i > 0 && curValue.lastIdentifier == i-1) {	//identifier must be right before (, else we have some expression like "( x + y() )"
					curValue.lastFunctionIdentifier = curValue.lastIdentifier;
					curValue.param = 0;
				} else {	//some expression
					curValue.lastFunctionIdentifier = -1;
				}
			} else if (curToken.token[0] == _param && curValue.lastFunctionIdentifier > -1) {
				curValue.param++;
			} else if (curToken.token[0] == _stop) {
				if (scopeLevel)	//scope cannot go below -1
					scopeLevel--;
				if (valueVec.size() > 0) {	//only pop level if scope was of actual function
					curValue = valueVec.back();
					valueVec.pop_back();
				} else {
					//invalidate curValue
					curValue = FunctionValues();
				}
			} else if (curToken.token[0] == _terminal) {
				//invalidate everything
				valueVec.clear();
				curValue = FunctionValues();
			}
		}
	}

	bool res = false;

	if (curValue.lastFunctionIdentifier == -1) {	//not in direct function. Start popping the stack untill we empty it, or a func IS found
		while(curValue.lastFunctionIdentifier == -1 && valueVec.size() > 0) {
			curValue = valueVec.back();
			valueVec.pop_back();
		}
	}
	if (curValue.lastFunctionIdentifier > -1) {
		Token funcToken = tokenVector.at(curValue.lastFunctionIdentifier);
		funcToken.token[funcToken.length] = 0;
		_currentParam = curValue.param;

		bool same = false;
		if(_ignoreCase)
			same = CompareNoCase(_funcName, funcToken.token) == 0;
		else
			same = (_funcName == funcToken.token);

		if (!same) {	//check if we need to reload data
			_funcName = funcToken.token;
			res = loadFunction();
		} else {
			res = true;
		}
	}
	return res;
}
//lint +e850

/*
Find function in XML structure and parse it
*/
bool FunctionCallTip::loadFunction() {
	reset();	//set everything back to 0
	//The functions should be ordered, but linear search because we cant access like array
	_curFunction = NULL;
	//Iterate through all keywords and find the correct function keyword
	TiXmlElement *funcNode = _pXmlKeyword;
	const TCHAR * name = NULL;
	for (; funcNode; funcNode = funcNode->NextSiblingElement(TEXT("KeyWord")) ) {
		name = funcNode->Attribute(TEXT("name"));
		if (!name)		//malformed node
			continue;
		int compVal = 0;
		if (_ignoreCase)
			compVal = CompareNoCase(name, _funcName.c_str());	//lstrcmpi doesnt work in this case
		else
			compVal = (_funcName == name);

		if (!compVal)	//found it?
		{
			const TCHAR* attrib = funcNode->Attribute(TEXT("func"));
			generic_string val = attrib ? attrib : TEXT("");
			if (val == TEXT("yes"))
			{
				//what we've been looking for
				_curFunction = funcNode;
				break;
			}
			else
			{
				//name matches, but not a function, abort the entire procedure
				return false;
			}
		}
	}

	//Nothing found
	if (!_curFunction)
		return false;

	stringVec paramVec;

	TiXmlElement *overloadNode = _curFunction->FirstChildElement(TEXT("Overload"));
	TiXmlElement *paramNode = NULL;
	for (; overloadNode ; overloadNode = overloadNode->NextSiblingElement(TEXT("Overload")) ) {
		const TCHAR * retVal = overloadNode->Attribute(TEXT("retVal"));
		if (!retVal)
			continue;	//malformed node
		_retVals.push_back(retVal);

		const TCHAR * description = overloadNode->Attribute(TEXT("descr"));
		if (description)
			_descriptions.push_back(description);
		else
			_descriptions.push_back(TEXT(""));	//"no description available"

		paramNode = overloadNode->FirstChildElement(TEXT("Param"));
		for (; paramNode ; paramNode = paramNode->NextSiblingElement(TEXT("Param")) ) {
			const TCHAR * param = paramNode->Attribute(TEXT("name"));
			if (!param)
				continue;	//malformed node
			paramVec.push_back(param);
		}
		_overloads.push_back(paramVec);
		paramVec.clear();

		_currentNrOverloads++;
	}

	_currentNrOverloads = (int)_overloads.size();

	if (_currentNrOverloads == 0)	//malformed node
		return false;

	return true;
}

void FunctionCallTip::showCalltip()
{
	if (_currentNrOverloads == 0)
    {
		//ASSERT
		return;
	}

	//Check if the current overload still holds. If the current param exceeds amount in overload, see if another one fits better (enough params)
	stringVec & params = _overloads.at(_currentOverload);
	size_t psize = params.size()+1, osize;
	if ((size_t)_currentParam >= psize) {
		osize = _overloads.size();
		for(size_t i = 0; i < osize; i++) {
			psize = _overloads.at(i).size()+1;
			if ((size_t)_currentParam < psize) {
				_currentOverload = i;
				break;
			}
		}
	}
	generic_string curRetValText = _retVals.at(_currentOverload);
	generic_string curDescriptionText = _descriptions.at(_currentOverload);
	bool hasDescr = true;
	if (!curDescriptionText[0])
		hasDescr = false;

	int nrParams = (int)params.size();

	std::wostringstream stream;

	if (_currentNrOverloads > 1) {
		stream << TEXT("\001") << _currentOverload+1 << TEXT(" of ") << _currentNrOverloads << TEXT("\002");
	}

	stream << curRetValText << TEXT(" ") << _funcName << TEXT(" (");

	int highlightstart = 0;
	int highlightend = 0;
	for(int i = 0; i < nrParams; i++) {
		if (i == _currentParam) {
			highlightstart = stream.str().length();
			highlightend = highlightstart + params.at(i).length();
		}
		stream << params.at(i);
		if (i < nrParams-1) {
			stream << TEXT(", ");
		}
	}

	stream << TEXT(")");
	if (hasDescr) {
		stream << TEXT("\n") << curDescriptionText;
	}

	if (isVisible())
		_pEditView->execute(SCI_CALLTIPCANCEL);
	else
		_startPos = _curPos;
	_pEditView->showCallTip(_startPos, stream.str().c_str());

	if (highlightstart != highlightend) {
		_pEditView->execute(SCI_CALLTIPSETHLT, highlightstart, highlightend);
	}
}

void FunctionCallTip::reset() {
	_currentOverload = 0;
	_currentParam = 0;
	//_curPos = 0;
	_startPos = 0;
	_overloads.clear();
	_currentNrOverloads = 0;
	_retVals.clear();
	_descriptions.clear();
}

void FunctionCallTip::cleanup() {
	reset();
	_pEditView = NULL;
}

bool FunctionCallTip::isVisible()
{
	return _pEditView?_pEditView->execute(SCI_CALLTIPACTIVE) == TRUE:false;
}
