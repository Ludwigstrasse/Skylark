@echo off
setlocal EnableExtensions
set "SCRIPT_DIR=%~dp0"
echo [INFO] This is an obsolete FormatWorkbench entry. Forwarding to SkylarkCamWorkbench M0 build.
call "%SCRIPT_DIR%Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat"
exit /b %ERRORLEVEL%
