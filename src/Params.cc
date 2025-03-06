#include "Params.h"
#include "DCRTPoly.h"

namespace openfhe
{

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
} // openfhe
