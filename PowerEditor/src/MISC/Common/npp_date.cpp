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
#include "npp_date.h"

Date::Date( unsigned long year, unsigned long month, unsigned long day )
{
	assert(year > 0 && year <= 9999); // I don't think Notepad++ will last till AD 10000 :)
	assert(month > 0 && month <= 12);
	assert(day > 0 && day <= 31);
	assert(!(month == 2 && day > 29) &&
		!(month == 4 && day > 30) &&
		!(month == 6 && day > 30) &&
		!(month == 9 && day > 30) &&
		!(month == 11 && day > 30));

	_year = year;
	_month = month;
	_day = day;
}

Date::Date( const TCHAR *dateStr )
{
	// timeStr should be Notepad++ date format : YYYYMMDD
	assert(dateStr);
	if (lstrlen(dateStr) == 8)
	{
		generic_string ds(dateStr);
		generic_string yyyy(ds, 0, 4);
		generic_string mm(ds, 4, 2);
		generic_string dd(ds, 6, 2);

		int y = generic_atoi(yyyy.c_str());
		int m = generic_atoi(mm.c_str());
		int d = generic_atoi(dd.c_str());

		if ((y > 0 && y <= 9999) && (m > 0 && m <= 12) && (d > 0 && d <= 31))
		{
			_year = y;
			_month = m;
			_day = d;
			return;
		}
	}
	now();
}

Date::Date( int nbDaysFromNow )
{
	const time_t oneDay = (60 * 60 * 24);

	time_t rawtime;
	tm timeinfo;

	time(&rawtime);
	rawtime += (nbDaysFromNow * oneDay);

	localtime_s(&timeinfo, &rawtime);

	_year = timeinfo.tm_year+1900;
	_month = timeinfo.tm_mon+1;
	_day = timeinfo.tm_mday;
}

void Date::now()
{
	time_t rawtime;
	tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	_year = timeinfo.tm_year+1900;
	_month = timeinfo.tm_mon+1;
	_day = timeinfo.tm_mday;
}

generic_string Date::toString()
{
	// Return Notepad++ date format : YYYYMMDD
	TCHAR dateStr[8+1];
	wsprintf(dateStr, TEXT("%04d%02d%02d"), _year, _month, _day);
	return dateStr;
}

bool Date::operator<( const Date & compare ) const
{
	if (this->_year != compare._year)
		return (this->_year < compare._year);
	if (this->_month != compare._month)
		return (this->_month < compare._month);
	return (this->_day < compare._day);
}

bool Date::operator>( const Date & compare ) const
{
	if (this->_year != compare._year)
		return (this->_year > compare._year);
	if (this->_month != compare._month)
		return (this->_month > compare._month);
	return (this->_day > compare._day);
}

bool Date::operator==( const Date & compare ) const
{
	return this->_year == compare._year &&
		   this->_month == compare._month &&
		   this->_day == compare._day;
}

bool Date::operator!=( const Date & compare ) const
{
	return this->_year != compare._year ||
		   this->_month != compare._month ||
		   this->_day != compare._day;
}
