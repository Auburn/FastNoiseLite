# FastNoiseLite conformance driver (PowerShell). See conformance/README.md.
# Usage: pwsh -NoProfile -File driver.ps1 VECTORS_DIR > output.txt
# The port's enums use the C++ ordinals, so config fields are cast directly.
param([string]$Dir)
if (-not $Dir) {
    [Console]::Error.WriteLine('usage: pwsh -File driver.ps1 VECTORS_DIR')
    exit 2
}
$ErrorActionPreference = 'Stop'
# Dot-sourcing also turns on the port's Set-StrictMode -Version Latest for this script.
. "$PSScriptRoot/../PSFastNoiseLite.ps1"

$inv = [Globalization.CultureInfo]::InvariantCulture
function Rows([string]$name) {
    foreach ($line in [IO.File]::ReadAllLines((Join-Path $Dir $name))) {
        $f = $line.Trim() -split '\s+'
        if ($f[0]) { , $f }
    }
}
function F([single]$v) { $v.ToString('G9', $inv) }
function S([string]$t) { [single]::Parse($t, $inv) }
$p2 = @(Rows 'points2d.txt' | ForEach-Object { , [double[]]$_ })
$p3 = @(Rows 'points3d.txt' | ForEach-Object { , [double[]]$_ })
$out = [Collections.Generic.List[string]]::new()
$errors = @{}  # exception message (quoted numbers collapsed) -> count, summarised on stderr

function Fail($e) { $errors[$e.Exception.Message -replace '"-?\d[^"]*"', '"#"'] += 1 }

# A call that throws skips its line (counted as missing by compare.py).
function Run($n, [int]$c) {
    for ($i = 0; $i -lt $p2.Count; $i++) {
        try { $out.Add("N2 $c $i $(F $n.GetNoise($p2[$i][0], $p2[$i][1]))") }
        catch { Fail $_ }
    }
    for ($i = 0; $i -lt $p3.Count; $i++) {
        try { $out.Add("N3 $c $i $(F $n.GetNoise($p3[$i][0], $p3[$i][1], $p3[$i][2]))") }
        catch { Fail $_ }
    }
    for ($i = 0; $i -lt $p2.Count; $i++) {
        [single]$x = $p2[$i][0]; [single]$y = $p2[$i][1]
        try { $n.DomainWarp([ref]$x, [ref]$y); $out.Add("W2 $c $i $(F $x) $(F $y)") }
        catch { Fail $_ }
    }
    for ($i = 0; $i -lt $p3.Count; $i++) {
        [single]$x = $p3[$i][0]; [single]$y = $p3[$i][1]; [single]$z = $p3[$i][2]
        try { $n.DomainWarp([ref]$x, [ref]$y, [ref]$z); $out.Add("W3 $c $i $(F $x) $(F $y) $(F $z)") }
        catch { Fail $_ }
    }
}

Run ([FastNoiseLite]::new()) -1
$c = 0
foreach ($f in @(Rows 'configs.txt')) {
    $n = [FastNoiseLite]::new()
    # The port has no SetSeed, SetFrequency, SetFractalType, SetFractalLacunarity,
    # SetFractalWeightedStrength, SetFractalPingPongStrength, SetCellularDistanceFunction,
    # SetCellularReturnType, SetCellularJitter or SetDomainWarpAmp; those fields are assigned
    # directly (the C++ setters are plain assignments too).
    $n.Seed = [int]$f[0]
    $n.Frequency = S $f[1]
    $n.SetNoiseType([FnlNoiseType][int]$f[2])
    $n.SetRotationType3D([FnlRotationType3D][int]$f[3])
    $n.FractalType = [FnlFractalType][int]$f[4]
    $n.SetFractalOctaves([int]$f[5])
    $n.Lacunarity = S $f[6]
    $n.SetFractalGain((S $f[7]))
    $n.WeightedStrength = S $f[8]
    $n.PingPongStrength = S $f[9]
    $n.CellularDistanceFunction = [FnlCellularDistanceFunction][int]$f[10]
    $n.CellularReturnType = [FnlCellularReturnType][int]$f[11]
    $n.CellularJitterModifier = S $f[12]
    $n.SetDomainWarpType([FnlDomainWarpType][int]$f[13])
    $n.DomainWarpAmp = S $f[14]
    Run $n ($c++)
}
[Console]::Out.Write(($out -join "`n") + "`n")
foreach ($e in $errors.GetEnumerator() | Sort-Object Value -Descending) {
    [Console]::Error.WriteLine("$($e.Value) calls threw: $($e.Key)")
}
