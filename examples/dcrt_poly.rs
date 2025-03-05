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

    let const_poly_2 = ffi::DCRTPolyGenFromConst(n, size, k_res, &val);
    // print the const_poly_2
    println!("const_poly_2: {:?}", const_poly_2);

    // assert that the two const_poly are equal
    assert_eq!(const_poly, const_poly_2);
}
