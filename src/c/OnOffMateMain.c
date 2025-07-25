/****************************************************************************************

	File		OnOffMateMain.c
	Why:		Main source code file.
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

#include <Windows.h>
/*
	We're not using the standard libraries to keep the executable size small.
	#ifdef _DEBUG
		// Gives us the _ASSERT macro.
		#include <crtdbg.h>
	#endif
*/
#include <stdint.h>
#include "./OnOffMateMain.h"
#include "./WinPowerHelpers.h"
#include "./WinRuntimeReplacements.h"
#include "./WinUTF8Console.h"
#include "./WakeOnLAN.h"

/*
	outPutHelp

	Prints a list with the parameters to the standard output/console.
*/
void outPutHelp (void)
{
	consoleOutU8	(
		"\n"
		ONOFFMATE_VERSION_STRTOT " - Hybernation, sleep, recycle bin, and power helper\n"
		"\n"
		"  OnOffMate [command]\n"
		"  oom [command]\n"
		"\n"
		"  Commands:\n"
		"    ? or /? or h or -h or --help       Outputs this help.\n"
		"    /a                                 Aborts a task with a grace period.\n"
		"    Abort                              Aborts a task with a grace period.\n"
		"    EmptyRecycleBin     [dir1] [...]   Empties either all recycle bins of all drives and\n"
		"                                       folders, or for [dir1], [dir2], etc only.\n"
		"    EmptyRecycleBinNC   [dir1] [...]   Empties recycle bins without confirmation.\n"
		"    EmptyRecycleBinNCP  [dir1] [...]   Empties recycle bins without confirmation and\n"
		"                                       progress bar.\n"
		"    EmptyRecycleBinNCPS [dir1] [...]   Empties recycle bins without confirmation, progress\n"
		"                                       bar, and sound.\n"
		"    EmptyRecycleBinNCS  [dir1] [...]   Empties recycle bins without confirmation and\n"
		"                                       sound.\n"
		"    EmptyRecycleBinNP   [dir1] [...]   Empties recycle bins without progress bar.\n"
		"    EmptyRecycleBinNPS  [dir1] [...]   Empties recycle bins without progress bar and sound.\n"
		"    EmptyRecycleBinNS   [dir1] [...]   Empties recycle bins without sound.\n"
		"    Hybernate                          Hybernates computer instantly.\n"
		"    HybernateAfter <hs>                Hybernates computer after <hs> seconds.\n"
		"    Lock                               Locks computer instantly.\n"
		"    LockAfter <ls>                     Locks computer after <ls> seconds.\n"
		"    Logoff                             Logs off current user.\n"
		"    LogoffAfter <os>                   Logs off current user after <os> seconds.\n"
		"    MonitorLowPower                    Puts the monitor(s) in low power mode.\n"
		"    MonitorLowPowerAfter <ps>          Puts the monitor(s) in low power mode after <ps>\n"
		"                                       seconds.\n"
		"    MonitorOff                         Switches the monitor(s) off instantly.\n"
		"    MonitorOffAfter <ps>               Switches the monitor(s) off after <ps> seconds.\n"
		"    MonitorOn                          Switches the monitor(s) on instantly.\n"
		"    MonitorOnAfter <ps>                Switches the monitor(s) on after <ps> seconds.\n"
		"    MonitorPowerOff                    Switches the monitor(s) off instantly.\n"
		"    MonitorPowerOffAfter <ps>          Switches the monitor(s) off after <ps> seconds.\n"
		"    MonitorPowerOn                     Switches the monitor(s) on instantly.\n"
		"    MonitorPowerOnAfter <ps>           Switches the monitor(s) on after <ps> seconds.\n"
		"    PowerOff                           Shuts down and powers off computer instantly.\n"
		"    PowerOffAfter <ps>                 Shuts down and powers off computer in <ps> seconds.\n"
		"    PowerOffMsgAfter <ps> <msg>        Shuts down and powers off computer in <ps> seconds\n"
		"                                       with message <msg>.\n"
		"    QueryRecycleBin     [dir1] [...]   Queries either all recycle bins of all drives and\n"
		"                                       folders, or for [dir1], [dir2], etc only.\n"
		"    Reboot                             Restarts/reboots computer instantly.\n"
		"    RebootAfter <rs>                   Restarts/reboots computer after <rs> seconds.\n"
		"    Restart                            Restarts/reboots computer instantly.\n"
		"    RestartAfter <rs>                  Restarts/reboots computer after <rs> seconds.\n"
		"    Shutdown                           Shuts down and powers off computer instantly.\n"
		"    ShutdownAfter <ds>                 Shuts down and powers off computer in <ds> seconds.\n"
		"    ShutdownMsgAfter <ds> <msg>        Shuts down and powers off computer in <ds> seconds\n"
		"                                       with message <msg>\n"
		"    Sleep                              Suspends (sleeps) computer instantly.\n"
		"    SleepAfter <ss>                    Suspends (sleeps) computer after <ss> seconds.\n"
		"    SleepWakeupAfter <ws>              Suspends (sleeps) computer instantly and wakes it\n"
		"                                       up again after <ws> seconds.\n"
		"    SleepAfterWakeupAfter <ss> <ws>    Suspends (sleeps) computer in <ss> seconds and\n"
		"                                       wakes it up again after <ws> seconds.\n"
		"    Suspend                            Suspends (sleeps) computer instantly.\n"
		"    SuspendAfter <ss>                  Suspends (sleeps) computer after <ss> seconds.\n"
		"    SuspendWakeupAfter <ws>            Suspends (sleeps) computer instantly and wakes it\n"
		"                                       up again after <ws> seconds.\n"
		"    SuspendAfterWakeupAfter <ss> <ws>  Suspends (sleeps) computer in <ss> seconds and\n"
		"                                       wakes it up again after <ws> seconds.\n"
		"    Ver                                Prints the version info.\n"
		"    Version                            Prints the version info.\n"
		"    WakeOnLAN <brip> <mac> [-f6]       Wakes the host with broadcast IP <brip> and MAC\n"
		"                                       address <mac>. For example, if the IP address of the\n"
		"                                       host to wake up is 192.168.0.97 and the subnet mask\n"
		"                                       is 255.255.255.0, use 192.168.0.255 for <brip>.\n"
		"                                       Argument -f6 forces IPv6 even if <brip> is IPv4.\n"
		"\n"
		"  Note that not every hardware supports all commands, that some options can be activated/\n"
		"  deactivated in the BIOS, and that others depend on Windows settings.\n"
		"\n"
		"  The original behaviour of the Shutdown... commands (shutting down without power off) has\n"
		"  been changed to be identical to the PowerOff... commands (shutting down and power off).\n"
					);
}

const WCHAR wcActionAborting		[]	= L"Aborting shutdown/powerdown";
const WCHAR wcActionHybernating		[]	= L"Hybernating workstation/computer";
const WCHAR wcActionLoggingOff		[]	= L"Logging off";
const WCHAR wcActionLocking			[]	= L"Locking workstation/computer";
const WCHAR wcActionMonitorLowPower	[]	= L"Switching monitor(s) to low power mode";
const WCHAR wcActionMonitorOff		[]	= L"Switching monitor(s) off";
const WCHAR wcActionMonitorOn		[]	= L"Switching monitor(s) on";
const WCHAR wcActionRestarting		[]	= L"Restarting workstation/computer";
const WCHAR wcActionShuttingDown	[]	= L"Shutting down workstation/computer";
const WCHAR wcActionSuspending		[]	= L"Suspending (sleeping) workstation/computer";
const WCHAR wcActionWakingUp		[]	= L"Waking up workstation/computer";
const WCHAR wcActionPowerOff		[]	= L"Shutting down and switching off workstation/computer";

void outputActionAborting (void)
{
	consoleOutW (wcActionAborting);
	consoleOutW (L"...");
}

void outputActionHybernating (void)
{
	consoleOutW (wcActionHybernating);
	consoleOutW (L"...");
}

void outputActionPowerOff (void)
{
	consoleOutW (wcActionPowerOff);
	consoleOutW (L"...");
}

void outputActionRestarting (void)
{
	consoleOutW (wcActionRestarting);
	consoleOutW (L"...");
}

void outputActionShuttingDown (void)
{
	consoleOutW (wcActionShuttingDown);
	consoleOutW (L"...");
}

void outputActionSuspending (void)
{
	consoleOutW (wcActionSuspending);
	consoleOutW (L"...");
}

void outputActionLocking (void)
{
	consoleOutW (wcActionLocking);
	consoleOutW (L"...");
}

void outputActionLoggingOff (void)
{
	consoleOutW (wcActionLoggingOff);
	consoleOutW (L"...");
}

void outputActionMonitorLowPower (void)
{
	consoleOutW (wcActionMonitorLowPower);
	consoleOutW (L"...");
}

void outputActionMonitorOff (void)
{
	consoleOutW (wcActionMonitorOff);
	consoleOutW (L"...");
}

void outputActionMonitorOn (void)
{
	consoleOutW (wcActionMonitorOn);
	consoleOutW (L"...");
}

void outputSHQUERYRBINFO (wchar_t *wc, SHQUERYRBINFO *pqi)
{
	wchar_t		wcNum [UBF_UINT64_SIZ];
	wchar_t		wcSiz [UBF_UINT64_SIZ];

	wstr_from_uint64 (wcNum, pqi->i64NumItems);
	wstr_from_uint64 (wcSiz, pqi->i64Size);

	consoleOutW (L"\nStats for recycle bin");
	if (wc)
	{
		consoleOutW (L" \"");
		consoleOutW (wc);
		consoleOutW (L"\":");
	} else
		consoleOutW (L":");
	consoleOutW (L"\nItems: ");
	consoleOutW (wcNum);
	consoleOutW (L"\nSize:  ");
	consoleOutW (wcSiz);
	consoleOutW (L" octets/bytes\n");
}

HRESULT qeryRecylceBin (wchar_t *name, SHQUERYRBINFO *pqi)
{
	HRESULT	hr;

	hr = SHQueryRecycleBinW (name, pqi);
	if (S_OK == hr)
	{
		outputSHQUERYRBINFO (name, pqi);
	} else
	{
		consoleOutW (L"\nError querying recycle bin");
		if (name)
		{
			consoleOutW (L" \"");
			consoleOutW (name);
			consoleOutW (L"\".\n");
		} else
			consoleOutW (L".\n ");
	}
	return hr;
}

bool queryRecycleBins (int *cArg, int nArgs, wchar_t **wcArgs)
{
	SHQUERYRBINFO	qi;
	HRESULT	hr;
	wchar_t *wcNext;
	int		n = 0;

	qi.cbSize = sizeof (SHQUERYRBINFO);
	while (wcNext = nextArgumentW (cArg, nArgs, wcArgs))
	{
		++ n;
		hr = qeryRecylceBin (wcNext, &qi);
	}
	if (!n)
	{
		hr = qeryRecylceBin (wcNext, &qi);
	}
	return S_OK == hr;
}

bool emptyRecycleBin (DWORD flags, int *cArg, int nArgs, WCHAR **wcArgs)
{
	HRESULT	hr;
	wchar_t *wcNext;
	int		n = 0;
	while (wcNext = nextArgumentW (cArg, nArgs, wcArgs))
	{
		++ n;
		hr = SHEmptyRecycleBinW (NULL, wcNext, flags);
		if (S_OK == hr)
		{
			/*
			consoleOutW (L"Recycle bin \"");
			consoleOutW (wcNext);
			consoleOutW (L"\" emptied. ");
			*/
		} else
		{
			if (E_UNEXPECTED == hr)
			{
				consoleOutW (L"Recycle bin \"");
				consoleOutW (wcNext);
				consoleOutW (L"\" already empty. ");
			} else
			{
				consoleOutW (L"Error emptying recycle bin \"");
				consoleOutW (wcNext);
				consoleOutW (L"\". ");
			}
		}
	}
	if (!n)
	{
		hr = SHEmptyRecycleBinW (NULL, L"", flags);
		if (S_OK == hr)
			; //consoleOutW (L"Recycle bin emptied. ");
		else
		if (E_UNEXPECTED == hr)
			consoleOutW (L"Recycle bin already empty. ");
		else
			consoleOutW (L"Error emptying recycle bin. ");
	}
	return S_OK == hr;
}

void ourmain (void)
{
	// The command-line arguments.
	int			nArgs;											// Amount of arguments.
	WCHAR		**wcArgs = cmdLineArgsW (&nArgs);				// Array of arguments.
	swallowExeArgW (&nArgs, &wcArgs);							// Suppress the executable.

	//doWeHaveInteractiveSessions ();

	AttachConsole (ATTACH_PARENT_PROCESS);
	SetCodePageToUTF8 ();
	SetConsoleEnableANSI ();

	bool			bCmdComplete						= false;
	WCHAR			*pwc								= NULL;
	numArg			evalArg								= enArgInvalid;
	uint64_t		n1, n2;

	wchar_t			wzIP4asIP6 [U_WAKEONLAN_IPV6_SIZ]	=	U_WAKEONLAN_IPV6V4_PFXW
															// No idea why, but without this
															//	MSVC generates a memset ().
															L"\0";
	const wchar_t	*wzIP								= wzIP4asIP6;

	if (!ObtainPrivilege (SE_SHUTDOWN_NAME))
	{
		consoleOutW (L"Error obtaining privilege SE_SHUTDOWN_NAME.\n");
		ExitProcess (EXIT_FAILURE);
	}
	if (nArgs)
	{
		int cArg = 0;
		while (cArg < nArgs)
		{
			if	(
						isArgumentW				(L"?",		wcArgs [cArg])
					||	isArgumentW				(L"/?",		wcArgs [cArg])
					||	isArgumentIgnoreCaseW	(L"-h",		wcArgs [cArg])
					||	isArgumentIgnoreCaseW	(L"--h",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW	(L"--help",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW	(L"h",		wcArgs [cArg])
					||	isArgumentIgnoreCaseW	(L"help",	wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outPutHelp ();
			} else
			if	(
						isArgumentIgnoreCaseW (L"/a", wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"Abort", wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outputActionAborting ();
				AbortShutdownOrFail ();
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBin", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (0, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNC", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOCONFIRMATION, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNCP", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNCPS", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNCS", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOCONFIRMATION | SHERB_NOSOUND, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNP", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOPROGRESSUI, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNPS", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOPROGRESSUI | SHERB_NOSOUND, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"EmptyRecycleBinNS", wcArgs [cArg]))
			{
				bCmdComplete = true;
				emptyRecycleBin (SHERB_NOSOUND, &cArg, nArgs, wcArgs);
			} else
			if	(isArgumentIgnoreCaseW (L"Hybernate", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionHybernating ();
				HybernateComputerOrFail ();
			} else
			if	(isArgumentIgnoreCaseW (L"HybernateAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionHybernating);
					HybernateComputerOrFail ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"Logoff", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionLoggingOff ();
				LogoffOrFail ();
			} else
			if	(isArgumentIgnoreCaseW (L"LogoffAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionLoggingOff);
					LogoffOrFail ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"Lock", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionLocking ();
				LockThisComputerOrFail ();
			} else
			if	(isArgumentIgnoreCaseW (L"LockAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionLocking);
					LockThisComputerOrFail ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"MonitorLowPower", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionMonitorLowPower ();
				MonitorLowPower ();
				consoleOutW (L"\nMonitor(s) switched to low power mode.\n");
			} else
			if	(isArgumentIgnoreCaseW (L"MonitorLowPowerAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionMonitorLowPower);
					MonitorLowPower ();
					consoleOutW (L"\nMonitor(s) switched to low power mode.\n");
				}
			} else
			if	(
						isArgumentIgnoreCaseW (L"MonitorOff", wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"MonitorPowerOff", wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outputActionMonitorOff ();
				MonitorPowerOff ();
				consoleOutW (L"\nMonitor(s) powered off.\n");
			} else
			if	(
						isArgumentIgnoreCaseW (L"MonitorOffAfter", wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"MonitorPowerOffAfter", wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionMonitorOff);
					MonitorPowerOff ();
					consoleOutW (L"\nMonitor(s) powered off.\n");
				}
			} else
			if	(
						isArgumentIgnoreCaseW (L"MonitorOn", wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"MonitorPowerOn", wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outputActionMonitorOn ();
				MonitorPowerOn ();
				consoleOutW (L"\nMonitor(s) powered on.\n");
			} else
			if	(
						isArgumentIgnoreCaseW (L"MonitorOnAfter", wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"MonitorPowerOnAfter", wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionMonitorOn);
					MonitorPowerOn ();
					consoleOutW (L"\nMonitor(s) powered on.\n");
				}
			} else
			if	(isArgumentIgnoreCaseW (L"PowerOff", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionPowerOff ();
				PowerOffComputerOrFail ();
			} else
			if	(isArgumentIgnoreCaseW (L"PowerOffAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionPowerOff);
					PowerOffComputerOrFail ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"PowerOffMsgAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					if (n1 <= MAXDWORD)
					{
						bCmdComplete = true;
						outputActionShuttingDown ();
						ShutdownComputerWithMsgAndGracePeriodW (nextArgumentW (&cArg, nArgs, wcArgs), (DWORD) n1);
					} else
						evalArg = enArgNumberTooBig;
				}
			} else
			if	(isArgumentIgnoreCaseW (L"QueryRecycleBin",	wcArgs [cArg]))
			{
				bCmdComplete = true;
				queryRecycleBins (&cArg, nArgs, wcArgs);
			} else
			if	(		isArgumentIgnoreCaseW (L"Reboot",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"Restart",	wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outputActionRestarting ();
				RestartComputerOrFail ();
			} else
			if	(		isArgumentIgnoreCaseW (L"RebootAfter",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"RestartAfter",	wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionRestarting);
					RestartComputerOrFail ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"Shutdown", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionShuttingDown ();
				ShutdownComputerOrFail ();
			} else
			if	(isArgumentIgnoreCaseW (L"ShutdownAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					if (n1 <= MAXDWORD)
					{
						bCmdComplete = true;
						waitForW (n1, wcActionShuttingDown);
						ShutdownComputerOrFail ();
					} else
						evalArg = enArgNumberTooBig;
				}
			} else
			if	(isArgumentIgnoreCaseW (L"ShutdownMsgAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					if (n1 <= MAXDWORD)
					{
						bCmdComplete = true;
						outputActionShuttingDown ();
						ShutdownComputerWithMsgAndGracePeriodW (nextArgumentW (&cArg, nArgs, wcArgs), (DWORD) n1);
					} else
						evalArg = enArgNumberTooBig;
				}
			} else
			if	(		isArgumentIgnoreCaseW (L"Sleep",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"Suspend",	wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outputActionSuspending ();
				SuspendComputerOrFail ();
			} else
			if	(		isArgumentIgnoreCaseW (L"SleepAfter",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"SuspendAfter",	wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionSuspending);
					SuspendComputerOrFail ();
				}
			} else
			if	(
						isArgumentIgnoreCaseW (L"SleepWakeupAfter",		wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"SuspendWakeupAfter",	wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					if (n1 <= MAXDWORD)
					{
						HANDLE h = StartThreadWakeupComputerAfter ((DWORD) n1);
						if (h)
						{
							outputActionSuspending ();
							consoleOutW (L" ");
							outWaitForW (n1, wcActionWakingUp);
							SuspendComputerOrFail ();
							WaitForSingleObject (h, INFINITE);
							bCmdComplete = true;
						} else
							evalArg = enArgTaskError;
					} else
						evalArg = enArgNumberTooBig;
				}
			} else
			if	(
						isArgumentIgnoreCaseW (L"SleepAfterWakeupAfter",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"SuspendAfterWakeupAfter",	wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					if (n1 <= MAXDWORD)
					{
						if (enArgIsNumber == (evalArg = compulsoryNumber (&n2, &cArg, nArgs, wcArgs)))
						{
							if (n1 <= MAXDWORD)
							{
								waitForW (n1, wcActionSuspending);
								HANDLE h = StartThreadWakeupComputerAfter ((DWORD) n2);
								if (h)
								{
									consoleOutW (L" ");
									outWaitForW (n2, wcActionWakingUp);
									SuspendComputerOrFail ();
									WaitForSingleObject (h, INFINITE);
									bCmdComplete = true;
								} else
									evalArg = enArgTaskError;
							} else
								evalArg = enArgNumberTooBig;
						}
					} else
						evalArg = enArgNumberTooBig;
				}
			} else
			if	(
						isArgumentIgnoreCaseW (L"Ver",		wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"Version",	wcArgs [cArg])
				)
			{
				consoleOutU8 (ONOFFMATE_VERSION_STRTOT);
				consoleOutU8 ("\n");
				bCmdComplete = true;
			}
			if (isArgumentIgnoreCaseW (L"WakeOnLAN", wcArgs [cArg]))
			{
				callWSAStartup ();
				evalArg = enArgMissingAfter;
				enum eWOLret wol = wolretSyntaxHst;
				wchar_t wcMAC [U_WAKEONLAN_MAC_SIZ];
				wchar_t *maca = NULL;
				wchar_t *host = nextArgumentW (&cArg, nArgs, wcArgs);
				if (host)
				{
					if (isGoodIPv4stringW (host) || isGoodIPv6stringW (host))
					{
						wol = wolretMissing;
						maca = nextArgumentW (&cArg, nArgs, wcArgs);
						if (maca)
						{
							bool bForceV6 = false;
							wchar_t *wcForceV6 = nextArgumentW (&cArg, nArgs, wcArgs);
							if (wcForceV6)
							{
								if	(
											isArgumentIgnoreCaseW (L"-f6", wcForceV6)
										&&	isGoodIPv4stringW (host)
									)
								{
									memcpyU (wzIP4asIP6 + U_WAKEONLAN_IPV6V4_PFX_LEN, host, sizeof (wchar_t) * (1 + strlenW (host)));
									bForceV6 = true;
								} else
								{
									wzIP = host;
									-- cArg;
								}
							} else
								wzIP = host;
							char *szErrPosition;
							wol = wakeOnLAN_W (wzIP, maca, bForceV6, &szErrPosition);
							makeUnifiedMACaddress (wcMAC, maca);
							bCmdComplete = true;
						}
					}
					switch (wol)
					{
						case wolretOk:
							consoleOutW (L"Magic WOL (Wake on LAN) packet sent to \"");
							consoleOutW (wzIP);
							consoleOutW (L"\" with MAC address \"");
							consoleOutW (wcMAC);
							consoleOutW (L"\" on UDP port 9.\n");
							break;
						case wolretSyntaxMAC:
							consoleOutW (L"Syntax error: \"");
							consoleOutW (maca);
							consoleOutW (L"\" is not a valid MAC address.\n");
							bCmdComplete = true;
							break;
						case wolretSyntaxHst:
							consoleOutW (L"Syntax error: \"");
							consoleOutW (host);
							consoleOutW (L"\" is not a valid broadcast IP address.\n");
							bCmdComplete = true;
							break;
						case wolretErrSend:
							consoleOutW (L"Error sending magic WOL (Wake on LAN) packet to \"");
							consoleOutW (wzIP);
							consoleOutW (L"\" with MAC address \"");
							makeUnifiedMACaddress (wcMAC, maca);
							consoleOutW (wcMAC);
							consoleOutW (L"\" on UDP port 9.\n");
							break;
						case wolretMissing:
							break;
					}
				}
			}
			if (bCmdComplete)
			{
				++ cArg;
				if (cArg < nArgs)
				{
					consoleOutW (L"\nIgnored argument(s):");
					while (cArg < nArgs)
					{
						consoleOutW (L" \"");
						consoleOutW (wcArgs [cArg]);
						consoleOutW (L"\"");
						++ cArg;
					}
					consoleOutW (L"\n");
				}
			} else
			{
				switch (evalArg)
				{
					case enArgInvalid:
						consoleOutU8 ("Syntax error. Unknown argument/parameter: \"");
						break;
					case enArgNoArg:
						consoleOutU8 ("Syntax error. Argument/parameter missing for \"");
						break;
					case enArgNumberTooBig:
						consoleOutU8 ("Syntax error. Number too big: \"");
						break;
					case enArgNotNumber:
						consoleOutU8 ("Syntax error. Argument/parameter is not a valid number: \"");
						break;
					case enArgTaskError:
						consoleOutU8 ("Error performing task.");
						break;
					case enArgMissingAfter:
						consoleOutU8 ("Syntax error. Argument/parameter missing after: \"");
						break;
					default:
						consoleOutU8 ("Syntax error. Argument/parameter missing for \"");
				}
				consoleOutW (wcArgs [cArg]);
				consoleOutU8 ("\".\n");
				break;
			}
			++ cArg;
		}
	} else
	{
		outPutHelp ();
	}
	CallWSACleanup ();
	ExitProcess (EXIT_SUCCESS);
}
