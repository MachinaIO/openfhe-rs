use openfhe::ffi;

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
}
