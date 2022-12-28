:: =================================================
:: SET DEBUGMODE HERE
:: =================================================
set debugMode=false
cls
@echo off

echo =========================================
echo                 Creating Resources
echo =========================================

mkdir "assets/gfx/resources"

REM Sprites
for /r "./assets/gfx/sprites" %%x in (*.png) do "c:/devkitpro/tools/bin/grit" "%%x" -gB4 -Mw4 -Mh4  -p! -ftc  -fa -o "assets/gfx/resources/spriteMaps" -pT15

REM Tilesets
for /r "./assets/gfx/tilesets" %%x in (*.png) do "c:/devkitpro/tools/bin/grit" "%%x" -gB4 -ftc -fh -m! -mR4 -fa -o "assets/gfx/resources/tileSets"
"c:/devkitpro/tools/bin/grit" "assets/gfx/tilesets/house2.png" -gB4  -ftc -fh -m! -mR4 -fa -o "assets/gfx/resources/tileSets" -pT9

REM Tilemaps
for /r "./assets/gfx/tilemaps" %%x in (*.png) do "c:/devkitpro/tools/bin/grit" "%%x" -gB4 -ftc -fh -m! -mR4 -fa -o "assets/gfx/resources/tileMaps"

REM Tidy Up
for /r "assets/gfx/resources" %%x in (*.h) do move "%%x" "assets/include/resources"
for /r "assets/gfx/resources" %%x in (*.c) do move "%%x" "source/resources"

rmdir "assets/gfx/resources"
echo "Resources created."
cls

echo =========================================
echo                 Moving Assets
echo =========================================

if %debugMode%==false xcopy "assets/misc/make/normal" "." /s /e /i /y
if %debugMode%==true  xcopy "assets/misc/make/debug" "." /s /e /i /y

make clean
echo =========================================
echo                 Compiling Music
echo =========================================
make
echo -----------------------------------------
del Makefile
copy "build\soundbank.h" "assets/soundbank.h"
echo -----------------------------------------
echo Compile Completed.

rm build/*
rmdir build
if %debugMode%==false for /r "." %%x in (*.elf) do del "%%x"
if %debugMode%==false for /r "." %%x in (*.gba) do "%%x"
::for /r "." %%x in (*.gba) do copy "%%x" G:\ezfla_up.bin
if %debugMode%==false for /r "." %%x in (*.gba) do del "%%x"