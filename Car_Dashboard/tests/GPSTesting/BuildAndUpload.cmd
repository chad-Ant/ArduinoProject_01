@echo off
setlocal

where arduino-cli >nul 2>&1
if errorlevel 1 (
    echo arduino-cli is not available on PATH.
    exit /b 1
)

for %%I in ("%~dp0.") do set "TEST_DIR=%%~fI"
for %%I in ("%TEST_DIR%\..\..") do set "DASHBOARD_DIR=%%~fI"
set "EXTRA_FLAGS=-I%DASHBOARD_DIR%\include -I%DASHBOARD_DIR%\src"

if "%~1"=="" (
    arduino-cli compile --fqbn arduino:samd:mkrzero --warnings all --build-property "compiler.cpp.extra_flags=%EXTRA_FLAGS%" "%TEST_DIR%"
) else (
    arduino-cli compile --upload --port "%~1" --fqbn arduino:samd:mkrzero --warnings all --build-property "compiler.cpp.extra_flags=%EXTRA_FLAGS%" "%TEST_DIR%"
)

exit /b %ERRORLEVEL%
