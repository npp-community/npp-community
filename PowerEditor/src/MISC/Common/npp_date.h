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

#ifndef NPP_DATE
#define NPP_DATE

class Date
{
public:
	Date() : _year(2008), _month(4), _day(26){}
	Date(unsigned long year, unsigned long month, unsigned long day);

	Date(const TCHAR *dateStr);

	// The constructor which makes the date of number of days from now
	// nbDaysFromNow could be negative if user want to make a date in the past
	// if the value of nbDaysFromNow is 0 then the date will be now
	Date(int nbDaysFromNow);

	void now();

	generic_string toString();

	bool operator<(const Date & compare) const;
	bool operator>(const Date & compare) const;
	bool operator==(const Date & compare) const;
	bool operator!=(const Date & compare) const;

private:
	unsigned long _year;
	unsigned long _month;
	unsigned long _day;
};

#endif // NPP_DATE
