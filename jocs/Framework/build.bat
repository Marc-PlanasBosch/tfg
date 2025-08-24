@echo off
REM Script de compilació per Windows
REM Utilitza Visual Studio Build Tools o MinGW

echo === Compilant Framework Genèric de Jocs ===

REM Crear directoris de build
if not exist "build" mkdir build
if not exist "build\Core" mkdir build\Core
if not exist "build\Examples" mkdir build\Examples

REM Intentar utilitzar cl.exe (Visual Studio)
where cl >nul 2>&1
if %errorlevel% == 0 (
    echo Utilitzant Visual Studio Build Tools...
    set COMPILER=cl
    set CXXFLAGS=/std:c++17 /EHsc /W3 /O2
    set INCLUDES=/I.\Core
    goto :compile
)

REM Intentar utilitzar g++ (MinGW)
where g++ >nul 2>&1
if %errorlevel% == 0 (
    echo Utilitzant MinGW...
    set COMPILER=g++
    set CXXFLAGS=-std=c++17 -Wall -Wextra -O2
    set INCLUDES=-I.\Core
    goto :compile
)

REM Si no es troba cap compilador
echo ERROR: No s'ha trobat cap compilador C++
echo.
echo Instal·la un d'aquests:
echo 1. Visual Studio Build Tools
echo 2. MinGW-w64
echo 3. MSYS2 amb MinGW
echo.
pause
exit /b 1

:compile
echo Compilant classes base...

REM Compilar GameDefinition
%COMPILER% %CXXFLAGS% %INCLUDES% /c Core\GameDefinition.cc /Fo:build\Core\GameDefinition.obj
if %errorlevel% neq 0 (
    echo Error compilant GameDefinition.cc
    pause
    exit /b 1
)

REM Compilar GameMap
%COMPILER% %CXXFLAGS% %INCLUDES% /c Core\GameMap.cc /Fo:build\Core\GameMap.obj
if %errorlevel% neq 0 (
    echo Error compilant GameMap.cc
    pause
    exit /b 1
)

REM Compilar GameBoard
%COMPILER% %CXXFLAGS% %INCLUDES% /c Core\GameBoard.cc /Fo:build\Core\GameBoard.obj
if %errorlevel% neq 0 (
    echo Error compilant GameBoard.cc
    pause
    exit /b 1
)

echo Compilant exemple...

REM Compilar exemple principal
%COMPILER% %CXXFLAGS% %INCLUDES% Examples\main_example.cc build\Core\*.obj /Fe:build\main_example.exe
if %errorlevel% neq 0 (
    echo Error compilant l'exemple
    pause
    exit /b 1
)

echo.
echo === Compilació completada amb èxit! ===
echo.
echo Executables generats:
echo - build\main_example.exe
echo.
echo Per executar:
echo   build\main_example.exe
echo.
pause
