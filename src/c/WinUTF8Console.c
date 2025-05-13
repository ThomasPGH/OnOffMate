/****************************************************************************************

File		WinUTF8Console.c
Why:		UTF-8 Windows console functions.
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

#include <Windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#include <stdint.h>
#include "./WinUTF8Console.h"
#include "./WinRuntimeReplacements.h"

bool SetCodePageToUTF8 (void)
{
	if (IsValidCodePage (CP_UTF8))
	{
		BOOL b;
		
		b = SetConsoleCP (CP_UTF8);
		b &= SetConsoleOutputCP (CP_UTF8);
		return (bool) b;
	}
	return false;
}

void consoleOutW (const WCHAR *wcText)
{
	DWORD	len		= strlenW (wcText);
	HANDLE	hstdout	= GetStdHandle (STD_OUTPUT_HANDLE);

	#ifdef _DEBUG
		BOOL b = WriteConsoleW (hstdout, wcText, len, NULL, NULL);
	#else
		WriteConsoleW (hstdout, wcText, len, NULL, NULL);
		// Only supports UTF-8:
		//WriteFile (hstdout, wcText, sizeof (WCHAR) * len, NULL, NULL);
	#endif
}

void consoleOutU8 (const char *szTextU8)
{
	DWORD	len		= (DWORD) strlenU (szTextU8);
	HANDLE	hstdout	= GetStdHandle (STD_OUTPUT_HANDLE);

	#ifdef _DEBUG
		BOOL b = WriteFile (hstdout, szTextU8, len, NULL, NULL);
	#else
		// Only supports UTF-8:
		WriteFile (hstdout, szTextU8, len, NULL, NULL);
	#endif
}

WCHAR **cmdLineArgsW (int *nArgs)
{
	return CommandLineToArgvW (GetCommandLineW (), nArgs);
}

BOOL PathsEqualfromEndW (WCHAR *exe, WCHAR *arg)
{
	DWORD	lene	= (DWORD) strlenW (exe);
	DWORD	lena	= (DWORD) strlenW (arg);

	if (lene && lena)
	{
		while (lene && lena && toupperW (*(exe + lene)) == toupperW (*(arg + lena)))
		{
			-- lene;
			-- lena;
		}
		if (0 == lene || 0 == lena)
		{	// One of the two parameters has been fully consumed.
			return TRUE;
		}
		if (lene && L'\\' == *(exe + lene))
		{
			return TRUE;
		}
		if (lena && L'\\' == *(arg + lena))
		{
			return TRUE;
		}
	}
	// If both strings have no length we want that they are not treated
	//	as equal.
	return FALSE;
}

BOOL swallowExeArgW (int *pargc, WCHAR **pargv [])
{
	if (*pargc)
	{
		WCHAR *arg	= *pargv [0];
		// Remove "." and ".." prefixes.
		if (L'.' == arg [0])
			++ arg;
		if (L'.' == arg [0])
			++ arg;
		WCHAR wcExe [4096];
		if (GetModuleFileNameW (NULL, wcExe, 4096) < 4096)
		{
			if (PathsEqualfromEndW (wcExe, arg))
			{	// Swallow the executable argument.
				*pargc -= 1;
				*pargv += 1;
				return TRUE;
			}
			WCHAR *wc = strrchrW (wcExe, L'.');
			if (wc)
				*wc = L'\0';
			if (PathsEqualfromEndW (wcExe, arg))
			{	// Swallow the executable argument.
				*pargc -= 1;
				// The argument array now needs to point to the next argument.
				*pargv += 1;
				return TRUE;
			}
		} else
		{	// The executable path is still too long.
			//ASSERT (FALSE);
		}
	}
	return FALSE;
}

bool isArgumentW (const WCHAR *wcFixed, WCHAR *wcArgument)
{
	size_t	lFixed = strlenW (wcFixed);
	size_t	lArgum = strlenW (wcArgument);

	if (lFixed == lArgum)
	{
		return !memcmpU (wcFixed, wcArgument, lFixed);
	}
	return false;
}

bool isArgumentIgnoreCaseW (const WCHAR *wcFixed, WCHAR *wcArgument)
{
	size_t	lFixed = strlenW (wcFixed);
	size_t	lArgum = strlenW (wcArgument);

	if (lFixed == lArgum)
	{
		return !stricmpW (wcFixed, wcArgument, lFixed);
	}
	return false;
}

bool numberArgumentW (uint64_t *pui, WCHAR *wcArgument)
{
	if (wcArgument)
	{
		size_t l = strlenW (wcArgument);

		if (wcArgument)
		{
			// We only deal with numbers.
			while (l)
			{
				if (isNotDigitW (wcArgument [l - 1]))
					return false;
				-- l;
			}
			bool b = ubf_uint64_from_strW (pui, wcArgument);
			return b;
		}
	}
	return false;
}

WCHAR *nextArgumentW (int *cArg, int nArgs, WCHAR **wcArgs)
{
	int i = *cArg + 1;

	if (i < nArgs)
	{
		WCHAR *wcRet = wcArgs [i];

		*cArg += 1;
		return wcRet;
	}
	return NULL;
}

numArg compulsoryNumber (uint64_t *pn, int *cArg, int nArgs, WCHAR **wcArgs)
{
	WCHAR *pwc;

	if ((pwc = nextArgumentW (cArg, nArgs, wcArgs)))
	{
		if (numberArgumentW (pn, pwc))
			return enArgIsNumber;
		else
			return enArgNotNumber;
	}
	return enArgNoArg;
}

void outWaitForW (uint64_t seconds, const WCHAR *wcTaskText)
{
	WCHAR wcSeconds [UBF_UINT64_SIZ];
	wstr_from_uint64 ((WCHAR *) &wcSeconds, seconds);

	consoleOutW (wcTaskText);
	consoleOutW (L" in ");
	consoleOutW (wcSeconds);
	consoleOutW (L" second(s)...");
}

void waitForW (uint64_t seconds, const WCHAR *wcTaskText)
{
	outWaitForW (seconds, wcTaskText);
	while (-- seconds)
	{
		Sleep (1000);
		/*	First every 10 seconds, then every second.
		if	(
					(seconds > 10)	&&	(0 == seconds % 10)
				||	(seconds < 11)
			)
		*/
		{
			consoleOutW (L" \r");
			outWaitForW (seconds, wcTaskText);
		}
	}
	// Clears current line, then positions cursor to the left-hand side.
	consoleOutW (L"\33[2K\r");
	consoleOutW (wcTaskText);
	consoleOutW (L"...");
}
