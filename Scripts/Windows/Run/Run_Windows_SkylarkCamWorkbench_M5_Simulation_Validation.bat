@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..") do set "ROOT=%%~fI"
set "BUILD_DIR=%ROOT%\out\build\windows-skylark-cam-workbench-vs2022-debug"
set "LOG_DIR=%ROOT%\out\logs"
set "EXE=%BUILD_DIR%\Tests\SkylarkCamWorkbench\Debug\SkylarkCamWorkbenchHeadless.exe"

if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"

echo ============================================================
echo [SkylarkCamWorkbench] M5 Simulation Validation
echo [ROOT ] %ROOT%
echo [EXE  ] %EXE%
echo ============================================================

if not exist "%EXE%" (
  echo [FAIL] Headless executable not found. Build first:
  echo        Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat
  goto :failed
)

"%EXE%" > "%LOG_DIR%\run_windows_skylark_cam_workbench_m5_simulation_validation.log" 2>&1
set "RC=%ERRORLEVEL%"
type "%LOG_DIR%\run_windows_skylark_cam_workbench_m5_simulation_validation.log"
if not "%RC%"=="0" goto :failed

echo [OK] M5 Simulation validation passed.
pause
exit /b 0

:failed
echo [FAILED] M5 Simulation validation failed.
pause
exit /b 1
