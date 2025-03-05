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

    let poly_modulus = poly.GetModulus();
    assert_eq!(poly_modulus, modulus);

    // sample trapdoor
    let sigma = 4.57825;
    let trapdoor = ffi::DCRTTrapdoorGen(n, size, k_res, sigma, 2, false);
    // fetch a polynomial from the trapdoor public matrix
    let public_matrix_poly_0_0 = trapdoor.GetPublicMatrixElement(0, 0);
    println!("public_matrix_poly_0_0: {:?}", public_matrix_poly_0_0);
}
