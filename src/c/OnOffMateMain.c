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
#include "./WinPowerHelpers.h"
#include "./WinUTF8Console.h"

/*
	outPutHelp

	Prints a list with the parameters to the standard output/console.
*/
void outPutHelp (void)
{
	consoleOutU8	(
		"\n"
		"OnOffMate - Ver. 1.000 (2025-05-13) - Hybernation, sleep, and power helper\n"
		"\n"
		"  OnOffMate [command]\n"
		"\n"
		"  Commands:\n"
		"    ? or /? h or -h or --help          Outputs this help\n"
		"    Hybernate                          Hybernates computer instantly\n"
		"    HybernateAfter <hs>                Hybernates computer after <hs> seconds\n"
		"    Lock                               Locks computer instantly\n"
		"    LockAfter <ls>                     Locks computer after <ls> seconds\n"
		"    Logoff                             Logs off current user\n"
		"    LogoffAfter <os>                   Logs off current user after <os> seconds\n"
		"    PowerOff                           Shuts down and powers off computer instantly\n"
		"    PowerOffAfter <ps>                 Shuts down and powers off computer in <ps> seconds\n"
		"    PowerOffMsgAfter <ps> <msg>        Shuts down and powers off computer in <ps> seconds\n"
		"                                       with message <msg>\n"
		"    Reboot                             Restarts/reboots computer instantly\n"
		"    RebootAfter <rs>                   Restarts/reboots computer after <rs> seconds\n"
		"    Restart                            Restarts/reboots computer instantly\n"
		"    RestartAfter <rs>                  Restarts/reboots computer after <rs> seconds\n"
		"    Shutdown                           Shuts down computer instantly\n"
		"    ShutdownAfter <ds>                 Shuts down computer in <ds> seconds\n"
		"    ShutdownMsgAfter <ds> <msg>        Shuts down computer in <ds> seconds with message\n"
		"                                       <msg>\n"
		"    Sleep                              Suspends (sleeps) computer instantly\n"
		"    SleepAfter <ss>                    Suspends (sleeps) computer after <ss> seconds\n"
		"    SleepWakeupAfter <ws>              Suspends (sleeps) computer instantly and wakes it\n"
		"                                       up again after <ws> seconds\n"
		"    SleepAfterWakeupAfter <ss> <ws>    Suspends (sleeps) computer in <ss> seconds and\n"
		"                                       wakes it up again after <ws> seconds\n"
		"    Suspend                            Suspends (sleeps) computer instantly\n"
		"    SuspendAfter <ss>                  Suspends (sleeps) computer after <ss> seconds\n"
		"    SuspendWakeupAfter <ws>            Suspends (sleeps) computer instantly and wakes it\n"
		"                                       up again after <ws> seconds\n"
		"    SuspendAfterWakeupAfter <ss> <ws>  Suspends (sleeps) computer in <ss> seconds and\n"
		"                                       wakes it up again after <ws> seconds\n"
					);
}

const WCHAR wcActionHybernating		[]	= L"Hybernating workstation/computer";
const WCHAR wcActionLoggingOff		[]	= L"Logging off";
const WCHAR wcActionLocking			[]	= L"Locking workstation/computer";
const WCHAR wcActionRestarting		[]	= L"Restarting workstation/computer";
const WCHAR wcActionShuttingDown	[]	= L"Shutting down workstation/computer";
const WCHAR wcActionSuspending		[]	= L"Suspending (sleeping) workstation/computer";
const WCHAR wcActionWakingUp		[]	= L"Waking up workstation/computer";
const WCHAR wcActionPowerOff		[]	= L"Shutting down and switching off workstation/computer";

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

void ourmain (void)
{
	// The command-line arguments.
	int			nArgs;											// Amount of arguments.
	WCHAR		**wcArgs = cmdLineArgsW (&nArgs);				// Array of arguments.
	swallowExeArgW (&nArgs, &wcArgs);							// Suppress the executable.

	//doWeHaveInteractiveSessions ();

	AttachConsole (ATTACH_PARENT_PROCESS);
	SetCodePageToUTF8 ();

	bool		bCmdComplete	= false;
	WCHAR		*pwc			= NULL;
	numArg		evalArg			= enArgInvalid;
	uint64_t	n1, n2;

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
			if	(isArgumentIgnoreCaseW (L"Hybernate", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionHybernating ();
				HybernateComputer ();
			} else
			if	(isArgumentIgnoreCaseW (L"HybernateAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionHybernating);
					HybernateComputer ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"Logoff", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionLoggingOff ();
				Logoff ();
			} else
			if	(isArgumentIgnoreCaseW (L"LogoffAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionLoggingOff);
					Logoff ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"Lock", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionLocking ();
				LockWorkStation ();
			} else
			if	(isArgumentIgnoreCaseW (L"LockAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionLocking);
					LockWorkStation ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"PowerOff", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionPowerOff ();
				PowerOffComputer ();
			} else
			if	(isArgumentIgnoreCaseW (L"PowerOffAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionPowerOff);
					PowerOffComputer ();
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
			if	(		isArgumentIgnoreCaseW (L"Reboot",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"Restart",	wcArgs [cArg])
				)
			{
				bCmdComplete = true;
				outputActionRestarting ();
				RestartComputer ();
			} else
			if	(		isArgumentIgnoreCaseW (L"RebootAfter",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"RestartAfter",	wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionRestarting);
					RestartComputer ();
				}
			} else
			if	(isArgumentIgnoreCaseW (L"Shutdown", wcArgs [cArg]))
			{
				bCmdComplete = true;
				outputActionShuttingDown ();
				ShutdownComputer ();
			} else
			if	(isArgumentIgnoreCaseW (L"ShutdownAfter", wcArgs [cArg]))
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					if (n1 <= MAXDWORD)
					{
						bCmdComplete = true;
						waitForW (n1, wcActionShuttingDown);
						ShutdownComputer ();
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
				SuspendComputer ();
			} else
			if	(		isArgumentIgnoreCaseW (L"SleepAfter",	wcArgs [cArg])
					||	isArgumentIgnoreCaseW (L"SuspendAfter",	wcArgs [cArg])
				)
			{
				if (enArgIsNumber == (evalArg = compulsoryNumber (&n1, &cArg, nArgs, wcArgs)))
				{
					bCmdComplete = true;
					waitForW (n1, wcActionSuspending);
					SuspendComputer ();
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
							outWaitForW (n1, wcActionWakingUp); consoleOutW (L" \n");
							SuspendComputer ();
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
									outWaitForW (n2, wcActionWakingUp); consoleOutW (L" \n");
									SuspendComputer ();
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
			}
			if (bCmdComplete)
			{
				++ cArg;
				if (cArg < nArgs)
					consoleOutW (L"Ignored argument(s):");
				while (cArg < nArgs)
				{
					consoleOutW (L" ");
					consoleOutW (wcArgs [cArg]);
					++ cArg;
				}
				consoleOutW (L"\n");
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
	ExitProcess (EXIT_SUCCESS);
}
