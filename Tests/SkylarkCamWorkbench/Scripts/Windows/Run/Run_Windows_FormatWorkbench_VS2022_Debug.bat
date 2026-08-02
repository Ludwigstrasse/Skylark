@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
call :FindSkylarkRoot "%SCRIPT_DIR%"
if defined ROOT_DIR call :StripTrailingSlash ROOT_DIR
if not defined ROOT_DIR (
    echo [FAIL] Cannot locate Skylark repository root.
    goto :failed
)

set "BUILD_DIR=%ROOT_DIR%\out\build\windows-format-workbench-vs2022"
set "EXE=%BUILD_DIR%\Tests\SkylarkFormatWorkbench\Debug\SkylarkFormatWorkbenchQt.exe"
if not exist "%EXE%" set "EXE=%BUILD_DIR%\Debug\SkylarkFormatWorkbenchQt.exe"
if not exist "%EXE%" (
    echo [FAIL] Executable not found.
    echo [ACTION] Run Scripts\Windows\Build\Build_Windows_FormatWorkbench_VS2022_Debug.bat first.
    echo [CHECKED] %BUILD_DIR%\Tests\SkylarkFormatWorkbench\Debug\SkylarkFormatWorkbenchQt.exe
    echo [CHECKED] %BUILD_DIR%\Debug\SkylarkFormatWorkbenchQt.exe
    goto :failed
)

echo [RUN] %EXE%
"%EXE%" %*
pause
exit /b 0

:FindSkylarkRoot
set "CANDIDATE=%~f1"
for /L %%N in (1,1,12) do (
    for %%I in ("!CANDIDATE!") do set "CLEAN=%%~fI"
    if exist "!CLEAN!\Source\SKCore\SKCore.cpp" if exist "!CLEAN!\Tests\SkylarkFormatWorkbench\CMakeLists.txt" (
        set "ROOT_DIR=!CLEAN!"
        exit /b 0
    )
    set "CANDIDATE=!CANDIDATE!\.."
)
exit /b 1

:StripTrailingSlash
setlocal EnableDelayedExpansion
set "VALUE=!%~1!"
:StripTrailingSlashLoop
if "!VALUE:~-1!"=="\" (
    set "VALUE=!VALUE:~0,-1!"
    goto :StripTrailingSlashLoop
)
if "!VALUE:~-1!"=="/" (
    set "VALUE=!VALUE:~0,-1!"
    goto :StripTrailingSlashLoop
)
endlocal & set "%~1=%VALUE%"
exit /b 0

:failed
echo [FAILED] Run_Windows_FormatWorkbench_VS2022_Debug.bat failed.
pause
exit /b 1
