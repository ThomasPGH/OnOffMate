#****************************************************************************************
#
#	File:		WarnDeprecated.pri
#	Why:		Qt project include file to emit compiler warnings if deprecated Qt APIs
#				 are used.
#	OS:			Qt.
#	Author:		Thomas
#	Created:	2020-01-15
#  
# History
# -------
#
# When			Who				What
# ---------------------------------------------------------------------------------------
# 2020-01-15	Thomas			Created. The idea and the content of this file has been
#								 acquired from
#								 https://github.com/Zalafina/Q7Zip/blob/master/Q7Zip/Q7Zip.pro .
#
#****************************************************************************************

# See https://doc.qt.io/qt-5/qtglobal.html#QT_DEPRECATED_WARNINGS .

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
