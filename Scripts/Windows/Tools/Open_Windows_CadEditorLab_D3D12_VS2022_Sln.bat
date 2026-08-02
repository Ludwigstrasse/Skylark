@echo off
setlocal
set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..\") do set "REPO_ROOT=%%~fI"
pushd "%REPO_ROOT%"
if not exist "CMakePresets.json" (
  echo [ERROR] CMakePresets.json not found under "%REPO_ROOT%"
  pause
  exit /b 1
)
start "" "%REPO_ROOT%\out\build\windows-cadeditor-d3d12-vs2022\Skylark.sln"
popd
