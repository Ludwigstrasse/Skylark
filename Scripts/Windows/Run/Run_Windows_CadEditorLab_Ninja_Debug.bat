@echo off
setlocal EnableExtensions

for %%I in ("%~dp0..\..\..") do set "REPO_ROOT=%%~fI"
set "EXE_DIR=%REPO_ROOT%\out\build\windows-cadeditor-ninja\Debug"
set "EXE_PATH=%EXE_DIR%\SkylarkCadEditorLab.exe"
set "ASSET_PATH=%EXE_DIR%\Tests\SkylarkCadEditorLab\Assets\geforce.csf.gz"

if not exist "%EXE_PATH%" (
    echo [ERROR] Executable not found:
    echo         "%EXE_PATH%"
    echo [INFO] Please run Build_Windows_CadEditorLab_Ninja_Debug.bat first.
    pause
    exit /b 1
)

pushd "%EXE_DIR%" >nul
if exist "%ASSET_PATH%" (
    "%EXE_PATH%" "%ASSET_PATH%"
) else (
    "%EXE_PATH%"
)
popd >nul

pause
endlocal
exit /b 0
