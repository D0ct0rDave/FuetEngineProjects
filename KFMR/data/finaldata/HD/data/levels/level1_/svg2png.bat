@echo off
FOR %%k IN (*.svg) DO C:\Utils\X-Inkscape\x-inkscape %%k --export-dpi=300 --export-background=#cccccc --export-background-opacity=0.0 --export-png=%%k.png
pause
exit

REM inkscape $i --export-dpi=20 --export-height=20 --export-background=#cccccc --export-background-opacity=0.0 --export-png=`echo $i | sed -e 's/svg$/png/'`; done

REM inkscape keyframe_icon.svg --export-width=10 --export-height=10 --export-background=#cccccc --export-background-opacity=0.0 --export-png=keyframe_icon.png
REM inkscape fkeyframe_icon.svg --export-width=10 --export-height=10 --export-background=#cccccc --export-background-opacity=0.0 --export-png=fkeyframe_icon.png
REM inkscape logo_icon.svg --export-width=32 --export-height=32 --export-background=#cccccc --export-background-opacity=0.0 --export-png=logo_icon.png
REM inkscape rect_edition.svg --export-width=170 --export-height=115 --export-background=#cccccc --export-background-opacity=0.0 --export-png=rect_edition.png

REM inkscape wrap_finalvalue_icon.svg --export-width=32 --export-height=10 --export-background=#cccccc --export-background-opacity=0.0 --export-png=wrap_finalvalue_icon.png
REM inkscape wrap_initialvalue_icon.svg --export-width=32 --export-height=8 --export-background=#cccccc --export-background-opacity=0.0 --export-png=wrap_initialvalue_icon.png
REM inkscape wrap_loop_icon.svg --export-width=23 --export-height=16 --export-background=#cccccc --export-background-opacity=0.0 --export-png=wrap_loop_icon.png
REM inkscape wrap_pingpong_icon.svg --export-width=28 --export-height=16 --export-background=#cccccc --export-background-opacity=0.0 --export-png=wrap_pingpong_icon.png

REM mogrify -format xpm *.png
REM rm *.png

