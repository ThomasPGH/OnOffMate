#****************************************************************************************
#
#	File:		CheckQtVersion.pri
#	Why:		Qt project include file to test for a minimum version of Qt.
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

# Define two qmake variables in the Qt project file (.pro) that includes this file
#	(CheckQtVersion.pri).
#
# Example:
# PROJECTNAME=YourProject
# QTVERREQUIRED=5.11.3

# Check that the Qt version used fulfills the minimum requirement.
!build_pass {
	message("Qt version is $${QT_VERSION}.")
	!versionAtLeast(QT_VERSION, $${QTVERREQUIRED}) {
		error("Unsupported Qt version "$${QT_VERSION}". $${PROJECTNAME} requires Qt version "$$QTVERREQUIRED" or higher.")
	}
}
