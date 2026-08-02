@echo off
setlocal EnableExtensions EnableDelayedExpansion
set "SCRIPT_DIR=%~dp0"
set "CANDIDATE=%SCRIPT_DIR%"
for /L %%N in (1,1,24) do (
    for %%I in ("!CANDIDATE!\.") do set "CLEAN=%%~fI"
    if exist "!CLEAN!\CMakeLists.txt" if exist "!CLEAN!\Source\SKCore\SKCore.cpp" (
        set "ROOT_DIR=!CLEAN!"
        goto :found_root
    )
    set "CANDIDATE=!CANDIDATE!\.."
)
echo [FAIL] Cannot locate Skylark repository root from "%SCRIPT_DIR%".
pause
exit /b 1
:found_root
set "SLN=%ROOT_DIR%\out\build\windows-skylark-cam-workbench-vs2022-debug\Skylark.sln"
if not exist "%SLN%" (
    echo [FAIL] Solution not found: %SLN%
    echo [ACTION] Run Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat first.
    pause
    exit /b 1
)
start "" "%SLN%"
exit /b 0
