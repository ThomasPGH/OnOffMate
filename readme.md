# OnOffMate

OnOffMate is tiny Windows command-line utility that serves as hybernation, sleep, and power helper.

It's a 64 bit application only.

The reason why it's so small is because it doesn't link to the Windows runtime libraries, and because it is written in C, obviously.

This is also the reason why it won't build for 32 bit Windows. Some 64 bit operations are not available on 32 bit CPUs but are emulated by the Windows runtime libaries. However, OnOffMate lacks implementations for those (at least currently, and most likely always will).


```
OnOffMate - Ver. 1.000 (2025-05-13) - Hybernation, sleep, and power helper

  OnOffMate [command]

  Commands:
    ? or /? h or -h or --help          Outputs this help
    Hybernate                          Hybernates computer instantly
    HybernateAfter <hs>                Hybernates computer after <hs> seconds
    Lock                               Locks computer instantly
    LockAfter <ls>                     Locks computer after <ls> seconds
    Logoff                             Logs off current user
    LogoffAfter <os>                   Logs off current user after <os> seconds
    PowerOff                           Shuts down and powers off computer instantly
    PowerOffAfter <ps>                 Shuts down and powers off computer in <ps> seconds
    PowerOffMsgAfter <ps> <msg>        Shuts down and powers off computer in <ps> seconds
                                       with message <msg>
    Reboot                             Restarts/reboots computer instantly
    RebootAfter <rs>                   Restarts/reboots computer after <rs> seconds
    Restart                            Restarts/reboots computer instantly
    RestartAfter <rs>                  Restarts/reboots computer after <rs> seconds
    Shutdown                           Shuts down computer instantly
    ShutdownAfter <ds>                 Shuts down computer in <ds> seconds
    ShutdownMsgAfter <ds> <msg>        Shuts down computer in <ds> seconds with message
                                       <msg>
    Sleep                              Suspends (sleeps) computer instantly
    SleepAfter <ss>                    Suspends (sleeps) computer after <ss> seconds
    SleepWakeupAfter <ws>              Suspends (sleeps) computer instantly and wakes it
                                       up again after <ws> seconds
    SleepAfterWakeupAfter <ss> <ws>    Suspends (sleeps) computer in <ss> seconds and
                                       wakes it up again after <ws> seconds
    Suspend                            Suspends (sleeps) computer instantly
    SuspendAfter <ss>                  Suspends (sleeps) computer after <ss> seconds
    SuspendWakeupAfter <ws>            Suspends (sleeps) computer instantly and wakes it
                                       up again after <ws> seconds
    SuspendAfterWakeupAfter <ss> <ws>  Suspends (sleeps) computer in <ss> seconds and
                                       wakes it up again after <ws> seconds
```
