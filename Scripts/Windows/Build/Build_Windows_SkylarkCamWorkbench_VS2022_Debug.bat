@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem ============================================================
rem SkylarkCamWorkbench - Windows VS2022 Debug Build
rem This script may be launched from either:
rem   1) Skylark\Scripts\Windows\Build
rem   2) Skylark\Tests\SkylarkCamWorkbench\Scripts\Windows\Build
rem It always walks upward to locate the real Skylark repository root.
rem ============================================================

set "SCRIPT_DIR=%~dp0"
call :FindSkylarkRoot "%SCRIPT_DIR%"
if errorlevel 1 goto :root_failed
call :NormalizePath ROOT_DIR

if not defined QT_ROOT set "QT_ROOT=C:\Qt\Qt5.15.14\5.15.14\msvc2019_64"
call :NormalizePath QT_ROOT

set "QT5_CMAKE_DIR=%QT_ROOT%\lib\cmake\Qt5"
set "BUILD_DIR=%ROOT_DIR%\out\build\windows-skylark-cam-workbench-vs2022-debug"
set "LOG_DIR=%ROOT_DIR%\out\logs"
set "CFG_LOG=%LOG_DIR%\configure_windows_skylark_cam_workbench_vs2022_debug.log"
set "BLD_LOG=%LOG_DIR%\build_windows_skylark_cam_workbench_vs2022_debug.log"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%" >nul 2>nul

cls
echo ============================================================
echo [SkylarkCamWorkbench] Windows VS2022 Debug Build
echo [ROOT   ] %ROOT_DIR%
echo [QT_ROOT] %QT_ROOT%
echo [BUILD  ] %BUILD_DIR%
echo [LOGS   ] %LOG_DIR%
echo ============================================================

if not exist "%ROOT_DIR%\CMakeLists.txt" (
    echo [FAIL] Invalid Skylark root: %ROOT_DIR%
    echo [HINT] CMakeLists.txt was not found.
    goto :failed
)
if not exist "%ROOT_DIR%\Source\SKCore\SKCore.cpp" (
    echo [FAIL] Invalid Skylark root: %ROOT_DIR%
    echo [HINT] Source\SKCore\SKCore.cpp was not found.
    goto :failed
)
if not exist "%ROOT_DIR%\Tests\SkylarkCamWorkbench\CMakeLists.txt" (
    echo [FAIL] Tests\SkylarkCamWorkbench\CMakeLists.txt was not found under the Skylark root.
    if exist "%ROOT_DIR%\Tests\SkylarkCamWorkbench\Tests\SkylarkCamWorkbench\CMakeLists.txt" (
        echo [DIAG] The patch zip appears to have been extracted inside Tests\SkylarkCamWorkbench.
        echo [ACTION] Re-extract the zip directly into: %ROOT_DIR%
    ) else (
        echo [ACTION] Re-extract the M0 patch zip directly into: %ROOT_DIR%
    )
    goto :failed
)
findstr /C:"SK_BUILD_CAM_WORKBENCH" "%ROOT_DIR%\CMakeLists.txt" >nul 2>nul
if errorlevel 1 (
    echo [FAIL] Root CMakeLists.txt does not contain SK_BUILD_CAM_WORKBENCH.
    echo [ACTION] The patch was not applied to the repository root. Re-extract the zip into: %ROOT_DIR%
    goto :failed
)
if not exist "%QT5_CMAKE_DIR%\Qt5Config.cmake" (
    echo [FAIL] Qt5Config.cmake not found: %QT5_CMAKE_DIR%\Qt5Config.cmake
    echo [ACTION] set QT_ROOT=C:\Qt\Qt5.15.14\5.15.14\msvc2019_64
    goto :failed
)
where cmake.exe >nul 2>nul
if errorlevel 1 (
    echo [FAIL] cmake.exe was not found in PATH.
    goto :failed
)
where cl.exe >nul 2>nul
if errorlevel 1 (
    if exist "%ROOT_DIR%\Scripts\Windows\Common\FindVsDevCmd.bat" (
        call "%ROOT_DIR%\Scripts\Windows\Common\FindVsDevCmd.bat"
        if errorlevel 1 goto :failed
        call "!VS_DEV_CMD!" -arch=x64
        if errorlevel 1 goto :failed
    ) else (
        echo [FAIL] cl.exe not found and Scripts\Windows\Common\FindVsDevCmd.bat is missing.
        goto :failed
    )
)

if exist "%BUILD_DIR%" (
    echo [INFO] Removing previous build directory: %BUILD_DIR%
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%" >nul 2>nul

pushd "%ROOT_DIR%" >nul
if errorlevel 1 (
    echo [FAIL] Cannot cd into Skylark root: %ROOT_DIR%
    goto :failed
)

echo [STEP] configure
echo [LOG ] %CFG_LOG%
cmake -S . -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64 ^
    "-DSK_BUILD_INSTALL=OFF" ^
    "-DSK_BUILD_CAM_WORKBENCH=ON" ^
    "-DSKCAM_BUILD_HEADLESS=ON" ^
    "-DSKCAM_BUILD_QT_APP=ON" ^
    "-DSKCAM_REQUIRE_QT=ON" ^
    "-DQt5_DIR:PATH=%QT5_CMAKE_DIR%" > "%CFG_LOG%" 2>&1
set "CONFIGURE_ERROR=%ERRORLEVEL%"
popd >nul
if not "%CONFIGURE_ERROR%"=="0" (
    echo [FAIL] configure failed. See %CFG_LOG%
    call :PrintLogTail "%CFG_LOG%" 180
    goto :failed
)

echo [STEP] build headless validator
echo [LOG ] %BLD_LOG%
cmake --build "%BUILD_DIR%" --config Debug --target SkylarkCamWorkbenchHeadless > "%BLD_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] headless target build failed. See %BLD_LOG%
    call :PrintLogTail "%BLD_LOG%" 220
    goto :failed
)

echo [STEP] build Qt desktop application
cmake --build "%BUILD_DIR%" --config Debug --target SkylarkCamWorkbench >> "%BLD_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] Qt desktop target build failed. See %BLD_LOG%
    call :PrintLogTail "%BLD_LOG%" 220
    goto :failed
)

set "GUI_EXE="
if exist "%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbench.exe" set "GUI_EXE=%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbench.exe"
if not defined GUI_EXE if exist "%BUILD_DIR%\Debug\SkylarkCamWorkbench.exe" set "GUI_EXE=%BUILD_DIR%\Debug\SkylarkCamWorkbench.exe"
if not defined GUI_EXE for /f "delims=" %%E in ('dir /s /b "%BUILD_DIR%\SkylarkCamWorkbench.exe" 2^>nul') do if not defined GUI_EXE set "GUI_EXE=%%E"
if not defined GUI_EXE (
    echo [FAIL] Build returned success, but SkylarkCamWorkbench.exe was not found.
    echo [CHECK] Generated executables under: %BUILD_DIR%
    dir /s /b "%BUILD_DIR%\*.exe" 2^>nul
    goto :failed
)

echo [OK] SkylarkCamWorkbench Windows VS2022 Debug build succeeded.
echo [GUI_EXE] %GUI_EXE%
pause
exit /b 0

:root_failed
echo [FAIL] Cannot locate Skylark repository root from "%SCRIPT_DIR%".
echo [HINT] The root must contain CMakeLists.txt and Source\SKCore\SKCore.cpp.
echo [ACTION] Run this script from inside the Skylark checkout, or re-extract the patch zip into the Skylark root.
goto :failed

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

:PrintLogTail
set "TAIL_FILE=%~1"
set "TAIL_COUNT=%~2"
where powershell.exe >nul 2>nul
if errorlevel 1 (
    type "%TAIL_FILE%"
) else (
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Get-Content -LiteralPath '%TAIL_FILE%' -Tail %TAIL_COUNT%"
)
exit /b 0

:failed
echo [FAILED] Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat failed.
pause
exit /b 1
