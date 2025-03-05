#pragma once
#include "openfhe/core/lattice/trapdoor.h"
#include "DCRTPoly.h"
#include "DCRTMatrix.h"

namespace openfhe
{

using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;

class DCRTTrapdoor final
{
    DCRTMatrix m_publicMatrix;
    RLWETrapdoorPair m_trapdoorPair;
public:
    DCRTTrapdoor() = default;
    DCRTTrapdoor(DCRTMatrix& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept;
    DCRTTrapdoor(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor(DCRTTrapdoor&&) = delete;
    DCRTTrapdoor& operator=(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor& operator=(DCRTTrapdoor&&) = delete;

    [[nodiscard]] std::unique_ptr<RLWETrapdoorPair> GetTrapdoorPair() const;
    [[nodiscard]] std::unique_ptr<DCRTMatrix> GetPublicMatrix() const;
};

// // Generator functions
// [[nodiscard]] std::unique_ptr<DCRTTrapdoor> DCRTTrapdoorGen(
//     usint n, 
//     size_t size, 
//     size_t kRes,
//     double sigma,
//     size_t d,
//     int64_t base,
//     bool balanced);
} // openfhe