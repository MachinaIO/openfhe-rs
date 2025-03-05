#include "DCRTPoly.h"
#include <sstream>

namespace openfhe
{

DCRTPoly::DCRTPoly(lbcrypto::DCRTPoly&& poly) noexcept
    : m_poly(std::move(poly))
{ }

const lbcrypto::DCRTPoly& DCRTPoly::GetPoly() const noexcept
{
    return m_poly;
}


rust::String DCRTPoly::GetString() const
{
    std::stringstream stream;
    stream << m_poly;
    return rust::String(stream.str());
}

rust::String DCRTPoly::GetModulus() const
{   
    return m_poly.GetModulus().ToString();
}

bool DCRTPoly::IsEqual(const DCRTPoly& other) const noexcept
{
    return m_poly == other.m_poly;
}

rust::Vec<rust::String> DCRTPoly::GetCoefficients() const
{   
    auto tempPoly = m_poly;
    tempPoly.SetFormat(Format::COEFFICIENT);

    lbcrypto::DCRTPoly::PolyLargeType polyLarge = tempPoly.CRTInterpolate();

    const lbcrypto::BigVector &coeffs = polyLarge.GetValues();

    rust::Vec<rust::String> result;
    for (size_t i = 0; i < coeffs.GetLength(); ++i)
    {
        result.push_back(rust::String(coeffs[i].ToString()));
    }

    return result;
}

std::unique_ptr<DCRTPoly> DCRTPoly::Negate() const
{
    return std::make_unique<DCRTPoly>(-m_poly);
}

// Arithmetic
std::unique_ptr<DCRTPoly> DCRTPolyAdd(const DCRTPoly& rhs, const DCRTPoly& lhs)
{
    return std::make_unique<DCRTPoly>(rhs.GetPoly() + lhs.GetPoly());
}

std::unique_ptr<DCRTPoly> DCRTPolyMul(const DCRTPoly& rhs, const DCRTPoly& lhs)
{
    return std::make_unique<DCRTPoly>(rhs.GetPoly() * lhs.GetPoly());
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

std::unique_ptr<DCRTPoly> DCRTPolyGenFromBug(usint n, size_t size, size_t kRes)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    typename lbcrypto::DCRTPoly::BugType bug;
    auto poly = lbcrypto::DCRTPoly(bug, params, Format::EVALUATION);
    return std::make_unique<DCRTPoly>(std::move(poly));
}

std::unique_ptr<DCRTPoly> DCRTPolyGenFromDug(usint n, size_t size, size_t kRes)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    typename lbcrypto::DCRTPoly::DugType dug;
    auto poly = lbcrypto::DCRTPoly(dug, params, Format::EVALUATION);
    return std::make_unique<DCRTPoly>(std::move(poly));
}

std::unique_ptr<DCRTPoly> DCRTPolyGenFromDgg(usint n, size_t size, size_t kRes, double sigma)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    typename lbcrypto::DCRTPoly::DggType dgg(sigma);
    auto poly = lbcrypto::DCRTPoly(dgg, params, Format::EVALUATION);
    return std::make_unique<DCRTPoly>(std::move(poly));
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
