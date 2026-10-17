// FastNoiseLite conformance driver (Rust). See conformance/README.md.
// Usage: driver VECTORS_DIR > output.txt
// build.sh builds this as src/main.rs of a scratch crate whose "f64" feature enables fastnoise-lite/f64.
use fastnoise_lite::*;
use std::io::{BufWriter, Write};
use std::panic::{catch_unwind, AssertUnwindSafe};

#[cfg(not(feature = "f64"))]
type Float = f32;
#[cfg(feature = "f64")]
type Float = f64;

const NT: [NoiseType; 6] = [
    NoiseType::OpenSimplex2,
    NoiseType::OpenSimplex2S,
    NoiseType::Cellular,
    NoiseType::Perlin,
    NoiseType::ValueCubic,
    NoiseType::Value,
];
const ROT: [RotationType3D; 3] = [RotationType3D::None, RotationType3D::ImproveXYPlanes, RotationType3D::ImproveXZPlanes];
const FT: [FractalType; 6] = [
    FractalType::None,
    FractalType::FBm,
    FractalType::Ridged,
    FractalType::PingPong,
    FractalType::DomainWarpProgressive,
    FractalType::DomainWarpIndependent,
];
const CD: [CellularDistanceFunction; 4] = [
    CellularDistanceFunction::Euclidean,
    CellularDistanceFunction::EuclideanSq,
    CellularDistanceFunction::Manhattan,
    CellularDistanceFunction::Hybrid,
];
const CR: [CellularReturnType; 7] = [
    CellularReturnType::CellValue,
    CellularReturnType::Distance,
    CellularReturnType::Distance2,
    CellularReturnType::Distance2Add,
    CellularReturnType::Distance2Sub,
    CellularReturnType::Distance2Mul,
    CellularReturnType::Distance2Div,
];
const WT: [DomainWarpType; 3] = [DomainWarpType::OpenSimplex2, DomainWarpType::OpenSimplex2Reduced, DomainWarpType::BasicGrid];

fn rows(dir: &str, name: &str) -> Vec<Vec<String>> {
    let path = format!("{dir}/{name}");
    let text = std::fs::read_to_string(&path).unwrap_or_else(|e| {
        eprintln!("cannot open {path}: {e}");
        std::process::exit(2)
    });
    text.lines()
        .map(|l| l.split_whitespace().map(String::from).collect::<Vec<_>>())
        .filter(|r| !r.is_empty())
        .collect()
}

// Prints one line, or nothing if the call panics (debug builds panic on integer overflow).
// {:e} prints the shortest string that parses back to the same f64, so no precision is lost.
fn put(out: &mut impl Write, tag: &str, c: i32, i: usize, f: impl FnOnce() -> Vec<f64>) {
    if let Ok(v) = catch_unwind(AssertUnwindSafe(f)) {
        let v: Vec<String> = v.iter().map(|x| format!("{x:e}")).collect();
        writeln!(out, "{tag} {c} {i} {}", v.join(" ")).unwrap();
    }
}

fn run(out: &mut impl Write, n: &FastNoiseLite, c: i32, p2: &[Vec<Float>], p3: &[Vec<Float>]) {
    for (i, p) in p2.iter().enumerate() {
        put(out, "N2", c, i, || vec![n.get_noise_2d(p[0], p[1]) as f64]);
    }
    for (i, p) in p3.iter().enumerate() {
        put(out, "N3", c, i, || vec![n.get_noise_3d(p[0], p[1], p[2]) as f64]);
    }
    for (i, p) in p2.iter().enumerate() {
        put(out, "W2", c, i, || {
            let (x, y) = n.domain_warp_2d(p[0], p[1]);
            vec![x as f64, y as f64]
        });
    }
    for (i, p) in p3.iter().enumerate() {
        put(out, "W3", c, i, || {
            let (x, y, z) = n.domain_warp_3d(p[0], p[1], p[2]);
            vec![x as f64, y as f64, z as f64]
        });
    }
}

fn main() {
    let dir = std::env::args().nth(1).unwrap_or_else(|| {
        eprintln!("usage: driver VECTORS_DIR");
        std::process::exit(2)
    });
    std::panic::set_hook(Box::new(|_| {})); // skipped lines show up as missing in the report
    let points = |name| -> Vec<Vec<Float>> {
        rows(&dir, name).iter().map(|r| r.iter().map(|v| v.parse::<f64>().unwrap() as Float).collect()).collect()
    };
    let (p2, p3) = (points("points2d.txt"), points("points3d.txt"));
    let mut out = BufWriter::new(std::io::stdout().lock());

    run(&mut out, &FastNoiseLite::new(), -1, &p2, &p3);

    for (c, f) in rows(&dir, "configs.txt").iter().enumerate() {
        let int = |k: usize| f[k].parse::<i32>().unwrap();
        let flt = |k: usize| f[k].parse::<f32>().unwrap();
        let mut n = FastNoiseLite::new();
        n.set_seed(Some(int(0)));
        n.set_frequency(Some(flt(1)));
        n.set_noise_type(Some(NT[int(2) as usize]));
        n.set_rotation_type_3d(Some(ROT[int(3) as usize]));
        n.set_fractal_type(Some(FT[int(4) as usize]));
        n.set_fractal_octaves(Some(int(5)));
        n.set_fractal_lacunarity(Some(flt(6)));
        n.set_fractal_gain(Some(flt(7)));
        n.set_fractal_weighted_strength(Some(flt(8)));
        n.set_fractal_ping_pong_strength(Some(flt(9)));
        n.set_cellular_distance_function(Some(CD[int(10) as usize]));
        n.set_cellular_return_type(Some(CR[int(11) as usize]));
        n.set_cellular_jitter(Some(flt(12)));
        n.set_domain_warp_type(Some(WT[int(13) as usize]));
        n.set_domain_warp_amp(Some(flt(14)));
        run(&mut out, &n, c as i32, &p2, &p3);
    }
}
