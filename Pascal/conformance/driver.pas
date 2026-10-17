{ FastNoiseLite conformance driver (Pascal). See conformance/README.md.
  Usage: driver VECTORS_DIR > output.txt }
program driver;
{$mode objfpc}{$H+}
uses SysUtils, FastNoiseLite;

type
  TPoint = array[0..2] of Double;
  TPoints = array of TPoint;

var
  p2, p3: TPoints;

function ReadPoints(const path: string; dims: Integer): TPoints;
var
  f: TextFile;
  n, k: Integer;
begin
  Result := nil;
  AssignFile(f, path);
  Reset(f);
  n := 0;
  while not SeekEof(f) do
  begin
    SetLength(Result, n + 1);
    for k := 0 to dims - 1 do
      Read(f, Result[n][k]);
    Inc(n);
  end;
  CloseFile(f);
end;

function G(v: Double): string; { Double, because the Single overload of FloatToStrF stops at ~10 digits }
begin
  Result := FloatToStrF(v, ffGeneral, 17, 0);
end;

{ Each line is computed before anything is written, so a call that raises (FPC unmasks
  floating-point exceptions) only drops that line. }
procedure Run(var s: FNL_STATE; c: Integer);
var
  i: Integer;
  x, y, z: Single;
begin
  for i := 0 to High(p2) do
    try
      WriteLn(Format('N2 %d %d %s', [c, i, G(fnlGetNoise2D(@s, p2[i][0], p2[i][1]))]));
    except
      on Exception do ;
    end;
  for i := 0 to High(p3) do
    try
      WriteLn(Format('N3 %d %d %s', [c, i, G(fnlGetNoise3D(@s, p3[i][0], p3[i][1], p3[i][2]))]));
    except
      on Exception do ;
    end;
  for i := 0 to High(p2) do
    try
      x := p2[i][0]; y := p2[i][1];
      fnlDomainWarp2D(@s, x, y);
      WriteLn(Format('W2 %d %d %s %s', [c, i, G(x), G(y)]));
    except
      on Exception do ;
    end;
  for i := 0 to High(p3) do
    try
      x := p3[i][0]; y := p3[i][1]; z := p3[i][2];
      fnlDomainWarp3D(@s, x, y, z);
      WriteLn(Format('W3 %d %d %s %s %s', [c, i, G(x), G(y), G(z)]));
    except
      on Exception do ;
    end;
end;

var
  dir: string;
  f: TextFile;
  s: FNL_STATE;
  c, seed, nt, rot, ft, octaves, cdist, cret, wt: LongInt;
  freq, lac, gain, ws, pp, jitter, amp: Single;
begin
  if ParamCount < 1 then
  begin
    WriteLn(StdErr, 'usage: driver VECTORS_DIR');
    Halt(2);
  end;
  dir := ParamStr(1);
  p2 := ReadPoints(dir + '/points2d.txt', 2);
  p3 := ReadPoints(dir + '/points3d.txt', 3);

  s := fnlCreateState();
  Run(s, -1);

  AssignFile(f, dir + '/configs.txt');
  Reset(f);
  c := 0;
  while not SeekEof(f) do
  begin
    ReadLn(f, seed, freq, nt, rot, ft, octaves, lac, gain, ws, pp, cdist, cret, jitter, wt, amp);
    s := fnlCreateState();
    s.seed := seed;
    s.frequency := freq;
    s.noise_type := FNL_NOISE_TYPE(nt);
    s.rotation_type_3D := FNL_ROTATION_TYPE_3D(rot);
    s.fractal_type := FNL_FRACTAL_TYPE(ft);
    s.octaves := octaves;
    s.lacunarity := lac;
    s.gain := gain;
    s.weighted_strength := ws;
    s.ping_pong_strength := pp;
    s.cellular_distance_func := FNL_CELLULAR_DISTANCE_FUNC(cdist);
    s.cellular_return_type := FNL_CELLULAR_RETURN_TYPE(cret);
    s.cellular_jitter_mod := jitter;
    s.domain_warp_type := FNL_DOMAIN_WARP_TYPE(wt);
    s.domain_warp_amp := amp;
    Run(s, c);
    Inc(c);
  end;
  CloseFile(f);
end.
