/****************************************************************************************

File		WinPowerHelpers.h
Why:		Windows power helpers.
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

#ifndef WINPOWERHELPERS_H
#define WINPOWERHELPERS_H

#include <stdbool.h>
#include <inttypes.h>
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

/*
	Time intervals valid for values in FILETIME structures.
*/
#ifndef FT_D_NANOSECOND
#define FT_D_NANOSECOND ((ULONGLONG) 100)						/* This is 1/100! */
#endif
#ifndef FT_MICROSECOND
#define FT_MICROSECOND ((ULONGLONG) 10)
#endif
#ifndef FT_MILLISECOND
#define FT_MILLISECOND ((ULONGLONG) 10000)
#endif
#ifndef FT_SECOND
#define FT_SECOND ((ULONGLONG) 10000000)
#endif
#ifndef FT_MINUTE
#define FT_MINUTE (60 * FT_SECOND)
#endif
#ifndef FT_HOUR
#define FT_HOUR   (60 * FT_MINUTE)
#endif
#ifndef FT_DAY
#define FT_DAY    (24 * FT_HOUR)
#endif

EXTERN_C_BEGIN

/*
	HybernateComputer

	Hybernates the local machine.
*/
bool HybernateComputer (void)
;

/*
	PowerOffComputer

	Shuts down the local machine and turns its power off.
*/
bool PowerOffComputer (void)
;

/*
	RestartComputer

	Restarts the local machine.
*/
bool RestartComputer (void)
;

/*
	SuspendComputer

	Suspends (sleeps) the local machine.
*/
bool SuspendComputer (void)
;

/*
	Logoff

	Shuts down all processes running in the logon session of the process that called this function.
	Then it logs the user off.
*/
bool Logoff (void)
;

/*
	ShutdownComputer

	Shuts down the workstation/computer but does not power it down.
*/
bool ShutdownComputer (void)
;

/*
	ShutdownComputerWithMsgAndGracePeriod

*/
bool ShutdownComputerWithMsgAndGracePeriodW (WCHAR *wcMsg, DWORD dwGracePeriod)
;

/*
	WakeupComputerAfter

	The parameter uiWaitTime is in seconds.
*/
bool WakeupComputerAfter (DWORD dwSeconds)
;

/*
	StartThreadWakeupComputerAfter

*/
HANDLE StartThreadWakeupComputerAfter (DWORD dwSeconds)
;

EXTERN_C_END

#endif														// Of #ifndef WINPOWERHELPERS_H.
