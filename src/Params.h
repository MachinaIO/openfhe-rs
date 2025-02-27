#pragma once

#include "openfhe/core/utils/exception.h"
#include "openfhe/pke/scheme/gen-cryptocontext-params.h"
#include "openfhe/pke/scheme/bfvrns/gen-cryptocontext-bfvrns-params.h"
#include "openfhe/pke/scheme/bgvrns/gen-cryptocontext-bgvrns-params.h"
#include "openfhe/pke/scheme/ckksrns/gen-cryptocontext-ckksrns-params.h"
#include "openfhe/core/lattice/hal/default/ildcrtparams.h"
#include "openfhe/core/lattice/hal/elemparams.h"
#include "openfhe/core/math/math-hal.h"

#include "rust/cxx.h"

#include <memory>

namespace openfhe
{

using COMPRESSION_LEVEL = lbcrypto::COMPRESSION_LEVEL;
using DecryptionNoiseMode = lbcrypto::DecryptionNoiseMode;
using EncryptionTechnique = lbcrypto::EncryptionTechnique;
using ExecutionMode = lbcrypto::ExecutionMode;
using KeySwitchTechnique = lbcrypto::KeySwitchTechnique;
using MultipartyMode = lbcrypto::MultipartyMode;
using MultiplicationTechnique = lbcrypto::MultiplicationTechnique;
using ProxyReEncryptionMode = lbcrypto::ProxyReEncryptionMode;
using ScalingTechnique = lbcrypto::ScalingTechnique;
using SCHEME = lbcrypto::SCHEME;
using SecretKeyDist = lbcrypto::SecretKeyDist;
using SecurityLevel = lbcrypto::SecurityLevel;

using Params = lbcrypto::Params;
using ParamsBFVRNS = lbcrypto::CCParams<lbcrypto::CryptoContextBFVRNS>;
using ParamsBGVRNS = lbcrypto::CCParams<lbcrypto::CryptoContextBGVRNS>;
using ParamsCKKSRNS = lbcrypto::CCParams<lbcrypto::CryptoContextCKKSRNS>;
using ILDCRTParams = lbcrypto::ILDCRTParams<lbcrypto::BigInteger>;

class ILDCRTParamsImpl final
{
    std::shared_ptr<ILDCRTParams> m_params;
public:
    ILDCRTParamsImpl() = default;
    ILDCRTParamsImpl(const std::shared_ptr<ILDCRTParams>& params) noexcept;
    ILDCRTParamsImpl(const ILDCRTParamsImpl&) = delete;
    ILDCRTParamsImpl(ILDCRTParamsImpl&&) = delete;
    ILDCRTParamsImpl& operator=(const ILDCRTParamsImpl&) = delete;
    ILDCRTParamsImpl& operator=(ILDCRTParamsImpl&&) = delete;

    [[nodiscard]] const std::shared_ptr<ILDCRTParams>& GetRef() const noexcept;
    [[nodiscard]] rust::String GetModulus() const;
    [[nodiscard]] uint32_t GetRingDimension() const;
};

// Generator functions
[[nodiscard]] std::unique_ptr<ParamsBFVRNS> GenParamsBFVRNS();
[[nodiscard]] std::unique_ptr<ParamsBFVRNS> GenParamsBFVRNSbyVectorOfString(
    const std::vector<std::string>& vals);
[[nodiscard]] std::unique_ptr<ParamsBGVRNS> GenParamsBGVRNS();
[[nodiscard]] std::unique_ptr<ParamsBGVRNS> GenParamsBGVRNSbyVectorOfString(
    const std::vector<std::string>& vals);
[[nodiscard]] std::unique_ptr<Params> GenParamsByScheme(const SCHEME scheme);
[[nodiscard]] std::unique_ptr<Params> GenParamsByVectorOfString(
    const std::vector<std::string>& vals);
[[nodiscard]] std::unique_ptr<ParamsCKKSRNS> GenParamsCKKSRNS();
[[nodiscard]] std::unique_ptr<ParamsCKKSRNS> GenParamsCKKSRNSbyVectorOfString(
    const std::vector<std::string>& vals);
[[nodiscard]] std::unique_ptr<ILDCRTParamsImpl> GenILDCRTParamsByOrderSizeBits(
    uint32_t corder, uint32_t depth, uint32_t bits);
} // openfhe
