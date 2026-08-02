@echo off
setlocal EnableExtensions
set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..\") do set "REPO_ROOT=%%~fI"
pushd "%REPO_ROOT%"
set "EXE=%REPO_ROOT%\out\build\windows-cadeditor-vulkan-ninja\Debug\SkylarkCadEditorLab.exe"
if not exist "%EXE%" (
  echo [ERROR] Executable not found: %EXE%
  echo [ERROR] Build likely failed earlier. Run the matching Build script first and check the generated build log.
  pause
  exit /b 1
)

if not exist "%REPO_ROOT%\out\logs" mkdir "%REPO_ROOT%\out\logs"
set "LOG=%REPO_ROOT%\out\logs\runtime-windows-cadeditor-vulkan-ninja-%RANDOM%-%RANDOM%.log"

echo ============================================================
echo SkylarkCadEditorLab Vulkan Runtime
echo ============================================================
echo [EXE] %EXE%
echo [LOG] %LOG%
echo [CMD] "%EXE%" --rhi=vulkan %*
echo ============================================================

> "%LOG%" echo [LAUNCH] %DATE% %TIME%
>> "%LOG%" echo [EXE] %EXE%
>> "%LOG%" echo [CMD] "%EXE%" --rhi=vulkan %*
>> "%LOG%" echo [INFO] The executable also creates a self runtime log under out\logs\runtime-windows-cadeditor-self-*.log.
"%EXE%" --rhi=vulkan %* >> "%LOG%" 2>&1
set "RESULT=%ERRORLEVEL%"

echo.
echo ============================================================
echo [EXIT CODE] %RESULT%
echo [LOG] %LOG%
echo ============================================================
type "%LOG%"
echo ============================================================
popd
pause
endlocal & exit /b %RESULT%
