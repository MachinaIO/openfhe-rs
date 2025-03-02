#pragma once
#include "Params.h"
#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/math/matrix.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "DCRTPoly.h"
#include "DCRTMatrix.h"
#include <vector>

namespace openfhe
{

using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;

class DCRTSquareMatTrapdoorImpl final
{
    DCRTMatrixImpl m_publicMatrix;
    RLWETrapdoorPair m_trapdoorPair;
public:
    DCRTSquareMatTrapdoorImpl() = default;
    DCRTSquareMatTrapdoorImpl(DCRTMatrixImpl&& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept;
    DCRTSquareMatTrapdoorImpl(const DCRTSquareMatTrapdoorImpl&) = delete;
    DCRTSquareMatTrapdoorImpl(DCRTSquareMatTrapdoorImpl&&) = delete;
    DCRTSquareMatTrapdoorImpl& operator=(const DCRTSquareMatTrapdoorImpl&) = delete;
    DCRTSquareMatTrapdoorImpl& operator=(DCRTSquareMatTrapdoorImpl&&) = delete;

    [[nodiscard]] const RLWETrapdoorPair& GetTrapdoor() const noexcept;
    [[nodiscard]] const DCRTMatrixImpl& GetPublicMatrix() const noexcept;
    [[nodiscard]] std::unique_ptr<RLWETrapdoorPair> GetTrapdoorPtr() const;
    // [[nodiscard]] std::unique_ptr<DCRTMatrixImpl> GetPublicMatrixPtr() const;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTSquareMatTrapdoorImpl> DCRTPolySquareMatTrapdoorGen(
    const ILDCRTParamsImpl& params,
    double sigma,
    size_t d,
    int64_t base,
    bool balanced);

// // Sample function
// [[nodiscard]] std::unique_ptr<Matrix> DCRTPolySquareMatGaussSamp(size_t n, size_t k, const DCRTSquareMatTrapdoorImpl& trapdoor, const DCRTPolyImpl& u, int64_t base, double sigma);

} // openfhe