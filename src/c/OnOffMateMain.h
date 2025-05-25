/****************************************************************************************

	File		OnOffMateMain.h
	Why:		Main header file.
	OS:			Windows
	Created:	2024-04-07

History
-------

When		Who				What
-----------------------------------------------------------------------------------------
2024-04-07	Thomas			Created.

****************************************************************************************/

/*
	This file is maintained as part of OnOffMate. See https://github.com/ThomasPGH/OnOffMate .
*/

/*
	This code is covered by the MIT License. See https://opensource.org/license/mit .

	Copyright (c) 2024, 2025 Thomas

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify,
	merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be included in all copies
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ONOFFMATEMAIN_H
#define ONOFFMATEMAIN_H

#ifdef _DEBUG
	#ifndef DEBUG
	#define DEBUG
	#endif
#endif

#define LENOFSTR(str) (sizeof (str) - 1)

#include "./externC.h"

#ifndef ONOFFMATE_VERSION_STRING
#define ONOFFMATE_VERSION_STRING		"1.002"
#endif
#ifndef ONOFFMATE_VERSION_DATEST
#define ONOFFMATE_VERSION_DATEST		"2025-05-25"
#endif
#ifndef ONOFFMATE_VERSION_STRTOT
#define ONOFFMATE_VERSION_STRTOT		"OnOffMate - Ver. " ONOFFMATE_VERSION_STRING " (" ONOFFMATE_VERSION_DATEST ")"
#endif

BEGIN_C_DECLS

// ...

END_C_DECLS

#endif														// Of #ifndef ONOFFMATEMAIN_H.
