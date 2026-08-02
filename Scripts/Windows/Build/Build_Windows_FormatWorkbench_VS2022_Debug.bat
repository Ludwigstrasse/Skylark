@echo off
setlocal EnableExtensions
set "SCRIPT_DIR=%~dp0"
echo [INFO] Build_Windows_FormatWorkbench_VS2022_Debug.bat is obsolete for M0.
echo [INFO] Forwarding to Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat.
call "%SCRIPT_DIR%Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat"
exit /b %ERRORLEVEL%
