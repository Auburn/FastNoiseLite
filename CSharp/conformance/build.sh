#!/usr/bin/env bash
# Conformance build for the C# port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float (source as shipped) and double (FNLfloat switched to System.Double in a copy).
# Each variant is a minimal net8.0 project that compiles the port plus driver.cs; RollForward lets it run on
# a newer .NET when the 8.0 runtime is not installed.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
export DOTNET_CLI_TELEMETRY_OPTOUT=1 DOTNET_NOLOGO=1
bash "$here/../../conformance/replace.sh" "$here/../FastNoiseLite.cs" FastNoiseLite_double.cs \
    'using FNLfloat = System.Single;' 'using FNLfloat = System.Double;'
build() { # build VARIANT PORT_SOURCE DEFINES
    mkdir -p "$1"
    cat > "$1/driver.csproj" <<EOF
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net8.0</TargetFramework>
    <RollForward>Major</RollForward>
    <DefineConstants>$3</DefineConstants>
    <EnableDefaultCompileItems>false</EnableDefaultCompileItems>
    <UseAppHost>false</UseAppHost>
  </PropertyGroup>
  <ItemGroup>
    <Compile Include="$2;$here/driver.cs" />
  </ItemGroup>
</Project>
EOF
    # No build servers left running after the script exits.
    dotnet build "$1/driver.csproj" -c Release -o "$1/bin" -nodeReuse:false -p:UseSharedCompilation=false
}
build float "$here/../FastNoiseLite.cs" ""
build double "$PWD/FastNoiseLite_double.cs" FNL_DOUBLE
cat > variants.txt <<EOF
float dotnet float/bin/driver.dll
double dotnet double/bin/driver.dll
EOF
