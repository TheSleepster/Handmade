@echo off
REM MSVC RELEASE
REM ================================================================================================================

REM /Ot /O2 for release
REM remove -Zi

Set CommonCompilerFlags=-GR- -EHa- -Ox -Ot -O2 -Oi -W4 -wd4189 -wd4200 -wd4996 -wd4706 -wd4530 -wd4100 -wd4201
Set CommonLinkerFlags=-incremental:no -OPT:REF kernel32.lib user32.lib gdi32.lib opengl32.lib -SUBSYSTEM:WINDOWS -ENTRY:WinMainCRTStartup
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
del *.pdb
cl -DSUGAR_SLOW=0 ../code/win32_Sugar.cpp -MT %CommonCompilerFlags% -link %CommonLinkerFlags% -OUT:"../data/Handmade.exe" 
popd
