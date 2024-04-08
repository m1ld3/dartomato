@echo off
setlocal

:: Set variables for the Qt installation, compilers, and any other necessary paths
set QT_DIR=C:\Qt\6.6.1\mingw_64
set QMAKE_EXECUTABLE=%QT_DIR%\bin\qmake.exe
set C_COMPILER=%QT_DIR%\bin\gcc.exe  :: Adjust this path if using a different GCC location
set CXX_COMPILER=%QT_DIR%\bin\g++.exe  :: Adjust this path if using a different G++ location

:: Ensure the batch file is called with two additional arguments
if "%~2"=="" (
    echo Usage: build.bat [BUILD_TYPE] [BUILD_DIR]
    echo Example: build.bat DEBUG C:\path\to\build\dir
    exit /b 1
)

:: Use the current directory as the source directory
set "SOURCE_DIR=%CD%"
set "BUILD_TYPE=%~1"
set "BUILD_DIR=%~2"

:: Configure additional CMake parameters
set CMAKE_PARAMS=-G Ninja
set CMAKE_PARAMS=%CMAKE_PARAMS% -DCMAKE_BUILD_TYPE:STRING=%BUILD_TYPE%
set CMAKE_PARAMS=%CMAKE_PARAMS% -DCMAKE_PROJECT_INCLUDE_BEFORE:PATH=%QT_DIR%\mkspecs\features\data\auto-setup.cmake
set CMAKE_PARAMS=%CMAKE_PARAMS% -DQT_QMAKE_EXECUTABLE:STRING=%QMAKE_EXECUTABLE%
set CMAKE_PARAMS=%CMAKE_PARAMS% -DCMAKE_PREFIX_PATH:STRING=%QT_DIR%
set CMAKE_PARAMS=%CMAKE_PARAMS% -DCMAKE_C_COMPILER:STRING=%C_COMPILER%
set CMAKE_PARAMS=%CMAKE_PARAMS% -DCMAKE_CXX_COMPILER:STRING=%CXX_COMPILER%

:: Print configuration
echo Source Directory: %SOURCE_DIR%
echo Build Directory:  %BUILD_DIR%
echo Build Type:       %BUILD_TYPE%
echo CMake Parameters: %CMAKE_PARAMS%

:: Run qt-cmake with specified parameters
echo Running qt-cmake...
C:\Qt\6.6.1\mingw_64\bin\qt-cmake %CMAKE_PARAMS% -S "%SOURCE_DIR%" -B "%BUILD_DIR%"
if ERRORLEVEL 1 (
    echo Configuration failed with qt-cmake
    exit /b 1
)

:: Change to build directory and run ninja
echo Changing to build directory and running ninja...
cd /d "%BUILD_DIR%"
ninja
if ERRORLEVEL 1 (
    echo Build failed with ninja
    exit /b 1
)

echo Build succeeded
endlocal
