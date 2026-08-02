@echo off
echo ==================================================
echo Skylark Windows CAD Editor Lab Ninja Debug Build
echo ==================================================
echo.
call "%~dp0Build_Windows_Generic.bat" windows-cadeditor-ninja Ninja Debug ^
  "-DSK_ENABLE_D3D11=ON" ^
  "-DSK_BUILD_CAD_EDITOR_LAB=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=cadscene-runtime-test"
exit /b %ERRORLEVEL%
