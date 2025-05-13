@echo.
@echo Script: %~n0%~x0
@echo.
@echo -----------------------------------------------------------------------
@echo This batch script first retains your own changes (for yourself only),
@echo carries out a pull (obtains changes from others), then exposes your
@echo changes for publication (commit), and finally pushes everything back
@echo to the server repository.
@echo.
@echo - Downloads changes from others:        Yes
@echo - Retains your own changes:             Yes
@echo - Exposes your changes to others:       Yes
@echo - Uploads your own changes:             Yes
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
@echo Executing: git commit
@git commit
@echo.
@echo Executing: git push -u origin HEAD:master
@git push -u origin HEAD:master
@echo.
@pause
