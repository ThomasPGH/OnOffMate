@echo.
@echo Script: %~n0%~x0
@echo.
@echo -----------------------------------------------------------------------
@echo This batch script first retains your own changes (for yourself only)
@echo and then carries out a pull (obtains changes from others).
@echo.
@echo - Downloads changes from others:        Yes
@echo - Retains your own changes:             Yes
@echo - Exposes your changes to others:       No
@echo - Uploads your own changes:             No
@echo.
@echo Press ENTER to carry out the update. Use CTRL-C or close the window
@echo to abort.
@echo.
@echo -----------------------------------------------------------------------
@pause
@echo.
@echo Executing: git add --all
@git add --all
@echo.
@echo Executing: git pull
@git pull
@echo.
@pause
