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
    // Compare the internal DCRTPoly objects for equality
    return m_poly == other.m_poly;
}

// Generator functions
std::unique_ptr<DCRTPoly> DCRTPolyGenFromConst(
    usint n,
    size_t size,
    size_t kRes,
    const rust::String& value)
{
    auto params        = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

    lbcrypto::PolyImpl<lbcrypto::BigVector> polyLarge(params, Format::COEFFICIENT);

    polyLarge = {std::string(value)};

    lbcrypto::DCRTPolyImpl<lbcrypto::BigVector> dcrtPoly(polyLarge, params);

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
