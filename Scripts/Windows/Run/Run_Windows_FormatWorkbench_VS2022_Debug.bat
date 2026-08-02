@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
call :FindSkylarkRoot "%SCRIPT_DIR%"
if not defined ROOT_DIR goto :root_failed
call :NormalizePath ROOT_DIR

set "BUILD_DIR=%ROOT_DIR%\out\build\windows-format-workbench-vs2022"
set "EXE=%BUILD_DIR%\Debug\SkylarkFormatWorkbenchQt.exe"
if not exist "%EXE%" set "EXE=%BUILD_DIR%\Tests\SkylarkFormatWorkbench\Debug\SkylarkFormatWorkbenchQt.exe"

if not exist "%EXE%" (
    for /f "delims=" %%E in ('dir /s /b "%BUILD_DIR%\SkylarkFormatWorkbenchQt.exe" 2^>nul') do (
        if not exist "%EXE%" set "EXE=%%E"
    )
)

if not exist "%EXE%" (
    echo [FAIL] Executable not found.
    echo [CHECKED] %BUILD_DIR%\Debug\SkylarkFormatWorkbenchQt.exe
    echo [CHECKED] %BUILD_DIR%\Tests\SkylarkFormatWorkbench\Debug\SkylarkFormatWorkbenchQt.exe
    echo [ACTION ] Run Scripts\Windows\Build\Build_Windows_FormatWorkbench_VS2022_Debug.bat first.
    pause
    exit /b 1
)

echo [RUN] %EXE%
"%EXE%"
pause
exit /b %ERRORLEVEL%

:FindSkylarkRoot
set "CANDIDATE=%~f1"
for /L %%N in (1,1,16) do (
    for %%I in ("!CANDIDATE!\.") do set "CLEAN=%%~fI"
    if exist "!CLEAN!\Source\SKCore\SKCore.cpp" if exist "!CLEAN!\Tests\SkylarkFormatWorkbench\CMakeLists.txt" (
        set "ROOT_DIR=!CLEAN!"
        exit /b 0
    )
    set "CANDIDATE=!CANDIDATE!\.."
)
exit /b 1

:NormalizePath
setlocal EnableDelayedExpansion
set "VALUE=!%~1!"
for %%I in ("!VALUE!\.") do set "VALUE=%%~fI"
:NormalizePathLoop
if "!VALUE:~-1!"=="\" (
    set "VALUE=!VALUE:~0,-1!"
    goto :NormalizePathLoop
)
if "!VALUE:~-1!"=="/" (
    set "VALUE=!VALUE:~0,-1!"
    goto :NormalizePathLoop
)
endlocal & set "%~1=%VALUE%"
exit /b 0

:root_failed
echo [FAIL] Cannot locate Skylark root.
pause
exit /b 1
