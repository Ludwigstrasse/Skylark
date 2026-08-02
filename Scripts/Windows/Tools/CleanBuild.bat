@echo off
setlocal EnableExtensions

for %%I in ("%~dp0..\..\..") do set "REPO_ROOT=%%~fI"
set "OUT_DIR=%REPO_ROOT%\out"

if "%~1"=="" (
    echo [INFO] Removing "%OUT_DIR%"
    if exist "%OUT_DIR%" rd /s /q "%OUT_DIR%"
) else (
    set "TARGET=%~1"
    echo [INFO] Removing build/install for "%TARGET%"
    if exist "%OUT_DIR%\build\%TARGET%" rd /s /q "%OUT_DIR%\build\%TARGET%"
    if exist "%OUT_DIR%\install\%TARGET%" rd /s /q "%OUT_DIR%\install\%TARGET%"
)

echo [OK] Clean completed.
pause
endlocal
exit /b 0
