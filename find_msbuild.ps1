$vswhere = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $path = & $vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe
    if ($path) {
        Write-Host "FOUND_MSBUILD:$path"
    } else {
        Write-Host "MSBUILD_NOT_FOUND"
    }
} else {
    Write-Host "VSWHERE_NOT_FOUND"
}
