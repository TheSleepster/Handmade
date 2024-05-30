@echo off

REM /Ot /O2 for release
REM remove -Zi

Set CommonCompilerFlags=-GR- -EHa- -Od -Oi -Zi -W4 -wd4189 -wd4200 -wd4996 -wd4706 -wd4530 -wd4100 -wd4201 
Set CommonLinkerFlags=-incremental:no kernel32.lib user32.lib gdi32.lib opengl32.lib -SUBSYSTEM:WINDOWS
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
del *.pdb
cl -DSUGAR_SLOW=1 ../code/win32_Sugar.cpp -MT %CommonCompilerFlags% -link %CommonLinkerFlags% -OUT:"Handmade.exe" 
cl -DSUGAR_SLOW=1 ../code/Sugar.cpp %CommonCompilerFlags% -MT -LD -link -incremental:no /PDB:Sugar_%RANDOM%%RANDOM%.pdb -EXPORT:GameUpdateAndRender -OUT:"GameCode.dll" 
popd

@echo ====================
@echo Compilation Complete
@echo ====================
