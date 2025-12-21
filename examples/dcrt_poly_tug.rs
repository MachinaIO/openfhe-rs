//! Example demonstrating the usage of DCRTPolyGenFromTug function.

use openfhe::ffi;

fn main() {
    println!("=== DCRTPoly TUG Distribution Example ===");

    // Parameters for DCRTPoly
    let n = 512; // Ring dimension / 2
    let size = 2; // Number of towers
    let k_res = 1; // Residue parameter

    // Generate a polynomial using TUG (Ternary Uniform Generator) distribution
    let poly_tug = ffi::DCRTPolyGenFromTug(n, size, k_res);

    println!("Generated polynomial using TUG distribution:");
    println!("Ring dimension: {}", n * 2);
    println!("Number of towers: {}", size);
    println!("Residue parameter: {}", k_res);

    // Get the modulus of the polynomial
    let modulus = poly_tug.GetModulus();
    println!("Modulus: {}", modulus);

    // For comparison, generate polynomials using other distributions
    println!("\n=== Comparing with other distributions ===");

    // BUG - Binary Uniform Generator
    let poly_bug = ffi::DCRTPolyGenFromBug(n, size, k_res);
    println!("BUG polynomial modulus: {}", poly_bug.GetModulus());

    // DUG - Discrete Uniform Generator
    let poly_dug = ffi::DCRTPolyGenFromDug(n, size, k_res);
    println!("DUG polynomial modulus: {}", poly_dug.GetModulus());

    // DGG - Discrete Gaussian Generator
    let sigma = 3.2;
    let poly_dgg = ffi::DCRTPolyGenFromDgg(n, size, k_res, sigma);
    println!(
        "DGG polynomial modulus (sigma={}): {}",
        sigma,
        poly_dgg.GetModulus()
    );

    // The distributions differ in how they generate the polynomial coefficients:
    // - BUG: Binary values (0 or 1)
    // - DUG: Discrete uniform values
    // - DGG: Discrete Gaussian values with standard deviation sigma
    // - TUG: Ternary values (-1, 0, or 1)

    println!("\nSuccessfully generated polynomials using different distributions!");
}
