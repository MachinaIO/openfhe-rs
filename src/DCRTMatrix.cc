#include "DCRTMatrix.h"

namespace openfhe
{

DCRTMatrixImpl::DCRTMatrixImpl(Matrix&& matrix) noexcept
    : m_matrix(std::move(matrix))
{ }

const Matrix& DCRTMatrixImpl::GetMatrix() const noexcept
{
    return m_matrix;
}

Matrix& DCRTMatrixImpl::GetMatrix() noexcept
{
    return m_matrix;
}

std::unique_ptr<DCRTPolyImpl> DCRTMatrixImpl::GetElement(size_t row, size_t col) const
{
    if (row >= m_matrix.GetRows() || col >= m_matrix.GetCols()) {
        return nullptr;
    }
    
    lbcrypto::DCRTPoly copy = m_matrix(row, col);
    return std::make_unique<DCRTPolyImpl>(std::move(copy));
}

void DCRTMatrixImpl::SetElement(size_t row, size_t col, std::unique_ptr<DCRTPolyImpl> element)
{
    m_matrix(row, col) = element->GetPoly();
}

size_t DCRTMatrixImpl::GetRows() const noexcept
{
    return m_matrix.GetRows();
}

size_t DCRTMatrixImpl::GetCols() const noexcept
{
    return m_matrix.GetCols();
}

// Generator functions
std::unique_ptr<DCRTMatrixImpl> DCRTMatrixCreate(const ILDCRTParamsImpl& params, size_t rows, size_t cols)
{
    auto zero_alloc = lbcrypto::DCRTPoly::Allocator(params.GetRef(), Format::EVALUATION);
    Matrix matrix(zero_alloc, cols, rows);
    return std::make_unique<DCRTMatrixImpl>(std::move(matrix));
}

} // namespace openfhe
