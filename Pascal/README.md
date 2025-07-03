## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```pascal

program NoiseTest;

{$mode objfpc}{$H+}

uses 
	cmem, fastnoiselite;

var
	noise : FNL_State;
	noiseVal : Single;
	x,y : Integer;

begin
  noise := fnlCreateState();
  noise.frequency:=0.001;
  noise.noise_type:=FNL_NOISE_PERLIN;
  noise.cellular_distance_func:=FNL_CELLULAR_DISTANCE_EUCLIDEAN;
  noise.domain_warp_type:=FNL_DOMAIN_WARP_OPENSIMPLEX2;
  noise.domain_warp_amp:=12.0;
  noise.fractal_type:= FNL_FRACTAL_PINGPONG;
  noise.weighted_strength:=0.8;   
  for x := 0 to 128 do
   for y := 0 to 128 do  
   begin
     noiseVal := (fnlGetNoise2D(@noise,x+offsetX,y) + 1.0) / 2.0; // 0..1
   end;
end.


```