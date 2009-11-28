echo off

::-------------------------------------------------
:: This script passes values from the hudson matrix
:: project to msbuild.
::-------------------------------------------------

:: Set the target solution from the Hudson passed var.
:: NPPCR Solution files use only the VC major version
:: Leave early if the solution doesn't exist.
set VER_MINOR=%NPPCR_BUILD_VC_VER:*.=%
set VER_MINOR=.%VER_MINOR%
call set VER_MAJOR=%%NPPCR_BUILD_VC_VER:%VER_MINOR%=%%
set SOLUTION=notepadPlus.%VER_MAJOR%.sln
if NOT EXIST "%NPPCR_ROOT_PATH%%SOLUTION%" exit 1

:: Remove anything that may be in the bin directory
set NPPCR_BIN_PATH=%NPPCR_ROOT_PATH%PowerEditor\bin\
if EXIST "%NPPCR_BIN_PATH%" del /q "%NPPCR_BIN_PATH%"

:: Clear environment variables for gtest/mock
if NOT [%NPPCR_GTEST_STYLE%]==[Environment] (
  set NPPCR_GTEST_DIR=
  set NPPCR_GTEST_DIR=
)

:: Setup the build environment
set VC_PATH_VER=%NPPCR_BUILD_VC_VER:.=%
set VC_VARS=VS%VC_PATH_VER%COMNTOOLS
call set VC_VARS=%%%VC_VARS%%%..\..\VC\vcvarsall.bat
call "%VC_VARS%" %NPPCR_BUILD_ARCH%

:: Build the msbuild command
set MSBUILD_OPTS=/v:detailed /t:rebuild /p:Configuration
set MSBUILD_OPTS=%MSBUILD_OPTS%=%NPPCR_BUILD_CONFIGURATION%
call msbuild %MSBUILD_OPTS% "%NPPCR_ROOT_PATH%%SOLUTION%"
if ERRORLEVEL 1 exit %ERRORLEVEL%

:: Only check debug builds for memory leaks
set NPPCR_BIN_PATH=%NPPCR_ROOT_PATH%PowerEditor\bin
set NPPCR_BIN=%NPPCR_BIN_PATH%\notepad++Debug.exe
if NOT EXIST "%NPPCR_BIN%" exit 0

:: Build the console debugger command
set CDB_PATH=%MS_DBGTOOLS_PATH%cdb.exe
set CDB_ARGS=-kqm -g -G -y
set NPPCR_DBG_CMD="%CDB_PATH%" %CDB_ARGS% "%NPPCR_BIN_PATH%" "%NPPCR_BIN%"

:: Call once for output and then to check output
call %NPPCR_DBG_CMD%
call %NPPCR_DBG_CMD%|FINDSTR /C:"Detected memory leaks"
if %ERRORLEVEL% EQU 0 exit 1

exit %ERRORLEVEL%
