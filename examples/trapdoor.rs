use std::marker::PhantomPinned;

use cxx::{let_cxx_string, CxxString, CxxVector};
use openfhe::ffi;
fn main() {
    // Parameters based on https://github.com/openfheorg/openfhe-development/blob/7b8346f4eac27121543e36c17237b919e03ec058/src/core/unittest/UnitTestTrapdoor.cpp#L314
    let n: u32 = 16;
    let size: u32 = 4; // Number of CRT
    let k_res: u32 = 51;
    let base: i64 = 8;

    let params = ffi::GenILDCRTParamsByOrderSizeBits(2 * n, size, k_res);

    // Fetch modulus and ring dimension
    let modulus = params.GetModulus();
    let ring_dimension = params.GetRingDimension();
    println!("modulus: {:?}", modulus);
    println!("ring_dimension: {:?}", ring_dimension);

    let u = ffi::DCRTPolyGenFromDug(&params);
    let _neg_u = u.Negate();

    let trapdoor = ffi::DCRTPolyTrapdoorGen(&params, base, false);

    let k = 68; // TODO: not hardcoded

    // test traits
    let_cxx_string!(one = "1");

    let constant_poly = ffi::DCRTPolyGenFromConst(&params, &one);
    let constant_poly_2 = ffi::DCRTPolyGenFromConst(&params, &one);
    assert_eq!(constant_poly, constant_poly_2);
    println!("constant_poly: {:?}", constant_poly);

    let _res = ffi::DCRTPolyGaussSamp(n.try_into().unwrap(), k, &trapdoor, &u, base);

    let sigma = 4.57825;

    let _e = ffi::DCRTPolyGenFromDgg(&params, sigma);

    let mut _s = ffi::DCRTPolyGenFromBug(&params);

    // print _s
    println!("_s: {:?}", _s);
    // Switch to COEFFICIENT format
    _s.as_mut().unwrap().SwitchFormat();

    println!("s: {:?}", _s);

    // Switch back to EVALUATION format
    _s.as_mut().unwrap().SwitchFormat();

    println!("s: {:?}", _s);

    let rhs = ffi::DCRTPolyGenFromDug(&params);
    let lhs = ffi::DCRTPolyGenFromDug(&params);

    let _out_add = ffi::DCRTPolyAdd(&rhs, &lhs);
    let _out_mul = ffi::DCRTPolyMul(&rhs, &lhs);

    // Gen poly from vec
    let mut coeffs = Vec::new();
    coeffs.push("1".to_string());
    let poly = ffi::DCRTPolyGenFromVec(&params, &coeffs);
    println!("poly: {:?}", poly);
}
