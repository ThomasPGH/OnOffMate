/****************************************************************************************

File		WinPowerHelpers.c
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

#include <Windows.h>
#include <powrprof.h>
#include "./WinPowerHelpers.h"
#include "./WinUTF8Console.h"

#define WPWR_STATE_HYBERNATE		(true)
#define WPWR_STATE_SUSPEND			(false)

#define WPWR_FORCE_APPS_CLOSED		(true)
#define WPWR_DONT_FORCE_APPS_CLOSED	(false)

#define WPWR_RESTART_AFTER_SHUTDOWN	(true)
#define WPWR_JUST_SHUTDOWN			(false)

DWORD	dwReason	=		SHTDN_REASON_MAJOR_OTHER
						|	SHTDN_REASON_MINOR_OTHER
						|	SHTDN_REASON_FLAG_PLANNED;


bool ObtainPrivilege (WCHAR *wcPrivilege)
{
	bool b	= false;

	HANDLE				hToken; 
	TOKEN_PRIVILEGES	tkp; 
 
	// Get a token for this process. 
	if (OpenProcessToken (GetCurrentProcess (), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		// Get the LUID for the shutdown privilege. 
		LookupPrivilegeValueW (NULL, wcPrivilege, &tkp.Privileges [0].Luid);

		tkp.PrivilegeCount = 1;									// One privilege to set.
		tkp.Privileges [0].Attributes = SE_PRIVILEGE_ENABLED;

		// Get the shutdown privilege for this process.
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
		b = ERROR_SUCCESS == GetLastError();

		CloseHandle (hToken);
	}
	return b;
}

bool AbortShutdown (void)
{
	bool b = AbortSystemShutdownW (NULL);
	return b;
}

bool AbortShutdownOrFail (void)
{
	bool b = AbortShutdown ();
	if (b)
	{
		consoleOutW (L"\nShutdown/power off aborted.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool HybernateComputer (void)
{
	BOOL b = SetSuspendState (WPWR_STATE_HYBERNATE, false, false);
	return b;
}

bool HybernateComputerOrFail (void)
{
	bool b = HybernateComputer ();
	if (b)
	{
		consoleOutW (L"\nWorkstation/computer hybernated.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool SuspendComputer (void)
{
	BOOL b = SetSuspendState (WPWR_STATE_SUSPEND, false, false);
	return b;
}

bool SuspendComputerOrFail (void)
{
	BOOL b = SetSuspendState (WPWR_STATE_SUSPEND, false, false);
	if (b)
	{
		consoleOutW (L"\nWorkstation/computer suspended.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool Logoff (void)
{
	bool b = ExitWindowsEx (EWX_LOGOFF, 0);
	return b;
}

bool LogoffOrFail (void)
{
	bool b = ExitWindowsEx (EWX_LOGOFF, 0);
	if (b)
	{
		consoleOutW (L"\nCurrent user logged off.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool LockThisComputer (void)
{
	bool b = LockWorkStation ();
	return b;
}

bool LockThisComputerOrFail (void)
{
	bool b = LockThisComputer ();
	if (b)
	{
		consoleOutW (L"\nWorkstation/computer locked.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool PowerOffComputer (void)
{
	bool b = ExitWindowsEx (EWX_POWEROFF | EWX_FORCEIFHUNG, dwReason);
	return b;
}

bool PowerOffComputerOrFail (void)
{
	bool b = PowerOffComputer ();
	if (b)
	{
		consoleOutW (L"\nFull shutdown of workstation/computer initiated.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool PowerOffComputerWithMsgAndGracePeriodW (WCHAR *wcMsg, DWORD dwGracePeriod)
{
	bool b = InitiateSystemShutdownExW	(
				NULL, wcMsg, dwGracePeriod, WPWR_DONT_FORCE_APPS_CLOSED, WPWR_JUST_SHUTDOWN, dwReason
										);
   return b;
}

bool RestartComputer (void)
{
	bool b = ExitWindowsEx (EWX_REBOOT | EWX_FORCEIFHUNG, dwReason);
	return b;
}

bool RestartComputerOrFail (void)
{
	bool b = RestartComputer ();
	if (b)
	{
		consoleOutW (L"\nRestart of workstation/computer initiated.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool ShutdownComputer (void)
{
	bool b = ExitWindowsEx (EWX_SHUTDOWN | EWX_FORCEIFHUNG, dwReason);
	return b;
}

bool ShutdownComputerOrFail (void)
{
	bool b = ShutdownComputer ();
	if (b)
	{
		consoleOutW (L"\nFull shutdown of workstation/computer initiated.\n");
		return true;
	}
	consoleOutWinErrorText (GetLastError ());
	return false;
}

bool ShutdownComputerWithMsgAndGracePeriodW (WCHAR *wcMsg, DWORD dwGracePeriod)
{
	bool b = InitiateSystemShutdownExW	(
				NULL, wcMsg, dwGracePeriod, WPWR_DONT_FORCE_APPS_CLOSED, WPWR_JUST_SHUTDOWN, dwReason
										);
	return b;
}

/*
	Unused for now. It's the callback function for SetWaitableTimer ().
*/
static void Ptimerapcroutine(
  /* [in, optional] */ LPVOID lpArgToCompletionRoutine,
  /* [in]           */ DWORD dwTimerLowValue,
  /* [in]           */ DWORD dwTimerHighValue
)
{

}

EXECUTION_STATE defES = ES_CONTINUOUS | ES_SYSTEM_REQUIRED;

bool WakeupComputerAfter (DWORD dwSeconds)
{
	int64_t			ift;

	ift = 0 - (dwSeconds * FT_SECOND);

	DWORD dw;
	if (INT64_MAX >= ift)
	{
		HANDLE hTimer = CreateWaitableTimerW (NULL, TRUE, NULL);
		if (hTimer)
		{
			LARGE_INTEGER li;
			li.QuadPart = ift;
			if (SetWaitableTimer (hTimer, &li, 0, NULL, NULL, TRUE))
			{
				dw = GetLastError ();
				if (ERROR_NOT_SUPPORTED == dw)
					return false;
				dw = WaitForSingleObject (hTimer, INFINITE);
				EXECUTION_STATE es = SetThreadExecutionState (defES);
				return ERROR_SUCCESS == dw;
			}
		}
	}
	return false;
}

DWORD WINAPI WakeupComputerThreadProc (void *pvoid)
{
	DWORD *pdwSeconds = pvoid;
	bool bSuccess = WakeupComputerAfter (*pdwSeconds);
	return ERROR_SUCCESS;
}

HANDLE StartThreadWakeupComputerAfter (DWORD dwSeconds)
{
	HANDLE hThr = CreateThread (NULL, 0, WakeupComputerThreadProc, &dwSeconds, CREATE_SUSPENDED, NULL);
	if (hThr)
	{
		ResumeThread (hThr);
		return hThr;
	}
	return NULL;
}
