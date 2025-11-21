# SFML Installation Script för Schackspel

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  SFML Installation för Schackspel" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Kontrollera om vcpkg finns
$vcpkgPath = "C:\vcpkg"
if (-Not (Test-Path $vcpkgPath)) {
    Write-Host "vcpkg hittades inte på $vcpkgPath" -ForegroundColor Yellow
    Write-Host "Vill du installera vcpkg? (J/N)" -ForegroundColor Yellow
    $response = Read-Host
    
    if ($response -eq "J" -or $response -eq "j") {
        Write-Host "Installerar vcpkg..." -ForegroundColor Green
        git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
        Set-Location C:\vcpkg
        .\bootstrap-vcpkg.bat
        .\vcpkg integrate install
    } else {
        Write-Host "Avbryter installation." -ForegroundColor Red
        exit
    }
}

# Installera SFML
Write-Host ""
Write-Host "Installerar SFML via vcpkg..." -ForegroundColor Green
Set-Location $vcpkgPath
.\vcpkg install sfml:x64-windows

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "  Installation klar!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Nu kan du öppna projektet i Visual Studio och bygga det." -ForegroundColor Cyan
Write-Host "Tryck på valfri tangent för att fortsätta..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
