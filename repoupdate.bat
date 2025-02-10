@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
echo off

rem git pull
git submodule update --recursive --remote
git pull --recurse-submodules

::Restore previous folder
@echo ********** END: %~n0%~x0 (%DATE% : %TIME%) ********** 
pause
