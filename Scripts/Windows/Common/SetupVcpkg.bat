@echo off
setlocal EnableExtensions

set "REPO_ROOT=%~1"
if "%REPO_ROOT%"=="" (
    echo [ERROR] SetupVcpkg: missing repo root
    exit /b 1
)

set "THIRD_PARTY_DIR=%REPO_ROOT%\third_party"
set "VCPKG_DIR=%THIRD_PARTY_DIR%\vcpkg"
set "VCPKG_JSON=%REPO_ROOT%\vcpkg.json"
set "BASELINE="

if not exist "%THIRD_PARTY_DIR%" mkdir "%THIRD_PARTY_DIR%"

if exist "%VCPKG_JSON%" (
    for /f "usebackq tokens=1,* delims=:" %%A in (`findstr /I /C:"\"builtin-baseline\"" "%VCPKG_JSON%"`) do (
        set "BASELINE=%%B"
    )
)

if defined BASELINE (
    set "BASELINE=%BASELINE: =%"
    set "BASELINE=%BASELINE:"=%"
    set "BASELINE=%BASELINE:,=%"
)

if not exist "%VCPKG_DIR%\.git" (
    echo [INFO] Cloning vcpkg into "%VCPKG_DIR%"
    git clone https://github.com/microsoft/vcpkg "%VCPKG_DIR%"
    if errorlevel 1 (
        echo [ERROR] Failed to clone vcpkg.
        exit /b 1
    )
)

if defined BASELINE (
    pushd "%VCPKG_DIR%" >nul
    echo [INFO] Syncing vcpkg to builtin-baseline %BASELINE%
    git fetch --all --tags
    git checkout %BASELINE%
    if errorlevel 1 (
        popd >nul
        echo [ERROR] Failed to checkout builtin-baseline %BASELINE%.
        exit /b 1
    )
    popd >nul
)

if not exist "%VCPKG_DIR%\vcpkg.exe" (
    echo [INFO] Bootstrapping vcpkg
    call "%VCPKG_DIR%\bootstrap-vcpkg.bat"
    if errorlevel 1 (
        echo [ERROR] Failed to bootstrap vcpkg.
        exit /b 1
    )
)

if not exist "%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake" (
    echo [ERROR] vcpkg toolchain file not found:
    echo         "%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake"
    exit /b 1
)

endlocal & set "VCPKG_ROOT=%VCPKG_DIR%"
exit /b 0
