::---------------------------------------------------------------
@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
::---------------------------------------------------------------

::@echo off

::---------------------------------------------------------------
::Description: 
::	Renders a single a SVG file into several PNG file.
::
::Params:
::	%1 : The svg file to render.
::	%2 : The directory where the data should be stored.
::
::EnvVars:
::		%INKSCAPE_CMD%: The command to invoke inkscape.
::
::Notes:
::		This file will not work if executed isolated. Some environment variables and
::		parameters should be set becore calling it.
::---------------------------------------------------------------

@echo on

SET INKSCAPE_CMD="C:\Archivos de programa\Inkscape\Inkscape.exe"
SET SRC_FILE=%1
SET DST_FILE=%~n1.png
SET SVG_WIDTH=20

%INKSCAPE_CMD% %SRC_FILE% --export-png %DST_FILE% --export-width %SVG_WIDTH%
::%INKSCAPE_CMD% %SRC_FILE% --export-png %DST_FILE%

::---------------------------------------------------------------
@echo ********** END: %~n0%~x0 (%DATE% : %TIME%) ********** 
::---------------------------------------------------------------
