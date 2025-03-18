#pragma once
#include "openfhe/core/lattice/trapdoor.h"
#include "DCRTPoly.h"

namespace openfhe
{

using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;

class DCRTTrapdoor final
{
    Matrix m_publicMatrix;
    Matrix m_trapdoorFirst;
    Matrix m_trapdoorSecond;
public:
    DCRTTrapdoor() = default;
    DCRTTrapdoor(Matrix&& publicMatrix, Matrix&& trapdoorFirst, Matrix&& trapdoorSecond) noexcept;
    DCRTTrapdoor(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor(DCRTTrapdoor&&) = delete;
    DCRTTrapdoor& operator=(const DCRTTrapdoor&) = delete;
    DCRTTrapdoor& operator=(DCRTTrapdoor&&) = delete;

    [[nodiscard]] std::unique_ptr<Matrix> GetTrapdoorFirst() const;
    [[nodiscard]] std::unique_ptr<Matrix> GetTrapdoorSecond() const;
    [[nodiscard]] std::unique_ptr<Matrix> GetPublicMatrix() const;
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

[[nodiscard]] std::unique_ptr<DCRTTrapdoor> DCRTSquareMatTrapdoorGen(
    usint n, 
    size_t size, 
    size_t kRes,
    size_t d,
    double sigma,
    int64_t base,
    bool balanced);

// Gauss sample functions
[[nodiscard]] std::unique_ptr<Matrix> DCRTTrapdoorGaussSamp(
    usint n, 
    usint k,
    const Matrix& publicMatrix, 
    const Matrix& trapdoorFirst, 
    const Matrix& trapdoorSecond,
    const DCRTPoly& u, 
    int64_t base, 
    double sigma);

[[nodiscard]] std::unique_ptr<Matrix> DCRTSquareMatTrapdoorGaussSamp(
    usint n, 
    usint k,
    const Matrix& publicMatrix, 
    const Matrix& trapdoorFirst, 
    const Matrix& trapdoorSecond,
    const Matrix& U, 
    int64_t base, 
    double sigma);
} // openfhe