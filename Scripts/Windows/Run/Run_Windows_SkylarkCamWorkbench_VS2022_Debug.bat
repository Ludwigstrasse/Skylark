@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
call :FindSkylarkRoot "%SCRIPT_DIR%"
if errorlevel 1 goto :root_failed
call :NormalizePath ROOT_DIR

set "BUILD_DIR=%ROOT_DIR%\out\build\windows-skylark-cam-workbench-vs2022-debug"
set "EXE="
if exist "%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbench.exe" set "EXE=%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbench.exe"
if not defined EXE if exist "%BUILD_DIR%\Debug\SkylarkCamWorkbench.exe" set "EXE=%BUILD_DIR%\Debug\SkylarkCamWorkbench.exe"
if not defined EXE for /f "delims=" %%E in ('dir /s /b "%BUILD_DIR%\SkylarkCamWorkbench.exe" 2^>nul') do if not defined EXE set "EXE=%%E"
if not defined EXE (
    echo [FAIL] Executable not found under: %BUILD_DIR%
    echo [ACTION] Run Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat first.
    pause
    exit /b 1
)
echo [RUN] %EXE%
"%EXE%" %*
pause
exit /b 0

:root_failed
echo [FAIL] Cannot locate Skylark repository root from "%SCRIPT_DIR%".
echo [HINT] The root must contain CMakeLists.txt and Source\SKCore\SKCore.cpp.
pause
exit /b 1

:FindSkylarkRoot
set "CANDIDATE=%~f1"
for /L %%N in (1,1,24) do (
    for %%I in ("!CANDIDATE!\.") do set "CLEAN=%%~fI"
    if exist "!CLEAN!\CMakeLists.txt" if exist "!CLEAN!\Source\SKCore\SKCore.cpp" (
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
