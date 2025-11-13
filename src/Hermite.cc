#include "Hermite.h"

#include "openfhe/src/lib.rs.h"

namespace openfhe
{

std::unique_ptr<std::vector<ComplexPair>> GetHermiteTrigCoefficientsByFunction(
    rust::Fn<int64_t(int64_t)> func, const uint32_t p, const size_t order,
    const double scale)
{
    auto coefficients = lbcrypto::GetHermiteTrigCoefficients(
        [&](int64_t x){ return func(x); }, p, order, scale);

    std::vector<ComplexPair> result;
    result.reserve(coefficients.size());
    for (const std::complex<double>& elem : coefficients)
    {
        result.push_back(ComplexPair{elem.real(), elem.imag()});
    }
    return std::make_unique<std::vector<ComplexPair>>(std::move(result));
}

} // openfhe

