@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
call :FindSkylarkRoot "%SCRIPT_DIR%"
if errorlevel 1 goto :root_failed
call :NormalizePath ROOT_DIR

set "BUILD_DIR=%ROOT_DIR%\out\build\windows-skylark-cam-workbench-vs2022-debug"
set "EXE="
if exist "%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbenchHeadless.exe" set "EXE=%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbenchHeadless.exe"
if not defined EXE if exist "%BUILD_DIR%\Debug\SkylarkCamWorkbenchHeadless.exe" set "EXE=%BUILD_DIR%\Debug\SkylarkCamWorkbenchHeadless.exe"
if not defined EXE for /f "delims=" %%E in ('dir /s /b "%BUILD_DIR%\SkylarkCamWorkbenchHeadless.exe" 2^>nul') do if not defined EXE set "EXE=%%E"
if not defined EXE (
    echo [FAIL] Executable not found under: %BUILD_DIR%
    echo [ACTION] Run Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat first.
    pause
    exit /b 1
)

echo ============================================================
echo [SkylarkCamWorkbench] M2 20M STL Pressure Entry
echo [EXE] %EXE%
echo [ARG] %~1
echo ============================================================
if "%~1"=="" (
    echo [INFO] No STL path passed. Running synthetic 20M chunk/render/upload/BVH/LOD pressure descriptor.
    "%EXE%" --m2-pressure20m
) else (
    echo [INFO] Running real Binary STL pressure file: %~1
    "%EXE%" --m2-pressure20m "%~1"
)
pause
exit /b %ERRORLEVEL%

:root_failed
echo [FAIL] Cannot locate Skylark repository root from "%SCRIPT_DIR%".
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
endlocal & set "%~1=%VALUE%"
exit /b 0
