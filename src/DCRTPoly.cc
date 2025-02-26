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

// Generator functions
std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromBug(const ILDCRTParams& params)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    typename lbcrypto::DCRTPoly::BugType bug;
    auto poly = lbcrypto::DCRTPoly(bug, params_ptr, Format::COEFFICIENT);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParams& params)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    typename lbcrypto::DCRTPoly::DugType dug;
    auto poly = lbcrypto::DCRTPoly(dug, params_ptr, Format::COEFFICIENT);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDgg(const ILDCRTParams& params, double sigma)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    typename lbcrypto::DCRTPoly::DggType dgg(sigma);
    auto poly = lbcrypto::DCRTPoly(dgg, params_ptr, Format::COEFFICIENT);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromConst(const ILDCRTParams& params, uint64_t value)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    lbcrypto::DCRTPoly poly(params_ptr, Format::COEFFICIENT);
    poly = {value};
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
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

} // openfhe
