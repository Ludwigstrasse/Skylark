@echo off
setlocal EnableExtensions
call "%~dp0Build_Windows_Generic.bat" windows-cadeditor-d3d12-vs2022 VS2022 Debug ^
  "-DSK_ENABLE_D3D12=ON" ^
  "-DSK_ENABLE_D3D11=ON" ^
  "-DSK_BUILD_CAD_EDITOR_LAB=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=cadscene-runtime-test"
set "RESULT=%ERRORLEVEL%"
endlocal & exit /b %RESULT%
