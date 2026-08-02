@echo off
setlocal EnableExtensions
for %%I in ("%~dp0..\..\..") do set "REPO_ROOT=%%~fI"
set "SLN_PATH=%REPO_ROOT%\out\build\windows-cadeditor-vs2022\Skylark.sln"

if not exist "%SLN_PATH%" (
    echo [ERROR] Solution not found:
    echo         "%SLN_PATH%"
    echo [INFO] Please run Build_Windows_CadEditorLab_VS2022_Debug.bat first.
    pause
    exit /b 1
)

start "" "%SLN_PATH%"
endlocal
exit /b 0
