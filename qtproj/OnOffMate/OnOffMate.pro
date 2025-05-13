#****************************************************************************************
#
#	File:		OnOffMate.pro
#				Software to power the computer off.
#	Why:		-
#	OS:			Qt
#	Author:		Thomas
#	Created:	2024-04-07
#  
# History
# -------
#
# When			Who				What
# ---------------------------------------------------------------------------------------
# 2024-04-07	Thomas			Created.
#
#****************************************************************************************

# The name of this project. Makes some code further down more flexible and portable.
PROJECTNAME=OnOffMate

# Required Qt version is 5.11.3.
#	See http://doc.qt.io/qt-5/qmake-test-function-reference.html#message-string
#	about the !build_pass: thingy.
QTVERREQUIRED=5.11.3

# The minimum version of the language tool required. It is used further down to check for
#	the correct version to be provided/built. Qt's standard filename extension for included
#	project files is ".pri" ("project include").
# Note that this project does not need the language tool.
#include (../../../../src/C/MLanguageTool/MLangToolMinVersion.pri)

# Tell the compiler to emit warnings if deprecated Qt APIs are used.
include (WarnDeprecated.pri)

# Perform the Qt version check.
include (CheckQtVersion.pri)

# Output the build platform.
include (OutputBuildPlatform.pri)

# Ensure that the setting "Build directory" in Qt Creator starts with the correct
#	folder name prefix.
# Note that in this project we don't care what the build folder is named.
# include (../../../../src/Qt/CheckBuildDirectoryName.pri)

TARGET = $${PROJECTNAME}	# See http://doc.qt.io/qt-5/qmake-variable-reference.html#target .
QT -= gui					# It is a console application.
TEMPLATE = app

CONFIG += c99				# See http://doc.qt.io/qt-5/qmake-variable-reference.html#config .
#CONFIG += c++11				# See http://doc.qt.io/qt-5/qmake-variable-reference.html#config .
CONFIG += force_debug_info	# Force the generation of debug information. On Windows, this creates
							#	a .pdb file.
							#	See https://stackoverflow.com/questions/6993061/build-qt-in-release-with-debug-info-mode .
CONFIG += console
CONFIG -= app_bundle
CONFIG -= gui
CONFIG -= qt				# See https://doc.qt.io/qt-5/qmake-variable-reference.html#config .

# See https://forum.qt.io/topic/96936/macos-mojave-and-qt-creator/3 . This warning might
#	pop up on OSX Mojave. I reckon it'll disappear with a newer version of Qt. According
#	to the documentation, Qt 5.12.0 is the first one to fully support Mojave (OSX 10.14).
CONFIG += sdk_no_version_check

# The required modules.
#QT += quick						# See http://doc.qt.io/qt-5/qmake-variable-reference.html#qt
#QT += quickcontrols2			#  and http://doc.qt.io/qt-5/qtmodules.html .
#QT += qml
#QT += core
#	multimedia \
#	sensors \
#	positioning

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# This is the Bad Boys RDP Blocker (bbRDPb) application.
DEFINES += U_BBRDPB_APP

# We don't need ubf_sockets.h in ubfstring.c/h for the File Copy Manager. This removes a few
#	functions from ubfstring.c/h.
DEFINES += UBF_STRING_DONT_BUILD_SOCKETS
# We also don't need any of the secure functions. This removes the dependency to Libsodium.
DEFINES += UBF_LANG_TEXTS_DONT_BUILD_SECURE_FNCTS
# We don't need the ROT 13 stuff for our configuration file.
DEFINES += UBF_LANG_TEXT_DONT_BUILD_ROT13
# We don't need the integer array functions.
DEFINES += UBF_LANG_TEXT_DONT_BUILD_INTS_UINTS
# We don't need debug hex dumps in ubfdebug.c/h.
DEFINES += UBF_DEBUG_NO_HEX_DUMP
# Suppresses the MSVC warnings for strcpy (), memcpy (), etc.
DEFINES += _CRT_SECURE_NO_WARNINGS
# We link to Advapi32.lib.
DEFINES += HAVE_ADVAPI32
# We link to Shell32.lib.
DEFINES += HAVE_SHELLAPI
# We link to Netapi32.lib
DEFINES += HAVE_NETAPI32

# We don't need NULL-safe SUBF_STRINGs.
DEFINES += UBF_STRING_DONT_BUILD_NULL_SAFE
# We also don't need the mem member in SUBF_STRINGs. This saves some space.
DEFINES += UBF_STRING_DONT_BUILD_MEM_OFFSET
# This is a single-threaded application.
DEFINES += UBF_SINGLE_THREADED
# No wildcards.
#DEFINES += HAVE_STRWILDCARDS
# Use current directory instead of UBF structure.
DEFINES += UBF_TYPE_DEFINITIONS_USE_CURR_DIR
DEFINES += UBF_DEBUG_USE_CURR_DIR
DEFINES += UBF_STRINTUINT_USE_CURR_DIR
DEFINES += UBF_SINGLEBITS_USE_CURR_DIR
DEFINES += UBF_STRWILDCARDS_USE_CURR_DIR
DEFINES += UBF_MEM_NO_UBFMEM

#DEFINES += AUDIO_IS_HEADLESS

# Additional libraries on Windows.
win32:LIBS += Ole32.lib
win32:LIBS += Shell32.lib
win32:LIBS += Advapi32.lib										# Windows service.
win32:LIBS += Netapi32.lib

# If this -ldl is missing, the linker on Linux complains with
#  "sqlite3.o: undefined reference to symbol 'dlclose@@GLIBC_2.2.5'".
linux:LIBS += \
		-ldl

HEADERS += \
	../../src/externC.h \
	../../src/WinAPI_U8.h

SOURCES += \
	../../src/WinAPI_U8.c

