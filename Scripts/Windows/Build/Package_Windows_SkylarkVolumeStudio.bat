@echo off
setlocal EnableExtensions

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..") do set "REPO_ROOT=%%~fI"
if "%REPO_ROOT:~-1%"=="\" set "REPO_ROOT=%REPO_ROOT:~0,-1%"

set "BUILD_DIR=%REPO_ROOT%\out\build\windows-volumestudio-vs2022-debug"
set "PACKAGE_DIR=%REPO_ROOT%\out\package\windows-volumestudio"
set "LOG_DIR=%REPO_ROOT%\out\logs"
set "PACKAGE_LOG=%LOG_DIR%\package_windows_volumestudio.log"

if not defined QT_ROOT set "QT_ROOT=C:\Qt\Qt5.15.14\5.15.14\msvc2019_64"

if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
if not exist "%PACKAGE_DIR%\bin" mkdir "%PACKAGE_DIR%\bin"
if not exist "%PACKAGE_DIR%\docs" mkdir "%PACKAGE_DIR%\docs"

echo ============================================================
echo [SkylarkVolumeStudio] Windows Package
echo [ROOT   ] %REPO_ROOT%
echo [BUILD  ] %BUILD_DIR%
echo [PACKAGE] %PACKAGE_DIR%
echo [QT     ] %QT_ROOT%
echo ============================================================

set "APP_EXE="
if exist "%BUILD_DIR%\bin\Debug\SkylarkVolumeStudio.exe" set "APP_EXE=%BUILD_DIR%\bin\Debug\SkylarkVolumeStudio.exe"
if not defined APP_EXE if exist "%BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug\SkylarkVolumeStudio.exe" set "APP_EXE=%BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug\SkylarkVolumeStudio.exe"
if not defined APP_EXE if exist "%BUILD_DIR%\Debug\SkylarkVolumeStudio.exe" set "APP_EXE=%BUILD_DIR%\Debug\SkylarkVolumeStudio.exe"

if not defined APP_EXE (
    echo [FAIL] SkylarkVolumeStudio.exe was not found.
    echo [ACTION] Run build_windows_vs2022.bat first.
    echo [CHECK ] %BUILD_DIR%\bin\Debug
    echo [CHECK ] %BUILD_DIR%\Tests\SkylarkVolumeStudio\Debug
    echo [CHECK ] %BUILD_DIR%\Debug
    pause
    exit /b 1
)

copy /Y "%APP_EXE%" "%PACKAGE_DIR%\bin\" > "%PACKAGE_LOG%" 2>&1
if errorlevel 1 (
    echo [FAIL] copy executable failed. See: "%PACKAGE_LOG%"
    powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%PACKAGE_LOG%') { Get-Content -Path '%PACKAGE_LOG%' -Tail 120 }"
    pause
    exit /b 2
)

if exist "%REPO_ROOT%\Tests\SkylarkVolumeStudio\docs" (
    xcopy /Y /E /I "%REPO_ROOT%\Tests\SkylarkVolumeStudio\docs" "%PACKAGE_DIR%\docs" >> "%PACKAGE_LOG%" 2>&1
    if errorlevel 1 (
        echo [FAIL] copy docs failed. See: "%PACKAGE_LOG%"
        powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%PACKAGE_LOG%') { Get-Content -Path '%PACKAGE_LOG%' -Tail 120 }"
        pause
        exit /b 3
    )
)

if exist "%QT_ROOT%\bin\windeployqt.exe" (
    "%QT_ROOT%\bin\windeployqt.exe" "%PACKAGE_DIR%\bin\SkylarkVolumeStudio.exe" >> "%PACKAGE_LOG%" 2>&1
    if errorlevel 1 (
        echo [FAIL] windeployqt failed. See: "%PACKAGE_LOG%"
        powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%PACKAGE_LOG%') { Get-Content -Path '%PACKAGE_LOG%' -Tail 120 }"
        pause
        exit /b 4
    )
) else (
    echo [WARN] windeployqt.exe was not found under "%QT_ROOT%\bin". >> "%PACKAGE_LOG%"
)

echo [OK] Windows package staged at "%PACKAGE_DIR%"
echo [APP] %PACKAGE_DIR%\bin\SkylarkVolumeStudio.exe
pause
exit /b 0
