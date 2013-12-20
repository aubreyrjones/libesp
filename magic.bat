@echo off

if "%OLDPATH%"=="" @set OLDPATH=%PATH%
if NOT "%1"=="" GOTO %1

rem ******************************** x86 *********************************************
rem *************************** Windows 8 SDK ****************************************

@set LIB=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib;%LIB%
@set LIB=C:\Program Files (x86)\Windows Kits\8.0\lib\win8\um\x86;%LIB%
@set LIBPATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib;%LIBPATH%
@set LIBPATH=C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x86;%LIBPATH%


@set INCLUDE=C:\Program Files (x86)\Windows Kits\8.0\include\um;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Windows Kits\8.0\include\winrt;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Windows Kits\8.0\include\shared;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\include;%INCLUDE%

@set DevEnvDir=C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE

@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE;%PATH%
@set PATH=C:\Program Files (x86)\Windows Kits\8.0\bin\x86;%PATH%


rem ********************** Windows 7 SDK (A little bit screwy) ***********************

rem @set LIB=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib;%LIB%
rem @set LIB=C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib;%LIB%
rem 
rem @set LIBPATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib;%LIBPATH%
rem @set LIBPATH=C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib;%LIBPATH%
rem 
rem @set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include;%INCLUDE%
rem @set INCLUDE=C:\Program Files\Microsoft SDKs\Windows\v7.1\Include;%INCLUDE%
rem 
rem @set DevEnvDir=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE
rem 
rem @set PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin;%PATH%
rem @set PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE;%PATH%



pushd C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
@call "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\Bin\dx_setenv.cmd" x86
popd

echo Tools/Vars set up for x86.

color 0d
GOTO Exit


rem ******************************** x64 *********************************************
:moremagic
:-moremagic
:-mm

@set LIB=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64;%LIB%
@set LIB=C:\Program Files (x86)\Windows Kits\8.0\lib\win8\um\x64;%LIB%

@set LIBPATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64;%LIBPATH%
@set LIBPATH=C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x64;%LIBPATH%

@set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\include;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Windows Kits\8.0\include\um;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Windows Kits\8.0\include\winrt;%INCLUDE%
@set INCLUDE=C:\Program Files (x86)\Windows Kits\8.0\include\shared;%INCLUDE%

@set DevEnvDir=C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE

@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\x86_amd64;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE;%PATH%
@set PATH=C:\Program Files (x86)\Windows Kits\8.0\bin\x64;%PATH%


pushd C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
@call "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\Bin\dx_setenv.cmd" AMD64
popd

echo Tools/Vars set up for x64.
@set MOREMAGIC=1

color 70
GOTO Exit


:old_magic
pushd C:\Program Files (x86)\Microsoft Visual Studio 11.0\
@call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86
echo VC Vars set up for x86
popd

pushd C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
@call "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\Bin\dx_setenv.cmd" x86
echo DirectX vars set up for x86
popd


color 0a
@echo g2g
GOTO Exit



:reset
:-r
:dispell
@set LIB=""
@set LIBPATH=""
@set INCLUDE=""
@set DevEnvDir=""
@set PATH=%OLDPATH%
@set MOREMAGIC=0
color 0e

:Exit
