@echo off
setlocal
set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..\") do set "REPO_ROOT=%%~fI"
set "SLN=%REPO_ROOT%\out\build\windows-cadscene-opengl-vs2022\Skylark.sln"
if not exist "%SLN%" (
  echo [ERROR] Solution not found: %SLN%
  pause
  exit /b 1
)
start "" "%SLN%"
