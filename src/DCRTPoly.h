#pragma once

#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/lattice/hal/default/dcrtpoly.h"
#include "openfhe/core/math/math-hal.h"
#include "Params.h"

namespace openfhe
{

class DCRTPoly final
{
    lbcrypto::DCRTPoly m_poly;
public:
    DCRTPoly(lbcrypto::DCRTPoly&& poly) noexcept;
    DCRTPoly(const DCRTPoly&) = delete;
    DCRTPoly(DCRTPoly&&) = delete;
    DCRTPoly& operator=(const DCRTPoly&) = delete;
    DCRTPoly& operator=(DCRTPoly&&) = delete;
};

class DCRTPolyParams final
{
    std::shared_ptr<lbcrypto::DCRTPoly::Params> m_params;
public:
    DCRTPolyParams() = default;
    DCRTPolyParams(const std::shared_ptr<lbcrypto::DCRTPoly::Params>& params) noexcept;
    DCRTPolyParams(const DCRTPolyParams&) = delete;
    DCRTPolyParams(DCRTPolyParams&&) = delete;
    DCRTPolyParams& operator=(const DCRTPolyParams&) = delete;
    DCRTPolyParams& operator=(DCRTPolyParams&&) = delete;

    [[nodiscard]] const std::shared_ptr<lbcrypto::DCRTPoly::Params>& GetRef() const noexcept;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTPolyParams> DCRTPolyGenNullParams();

using DCRTPolyImpl = lbcrypto::DCRTPolyImpl<lbcrypto::BigVector>;

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParams& params);

} // openfhe
