/****************************************************************************************

File		WinUTF8Console.h
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

#ifndef WINUTF8CONSOLE_H
#define WINUTF8CONSOLE_H

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

#ifndef WINUTF8CONSOLE_OUTTHRESH
#define WINUTF8CONSOLE_OUTTHRESH			(512)
#endif

EXTERN_C_BEGIN

/*
	SetCodePageToUTF8

	Sets console input and output code page to UTF8 (CP_UTF8).

	The function returns true if successful, false otherwise. The function may return
	false if CP_UTF8 is not installed.
*/
bool SetCodePageToUTF8 (void)
;

/*
	SetConsoleEnableANSI

	Calls GetConsoleMode () and SetConsoleMode to set the console mode flag
	ENABLE_VIRTUAL_TERMINAL_PROCESSING, which enables processing of ANSI escape
	sequences.
*/
bool SetConsoleEnableANSI (void)
;

/*
	consoleOutW

	Outputs to the console.
*/
void consoleOutW (const WCHAR *wcText)
;

/*
	consoleOutU8

	Outputs to the console.
*/
void consoleOutU8 (const char *szTextU8)
;

/*
	cmdLineArgs

	Returns the command line arguments as an array of WCHAR * and the amount of
	arguments in nArgs.
*/
WCHAR **cmdLineArgsW (int *nArgs)
;

/*
	swallowExeArg

	Swallows the first argument if it is the command-line used to invoke the
	application. Since Windows does not always add this "executable argument"
	calling this function on our command-line parameters gives us the same
	result independent on how the application has been invoked.

	See
	https://docs.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-160
	for more information. Excerpt:
	"By convention, argv[0] is the filename of the program. However, on Windows it's possible
	to spawn a process by using CreateProcess. If you use both the first and second arguments
	(lpApplicationName and lpCommandLine), argv[0] may not be the executable name. You can use
	GetModuleFileName to retrieve the executable name, and its fully-qualified path."
*/
BOOL swallowExeArgW (int *pargc, WCHAR **pargv []);

/*
	isArgumentW

	Returns true if the NUL-terminated argument wcArgument is identical to the NUL-terminated
	fixed text wcFixed, false if not.
*/
bool isArgumentW (const WCHAR *wcFixed, WCHAR *wcArgument)
;

/*
	isArgumentIgnoreCaseW

	Returns true if the NUL-terminated argument wcArgument is identical to the NUL-terminated
	fixed text wcFixed, false if not. The function treats upper and lower case characters as
	identical.
*/
bool isArgumentIgnoreCaseW (const WCHAR *wcFixed, WCHAR *wcArgument)
;

/*
	numberArgumentW

	Stores the number in wcArgument at the address pui points to and returns true. If wcArgument
	contains anything else the function returns false.
*/
bool numberArgumentW (uint64_t *pui, WCHAR *wcArgument)
;

/*
	nextArgumentW

	Returns the next argument if there is one, NULL if not.

	cArg			The current index within wcArgs. It is incremented if there is a next argument.
	nArgs			The total amount of arguments available. In a standard main () function this
					is the argc parameter.
	wcArgs			A pointer to the array of arguments. In a standard Windows main () function this
					is the argv parameter as UTF-16 strings.
*/
WCHAR *nextArgumentW (int *cArg, int nArgs, WCHAR **wcArgs)
;

enum enargtst
{
	enArgInvalid,
	enArgNoArg,
	enArgIsNumber,
	enArgNumberTooBig,
	enArgNotNumber,
	enArgTaskError
};
typedef enum emargtest	numArg;

/*
	compulsoryNumber

*/
numArg compulsoryNumber (uint64_t *pn, int *cArg, int nArgs, WCHAR **wcArgs)
;

/*
	outWaitForW

*/
void outWaitForW (uint64_t seconds, const WCHAR *wcTaskText)
;

/*
	waitForW

	Outputs the text wcTaskText and pauses execution for the specified amount of seconds. The
	function also outputs the text when the time has elapsed. If the time to wait (seconds) is
	greater than 10, the function outputs this text every 10 seconds.

	Example:
		waitForW (5, L"Suspending (sleeping) computer");

		-> Outputs "Suspending (sleeping) computer in 5 second(s)..."
		-> Outputs "Suspending (sleeping) computer in 0 second(s)..."
*/
void waitForW (uint64_t seconds, const WCHAR *wcTaskText)
;

/*
	consoleOutWinErrorText

*/
void consoleOutWinErrorText (DWORD dwError)
;

EXTERN_C_END

#endif														// Of #ifndef WINUTF8CONSOLE_H.
