@echo off
setlocal

rem Build OpenMugen from the directory containing this script.
cd /d "%~dp0"

set "BUILD_TYPE=%~1"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=Release"

where cmake >nul 2>nul
if errorlevel 1 (
    echo CMake was not found on PATH.
    echo Install CMake and a C++ compiler, then run this script again.
    exit /b 1
)

echo Configuring OpenMugen (%BUILD_TYPE%)...
cmake -S . -B build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

echo Building OpenMugen...
cmake --build build --config %BUILD_TYPE% --parallel
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

if not exist "build\data" (
    echo Runtime data was not copied by the build. Copying it now...
    xcopy /E /I /Y "data" "build\data" >nul
    if errorlevel 1 (
        echo Failed to copy runtime data.
        exit /b 1
    )
)

if exist "build\%BUILD_TYPE%\OpenMugen.exe" (
    set "EXE=build\%BUILD_TYPE%\OpenMugen.exe"
) else (
    set "EXE=build\OpenMugen.exe"
)

if not exist "%EXE%" (
    echo Build completed, but OpenMugen.exe was not found.
    exit /b 1
)

echo.
echo Build completed successfully.
echo Executable: %EXE%
echo Runtime data: build\data
echo.
echo Run from the build directory so the data folder is found:
echo   cd build
echo   OpenMugen.exe

endlocal
