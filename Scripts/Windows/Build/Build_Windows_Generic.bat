@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "BUILD_NAME=%~1"
set "GENERATOR_KIND=%~2"
set "BUILD_CONFIG=%~3"
shift
shift
shift
set "EXTRA_CMAKE_ARGS="
:collect_extra_args
if "%~1"=="" goto :collect_extra_args_done

set "ARG=%~1"
set "NEXT_ARG=%~2"

rem Windows batch may split an unquoted -DVAR=VALUE into -DVAR and VALUE.
rem Rebuild it here so CMake receives the canonical -DVAR=VALUE form.
if not "!ARG:~0,2!"=="-D" goto :append_extra_arg
if not "!ARG!"=="!ARG:=!" goto :append_extra_arg
if "%~2"=="" goto :append_extra_arg
if "!NEXT_ARG:~0,1!"=="-" goto :append_extra_arg
set "ARG=!ARG!=%~2"
shift

:append_extra_arg
if defined EXTRA_CMAKE_ARGS (
    set "EXTRA_CMAKE_ARGS=!EXTRA_CMAKE_ARGS! !ARG!"
) else (
    set "EXTRA_CMAKE_ARGS=!ARG!"
)
shift
goto :collect_extra_args
:collect_extra_args_done
if "%BUILD_NAME%"=="" (
    echo [ERROR] Build_Windows_Generic: missing BUILD_NAME
    goto :fail_early
)

if "%GENERATOR_KIND%"=="" (
    echo [ERROR] Build_Windows_Generic: missing GENERATOR_KIND
    goto :fail_early
)

if "%BUILD_CONFIG%"=="" set "BUILD_CONFIG=Debug"


call "%~dp0..\Common\Common.bat" init "%~dp0" "%BUILD_NAME%"
if errorlevel 1 goto :fail_early

call "%~dp0..\Common\FindVsDevCmd.bat"
if errorlevel 1 goto :fail

call "%VS_DEV_CMD%" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 (
    echo [ERROR] Failed to initialize VS developer environment.
    goto :fail
)

call "%~dp0..\Common\SetupVcpkg.bat" "%SK_REPO_ROOT%"
if errorlevel 1 goto :fail

set "CMAKE_BUILD_PARALLEL_LEVEL=%NUMBER_OF_PROCESSORS%"
if "%CMAKE_BUILD_PARALLEL_LEVEL%"=="" set "CMAKE_BUILD_PARALLEL_LEVEL=8"

if /I "%GENERATOR_KIND%"=="Ninja" (
    set "CMAKE_GENERATOR=Ninja Multi-Config"
    set "GENERATOR_ARGS=-G "Ninja Multi-Config""
) else if /I "%GENERATOR_KIND%"=="VS2022" (
    set "CMAKE_GENERATOR=Visual Studio 17 2022"
    set "GENERATOR_ARGS=-G "Visual Studio 17 2022" -A x64"
) else (
    echo [ERROR] Unsupported generator kind: %GENERATOR_KIND%
    goto :fail
)

(
echo ==================================================
echo Skylark Windows Build
echo ==================================================
echo Repo Root      : %SK_REPO_ROOT%
echo Build Name     : %BUILD_NAME%
echo Generator Kind : %GENERATOR_KIND%
echo Build Config   : %BUILD_CONFIG%
echo Build Dir      : %SK_BUILD_DIR%
echo Install Dir    : %SK_INSTALL_DIR%
echo VCPKG_ROOT     : %VCPKG_ROOT%
echo ==================================================
) > "%SK_LOG_FILE%"
> "%SK_LAST_LOG_POINTER%" echo %SK_LOG_FILE%

echo ==================================================
echo Skylark Windows Build
echo ==================================================
echo [INFO] Repo root        : %SK_REPO_ROOT%
echo [INFO] Build name       : %BUILD_NAME%
echo [INFO] Generator kind   : %GENERATOR_KIND%
echo [INFO] Build config     : %BUILD_CONFIG%
echo [INFO] Build dir        : %SK_BUILD_DIR%
echo [INFO] Install dir      : %SK_INSTALL_DIR%
echo [INFO] Log file         : %SK_LOG_FILE%
echo [INFO] VCPKG_ROOT       : %VCPKG_ROOT%
echo [INFO] CMAKE parallel   : %CMAKE_BUILD_PARALLEL_LEVEL%

echo [INFO] Running configure...
cmake -S "%SK_REPO_ROOT%" -B "%SK_BUILD_DIR%" %GENERATOR_ARGS% ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DCMAKE_INSTALL_PREFIX="%SK_INSTALL_DIR%" ^
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
  -DSK_BUILD_INSTALL=ON ^
  -DSK_BUILD_LEGACY=OFF ^
  -DSK_ENABLE_WARNINGS_AS_ERRORS=OFF ^
  -DSK_ENABLE_D3D11=OFF ^
  -DSK_ENABLE_D3D12=OFF ^
  -DSK_ENABLE_OPENGL=OFF ^
  -DSK_ENABLE_VULKAN=OFF ^
  %EXTRA_CMAKE_ARGS% >> "%SK_LOG_FILE%" 2>&1
if errorlevel 1 (
    echo [ERROR] CMake configure failed. See log: "%SK_LOG_FILE%"
    goto :fail
)

echo [INFO] Running build...
cmake --build "%SK_BUILD_DIR%" --config %BUILD_CONFIG% --parallel >> "%SK_LOG_FILE%" 2>&1
if errorlevel 1 (
    echo [ERROR] CMake build failed. See log: "%SK_LOG_FILE%"
    goto :fail
)

echo.
echo [OK] Build completed successfully.
echo [OK] Log: "%SK_LOG_FILE%"
echo.
pause
endlocal
exit /b 0

:fail
echo.
echo [ERROR] Build failed.
echo [ERROR] Log: "%SK_LOG_FILE%"
echo.
pause
endlocal
exit /b 1

:fail_early
echo.
echo [ERROR] Build failed before build initialization.
echo.
pause
endlocal
exit /b 1
