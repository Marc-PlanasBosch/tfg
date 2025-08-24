# Script de compilació per PowerShell
# Utilitza Visual Studio Build Tools o MinGW

Write-Host "=== Compilant Framework Genèric de Jocs ===" -ForegroundColor Green

# Crear directoris de build
if (!(Test-Path "build")) { New-Item -ItemType Directory -Path "build" }
if (!(Test-Path "build\Core")) { New-Item -ItemType Directory -Path "build\Core" }
if (!(Test-Path "build\Examples")) { New-Item -ItemType Directory -Path "build\Examples" }

# Intentar utilitzar cl.exe (Visual Studio)
$clPath = Get-Command cl -ErrorAction SilentlyContinue
if ($clPath) {
    Write-Host "Utilitzant Visual Studio Build Tools..." -ForegroundColor Yellow
    $COMPILER = "cl"
    $CXXFLAGS = "/std:c++17 /EHsc /W3 /O2"
    $INCLUDES = "/I.\Core"
}
# Intentar utilitzar g++ (MinGW)
elseif (Get-Command g++ -ErrorAction SilentlyContinue) {
    Write-Host "Utilitzant MinGW..." -ForegroundColor Yellow
    $COMPILER = "g++"
    $CXXFLAGS = "-std=c++17 -Wall -Wextra -O2"
    $INCLUDES = "-I.\Core"
}
else {
    Write-Host "ERROR: No s'ha trobat cap compilador C++" -ForegroundColor Red
    Write-Host ""
    Write-Host "Instal·la un d'aquests:" -ForegroundColor Yellow
    Write-Host "1. Visual Studio Build Tools" -ForegroundColor White
    Write-Host "2. MinGW-w64" -ForegroundColor White
    Write-Host "3. MSYS2 amb MinGW" -ForegroundColor White
    Write-Host ""
    Read-Host "Prem Enter per sortir"
    exit 1
}

Write-Host "Compilant classes base..." -ForegroundColor Cyan

# Compilar GameDefinition
Write-Host "Compilant GameDefinition..." -ForegroundColor Gray
if ($COMPILER -eq "cl") {
    & cl $CXXFLAGS $INCLUDES /c Core\GameDefinition.cc /Fo:build\Core\GameDefinition.obj
} else {
    & g++ $CXXFLAGS $INCLUDES -c Core\GameDefinition.cc -o build\Core\GameDefinition.o
}
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error compilant GameDefinition.cc" -ForegroundColor Red
    Read-Host "Prem Enter per sortir"
    exit 1
}

# Compilar GameMap
Write-Host "Compilant GameMap..." -ForegroundColor Gray
if ($COMPILER -eq "cl") {
    & cl $CXXFLAGS $INCLUDES /c Core\GameMap.cc /Fo:build\Core\GameMap.obj
} else {
    & g++ $CXXFLAGS $INCLUDES -c Core\GameMap.cc -o build\Core\GameMap.o
}
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error compilant GameMap.cc" -ForegroundColor Red
    Read-Host "Prem Enter per sortir"
    exit 1
}

# Compilar GameBoard
Write-Host "Compilant GameBoard..." -ForegroundColor Gray
if ($COMPILER -eq "cl") {
    & cl $CXXFLAGS $INCLUDES /c Core\GameBoard.cc /Fo:build\Core\GameBoard.obj
} else {
    & g++ $CXXFLAGS $INCLUDES -c Core\GameBoard.cc -o build\Core\GameBoard.obj
}
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error compilant GameBoard.cc" -ForegroundColor Red
    Read-Host "Prem Enter per sortir"
    exit 1
}

Write-Host "Compilant exemple..." -ForegroundColor Cyan

# Compilar exemple principal
Write-Host "Compilant exemple principal..." -ForegroundColor Gray
if ($COMPILER -eq "cl") {
    & cl $CXXFLAGS $INCLUDES Examples\main_example.cc build\Core\*.obj /Fe:build\main_example.exe
} else {
    & g++ $CXXFLAGS $INCLUDES Examples\main_example.cc build\Core\*.o -o build\main_example.exe
}
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error compilant l'exemple" -ForegroundColor Red
    Read-Host "Prem Enter per sortir"
    exit 1
}

Write-Host ""
Write-Host "=== Compilació completada amb èxit! ===" -ForegroundColor Green
Write-Host ""
Write-Host "Executables generats:" -ForegroundColor Yellow
Write-Host "- build\main_example.exe" -ForegroundColor White
Write-Host ""
Write-Host "Per executar:" -ForegroundColor Yellow
Write-Host "  build\main_example.exe" -ForegroundColor White
Write-Host ""

Read-Host "Prem Enter per continuar"
