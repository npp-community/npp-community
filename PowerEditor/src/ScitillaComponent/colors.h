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

#ifndef COLORS_H
#define COLORS_H

#define red			            (RGB(0xFF,    0,    0))
#define darkRed                 (RGB(0x80,    0,    0))
#define offWhite		        (RGB(0xFF, 0xFB, 0xF0))
#define darkGreen	            (RGB(0,    0x80,    0))
#define liteGreen	            (RGB(0,    0xFF,    0))
#define blueGreen	            (RGB(0,    0x80, 0x80))
#define liteRed					(RGB(0xFF, 0xAA, 0xAA))
#define liteBlueGreen			(RGB(0xAA, 0xFF, 0xC8))

#define liteBlue		        (RGB(0xA6, 0xCA, 0xF0))
#define veryLiteBlue            (RGB(0xC4, 0xF9, 0xFD))
#define extremeLiteBlue         (RGB(0xF2, 0xF4, 0xFF))

#define darkBlue  	            (RGB(0,       0, 0x80))
#define blue      	            (RGB(0,       0, 0xFF))
#define black     	            (RGB(0,       0,    0))
#define white     	            (RGB(0xFF, 0xFF, 0xFF))
#define darkGrey      	        (RGB(64,     64,   64))
#define grey      	            (RGB(128,   128,  128))
#define liteGrey  	            (RGB(192,   192,  192))
#define veryLiteGrey  	        (RGB(224,   224,  224))
#define brown     	            (RGB(128,    64,    0))
#define greenBlue 	            (RGB(192,   128,   64))
#define darkYellow				(RGB(0xFF, 0xC0,    0))
#define yellow    	            (RGB(0xFF, 0xFF,    0))
#define lightYellow				(RGB(0xFF, 0xFF, 0xD5))
#define cyan      	            (RGB(0,    0xFF, 0xFF))
#define orange		            (RGB(0xFF, 0x80, 0x00))
#define purple		            (RGB(0x80, 0x00, 0xFF))
#define deepPurple	            (RGB(0x87, 0x13, 0x97))

#define extremeLitePurple       (RGB(0xF8, 0xE8, 0xFF))
#define veryLitePurple          (RGB(0xE7, 0xD8, 0xE9))
#define liteBerge				(RGB(0xFE, 0xFC, 0xF5))
#define berge					(RGB(0xFD, 0xF8, 0xE3))
/*
#define RGB2int(color)
    (((((long)color) & 0x0000FF) << 16) | ((((long)color) & 0x00FF00)) | ((((long)color) & 0xFF0000) >> 16))
*/
#endif //COLORS_H

