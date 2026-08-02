@echo off
setlocal EnableExtensions
call "%~dp0Build_Windows_Generic.bat" windows-cadeditor-vulkan-ninja Ninja Debug ^
  "-DSK_ENABLE_VULKAN=ON" ^
  "-DSK_BUILD_CAD_EDITOR_LAB=ON" ^
  "-DVCPKG_MANIFEST_FEATURES=cadscene-runtime-test;vulkan"
set "RESULT=%ERRORLEVEL%"
endlocal & exit /b %RESULT%
