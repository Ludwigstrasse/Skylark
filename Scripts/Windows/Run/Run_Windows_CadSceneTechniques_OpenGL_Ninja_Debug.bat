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
set "EXE=%REPO_ROOT%\out\build\windows-cadscene-opengl\Debug\SkylarkCadSceneTechniques.exe"
if not exist "%EXE%" (
  echo [ERROR] Executable not found: %EXE%
  echo [ERROR] Build likely failed earlier. Run the matching Build script first and check the generated build log.
  pause
  exit /b 1
)
"%EXE%" %*
popd
