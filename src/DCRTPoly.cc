#include "DCRTPoly.h"
#include "openfhe/src/lib.rs.h"

namespace openfhe
{

DCRTPolyImpl::DCRTPolyImpl(lbcrypto::DCRTPoly&& poly) noexcept
    : m_poly(std::move(poly))
{ }

const lbcrypto::DCRTPoly& DCRTPolyImpl::GetPoly() const noexcept
{
    return m_poly;
}


// Generator functions
std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromBug(const ILDCRTParams& params)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    typename lbcrypto::DCRTPoly::BugType bug;
    auto poly = lbcrypto::DCRTPoly(bug, params_ptr, Format::EVALUATION);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParams& params)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    typename lbcrypto::DCRTPoly::DugType dug;
    auto poly = lbcrypto::DCRTPoly(dug, params_ptr, Format::EVALUATION);
    return std::make_unique<DCRTPolyImpl>(std::move(poly));
}

std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDgg(const ILDCRTParams& params, double sigma)
{
    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);
    typename lbcrypto::DCRTPoly::DggType dgg(sigma);
    auto poly = lbcrypto::DCRTPoly(dgg, params_ptr, Format::EVALUATION);
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

// std::unique_ptr<DCRTPolyImpl> DCRTPolyAdd(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs)
// {
//     auto res = rhs + lhs;
//     return std::make_unique<DCRTPolyImpl>(res);
// }

// std::unique_ptr<DCRTPolyImpl> DCRTPolyMul(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs)
// {
//     auto res = rhs * lhs;
//     return std::make_unique<DCRTPolyImpl>(res);
// }

} // openfhe
