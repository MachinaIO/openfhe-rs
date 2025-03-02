use std::marker::PhantomPinned;

use cxx::{let_cxx_string, CxxString, CxxVector};
use openfhe::ffi;
fn main() {
    // Parameters based on https://github.com/openfheorg/openfhe-development/blob/7b8346f4eac27121543e36c17237b919e03ec058/src/core/unittest/UnitTestTrapdoor.cpp#L314
    let n: u32 = 16;
    let size: u32 = 4; // Number of CRT
    let k_res: u32 = 51;
    let base: i64 = 2;

    let params = ffi::GenILDCRTParamsByOrderSizeBits(2 * n, size, k_res);

    // Fetch modulus and ring dimension
    let modulus = params.GetModulus();
    let ring_dimension = params.GetRingDimension();
    println!("modulus: {:?}", modulus);
    println!("ring_dimension: {:?}", ring_dimension);

    let u = ffi::DCRTPolyGenFromDug(&params);
    let _neg_u = u.Negate();

    // get modulus from u
    let modulus_from_u = u.GetModulus();
    assert_eq!(modulus, modulus_from_u);

    let sigma = 4.57825;
    let d = 3;

    // Generate matrix
    let mut matrix = ffi::DCRTMatrixCreate(&params, d, d);

    // Fill matrix with random polys
    for i in 0..d {
        for j in 0..d {
            let poly = ffi::DCRTPolyGenFromDug(&params);
            // print the poly and its index
            println!("poly: {:?}, index: {:?}", poly, (i, j));
            matrix.as_mut().unwrap().SetElement(i, j, &poly);
        }
    }

    // Get matrix elements and check that they are the same
    for i in 0..d {
        for j in 0..d {
            let poly = matrix.as_mut().unwrap().GetElement(i, j);
            println!("poly: {:?}, index: {:?}", poly, (i, j));
        }
    }

    let trapdoor_output = ffi::DCRTPolySquareMatTrapdoorGen(&params, sigma, d, base, false);
    let _trapdoor = trapdoor_output.GetTrapdoorPtr();
    let _public_matrix = trapdoor_output.GetPublicMatrixPtr();

    // print the sizes of the public matrix
    println!("public_matrix: {:?}", _public_matrix.GetRows());
    println!("public_matrix: {:?}", _public_matrix.GetCols());

    // test traits
    let one = String::from("1");

    let constant_poly = ffi::DCRTPolyGenFromConst(&params, &one);
    let constant_poly_2 = ffi::DCRTPolyGenFromConst(&params, &one);
    assert_eq!(constant_poly, constant_poly_2);
    println!("constant_poly: {:?}", constant_poly);

    let k = 204; // TODO: not hardcoded

    let preimage_matrix = ffi::DCRTPolySquareMatGaussSamp(
        n.try_into().unwrap(),
        k,
        &_public_matrix,
        &_trapdoor,
        &matrix,
        base,
        sigma,
    );

    let _e = ffi::DCRTPolyGenFromDgg(&params, sigma);

    let mut _s = ffi::DCRTPolyGenFromBug(&params);

    // print _s
    println!("_s: {:?}", _s);
    // Switch to COEFFICIENT format
    _s.as_mut().unwrap().SwitchFormatImpl();

    println!("s: {:?}", _s);

    // Switch back to EVALUATION format
    _s.as_mut().unwrap().SwitchFormatImpl();

    println!("s: {:?}", _s);

    let rhs = ffi::DCRTPolyGenFromDug(&params);
    let lhs = ffi::DCRTPolyGenFromDug(&params);

    let _out_add = ffi::DCRTPolyAdd(&rhs, &lhs);
    let _out_mul = ffi::DCRTPolyMul(&rhs, &lhs);

    // Gen poly from vec
    let mut coeffs = Vec::with_capacity(n as usize);
    for i in 0..n {
        coeffs.push(i.to_string());
    }

    let poly = ffi::DCRTPolyGenFromVec(&params, &coeffs);
    let get_coeffs = poly.GetCoefficients();
    assert_eq!(coeffs, get_coeffs);
}
