@echo ********** BEGIN: %~n0%~x0 (%DATE% : %TIME%) **********
::@echo off

::---------------------------------------------------------------
::Description: 
::	Renders a single element inside a SVG file into a single PNG file.
::
::Params:
::	%1 : The directory where the data should be stored.
::	%2 : The svg file to extract and render the element list.
::		%3 : The ID of the element inside the svg file to render
::		%4 : The quality specific to the destination platform.
::
::EnvVars:
::		%INKSCAPE_CMD%: The command to invoke inkscape.
::
::Notes:
::		This file will not work if executed isolated. Some environment variables and
::		parameters should be set becore calling it.
::---------------------------------------------------------------

@echo on
SET SRC_FILE=%1
SET DST_FILE=%2

::Process textures as if they are for the production platform
%INKSCAPE_CMD% %SRC_FILE% --export-id %2 --export-use-hints --export-background=#808080 --export-background-opacity=0.0 --export-png %DST_FILE%.png --export-width 20

@echo ********** END: %~n0%~x0 (%DATE% : %TIME%) ********** 
