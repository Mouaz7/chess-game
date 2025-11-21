$msbuild = "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
if (-not (Test-Path $msbuild)) {
    # Fallback to vswhere if path is wrong (e.g. if "18" was dynamic)
    $vswhere = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe
    }
}

if (-not $msbuild -or -not (Test-Path $msbuild)) {
    Write-Host "Error: MSBuild not found."
    exit 1
}

Write-Host "Building project with $msbuild..."
& $msbuild Schack.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /v:m

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful! Launching game..."
    $exePath = "x64\Release\Schack.exe"
    if (Test-Path $exePath) {
        Start-Process $exePath
    } else {
        Write-Host "Error: Executable not found at $exePath"
        # Try to find it recursively
        $found = Get-ChildItem -Path . -Filter "Schack.exe" -Recurse
        if ($found) {
            Write-Host "Found at $($found.FullName), launching..."
            Start-Process $found.FullName
        }
    }
} else {
    Write-Host "Build failed."
    exit 1
}
