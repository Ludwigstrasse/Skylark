@echo off
echo ==================================================
echo Skylark Windows OpenGL Debug Build
echo ==================================================
echo.
call "%~dp0Build_Windows_Generic.bat" windows-opengl VS2022 Debug ^
  "-DSK_ENABLE_OPENGL=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=opengl"
exit /b %ERRORLEVEL%
