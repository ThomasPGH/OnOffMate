#****************************************************************************************
#
#	File:		OutputBuildPlatform.pri
#	Why:		Qt project include file to output the build platform when qmake runs.
#	OS:			Qt.
#	Author:		Thomas
#	Created:	2020-01-15
#  
# History
# -------
#
# When			Who				What
# ---------------------------------------------------------------------------------------
# 2020-01-15	Thomas			Created.
#
#****************************************************************************************

# Outputs the host OS. See http://doc.qt.io/qt-5/qmake-variable-reference.html#qmake-host .
# message ($$QMAKE_HOST.arch)
# This gives us the build platform:
BUILDPLATFORM=$$QMAKE_HOST.os
# On Windows this simply says Windows. On Linux this simply says Linux. Sounds reasonable.
!build_pass:message (Build platform is "$$BUILDPLATFORM".)
