use num_bigint::BigUint;
use num_traits::Num;
use openfhe::ffi::{self, GetMatrixElement};
use openfhe::parse_coefficients_bytes;

fn main() {
    let val = String::from("123456789099999");
    // Parameters based on https://github.com/openfheorg/openfhe-development/blob/7b8346f4eac27121543e36c17237b919e03ec058/src/core/unittest/UnitTestTrapdoor.cpp#L314
    let n: u32 = 16;
    let size: usize = 4; // Number of CRT
    let k_res: usize = 51;

    let const_poly = ffi::DCRTPolyGenFromConst(n, size, k_res, &val);
    // print the const_poly
    println!("const_poly: {:?}", const_poly);

    let modulus = ffi::GenModulus(n, size, k_res);
    println!("modulus: {:?}", modulus);

    let const_poly_2 = ffi::DCRTPolyGenFromConst(n, size, k_res, &val);
    // print the const_poly_2
    println!("const_poly_2: {:?}", const_poly_2);

    // assert that the two const_poly are equal
    assert_eq!(const_poly, const_poly_2);

    let const_poly_one = ffi::DCRTPolyGenFromConst(n, size, k_res, &String::from("1"));
    let negated_poly_one = const_poly_one.Negate();
    println!("negated_poly_one: {:?}", negated_poly_one);

    let coeffs = vec![
        String::from("123456789099999"),
        String::from("1234567842539099999"),
        String::from("31232189328123893128912983"),
        String::from("24535423544252452453"),
    ];

    let poly = ffi::DCRTPolyGenFromVec(n, size, k_res, &coeffs);
    let poly_2 = ffi::DCRTPolyGenFromVec(n, size, k_res, &coeffs);

    // assert that the two poly are equal
    assert_eq!(poly, poly_2);

    // perform polynomial addition
    let poly_add = ffi::DCRTPolyAdd(&poly, &poly_2);
    println!("poly_add: {:?}", poly_add);

    // perform polynomial multiplication
    let poly_mul = ffi::DCRTPolyMul(&poly, &poly_2);
    println!("poly_mul: {:?}", poly_mul);

    // get the coefficients of the polynomials
    let coeffs_poly = poly.GetCoefficients();
    println!("coeffs_poly: {:?}", coeffs_poly);
    let coeffs_poly_2 = poly_2.GetCoefficients();
    println!("coeffs_poly_2: {:?}", coeffs_poly_2);
    let coeffs_poly_add = poly_add.GetCoefficients();
    println!("coeffs_poly_add: {:?}", coeffs_poly_add);

    let coeffs_poly_bytes = poly.GetCoefficientsBytes();
    println!("coeffs_poly_bytes: {:?}", coeffs_poly_bytes);

    // decode coeff_poly_bytes
    let parsed_coefficients = parse_coefficients_bytes(&coeffs_poly_bytes);
    println!("decoded mod: {:?}", parsed_coefficients.modulus);
    println!("decoded coeffs: {:?}", parsed_coefficients.coefficients);

    let poly_modulus = poly.GetModulus();
    assert_eq!(poly_modulus, modulus);

    let sigma = 4.57825;
    let base = 2;
    let modulus_big_uint = BigUint::from_str_radix(&modulus, 10).unwrap();
    let k = modulus_big_uint.bits() as u32;

    // ** gen trapdoor **
    let trapdoor_output = ffi::DCRTTrapdoorGen(n, size, k_res, sigma, base, false);
    let trapdoor = trapdoor_output.GetTrapdoorPair();
    let public_matrix = trapdoor_output.GetPublicMatrix();

    // sample a target polynomial
    let u = ffi::DCRTPolyGenFromDug(n, size, k_res);

    // generate a preimage such that public_matrix * preimage = target_polynomial
    let _preimage = ffi::DCRTTrapdoorGaussSamp(n, k, &public_matrix, &trapdoor, &u, base, sigma);

    // ** gen trapdoor for a square matrix target of size 2x2 **
    let d = 2;
    let trapdoor_output_square =
        ffi::DCRTSquareMatTrapdoorGen(n, size, k_res, d, sigma, base, false);

    let trapdoor_square = trapdoor_output_square.GetTrapdoorPair();
    let public_matrix_square = trapdoor_output_square.GetPublicMatrix();

    // build the target matrix by sampling a random polynomial for each element
    let mut target_matrix = ffi::MatrixGen(n, size, k_res, d, d);
    for i in 0..d {
        for j in 0..d {
            let poly = ffi::DCRTPolyGenFromDug(n, size, k_res);
            ffi::SetMatrixElement(target_matrix.as_mut().unwrap(), i, j, &poly);
        }
    }

    // generate a preimage such that public_matrix_square * preimage = target_matrix
    let _preimage_square = ffi::DCRTSquareMatTrapdoorGaussSamp(
        n,
        k,
        &public_matrix_square,
        &trapdoor_square,
        &target_matrix,
        base,
        sigma,
    );

    // generate preimage and store it to file system
    let path = String::from("data/preimageID.bin");

    ffi::DCRTSquareMatTrapdoorGaussSampToFs(
        n,
        k,
        &public_matrix_square,
        &trapdoor_square,
        &target_matrix,
        base,
        sigma,
        &path,
    );

    // fetch the matrix from the file system
    let preimage = ffi::GetMatrixFromFs(n, size, k_res, &path);
    let nrow = ffi::GetMatrixRows(&preimage);
    let ncol = ffi::GetMatrixCols(&preimage);

    let dummy_poly = ffi::DCRTPolyGenFromDug(n, size, k_res);
    let decomposed_poly = dummy_poly.Decompose(1);
    let poly_0_0 = GetMatrixElement(&decomposed_poly, 0, 0);

    let sample = ffi::GenerateIntegerKarney(0.0, 4.0);
    println!("sample: {:?}", sample);

    // Example of using DCRTGaussSampGqArbBase

    // Parameters
    let c: f64 = (base as f64 + 1.0) * sigma; // Typically c = (base + 1) * sigma

    let syndrome_poly = ffi::DCRTPolyGenFromDug(n, size, k_res);

    // Loop over each tower_idx which is taken from `size`
    for tower_idx in 0..size {
        // Call DCRTGaussSampGqArbBase for each tower
        let digits =
            ffi::DCRTGaussSampGqArbBase(&syndrome_poly, c, n, size, k_res, base, sigma, tower_idx);

        println!("Tower {}: Sampled {} digits", tower_idx, digits.len());

        // Print first few digits (to avoid excessive output)
        let display_count = std::cmp::min(10, digits.len());
        println!(
            "First {} digits: {:?}",
            display_count,
            &digits[0..display_count]
        );
    }
}
