use openfhe::ffi;

fn main() {
    // Parameters based on https://github.com/openfheorg/openfhe-development/blob/7b8346f4eac27121543e36c17237b919e03ec058/src/core/unittest/UnitTestTrapdoor.cpp#L314
    let n: u32 = 16;
    let size: u32 = 4;
    let k_res: u32 = 51;
    let base: i64 = 8;

    let params = ffi::GenILDCRTParamsByOrderSizeBits(2 * n, size, k_res);

    let u = ffi::DCRTPolyGenFromDug(&params);
    let _neg_u = u.Negate();

    println!("{}", u.GetString());

    let trapdoor = ffi::DCRTPolyTrapdoorGen(&params, base, false);

    let k = 68; // TODO: not hardcoded

    let _constant_poly = ffi::DCRTPolyGenFromConst(&params, 1);

    let _res = ffi::DCRTPolyGaussSamp(n.try_into().unwrap(), k, &trapdoor, &u, base);

    let sigma = 4.57825;

    let _e = ffi::DCRTPolyGenFromDgg(&params, sigma);

    let _s = ffi::DCRTPolyGenFromBug(&params);

    let rhs = ffi::DCRTPolyGenFromDug(&params);
    let lhs = ffi::DCRTPolyGenFromDug(&params);

    let _out_add = ffi::DCRTPolyAdd(&rhs, &lhs);
    let _out_mul = ffi::DCRTPolyMul(&rhs, &lhs);
}
