# OnOffMate (oom)

OnOffMate (oom) is a tiny Windows command-line utility that serves as
hybernation, sleep, recycle bin, and power helper.

The software was originally created out of the necessity to put my works laptop to sleep after
a short delay to give me enough time to hit a key on the keyboard twice for my
[KVM switch](https://en.wikipedia.org/wiki/KVM_switch) to move on to one of my other PCs.
Without OnOffMate's delay the keypresses would prevent the laptop from going to sleep, probably
because of a USB driver issue.

OnOffMate is a 64 bit Windows application only.

The reason why it's so small is because it doesn't link to the Windows runtime libraries, and
because it is written in C, obviously.

Not linking to the Windows C runtime libraries (CRT) is also the reason why it won't build for 32 bit
Windows. Some 64 bit operations are not available on 32 bit CPUs but are emulated by the Windows
runtime libaries. However, OnOffMate lacks implementations for those.


```
OnOffMate - Ver. 1.003 (2025-06-04) - Hybernation, sleep, recycle bin, and power helper

  OnOffMate [command]

  Commands:
    ? or /? or h or -h or --help       Outputs this help.
    /a                                 Aborts a task with a grace period.
    Abort                              Aborts a task with a grace period.
    EmptyRecycleBin     [dir1] [...]   Empties either all recycle bins of all drives and
                                       folders, or for [dir1], [dir2], etc only.
    EmptyRecycleBinNC   [dir1] [...]   Empties recycle bins without confirmation.
    EmptyRecycleBinNCP  [dir1] [...]   Empties recycle bins without confirmation and
                                       progress bar.
    EmptyRecycleBinNCPS [dir1] [...]   Empties recycle bins without confirmation, progress.
                                       bar, and sound.
    EmptyRecycleBinNCS  [dir1] [...]   Empties recycle bins without confirmation and
                                       sound.
    EmptyRecycleBinNP   [dir1] [...]   Empties recycle bins without progress bar.
    EmptyRecycleBinNPS  [dir1] [...]   Empties recycle bins without progress bar and sound.
    EmptyRecycleBinNS   [dir1] [...]   Empties recycle bins without sound.
    Hybernate                          Hybernates computer instantly.
    HybernateAfter <hs>                Hybernates computer after <hs> seconds.
    Lock                               Locks computer instantly.
    LockAfter <ls>                     Locks computer after <ls> seconds.
    Logoff                             Logs off current user.
    LogoffAfter <os>                   Logs off current user after <os> seconds.
    PowerOff                           Shuts down and powers off computer instantly.
    PowerOffAfter <ps>                 Shuts down and powers off computer in <ps> seconds.
    PowerOffMsgAfter <ps> <msg>        Shuts down and powers off computer in <ps> seconds
                                       with message <msg>.
    QueryRecycleBin     [dir1] [...]   Queries either all recycle bins of all drives and
                                       folders, or for [dir1], [dir2], etc only.
    Reboot                             Restarts/reboots computer instantly.
    RebootAfter <rs>                   Restarts/reboots computer after <rs> seconds.
    Restart                            Restarts/reboots computer instantly.
    RestartAfter <rs>                  Restarts/reboots computer after <rs> seconds.
    Shutdown                           Shuts down and powers off computer instantly.
    ShutdownAfter <ds>                 Shuts down and powers off computer in <ds> seconds.
    ShutdownMsgAfter <ds> <msg>        Shuts down and powers off computer in <ds> seconds
                                       with message <msg>
    Sleep                              Suspends (sleeps) computer instantly.
    SleepAfter <ss>                    Suspends (sleeps) computer after <ss> seconds.
    SleepWakeupAfter <ws>              Suspends (sleeps) computer instantly and wakes it
                                       up again after <ws> seconds.
    SleepAfterWakeupAfter <ss> <ws>    Suspends (sleeps) computer in <ss> seconds and
                                       wakes it up again after <ws> seconds.
    Suspend                            Suspends (sleeps) computer instantly.
    SuspendAfter <ss>                  Suspends (sleeps) computer after <ss> seconds.
    SuspendWakeupAfter <ws>            Suspends (sleeps) computer instantly and wakes it
                                       up again after <ws> seconds.
    SuspendAfterWakeupAfter <ss> <ws>  Suspends (sleeps) computer in <ss> seconds and
                                       wakes it up again after <ws> seconds.
    Ver                                Prints the version info.
    Version                            Prints the version info.
    WakeOnLAN <brip> <mac> [-f6]       Wakes the host with broadcast IP <brip> and MAC
                                       address <mac>. For example, if the IP address of the
                                       host to wake up is 192.168.0.97 and the subnet mask
                                       is 255.255.255.0, use 192.168.0.255 for <brip>.
                                       Argument -f6 forces IPv6 even if <brip> is IPv4.

  The original behaviour of the Shutdown... commands (shutting down without power off) has
  been changed to be identical to the PowerOff... commands (shutting down and power off).
```

# Installation

It's a command-line utility.

Place both executables (__OnOffMate.exe__ and __oom.exe__) in a folder
that's in your **Path** environment variable. Both executables are identical, and **oom.exe** only exists
because "oom sleepafter 30" is easier to type than "onoffmate sleepafter 30".

I for instance got a whole bunch of command-line utilities in a folder **C:\bin**. If you
haven't got one yet, I recommend that you prepare a similar folder and place its name in your **Path**
environment variable. The folder's location or name don't really matter.

To update your *Path* variable, simply open **Settings**. That's the sprocket icon in the
start menu. In Settings, start typing "envi" inside the search bar, which pulls up a list
that contains two entries. One is for editing system environment variables, the other one
is for changing environment variables for your account only. Edit the **Path** variable and
and add the folder where you placed __OnOffMate.exe__ and __oom.exe__, which is **C:\bin** in my case.
The process is more or less the same for all currently supported Windows versions.

The next time you open a command-line window, both executables will be available. Note that
if you had a command-line window open already before you added the folder to your **Path** variable,
the change won't affect this open instance. It only applies to command-line windows openend after
the change.


# Building OnOffMate (oom)

Open the file **OnOffMate.sln** in folder **msproj\msvc2019\OnOffMate** with Microsoft Visual
Studio 2019, go to the **Build** menu and pick the **Batch Build...** submenu. Select the two
**x64** platforms and click on **Rebuild**.

I'm sure it'd build with newer or older versions of Microsoft Visual Studio too. However, building
with an older version is not that straight-forward as you'd have to change the version of MSVC
inside the *.sln* file first, while newer versions offer to do this for you.

Note that [Microsoft Visual Studio](https://visualstudio.microsoft.com/) (or short, **Visual Studio**)
is __*not*__ the same as [Visual Studio Code](https://code.visualstudio.com/).
