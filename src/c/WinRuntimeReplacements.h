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

#include <Windows.h>
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
#define USE_STRLEN					((size_t) -1)
#endif

#ifndef UBF_UINT64_LEN
#define UBF_UINT64_LEN				(20)
#endif
#ifndef UBF_UINT64_SIZ
#define UBF_UINT64_SIZ				(UBF_UINT64_LEN + 1)
#endif
#ifndef UBF_INVALID_HEX_CHAR
#define UBF_INVALID_HEX_CHAR		((unsigned char) -1)
#endif

#define ubf_assert(x)

EXTERN_C_BEGIN

/*
*/
bool isDigitU (unsigned char c)
;

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
	memsetU
	memcmpU

	Because we are not using the run-time library to obtain a small executable size we
	got to provide our own standard functions.
*/
void *memcpyU (void *dest, const void *src, size_t len);
void *memsetU (void *dest, uint8_t ch, size_t len);
int memcmpU (const void *str1, const void *str2, size_t count);

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
size_t strlenW (const WCHAR *ch)
;

/*
	strlenU

	Returns the length of the NUL terminated string sz.
*/
size_t strlenU (const char *sz)
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

/*
	asc_hex_from_dword_W

	Writes the 8 characters representing the hexadecimal notation of the 32 bit data
	ui to the location pc points to. The function writes exactly 8 ASCII characters
	to pc. No NUL terminator is written.
*/
void asc_hex_from_dword_W (WCHAR *pc, uint32_t ui)
;

/*
	ubf_value_of_ASCII_hex
	
	Returns the value of the hexadecimal character in c. The function returns
	UBF_INVALID_HEX_CHAR, which is defined as (unsigned char) -1 if c does not
	contain a valid hexadecimal character.
	
	Valid hexadecimal characters are the digits "0" to "9", and the letters
	"A" to "F" or "a" to "f".
	"0" returns 0.
	"1" returns 1.
	...
	"9" returns 9.
	"A" returns 10.
	...
	"F" returns 15.
	"a" returns 10.
	...
	"f" returns 15.
	Anything else returns UBF_INVALID_HEX_CHAR, which is defined as (unsigned char) -1.
*/
#ifndef ubf_value_of_ASCII_hex
	#define ubf_value_of_ASCII_hex_macro(c)				\
		(												\
			(unsigned char) (c) >= '0' && (unsigned char) (c) <= '9' ? (unsigned char) (c) - '0' :\
			(unsigned char) (c) >= 'a' && (unsigned char) (c) <= 'f' ? (unsigned char) (c) - 'a' + 10 :\
			(unsigned char) (c) >= 'A' && (unsigned char) (c) <= 'F' ? (unsigned char) (c) - 'A' + 10 :\
			UBF_INVALID_HEX_CHAR						\
		)
	#define ubf_value_of_ASCII_hex(c)	ubf_value_of_ASCII_hex_macro (c)
#endif

/*
	ubf_octet_from_hex
	
	Reads up to two hexadecimal characters from chHx and writes the resulting
	binary octet to the location o points to.
	
	The function expects that it can read up to two characters from chHx.
	It does not perform a NUL check.
		
	The function returns the amount of hexadecimal characters that were
	read and used from the location chHx points to to produce the octet
	stored at o, which can be either 0 (if the first character is not a hex
	character), 1 (if only the first character is a hexadecimal character),
	or 2 (if both characters are hexadecimal).
*/
size_t ubf_octet_from_hex (unsigned char *o, const char *chHx);

/*
*/
int reqUTF8size (const WCHAR *wcU16)
;

/*
*/
int UTF8_from_WinU16 (char *chU8, int sizeU8, const WCHAR *wcU16)
;

/*
*/
int UTF8_from_WinU16l (char *chU8, int sizeU8, const WCHAR *wcU16, int lenU16)
;

EXTERN_C_END

#endif														// Of #ifndef WINRUNTIMEREPLACEMENTS_H.
