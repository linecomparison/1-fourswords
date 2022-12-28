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
rm textures.bat
rm compile.bat