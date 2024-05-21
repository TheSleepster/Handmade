@echo off

Set CommonCompilerFlags=-MD -GR- -EHa- -Oi -Zi -W4 -wd4189 -wd4200 -wd4996 -wd4706 -wd4530 -wd4100
Set CommonLinkerFlags=kernel32.lib user32.lib gdi32.lib opengl32.lib
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl -DSUGAR_SLOW=1 ../code/win32_Sugar.cpp -I%STBImageInclude% %CommonCompilerFlags% /link /MACHINE:X64 /OUT:"Sugar.exe" %CommonLinkerFlags%
popd

@echo ====================
@echo Compilation Complete
@echo ====================
