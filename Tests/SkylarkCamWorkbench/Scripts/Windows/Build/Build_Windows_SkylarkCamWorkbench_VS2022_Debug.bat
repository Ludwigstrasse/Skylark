@echo off
setlocal EnableExtensions EnableDelayedExpansion
set "SCRIPT_DIR=%~dp0"
set "CANDIDATE=%SCRIPT_DIR%"
for /L %%N in (1,1,24) do (
    for %%I in ("!CANDIDATE!\.") do set "CLEAN=%%~fI"
    if exist "!CLEAN!\Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat" if exist "!CLEAN!\Source\SKCore\SKCore.cpp" (
        call "!CLEAN!\Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat"
        exit /b !ERRORLEVEL!
    )
    set "CANDIDATE=!CANDIDATE!\.."
)
echo [FAIL] Cannot locate Skylark repository root from "%SCRIPT_DIR%".
pause
exit /b 1
