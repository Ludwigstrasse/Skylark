@echo off
echo ==================================================
echo Skylark Windows MSVC Debug Build
echo ==================================================
echo.
call "%~dp0Build_Windows_Generic.bat" windows-msvc Ninja Debug
exit /b %ERRORLEVEL%
