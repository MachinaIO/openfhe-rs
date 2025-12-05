#include "Params.h"
#include "DCRTPoly.h"

namespace openfhe
{

ElementParams::ElementParams(
    std::shared_ptr<lbcrypto::ILDCRTParams<lbcrypto::DCRTPoly::Integer>> params) noexcept
    : m_params(std::move(params))
{ }
const std::shared_ptr<lbcrypto::ILDCRTParams<lbcrypto::DCRTPoly::Integer>>&
    ElementParams::GetRef() const noexcept
{
    return m_params;
}

std::unique_ptr<ParamsBFVRNS> GenParamsBFVRNS()
{
    return std::make_unique<ParamsBFVRNS>();
}
std::unique_ptr<ParamsBFVRNS> GenParamsBFVRNSbyVectorOfString(const std::vector<std::string>& vals)
{
    return std::make_unique<ParamsBFVRNS>(vals);
}
std::unique_ptr<ParamsBGVRNS> GenParamsBGVRNS()
{
    return std::make_unique<ParamsBGVRNS>();
}
std::unique_ptr<ParamsBGVRNS> GenParamsBGVRNSbyVectorOfString(const std::vector<std::string>& vals)
{
    return std::make_unique<ParamsBGVRNS>(vals);
}
std::unique_ptr<Params> GenParamsByScheme(const SCHEME scheme)
{
    return std::make_unique<Params>(scheme);
}
std::unique_ptr<Params> GenParamsByVectorOfString(const std::vector<std::string>& vals)
{
    return std::make_unique<Params>(vals);
}
std::unique_ptr<ParamsCKKSRNS> GenParamsCKKSRNS()
{
    return std::make_unique<ParamsCKKSRNS>();
}
std::unique_ptr<ParamsCKKSRNS> GenParamsCKKSRNSbyVectorOfString(
    const std::vector<std::string>& vals)
{
    return std::make_unique<ParamsCKKSRNS>(vals);
}
rust::String GenModulus(
    usint n, size_t size, size_t kRes)
{
    auto params        = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    return rust::String(params->GetModulus().ToString());
}

/// Return the CRT primes q_0, ... ,q_{L‑1}
rust::Vec<rust::String> GenCRTBasis(  
        usint n,
        std::size_t depth,
        std::size_t bits) {

    using lbcrypto::BigInteger;
    using lbcrypto::ILDCRTParams;

    auto params = std::make_shared<ILDCRTParams<BigInteger>>(2 * n, depth, bits);

    rust::Vec<rust::String> out;
    out.reserve(params->GetParams().size());

    for (const auto& tower : params->GetParams())
        out.push_back(rust::String(tower->GetModulus().ToString()));

    return out;
}
} // openfhe
