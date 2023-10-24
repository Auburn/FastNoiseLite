// USAGE: Run with `cargo run --release` from the RustTest directory. Omit `--release` when testing for math operator overflows.

extern crate fastnoise_lite;
use fastnoise_lite::*;
use rayon::prelude::*;

const SKIP_WINDOW: usize = 7919;

const WIDTH: usize = 512;
const HEIGHT: usize = 512;

const SEED: [i32; 2] = [-10, 1337];
const FREQUENCY: [f32; 4] = [-0.001, 0.01, 0.1, 10.];
const NOISE_TYPE: [NoiseType; 6] = [
    NoiseType::OpenSimplex2,
    NoiseType::OpenSimplex2S,
    NoiseType::Cellular,
    NoiseType::Perlin,
    NoiseType::ValueCubic,
    NoiseType::Value,
];
const ROTATION_TYPE_3D: [RotationType3D; 3] = [
    RotationType3D::None,
    RotationType3D::ImproveXYPlanes,
    RotationType3D::ImproveXZPlanes,
];

const FRACTAL_TYPE: [FractalType; 6] = [
    FractalType::None,
    FractalType::FBm,
    FractalType::Ridged,
    FractalType::PingPong,
    FractalType::DomainWarpProgressive,
    FractalType::DomainWarpIndependent,
];
const OCTAVES: [i32; 3] = [1, 2, 5];
// Only when octaves != 1
const LACUNARITY: [f32; 3] = [-0.1, 0., 1.];
// Only when octaves != 1
const GAIN: [f32; 3] = [-1.5, 0., 0.1];
// Only when octaves != 1
const WEIGHTED_STRENGTH: [f32; 3] = [-1.5, 0., 0.1];
// Only when fractal_type = FractalType::PingPong
const PING_PONG_STRENGTH: [f32; 3] = [-1.5, 0., 0.1];

// Only when noise_type = NoiseType::Cellular
const CELLULAR_DISTANCE_FUNCTION: [CellularDistanceFunction; 4] = [
    CellularDistanceFunction::Euclidean,
    CellularDistanceFunction::EuclideanSq,
    CellularDistanceFunction::Manhattan,
    CellularDistanceFunction::Hybrid,
];
// Only when noise_type = NoiseType::Cellular
const CELLULAR_RETURN_TYPE: [CellularReturnType; 7] = [
    CellularReturnType::CellValue,
    CellularReturnType::Distance,
    CellularReturnType::Distance2,
    CellularReturnType::Distance2Add,
    CellularReturnType::Distance2Sub,
    CellularReturnType::Distance2Mul,
    CellularReturnType::Distance2Div,
];
// Only when noise_type = NoiseType::Cellular
const CELLULAR_JITTER_MODIFIER: [f32; 3] = [-1.5, 0., 0.1];

// Only when noise_type = NoiseType::Cellular
const DOMAIN_WARP_TYPE: [DomainWarpType; 3] = [
    DomainWarpType::OpenSimplex2,
    DomainWarpType::OpenSimplex2Reduced,
    DomainWarpType::BasicGrid,
];
// Only when noise_type = NoiseType::Cellular
const DOMAIN_WARP_AMP: [f32; 3] = [-100., 0., 1000.];

fn main() {
    let mut base_options = vec![];
    for a in SEED {
        for b in FREQUENCY {
            for c in ROTATION_TYPE_3D {
                base_options.push((a, b, c));
            }
        }
    }

    let mut octaves_options = vec![];
    for a in OCTAVES {
        if a != 1 {
            for b in LACUNARITY {
                for c in GAIN {
                    for d in WEIGHTED_STRENGTH {
                        octaves_options.push((a, b, c, d));
                    }
                }
            }
        } else {
            octaves_options.push((a, 2., 0.5, 0.));
        }
    }

    let mut fractal_type_options = vec![];
    for a in FRACTAL_TYPE {
        if a == FractalType::PingPong {
            for b in PING_PONG_STRENGTH {
                fractal_type_options.push((a, b));
            }
        } else {
            fractal_type_options.push((a, 2.));
        }
    }

    let mut noise_type_options = vec![];
    for a in NOISE_TYPE {
        if a == NoiseType::Cellular {
            for b in CELLULAR_DISTANCE_FUNCTION {
                for c in CELLULAR_RETURN_TYPE {
                    for d in CELLULAR_JITTER_MODIFIER {
                        for e in DOMAIN_WARP_TYPE {
                            for f in DOMAIN_WARP_AMP {
                                noise_type_options.push((a, b, c, d, e, f));
                            }
                        }
                    }
                }
            }
        } else {
            noise_type_options.push((
                a,
                CellularDistanceFunction::EuclideanSq,
                CellularReturnType::Distance,
                1.,
                DomainWarpType::OpenSimplex2,
                1.,
            ));
        }
    }

    let total = base_options.len()
        * octaves_options.len()
        * fractal_type_options.len()
        * noise_type_options.len();

    let mut options = Vec::with_capacity(total);
    for &(b1, b2, b3) in &base_options {
        for &(o1, o2, o3, o4) in &octaves_options {
            for &(f1, f2) in &fractal_type_options {
                for &(n1, n2, n3, n4, n5, n6) in &noise_type_options {
                    options.push((b1, b2, b3, o1, o2, o3, o4, f1, f2, n1, n2, n3, n4, n5, n6));
                }
            }
        }
    }

    let mut options_subset = vec![];
    for i in 0..(total / SKIP_WINDOW) {
        options_subset.push(options[i * SKIP_WINDOW]);
    }

    println!(
        "Writing {} images, a subset of the total {}",
        options_subset.len(),
        total
    );

    options_subset.par_iter().enumerate().for_each(
        |(
            index,
            (
                seed,
                frequency,
                rotation_type_3d,
                octaves,
                lacunarity,
                gain,
                weighted_strength,
                fractal_type,
                ping_pong_strength,
                noise_type,
                cellular_distance_function,
                cellular_return_type,
                cellular_jitter_modifier,
                domain_warp_type,
                domain_warp_amp,
            ),
        )| {
            let index = index * SKIP_WINDOW;
            // if index != 3072572 {
            //     return;
            // }

            let mut noise = FastNoiseLite::with_seed(*seed);
            noise.set_frequency(Some(*frequency));
            noise.set_rotation_type_3d(Some(*rotation_type_3d));

            noise.set_fractal_octaves(Some(*octaves));
            noise.set_fractal_lacunarity(Some(*lacunarity));
            noise.set_fractal_gain(Some(*gain));
            noise.set_fractal_weighted_strength(Some(*weighted_strength));

            noise.set_fractal_type(Some(*fractal_type));
            noise.set_fractal_ping_pong_strength(Some(*ping_pong_strength));

            noise.set_noise_type(Some(*noise_type));
            noise.set_cellular_distance_function(Some(*cellular_distance_function));
            noise.set_cellular_return_type(Some(*cellular_return_type));
            noise.set_cellular_jitter(Some(*cellular_jitter_modifier));
            noise.set_domain_warp_type(Some(*domain_warp_type));
            noise.set_domain_warp_amp(Some(*domain_warp_amp));

            // let mut bytes = [0; WIDTH * HEIGHT];
            // for x in 0..WIDTH {
            //     for y in 0..HEIGHT {
            //         let value = noise.get_noise_2d(x as f64, y as f64);
            //         let value = ((value + 1.) / 2. * 255.).clamp(0., 255.) as u8;
            //         bytes[x + y * WIDTH] = value;
            //     }
            // }
            // std::fs::write(format!("../../images-rust/{index}.bytes"), bytes).unwrap();

            // ///
            // let value = noise.get_noise_2d((184 - 1) as f64, (109 - 1) as f64);
            // let value = ((value + 1.) / 2. * 255.).clamp(0., 255.) as u8;
            // println!("Rust value: {}", value);
            // ///

            let mut image = image::ImageBuffer::new(WIDTH as u32, HEIGHT as u32);
            for (x, y, pixel) in image.enumerate_pixels_mut() {
                let value = noise.get_noise_2d(4194280. + x as f32, 4194280. + y as f32);
                let value = ((value + 1.) / 2. * 255.).clamp(0., 255.) as u8;
                *pixel = image::Rgb([value, value, value]);
            }
            // After the PNGs are generated, use this command from the Windows terminal to convert them to BMPs:
            // for %i in (*.png) do ffmpeg -i "%i" -c:v bmp -pix_fmt rgb24 "%~ni.bmp"
            image
                .save(format!("../../images-rust/{index}.png"))
                .unwrap();
        },
    );
}
