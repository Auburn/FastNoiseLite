## Getting Started

First, you'll want to copy all the code from the PSFastNoiseLite file into your own PowerShell code. Because PSFastNoiseLite is designed as a class, it's easier to do this than modularizing. Copy from line 38 onward.

Here's an example for creating a 128x128 array of OpenSimplex2 noise:

```PowerShell
# CREATE AND CONFIGURE A FAST NOISE OBJECT
[FastNoiseLite]$Noise = [FastNoiseLite]::new()
$Noise.SetNoiseType([FnlNoiseType]::OpenSimplex2)

# GATHER NOISE DATA
$TestNoise = New-Object 'Single[,]' 128, 128

For([Int]$X = 0; $X -LT 128; $X++) {
    For([Int]$Y = 0; $Y -LT 128; $Y++) {
        $TestNoise[$X, $Y] = $Noise.GetNoise($X, $Y)
    }
}

# DO SOMETHING WITH THIS DATA...
# WRITE TO CONSOLE
# Write-Host "$($TestNoise)"
```