/****************************************************************************************

File		WinRuntimeReplacements.c
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
	This file is maintained as part of OnOffMate. See https://github.com/thomaspgh/onoffmate .
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


	The function wstr_from_uint64 () is (c) Lukas Chmela and derived from the versions
	at http://www.strudel.org.uk/itoa/ :

	* C++ version 0.4 char* style "itoa":
	* Written by Lukas Chmela
	* Released under GPLv3.

*/

#include <Windows.h>
#include <stdint.h>
#include "./WinRuntimeReplacements.h"

bool isDigitW (WCHAR wc)
{
	return wc >= L'0' && wc <= L'9';
}

bool isNotDigitW (WCHAR wc)
{
	return wc < L'0' || wc > L'9';
}

void *memcpyU (void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;

	while (len--)
	{
		*d++ = *s++;
	}
	return dest;
}

int memcmpU (const void *str1, const void *str2, size_t count)
{
	register const unsigned char *s1 = (const unsigned char*) str1;
	register const unsigned char *s2 = (const unsigned char*) str2;

	while (count-- > 0)
	{
		if (*s1++ != *s2++)
			return s1[-1] < s2[-1] ? -1 : 1;
	}
	return 0;
}

int stricmpW (const unsigned short *wc1, const unsigned short *wc2, size_t count)
{
	register const unsigned short *s1 = wc1;
	register const unsigned short *s2 = wc2;

	while (count-- > 0)
	{
		if (toupperW (*s1++) != toupperW (*s2++))
			return toupperW (s1[-1]) < toupperW (s2[-1]) ? -1 : 1;
	}
	return 0;
}

DWORD strlenW (const WCHAR *ch)
{
	DWORD	dwLen = 0;

	while (*ch)
	{
		++ ch;
		++ dwLen;
	}
	return dwLen;
}

DWORD strlenU (const char *sz)
{
	DWORD	dwLen = 0;

	while (*sz)
	{
		++ sz;
		++ dwLen;
	}
	return dwLen;
}

WCHAR *strrchrW (WCHAR *pwc, WCHAR wc)
{
	DWORD	dwLen	= strlenW (pwc);
	WCHAR	*wco	= pwc + dwLen;

	while (dwLen --)
	{
		-- wco;
		if (wc == *wco)
		{
			return wco;
		}
	}
	return NULL;
}
/*
	See
	https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strrchr-wcsrchr-mbsrchr-mbsrchr-l?view=msvc-170 .
	This lets us re-use swallowExeArg () from other projects.
*/
#define wcsrchr(p, w)	wrchr (p, w)

/*
	Converts wc to upper case.
*/
WCHAR toupperW (WCHAR wc)
{
	if ((unsigned) wc >= (unsigned) L'a' && (unsigned) wc <= (unsigned) L'z')
		return (unsigned) wc - (unsigned) L'a' + (unsigned) L'A';
	return wc;
}

bool isWhiteSpaceW (WCHAR wc)
{
	return	(
		L' ' == wc || L'\t' == wc || L'\v' == wc || L'\f' == wc || L'\n' == wc || L'\r' == wc
			);
}

size_t nTrailingWhiteSpaceW (WCHAR *pwc)
{
	size_t l = strlenW (pwc);
	size_t r = 0;

	while (l && isWhiteSpaceW (pwc [l - 1]))
	{
		++ r;
		-- l;
	}
	return r;
}

bool ubf_uint64_from_strW (uint64_t * ui, const WCHAR *wcStr)
{
	uint64_t		u		= 0;
	const WCHAR		*ch;
	WCHAR			c;

	ch = (const WCHAR *) wcStr;
	if (ch)
	{
		c = *ch;
		if (L'+' == c)
		{
			ch ++;
			c = *ch;
		}
		while (c)
		{
			if (isNotDigitW (c))
				break;
			c -= L'0';
			if (u > UINT64_MAX / 10)
				return FALSE;
			u *= 10;
			if (u > UINT64_MAX - c)
				return FALSE;
			u += c;
			ch ++;
			c = *ch;
		}
	}
	if (ui)
		*ui = u;
	return TRUE;
}

/*
	See http://www.strudel.org.uk/itoa/ .
	2018-04-09, Thomas.
*/
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukas Chmela
 * Released under GPLv3.

 */

WCHAR	wc_ito_alphabet []	=
		L"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

size_t wstr_from_uint64 (WCHAR* result, uint64_t ui64)
{
	WCHAR*		ptr			= result, *ptr1 = result, tmp_char;
	uint64_t	tmp_value;
	WCHAR		*r;

	do {
		tmp_value = ui64;
		ui64 /= 10;
		*ptr++ = wc_ito_alphabet [35 + (tmp_value - ui64 * 10)];
	} while (ui64);
	r = ptr;
	*ptr-- = L'\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return ((size_t) (r - result));
}
