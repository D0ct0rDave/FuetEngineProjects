@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
::@echo off

::---------------------------------------------------------------
::Description: 
::	Renders all the renderable elements in a SVG file to png.
::
::Params:
::	%1 : The directory where the data should be retrieved/stored.
::	%2 : The svg file to extract and render the element list.
::
::EnvVars:
::	%DATABUILDPROCS_DIR% : The folder where the script processes are stored.
::
::Notes:
::		This file will not work if executed isolated. Some environment variables and
::		parameters should be set before calling it.
::---------------------------------------------------------------

SET ELEM_LIST_FILE=svg_elem_list.txt

::Retrieve the list of renderable objects
call %DATABUILDPROCS_DIR%/get_svg_elem_list.bat %1 %ELEM_LIST_FILE%

::for every object inside the SVG file:
FOR /F %%i in (%ELEM_LIST_FILE%) do call svg_elem2tex.bat %1 %%i

::Delete temp file
::del %ELEM_LIST_FILE%
@echo ********** END: %~n0%~x0 (%DATE% : %TIME%) ********** 
