#!/usr/bin/env bash
# Conformance runner for the C# port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: float (source as shipped) and double (FNLfloat switched to System.Double in a copy).
# Each variant is a minimal net8.0 project written into OUT_DIR that compiles the port plus driver.cs;
# RollForward lets it run on a newer .NET when the 8.0 runtime is not installed.
set -euo pipefail
out=$(cd "$1" && pwd); shift # absolute, because the project files refer to it
here=$(cd "$(dirname "$0")" && pwd)
export DOTNET_CLI_TELEMETRY_OPTOUT=1 DOTNET_NOLOGO=1
sed 's|^using FNLfloat = System.Single;|using FNLfloat = System.Double;|' "$here/../FastNoiseLite.cs" > "$out/FastNoiseLite_double.cs"
build() { # build VARIANT PORT_SOURCE DEFINES
    mkdir -p "$out/$1"
    cat > "$out/$1/driver.csproj" <<EOF
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
    dotnet build "$out/$1/driver.csproj" -c Release -o "$out/$1/bin" -nodeReuse:false -p:UseSharedCompilation=false
}
build float "$here/../FastNoiseLite.cs" ""
build double "$out/FastNoiseLite_double.cs" FNL_DOUBLE
for v in "$@"; do
    for t in float double; do
        dotnet "$out/$t/bin/driver.dll" "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done
