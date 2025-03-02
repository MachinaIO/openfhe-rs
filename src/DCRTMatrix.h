#pragma once

#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/math/matrix.h"
#include "DCRTPoly.h"
#include <memory>
#include "openfhe/core/math/math-hal.h"
#include "openfhe/core/lattice/hal/default/dcrtpoly.h"
#include "Params.h"
#include "rust/cxx.h"

namespace openfhe
{

using Matrix = lbcrypto::Matrix<lbcrypto::DCRTPoly>;

class DCRTMatrixImpl final
{
    Matrix m_matrix;
public:
    DCRTMatrixImpl() = default;
    DCRTMatrixImpl(Matrix&& matrix) noexcept;
    DCRTMatrixImpl(const DCRTMatrixImpl&) = delete;
    DCRTMatrixImpl(DCRTMatrixImpl&& other) noexcept 
        : m_matrix(std::move(other.m_matrix)) {}  
    DCRTMatrixImpl& operator=(const DCRTMatrixImpl&) = delete;
    DCRTMatrixImpl& operator=(DCRTMatrixImpl&&) = delete;

    [[nodiscard]] const Matrix& GetMatrix() const noexcept;
    [[nodiscard]] Matrix& GetMatrix() noexcept;
    [[nodiscard]] std::unique_ptr<DCRTPolyImpl> GetElement(size_t row, size_t col) const;
void SetElement(size_t row, size_t col, const std::unique_ptr<DCRTPolyImpl>& element);    [[nodiscard]] size_t GetRows() const noexcept;
    [[nodiscard]] size_t GetCols() const noexcept;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTMatrixImpl> DCRTMatrixCreate(const ILDCRTParamsImpl& params, size_t rows, size_t cols);

} // namespace openfhe
