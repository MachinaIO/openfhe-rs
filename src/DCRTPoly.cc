#include "DCRTPoly.h"
#include "openfhe/src/lib.rs.h"
#include <sstream>

namespace openfhe
{

DCRTPolyImpl::DCRTPolyImpl(lbcrypto::DCRTPoly&& poly) noexcept
    : m_poly(std::move(poly))
{ }

const lbcrypto::DCRTPoly& DCRTPolyImpl::GetPoly() const noexcept
{
    return m_poly;
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyImpl::Negate() const 
{
    auto res = -m_poly;
    return std::make_unique<DCRTPolyImpl>(std::move(res));
}

rust::String DCRTPolyImpl::GetString() const
{
    std::stringstream stream;
    stream << m_poly;
    return rust::String(stream.str());
}

bool DCRTPolyImpl::IsEqual(const DCRTPolyImpl& other) const noexcept {
    return m_poly == other.GetPoly();
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyAdd(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs)
{
    auto res = rhs.GetPoly() + lhs.GetPoly();
    return std::make_unique<DCRTPolyImpl>(std::move(res));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyMul(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs)
{
    auto res = rhs.GetPoly() * lhs.GetPoly();
    return std::make_unique<DCRTPolyImpl>(std::move(res));
}

// Generator functions
std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromBug(const ILDCRTParamsImpl& params)
{
    typename lbcrypto::DCRTPoly::BugType bug;
    auto poly = lbcrypto::DCRTPoly(bug, params.GetRef(), Format::EVALUATION);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParamsImpl& params)
{
    typename lbcrypto::DCRTPoly::DugType dug;
    auto poly = lbcrypto::DCRTPoly(dug, params.GetRef(), Format::EVALUATION);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDgg(const ILDCRTParamsImpl& params, double sigma)
{
    typename lbcrypto::DCRTPoly::DggType dgg(sigma);
    auto poly = lbcrypto::DCRTPoly(dgg, params.GetRef(), Format::EVALUATION);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromConst(const ILDCRTParamsImpl& params, const rust::String& value)
{
    // Create a BigVector
    lbcrypto::BigVector bigVec(params.GetRingDimension(), params.GetRef()->GetModulus());
    bigVec[0] = lbcrypto::BigInteger(std::string(value));

    // Create a PolyLargeType (which is a polynomial with BigInteger coefficients)
    lbcrypto::DCRTPoly::PolyLargeType polyLarge(params.GetRef());
    polyLarge.SetValues(bigVec, Format::COEFFICIENT);

    // Convert polyLarge to a DCRTPoly
    lbcrypto::DCRTPoly poly_small(polyLarge, params.GetRef());

    // switch poly_small to EVALUATION format
    poly_small.SetFormat(Format::EVALUATION);

    return std::make_unique<DCRTPolyImpl>(std::move(poly_small));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromVec(const ILDCRTParamsImpl& params, const rust::Vec<rust::String>& values)
{   
    // Create a BigVector
    lbcrypto::BigVector bigVec(params.GetRingDimension(), params.GetRef()->GetModulus());
    for (size_t i = 0; i < values.size(); ++i) {
        bigVec[i] = lbcrypto::BigInteger(std::string(values[i]));
    }

    // Create a PolyLargeType (which is a polynomial with BigInteger coefficients)
    lbcrypto::DCRTPoly::PolyLargeType polyLarge(params.GetRef());
    polyLarge.SetValues(bigVec, Format::COEFFICIENT);

    // Convert polyLarge to a DCRTPoly
    lbcrypto::DCRTPoly poly_small(polyLarge, params.GetRef());

    // switch poly_small to EVALUATION format
    poly_small.SetFormat(Format::EVALUATION);

    return std::make_unique<DCRTPolyImpl>(std::move(poly_small));
}

void DCRTPolyImpl::SwitchFormatImpl() 
{
    m_poly.SwitchFormat();
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


rust::Vec<rust::String> DCRTPolyImpl::GetCoefficients() const
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

} // openfhe
