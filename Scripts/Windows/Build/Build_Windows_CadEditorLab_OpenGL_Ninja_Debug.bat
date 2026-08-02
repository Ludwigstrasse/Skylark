@echo off
echo ==================================================
echo Skylark Windows CAD Editor Lab OpenGL Ninja Debug Build
echo ==================================================
echo.
call "%~dp0Build_Windows_Generic.bat" windows-cadeditor-opengl-ninja Ninja Debug ^
  "-DSK_ENABLE_OPENGL=ON" ^
  "-DSK_BUILD_CAD_EDITOR_LAB=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=opengl;cadscene-runtime-test"
exit /b %ERRORLEVEL%
