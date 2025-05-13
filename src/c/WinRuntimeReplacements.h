/****************************************************************************************

File		WinRuntimeReplacements.h
Why:		Functions to replace the standard Windows runtime libraries.
OS:			Windows
Created:	2024-04-08

History
-------

When		Who				What
-----------------------------------------------------------------------------------------
2024-04-08	Thomas			Created.

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

#ifndef WINRUNTIMEREPLACEMENTS_H
#define WINRUNTIMEREPLACEMENTS_H

#include <stdbool.h>
#include "./externC.h"

#ifndef ASSERT
#define ASSERT(x)	_ASSERT(x)
#endif
#ifndef assert
#define assert(x)	_ASSERT(x)
#endif

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif
#ifndef DEBUG
#ifndef RELEASE
#define RELEASE
#endif
#endif

#ifndef USE_STRLEN
#define USE_STRLEN							((size_t) -1)
#endif

#ifndef UBF_UINT64_LEN
#define UBF_UINT64_LEN						(20)
#endif
#ifndef UBF_UINT64_SIZ
#define UBF_UINT64_SIZ						(UBF_UINT64_LEN + 1)
#endif

EXTERN_C_BEGIN

/*
*/
bool isDigitW (WCHAR wc)
;

/*
*/
bool isNotDigitW (WCHAR wc)
;

/*
	memcpyU
	memcmpU

	Because we are not using the run-time library to obtain a small executable size we
	got to provide our own standard functions.
*/
void *memcpyU (void *dest, const void *src, size_t len)
;
int memcmpU (const void *str1, const void *str2, size_t count)
;

/*
	stricmpW

	Case insensitive string comparison.
*/
int stricmpW (const unsigned short *wc1, const unsigned short *wc2, size_t count)
;

/*
	strlenW

	Returns the length of the NUL terminated wide character (UTF-16) string ch.
*/
DWORD strlenW (const WCHAR *ch)
;

/*
	strlenU

	Returns the length of the NUL terminated string sz.
*/
DWORD strlenU (const char *sz)
;

/*
	strrchrW

	Our strrchr () for WCHARs. Scans a string for the last occurrence of a character.
*/
WCHAR *strrchrW (WCHAR *pwc, WCHAR wc)
;

/*
	Returns wc as an upper case character.
*/
WCHAR toupperW (WCHAR wc)
;

/*
	isWhiteSpaceW

	Returns true if wc is considered white space, false otherwise.
*/
bool isWhiteSpaceW (WCHAR wc)
;

/*
	nTrailingWhiteSpaceW

	Counts the amount of trailing white space characters.
*/
size_t nTrailingWhiteSpaceW (WCHAR *pwc)
;

/*
	ubf_uint64_from_strW

*/
bool ubf_uint64_from_strW (uint64_t * ui, const WCHAR *wcStr)
;

/*
	WCHAR version of ubf_str_from_uint64 ().

	Returns an ASCII representation of the value of ui64, in decimal (base 10). The
	buffer result points to must be at least UBF_UINT64_SIZ WCHARs long, which is
	UBF_UINT64_LEN + 1, or 21.

	The function returns the amount of digits written to result, not counting the
	terminating NUL character.
*/
size_t wstr_from_uint64 (WCHAR* result, uint64_t ui64)
;

EXTERN_C_END

#endif														// Of #ifndef WINRUNTIMEREPLACEMENTS_H.
