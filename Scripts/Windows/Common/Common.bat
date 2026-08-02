@echo off
setlocal EnableExtensions

set "ACTION=%~1"
if /I "%ACTION%"=="init" goto :init
if /I "%ACTION%"=="usage" goto :usage

echo [ERROR] Common.bat: unknown action "%ACTION%"
exit /b 1

:init
set "SCRIPT_DIR=%~2"
set "BUILD_NAME=%~3"

if "%SCRIPT_DIR%"=="" (
    echo [ERROR] Common.bat init: missing SCRIPT_DIR
    exit /b 1
)

if "%BUILD_NAME%"=="" (
    echo [ERROR] Common.bat init: missing BUILD_NAME
    exit /b 1
)

for %%I in ("%SCRIPT_DIR%\..\..\..") do set "REPO_ROOT=%%~fI"
if not exist "%REPO_ROOT%\CMakeLists.txt" (
    echo [ERROR] Repository root is invalid: "%REPO_ROOT%"
    exit /b 1
)

set "OUT_DIR=%REPO_ROOT%\out"
set "BUILD_ROOT=%OUT_DIR%\build"
set "INSTALL_ROOT=%OUT_DIR%\install"
set "LOG_ROOT=%OUT_DIR%\logs"
set "RUN_ROOT=%OUT_DIR%\run"
set "BUILD_DIR=%BUILD_ROOT%\%BUILD_NAME%"
set "INSTALL_DIR=%INSTALL_ROOT%\%BUILD_NAME%"

if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"
if not exist "%BUILD_ROOT%" mkdir "%BUILD_ROOT%"
if not exist "%INSTALL_ROOT%" mkdir "%INSTALL_ROOT%"
if not exist "%LOG_ROOT%" mkdir "%LOG_ROOT%"
if not exist "%RUN_ROOT%" mkdir "%RUN_ROOT%"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

set "RAND_A=%RANDOM%"
set "RAND_B=%RANDOM%"
set "LOG_FILE=%LOG_ROOT%\%BUILD_NAME%_%RAND_A%_%RAND_B%.log"
set "LAST_LOG_POINTER=%LOG_ROOT%\last_log_path.txt"

endlocal & (
    set "SK_REPO_ROOT=%REPO_ROOT%"
    set "SK_OUT_DIR=%OUT_DIR%"
    set "SK_BUILD_ROOT=%BUILD_ROOT%"
    set "SK_INSTALL_ROOT=%INSTALL_ROOT%"
    set "SK_LOG_ROOT=%LOG_ROOT%"
    set "SK_RUN_ROOT=%RUN_ROOT%"
    set "SK_BUILD_DIR=%BUILD_DIR%"
    set "SK_INSTALL_DIR=%INSTALL_DIR%"
    set "SK_LOG_FILE=%LOG_FILE%"
    set "SK_LAST_LOG_POINTER=%LAST_LOG_POINTER%"
)
exit /b 0

:usage
echo Usage:
echo   call Common.bat init ^<SCRIPT_DIR^> ^<BUILD_NAME^>
exit /b 0
