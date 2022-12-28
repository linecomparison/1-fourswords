:: =================================================
:: SET DEBUGMODE HERE
:: =================================================
set debugMode=false
set patchHeader=true
cls
@echo off
echo =========================================
echo                 Creating Resources
echo =========================================

mkdir "assets/gfx/resources"

REM Sprites
"f:/programs/tools/programming/devkitPro/tools/bin/grit" "./assets/gfx/sprites/link.png" -ff "assets/gfx/sprites/grit/link.grit" -o "assets/gfx/resources/spriteMaps"
"f:/programs/tools/programming/devkitpro/tools/bin/grit" "./assets/gfx/sprites/cursor.png" -ff "assets/gfx/sprites/grit/cursor.grit"  -o "assets/gfx/resources/spriteMaps"

REM Tilesets
"f:/programs/tools/programming/devkitpro/tools/bin/grit" "assets/gfx/tilesets/house2.png" -ff "assets/gfx/tilesets/grit/house2.grit" -o "assets/gfx/resources/tileSets" 
"f:/programs/tools/programming/devkitpro/tools/bin/grit" "assets/gfx/tilesets/pause.png" -ff "assets/gfx/tilesets/grit/pause.grit" -o "assets/gfx/resources/tileSets"

REM Tidy Up
for /r "assets/gfx/resources" %%x in (*.h) do move "%%x" "assets/include/resources"
for /r "assets/gfx/resources" %%x in (*.c) do move "%%x" "source/resources"

rmdir "assets/gfx/resources"
echo "Resources created."

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
::copy "build\soundbank.h" "assets/soundbank.h"
echo -----------------------------------------
echo Compile Completed.

rm build/*
rmdir build

echo =========================================
echo                 Patching Header
echo =========================================
if %patchHeader%==true copy "f:\programs\tools\programming\devkitpro\tools\bin\liteips.exe"
if %patchHeader%==true copy "assets\misc\patches\header.ips"
if %patchHeader%==true for %%x in (*.gba) do liteips.exe header.ips "%%x"
if %patchHeader%==true rm liteips.exe
if %patchHeader%==true rm header.ips
if %debugMode%==false for /r "." %%x in (*.elf) do del "%%x"
if %debugMode%==false for %%x in (*.gba) do "%%x"

::for /r "." %%x in (*.gba) do copy "%%x" G:\ezfla_up.bin
::for /r "." %%x in (*.gba) do copy "%%x" G:\fourswords.gba
::if %debugMode%==false for /r "." %%x in (*.gba) do del "%%x"

if %debugMode%==false for /r "." %%x in (*.gba) do del "%%x"
::if %debugMode%==false for /r "." %%x in (*.sav) do del "%%x"




