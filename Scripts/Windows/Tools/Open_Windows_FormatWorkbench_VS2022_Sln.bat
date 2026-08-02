@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
call :FindSkylarkRoot "%SCRIPT_DIR%"
if defined ROOT_DIR call :StripTrailingSlash ROOT_DIR
if not defined ROOT_DIR (
    echo [FAIL] Cannot locate Skylark repository root.
    goto :failed
)

set "SLN=%ROOT_DIR%\out\build\windows-format-workbench-vs2022\Skylark.sln"
if not exist "%SLN%" (
    echo [FAIL] Solution not found: %SLN%
    echo [ACTION] Run Scripts\Windows\Build\Build_Windows_FormatWorkbench_VS2022_Debug.bat first.
    goto :failed
)

start "" "%SLN%"
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
echo [FAILED] Open_Windows_FormatWorkbench_VS2022_Sln.bat failed.
pause
exit /b 1
