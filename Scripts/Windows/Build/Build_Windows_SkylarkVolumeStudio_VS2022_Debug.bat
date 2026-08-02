@echo off
setlocal EnableExtensions

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..") do set "REPO_ROOT=%%~fI"
if "%REPO_ROOT:~-1%"=="\" set "REPO_ROOT=%REPO_ROOT:~0,-1%"

set "BUILD_DIR=%REPO_ROOT%\out\build\windows-volumestudio-vs2022-debug"
set "INSTALL_DIR=%REPO_ROOT%\out\install\windows-volumestudio-vs2022-debug"
set "LOG_DIR=%REPO_ROOT%\out\logs"
set "CONFIGURE_LOG=%LOG_DIR%\configure_windows_volumestudio_vs2022_debug.log"
set "BUILD_LOG=%LOG_DIR%\build_windows_volumestudio_vs2022_debug.log"
set "RUN_LOG=%LOG_DIR%\run_windows_volumestudio_validation.log"

if not defined QT_ROOT set "QT_ROOT=C:\Qt\Qt5.15.14\5.15.14\msvc2019_64"

if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cls
echo ============================================================
echo [SkylarkVolumeStudio] Windows VS2022 Debug Build
echo [ROOT   ] %REPO_ROOT%
echo [BUILD  ] %BUILD_DIR%
echo [INSTALL] %INSTALL_DIR%
echo [LOGS   ] %LOG_DIR%
echo [QT     ] %QT_ROOT%
echo ============================================================

where cmake >nul 2>nul
if errorlevel 1 (
    echo [FAIL] cmake was not found in PATH.
    echo [HINT] Install CMake or add it to PATH.
    pause
    exit /b 1
)

echo [STEP] configure
cmake -S "%REPO_ROOT%" -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
    -DSK_BUILD_VOLUME_STUDIO=ON ^
    -DSK_BUILD_VOLUME_STUDIO_QT=ON ^
    -DCMAKE_PREFIX_PATH="%QT_ROOT%" ^
    -DSK_BUILD_SMOKE_TEST=OFF ^
    -DSK_BUILD_CADSCENE_TEST_APP=OFF ^
    -DSK_BUILD_CAD_EDITOR_LAB=OFF ^
    -DSK_ENABLE_OPENGL=OFF ^
    -DSK_ENABLE_VULKAN=OFF ^
    -DSK_BUILD_INSTALL=ON ^
    > "%CONFIGURE_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] configure failed. See: "%CONFIGURE_LOG%"
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%CONFIGURE_LOG%') { Get-Content -Path '%CONFIGURE_LOG%' -Tail 100 }"
    pause
    exit /b 10
)

echo [STEP] build validation
echo [LOG ] %BUILD_LOG%
cmake --build "%BUILD_DIR%" --config Debug --target SkylarkVolumeStudioValidation > "%BUILD_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] validation target build failed. See: "%BUILD_LOG%"
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%BUILD_LOG%') { Get-Content -Path '%BUILD_LOG%' -Tail 120 }"
    pause
    exit /b 20
)

echo [STEP] build Qt application
cmake --build "%BUILD_DIR%" --config Debug --target SkylarkVolumeStudio >> "%BUILD_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] Qt application target build failed. See: "%BUILD_LOG%"
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%BUILD_LOG%') { Get-Content -Path '%BUILD_LOG%' -Tail 120 }"
    pause
    exit /b 21
)

set "VALIDATION_EXE="
if exist "%BUILD_DIR%\bin\Debug\SkylarkVolumeStudioValidation.exe" set "VALIDATION_EXE=%BUILD_DIR%\bin\Debug\SkylarkVolumeStudioValidation.exe"
if not defined VALIDATION_EXE if exist "%BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug\SkylarkVolumeStudioValidation.exe" set "VALIDATION_EXE=%BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug\SkylarkVolumeStudioValidation.exe"
if not defined VALIDATION_EXE if exist "%BUILD_DIR%\Debug\SkylarkVolumeStudioValidation.exe" set "VALIDATION_EXE=%BUILD_DIR%\Debug\SkylarkVolumeStudioValidation.exe"

if not defined VALIDATION_EXE (
    echo [FAIL] validation executable was not found.
    echo [CHECK] %BUILD_DIR%\bin\Debug
    echo [CHECK] %BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug
    echo [CHECK] %BUILD_DIR%\Debug
    echo [LOG  ] %BUILD_LOG%
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%BUILD_LOG%') { Get-Content -Path '%BUILD_LOG%' -Tail 80 }"
    pause
    exit /b 30
)

echo [STEP] run validation
echo [EXE ] %VALIDATION_EXE%
"%VALIDATION_EXE%" > "%RUN_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] validation failed. See: "%RUN_LOG%"
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%RUN_LOG%') { Get-Content -Path '%RUN_LOG%' -Tail 120 }"
    pause
    exit /b 31
)
type "%RUN_LOG%"

set "APP_EXE="
if exist "%BUILD_DIR%\bin\Debug\SkylarkVolumeStudio.exe" set "APP_EXE=%BUILD_DIR%\bin\Debug\SkylarkVolumeStudio.exe"
if not defined APP_EXE if exist "%BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug\SkylarkVolumeStudio.exe" set "APP_EXE=%BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug\SkylarkVolumeStudio.exe"
if not defined APP_EXE if exist "%BUILD_DIR%\Debug\SkylarkVolumeStudio.exe" set "APP_EXE=%BUILD_DIR%\Debug\SkylarkVolumeStudio.exe"

if not defined APP_EXE (
    echo [FAIL] SkylarkVolumeStudio.exe was not found after the Qt application target build.
    echo [CHECK] %BUILD_DIR%\bin\Debug
    echo [CHECK] %BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug
    echo [CHECK] %BUILD_DIR%\Debug
    echo [LOG  ] %BUILD_LOG%
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%BUILD_LOG%') { Get-Content -Path '%BUILD_LOG%' -Tail 120 }"
    pause
    exit /b 32
)

echo [OK] Windows VS2022 build and validation completed.
echo [APP] %APP_EXE%
echo [BIN] %BUILD_DIR%\bin\Debug
echo [NEXT] Run package_windows.bat to stage the executable and Qt runtime.
pause
exit /b 0
