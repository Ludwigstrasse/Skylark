@echo off
echo ==================================================
echo Skylark Windows CAD Scene Techniques OpenGL Ninja Debug Build
echo ==================================================
echo.
call "%~dp0Build_Windows_Generic.bat" windows-cadscene-techniques-opengl-ninja Ninja Debug ^
  "-DSK_ENABLE_OPENGL=ON" ^
  "-DSK_BUILD_CADSCENE_TEST_APP=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=opengl;cadscene-test"
exit /b %ERRORLEVEL%
