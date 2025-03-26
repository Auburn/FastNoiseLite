$scriptPath = $PSScriptRoot
$emsdkVer = "3.1.52"
$emsdkPath = Join-Path -Path $PSScriptRoot -ChildPath "emsdk" | Join-Path -ChildPath $emsdkVer

if (Test-Path -Path $emsdkPath -PathType Container) {
    Write-Host "Found existing emsdk"
    & "$emsdkPath\emsdk_env.ps1"
} else {
    Write-Host "Building emsdk"
    git clone --branch $emsdkVer "https://github.com/emscripten-core/emsdk.git" $emsdkPath
    & "$emsdkPath\emsdk.ps1" install $emsdkVer
    & "$emsdkPath\emsdk.ps1" activate $emsdkVer
}

emcc --version

emcmake cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
