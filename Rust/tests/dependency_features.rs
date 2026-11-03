use std::process::Command;

#[test]
fn std_does_not_activate_optional_math_dependencies() {
    let output = Command::new(env!("CARGO"))
        .current_dir(env!("CARGO_MANIFEST_DIR"))
        .args([
            "tree",
            "--locked",
            "--no-default-features",
            "--features",
            "std",
            "--edges",
            "normal",
            "--prefix",
            "none",
        ])
        .output()
        .expect("could not inspect the std dependency graph");
    assert!(
        output.status.success(),
        "{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let dependencies = String::from_utf8(output.stdout).unwrap();
    assert!(
        !dependencies
            .lines()
            .any(|line| line.starts_with("num-traits ")),
        "std unexpectedly enables num-traits:\n{dependencies}"
    );
}
