#pragma once
#include "Params.h"
#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/math/matrix.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "DCRTPoly.h"
#include <vector>

namespace openfhe
{

using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;

class DCRTTrapdoorImpl final
{
    std::vector<std::unique_ptr<DCRTPolyImpl>> m_publicVector;
    RLWETrapdoorPair m_trapdoorPair;
public:
    DCRTTrapdoorImpl() = default;
    DCRTTrapdoorImpl(std::vector<std::unique_ptr<DCRTPolyImpl>>&& publicVector, RLWETrapdoorPair&& trapdoorPair) noexcept;
    DCRTTrapdoorImpl(const DCRTTrapdoorImpl&) = delete;
    DCRTTrapdoorImpl(DCRTTrapdoorImpl&&) = delete;
    DCRTTrapdoorImpl& operator=(const DCRTTrapdoorImpl&) = delete;
    DCRTTrapdoorImpl& operator=(DCRTTrapdoorImpl&&) = delete;

    [[nodiscard]] std::unique_ptr<RLWETrapdoorPair> GetTrapdoorPtr() const;
    // [[nodiscard]] std::unique_ptr<std::vector<DCRTPolyImpl>> GetPublicVectorPtr() const;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTTrapdoorImpl> DCRTPolyTrapdoorGen(
    const ILDCRTParamsImpl& params,
    int64_t base,
    bool balanced);

// // Sample function
// [[nodiscard]] std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const DCRTTrapdoorImpl& trapdoor, const DCRTPolyImpl& u, int64_t base);
} // openfhe