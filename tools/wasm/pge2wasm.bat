@echo off
:: Convenience Utility to build projects using olc::PixelGameEngine, using
:: Emscripten, producing WASM based output.
::
:: OneLoneCoder.com 2021 - Released under OLC-3 license
::
:: v1.00: Initial Release

setlocal enabledelayedexpansion enableextensions

:: Customize here ===========================================

:: Location of Emscripten SDK
set EMSDK="e:\pge_ems\emsdk\"

:: Location of olc::PixelGameEngine header file
set OLCPGE=".\"
set OLCPGE="e:\pge_dev\olcPixelGameEngine_dev\Deploy"

:: ==========================================================

set WORKINGDIR=%CD%

if not exist %EMSDK% (
	echo Error: No Emscripten SDK folder found!
	goto :fail
)

if not exist %OLCPGE% (
	echo Error: Invalid PGE Location!
	goto :fail
)

if "%1"=="build" goto :build
if "%1"=="run" goto :run
if "%1"=="clean" goto :clean
goto :error
	
:build 
	:: Configure path variables
	cd %EMSDK%
	call emsdk_env.bat 
	
	:: Create working folder
	cd %WORKINGDIR%
	if not exist ".\WASM" (
		echo Creating .\WASM output folder
		mkdir ".\WASM"
	)
	
	:: Grab all cpp files if no specific file is given
	if "%~2"=="" goto :graball	
	set CPP=%~2	
	goto :embuild
	
:graball	
	echo Gathering *.cpp files from
	echo    %CD%
	set CPP=
	for %%x in (%CD%\*.cpp) do set CPP=!CPP! %%x
	set CPP=%CPP:~1%

:embuild	

echo %CPP%
	if exist "./assets" (
		echo Starting Build with assets...
		call em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 %CPP% -o .\WASM\pge.html -I %OLCPGE% --preload-file .\assets
	) else (
		echo Starting Build without assets...
		call em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 %CPP% -o .\WASM\pge.html -I %OLCPGE%
	)
	
	echo Build Completed
	goto :success

:run
	:: Configure path variables
	cd %EMSDK%
	call emsdk_env.bat
	cd %WORKINGDIR%
	emrun .\WASM\pge.html
	goto :success

:clean
	if exist ".\WASM" (
		rmdir /s /q .\WASM
	)
	goto :success

:error
	echo Error: Incorrect Input
	goto :fail

:success
	echo Exit With Success
	goto :leave

:fail
	echo Exit with Failure
	goto :leave
	
:leave
	exit
	



