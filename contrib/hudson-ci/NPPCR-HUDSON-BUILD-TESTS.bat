ECHO off
REM  Hudson to MSBuild script for N++CR

:: Google TEST and Mock paths
if NOT [%NPPCR_GTEST_STYLE%]==[ENVIRONMENT] (
  set GOOGLE_TEST_DIR=
  set GOOGLE_MOCK_DIR=
) else (
	if NOT [%GOOGLE_TEST_DIR:~-1%]==[\] set GOOGLE_TEST_DIR = %GOOGLE_TEST_DIR%\
	if NOT [%GOOGLE_MOCK_DIR:~-1%]==[\] set GOOGLE_MOCK_DIR = %GOOGLE_MOCK_DIR%\
)

:: Target solution name from the build's VC version
set VER_MINOR=%NPPCR_BUILD_VC_VER:*.=%
set VER_MINOR=.%VER_MINOR%
call set VER_MAJOR=%%NPPCR_BUILD_VC_VER:%VER_MINOR%=%%
set SOLUTION=notepadPlus.%VER_MAJOR%.sln
echo Build test target solution:: %SOLUTION%

:: VC environment from the build's VC version
set VC_PATH_VER=%NPPCR_BUILD_VC_VER:.=%
set VC_VARS=VS%VC_PATH_VER%COMNTOOLS
call set VC_VARS=%%%VC_VARS%%%..\..\VC\vcvarsall.bat
echo Build test VC variable target:: %VC_VARS%

:: MSBuild command
set MSBUILD_OPTS=/v:detailed /t:rebuild /p:Configuration
set MSBUILD_OPTS=%MSBUILD_OPTS%=%NPPCR_BUILD_CONFIGURATION%
echo Build test MSBuild target:: %MSBUILD_OPTS%

:: Console debugger command
set NPPCR_BIN_PATH=%NPPCR_ROOT_PATH%PowerEditor\bin
set NPPCR_BIN=%NPPCR_BIN_PATH%\notepad++Debug.exe
set CDB_PATH=%MS_DBGTOOLS_PATH%cdb.exe
set CDB_ARGS=-kqm -g -G -y
set NPPCR_DBG_CMD="%CDB_PATH%" %CDB_ARGS% "%NPPCR_BIN_PATH%" "%NPPCR_BIN%" -leakdetect
echo Build test Mem leak target:: %NPPCR_DBG_CMD%

:: Main routine
if NOT EXIST "%NPPCR_ROOT_PATH%%SOLUTION%" exit 1
call "%VC_VARS%" %NPPCR_BUILD_ARCH%
if %ERRORLEVEL% EQU 0 call msbuild %%MSBUILD_OPTS%% "%NPPCR_ROOT_PATH%%SOLUTION%"
if NOT [%NPPCR_BUILD_CONFIGURATION%]==[Debug] EXIT %ERRORLEVEL%
if NOT %ERRORLEVEL% EQU 0 (
	call :CLEAN_UP
	exit 1
)

call %NPPCR_DBG_CMD%
if NOT %ERRORLEVEL% EQU 0 (
	call :CLEAN_UP
	exit 1
)

call %NPPCR_DBG_CMD%|FINDSTR /C:"Detected memory leaks"
if %ERRORLEVEL% EQU 1 (
	call :CLEAN_UP
	exit 0
) else (
	call :CLEAN_UP
	exit 1
)

:CLEAN_UP
call msbuild /t:clean "%NPPCR_ROOT_PATH%%SOLUTION%"
goto :EOF
