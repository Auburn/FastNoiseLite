$scriptPath = $PSScriptRoot
$emsdkVer = "3.1.44"
$emsdkPath = Join-Path -Path $PSScriptRoot -ChildPath "emsdk"

if (Test-Path -Path $emsdkPath -PathType Container) {
    Write-Host "Found existing emsdk"
    ./emsdk/emsdk_env.ps1
} else {
    Write-Host "Building emsdk"
    git clone --branch $emsdkVer "https://github.com/emscripten-core/emsdk.git" $emsdkPath
    ./emsdk/emsdk.ps1 install $emsdkVer
    ./emsdk/emsdk.ps1 activate $emsdkVer
}

emcc --version

emcmake cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
