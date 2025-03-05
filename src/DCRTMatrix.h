#pragma once

#include "openfhe/core/math/matrix.h"
#include "DCRTPoly.h"

namespace openfhe
{

using Matrix = lbcrypto::Matrix<lbcrypto::DCRTPoly>;

class DCRTMatrix final
{
    Matrix m_matrix;
public:
    DCRTMatrix() = default;
    DCRTMatrix(Matrix&& matrix) noexcept;
    DCRTMatrix(const DCRTMatrix& other);
    DCRTMatrix(DCRTMatrix&&) = delete;
    DCRTMatrix& operator=(const DCRTMatrix&) = delete;
    DCRTMatrix& operator=(DCRTMatrix&&) = delete;

    [[nodiscard]] std::unique_ptr<DCRTPoly> GetElement(size_t row, size_t col) const;
    [[nodiscard]] const Matrix& GetMatrix() const noexcept;
};

// // Generator functions
// [[nodiscard]] std::unique_ptr<DCRTMatrix> DCRTMatrixGenNull(usint n, size_t size, size_t kRes, size_t rows, size_t cols);
} // namespace openfhe