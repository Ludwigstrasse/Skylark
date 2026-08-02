@echo off
setlocal EnableExtensions
set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..\") do set "REPO_ROOT=%%~fI"
pushd "%REPO_ROOT%"
set "EXE=%REPO_ROOT%\out\build\windows-cadeditor-opengl-vs2022\Debug\SkylarkCadEditorLab.exe"
if not exist "%EXE%" (
  echo [ERROR] Executable not found: %EXE%
  echo [ERROR] Build likely failed earlier. Run Scripts\Windows\Build\Build_Windows_CadEditorLab_OpenGL_VS2022_Debug.bat first.
  pause
  exit /b 1
)
if not exist "%REPO_ROOT%\out\logs" mkdir "%REPO_ROOT%\out\logs"
set "LOG=%REPO_ROOT%\out\logs\runtime-windows-cadeditor-opengl-vs2022-%RANDOM%-%RANDOM%.log"
echo ============================================================
echo SkylarkCadEditorLab OpenGL Runtime
echo ============================================================
echo [EXE] %EXE%
echo [LOG] %LOG%
echo [CMD] "%EXE%" --rhi=opengl %*
echo ============================================================
> "%LOG%" echo [LAUNCH] %DATE% %TIME%
>> "%LOG%" echo [EXE] %EXE%
>> "%LOG%" echo [CMD] "%EXE%" --rhi=opengl %*
>> "%LOG%" echo [INFO] Expected source fix stamp: CAD_EDITOR_LAB_BLANK_FREEZE_FIX3_20260428
"%EXE%" --rhi=opengl %* >> "%LOG%" 2>&1
set "RESULT=%ERRORLEVEL%"
findstr /C:"CAD_EDITOR_LAB_BLANK_FREEZE_FIX3_20260428" "%LOG%" >nul
if errorlevel 1 (
  echo [STALE-EXE] This executable was not rebuilt from the fixed source. Run the matching Build script, then run this script again.
  if "%RESULT%"=="0" set "RESULT=9002"
)
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
