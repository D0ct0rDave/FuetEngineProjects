@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
echo off

git pull
git submodule update --init --recursive

::Restore previous folder
@echo ********** END: %~n0%~x0 (%DATE% : %TIME%) ********** 
