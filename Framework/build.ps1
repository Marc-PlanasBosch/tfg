# Script de PowerShell equivalent al Makefile
# Compila el Framework Genèric de Jocs

param(
    [string]$Target = "all"
)

# Configuració
$CXX = "g++"
$CXXFLAGS = "-std=c++17 -Wall -Wextra -O2 -g"
$INCLUDES = "-I./Core"
$LIBS = ""

# Directoris
$CORE_DIR = "Core"
$EXAMPLES_DIR = "Examples"
$BUILD_DIR = "build"

# Funció per compilar
function Compile-Core {
    Write-Host "Compilant classes base..." -ForegroundColor Green
    
    # Crear directoris si no existeixen
    if (!(Test-Path $BUILD_DIR)) { New-Item -ItemType Directory -Path $BUILD_DIR -Force }
    if (!(Test-Path "$BUILD_DIR\Core")) { New-Item -ItemType Directory -Path "$BUILD_DIR\Core" -Force }
    if (!(Test-Path "$BUILD_DIR\Examples")) { New-Item -ItemType Directory -Path "$BUILD_DIR\Examples" -Force }
    
    # Compilar fitxers del Core
    $coreFiles = Get-ChildItem "$CORE_DIR\*.cc"
    foreach ($file in $coreFiles) {
        $outputFile = "$BUILD_DIR\Core\$($file.BaseName).o"
        Write-Host "Compilant $($file.Name)..." -ForegroundColor Yellow
        
        $cmd = "$CXX $CXXFLAGS $INCLUDES -c $($file.FullName) -o $outputFile"
        Invoke-Expression $cmd
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Error compilant $($file.Name)" -ForegroundColor Red
            exit 1
        }
    }
}

function Compile-Examples {
    Write-Host "Compilant exemples..." -ForegroundColor Green
    
    # Compilar exemples
    $exampleFiles = Get-ChildItem "$EXAMPLES_DIR\*.cc"
    $coreObjects = Get-ChildItem "$BUILD_DIR\Core\*.o" | ForEach-Object { $_.FullName }
    
    foreach ($file in $exampleFiles) {
        $outputFile = "$BUILD_DIR\$($file.BaseName).exe"
        Write-Host "Compilant $($file.Name)..." -ForegroundColor Yellow
        
        $cmd = "$CXX $CXXFLAGS $INCLUDES $($file.FullName) $($coreObjects -join ' ') -o $outputFile $LIBS"
        Invoke-Expression $cmd
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Error compilant $($file.Name)" -ForegroundColor Red
            exit 1
        }
    }
}

function Run-Example {
    Write-Host "Executant exemple..." -ForegroundColor Green
    if (Test-Path "$BUILD_DIR\main_example.exe") {
        Set-Location $BUILD_DIR
        .\main_example.exe
        Set-Location ..
    } else {
        Write-Host "Error: No s'ha trobat l'executable main_example.exe" -ForegroundColor Red
    }
}

function Clean-Build {
    Write-Host "Netejant build..." -ForegroundColor Green
    if (Test-Path $BUILD_DIR) {
        Remove-Item -Recurse -Force $BUILD_DIR
        Write-Host "Directori build eliminat" -ForegroundColor Yellow
    }
}

# Executar segons el target
switch ($Target) {
    "all" {
        Compile-Core
        Compile-Examples
        Write-Host "Compilació completada!" -ForegroundColor Green
    }
    "core" {
        Compile-Core
    }
    "examples" {
        Compile-Examples
    }
    "run" {
        Compile-Core
        Compile-Examples
        Run-Example
    }
    "clean" {
        Clean-Build
    }
    default {
        Write-Host "Targets disponibles:" -ForegroundColor Cyan
        Write-Host "  all      - Compila tot (per defecte)" -ForegroundColor White
        Write-Host "  core     - Compila només les classes base" -ForegroundColor White
        Write-Host "  examples - Compila només els exemples" -ForegroundColor White
        Write-Host "  run      - Compila i executa" -ForegroundColor White
        Write-Host "  clean    - Neteja els fitxers compilats" -ForegroundColor White
    }
}
