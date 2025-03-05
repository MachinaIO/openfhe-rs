#include "DCRTPoly.h"
#include <sstream>

namespace openfhe
{

DCRTPoly::DCRTPoly(lbcrypto::DCRTPoly&& poly) noexcept
    : m_poly(std::move(poly))
{ }

rust::String DCRTPoly::GetString() const
{
    std::stringstream stream;
    stream << m_poly;
    return rust::String(stream.str());
}

bool DCRTPoly::IsEqual(const DCRTPoly& other) const noexcept
{
    return m_poly == other.m_poly;
}

// Generator functions
std::unique_ptr<DCRTPoly> DCRTPolyGenFromConst(
    usint n,
    size_t size,
    size_t kRes,
    const rust::String& value)
{   
    // Create params
    auto params        = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

    // Create a BigVector
    lbcrypto::BigVector bigVec(params -> GetRingDimension(), params ->GetModulus());
    bigVec[0] = lbcrypto::BigInteger(std::string(value));

    // Create a Poly that supports BigInteger coefficients)
    lbcrypto::PolyImpl<lbcrypto::BigVector> polyLarge(params, Format::COEFFICIENT);
    polyLarge.SetValues(bigVec, Format::COEFFICIENT);

    // Convert polyLarge to a DCRTPoly
    lbcrypto::DCRTPoly dcrtPoly(polyLarge, params);

    // switch dcrtPoly to EVALUATION format
    dcrtPoly.SetFormat(Format::EVALUATION);

    return std::make_unique<DCRTPoly>(std::move(dcrtPoly));
}

std::unique_ptr<DCRTPoly> DCRTPolyGenFromVec(
    usint n,
    size_t size,
    size_t kRes,
    const rust::Vec<rust::String>& values)
{   
    // Create params
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

    // Create a BigVector
    lbcrypto::BigVector bigVec(params->GetRingDimension(), params->GetModulus());
        for (size_t i = 0; i < values.size() && i < params->GetRingDimension(); i++) {
        bigVec[i] = lbcrypto::BigInteger(std::string(values[i]));
    }

    // Create a Poly that supports BigInteger coefficients
    lbcrypto::PolyImpl<lbcrypto::BigVector> polyLarge(params, Format::COEFFICIENT);
    polyLarge.SetValues(bigVec, Format::COEFFICIENT);

    // Convert polyLarge to a DCRTPoly
    lbcrypto::DCRTPoly dcrtPoly(polyLarge, params);

    // switch dcrtPoly to EVALUATION format
    dcrtPoly.SetFormat(Format::EVALUATION);

    return std::make_unique<DCRTPoly>(std::move(dcrtPoly));
}

DCRTPolyParams::DCRTPolyParams(const std::shared_ptr<lbcrypto::DCRTPoly::Params>& params) noexcept
    : m_params(params)
{ }
const std::shared_ptr<lbcrypto::DCRTPoly::Params>& DCRTPolyParams::GetRef() const noexcept
{
    return m_params;
}

// Generator functions
std::unique_ptr<DCRTPolyParams> DCRTPolyGenNullParams()
{
    return std::make_unique<DCRTPolyParams>();
}

} // openfhe
