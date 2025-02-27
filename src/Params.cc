#include "Params.h"

namespace openfhe
{

ILDCRTParamsImpl::ILDCRTParamsImpl(const std::shared_ptr<ILDCRTParams>& params) noexcept
    : m_params(params)
{ }
const std::shared_ptr<ILDCRTParams>& ILDCRTParamsImpl::GetRef() const noexcept
{
    return m_params;
}

rust::String ILDCRTParamsImpl::GetModulus() const
{
    return m_params -> GetModulus().ToString();
}

uint32_t ILDCRTParamsImpl::GetRingDimension() const
{
    return m_params->GetRingDimension();
}

// Generator functions

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
std::unique_ptr<ILDCRTParamsImpl> GenILDCRTParamsByOrderSizeBits(
    uint32_t corder, uint32_t depth, uint32_t bits)
{   
    auto params = std::make_shared<ILDCRTParams>(corder, depth, bits);
    return std::make_unique<ILDCRTParamsImpl>(params);
}
} // openfhe
