@echo off
setlocal EnableExtensions
call "%~dp0Build_Windows_Generic.bat" windows-cadeditor-d3d11-ninja Ninja Debug ^
  "-DSK_ENABLE_D3D11=ON" ^
  "-DSK_BUILD_CAD_EDITOR_LAB=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=cadscene-runtime-test"
set "RESULT=%ERRORLEVEL%"
endlocal & exit /b %RESULT%
