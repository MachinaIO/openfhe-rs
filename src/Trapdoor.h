#pragma once
#include "Params.h"
#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/math/matrix.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "DCRTPoly.h"
#include "rust/cxx.h"

namespace openfhe
{

using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;

class DCRTTrapdoor final
{
    rust::Vec<std::unique_ptr<DCRTPolyImpl>> m_publicVector;
    RLWETrapdoorPair m_trapdoorPair;
public:
    DCRTTrapdoor() = default;
    DCRTTrapdoor(rust::Vec<std::unique_ptr<DCRTPolyImpl>>&& publicVector, RLWETrapdoorPair&& trapdoorPair) noexcept;
    DCRTTrapdoor(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor(DCRTTrapdoor&&) = delete;
    DCRTTrapdoor& operator=(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor& operator=(DCRTTrapdoor&&) = delete;

    [[nodiscard]] std::unique_ptr<RLWETrapdoorPair> GetTrapdoorPtr() const;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTTrapdoor> DCRTPolyTrapdoorGen(
    const ILDCRTParamsImpl& params,
    int64_t base,
    bool balanced);

// // Sample function
// [[nodiscard]] std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const DCRTTrapdoor& trapdoor, const DCRTPolyImpl& u, int64_t base);
} // openfhe