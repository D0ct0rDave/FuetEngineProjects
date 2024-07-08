@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
::@echo off

::---------------------------------------------------------------
::Description: 
::	Processes the textures inside a given folder and converts them from SVG to PNG files.
::
::Params:
::	%1 : The directory from which to get and store the data.
::
::EnvVars:
::	%DATABUILDPROCS_DIR% : The folder where the script processes are stored.
::	%PLATFORM% : The destination platform the datas will be generated for.
::		%SVG_QUALITY%: The quality specified to render the textures.
::
::Notes:
::		This file will not work if executed isolated. Some environment variables and
::		parameters should be set before calling it.
::---------------------------------------------------------------

::for every svg image
@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
::@echo off
call %CODERCHILDDEV%\shared\toolchain\build\procs\env_vars.bat
SET INKSCAPE_DIR=C:\Archivos de programa\Inkscape
SET INKSCAPE_CMD="%INKSCAPE_DIR%\inkscape"

::Retrieve the list of renderable objects
FOR /r %%i IN (*.svg) DO call svg_elements2tex.bat %%i
	
FOR /r %%i IN (*.png) DO %GRAPHICS_MAGICK_CMD% %%i %%~ni.xpm


::Restore the current folder
popd

::Create xpm

