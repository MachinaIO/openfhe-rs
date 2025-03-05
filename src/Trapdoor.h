#pragma once
#include "openfhe/core/lattice/trapdoor.h"
#include "openfhe/core/math/matrix.h"
#include "DCRTPoly.h"

namespace openfhe
{

using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;
using Matrix = lbcrypto::Matrix<lbcrypto::DCRTPoly>;

class DCRTTrapdoor final
{
    Matrix m_publicMatrix;
    RLWETrapdoorPair m_trapdoorPair;
public:
    DCRTTrapdoor() = default;
    DCRTTrapdoor(Matrix&& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept;
    DCRTTrapdoor(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor(DCRTTrapdoor&&) = delete;
    DCRTTrapdoor& operator=(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor& operator=(DCRTTrapdoor&&) = delete;

    [[nodiscard]] std::unique_ptr<RLWETrapdoorPair> GetTrapdoorPair() const;
    [[nodiscard]] std::unique_ptr<DCRTPoly> GetPublicMatrixElement(size_t row, size_t col) const;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTTrapdoor> DCRTTrapdoorGen(
    usint n, 
    size_t size, 
    size_t kRes,
    double sigma,
    int64_t base,
    bool balanced);

// Matrix functions
[[nodiscard]] std::unique_ptr<Matrix> MatrixGen(
    usint n, 
    size_t size, 
    size_t kRes,
    size_t nrow, 
    size_t ncol);

void SetMatrixElement(
    Matrix& matrix, 
    size_t row, 
    size_t col, 
    const DCRTPoly& element);
} // openfhe