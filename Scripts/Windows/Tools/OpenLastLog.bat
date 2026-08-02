@echo off
setlocal EnableExtensions

for %%I in ("%~dp0..\..\..") do set "REPO_ROOT=%%~fI"
set "POINTER=%REPO_ROOT%\out\logs\last_log_path.txt"

if not exist "%POINTER%" (
    echo [ERROR] Last log pointer not found:
    echo         "%POINTER%"
    pause
    exit /b 1
)

set "LOG_FILE="
set /p LOG_FILE=<"%POINTER%"

if "%LOG_FILE%"=="" (
    echo [ERROR] Last log path is empty.
    pause
    exit /b 1
)

if not exist "%LOG_FILE%" (
    echo [ERROR] Log file not found:
    echo         "%LOG_FILE%"
    pause
    exit /b 1
)

start "" notepad "%LOG_FILE%"
endlocal
exit /b 0
