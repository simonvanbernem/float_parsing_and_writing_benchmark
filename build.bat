
@echo off
if not exist "build" mkdir build

where cl > nul 2>&1
rem where returns error code 1 if it can't find the executable, which we can check with %errorlevel%

if %errorlevel%==1 (
    echo calling vcvarsall
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 > nul
)


cl.exe main.cpp -Fobuild/main.obj -Fdbuild/main.pdb -std:c++20 /link /out:build/main.exe