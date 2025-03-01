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
using Matrix = lbcrypto::Matrix<lbcrypto::DCRTPoly>;

class DCRTTrapdoorImpl final
{
    std::vector<DCRTPolyImpl> m_publicVector;
    RLWETrapdoorPair m_trapdoorPair;
public:
    DCRTTrapdoorImpl() = default;
    DCRTTrapdoorImpl(std::vector<DCRTPolyImpl>&& publicVector, RLWETrapdoorPair&& trapdoorPair) noexcept;
    DCRTTrapdoorImpl(const DCRTTrapdoorImpl&) = delete;
    DCRTTrapdoorImpl(DCRTTrapdoorImpl&&) = delete;
    DCRTTrapdoorImpl& operator=(const DCRTTrapdoorImpl&) = delete;
    DCRTTrapdoorImpl& operator=(DCRTTrapdoorImpl&&) = delete;

    [[nodiscard]] const RLWETrapdoorPair& GetTrapdoor() const noexcept;
    [[nodiscard]] std::unique_ptr<RLWETrapdoorPair> GetTrapdoorPtr() const;
    [[nodiscard]] std::unique_ptr<DCRTPolyImpl> GetPolyAtIndex(size_t index) const;
    [[nodiscard]] size_t GetPublicVectorSize() const noexcept;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTTrapdoorImpl> DCRTPolyTrapdoorGen(
    const ILDCRTParamsImpl& params,
    int64_t base,
    bool balanced);

// Sample function
[[nodiscard]] std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const DCRTTrapdoorImpl& trapdoor, const DCRTPolyImpl& u, int64_t base);
} // openfhe