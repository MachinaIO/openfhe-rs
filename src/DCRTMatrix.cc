#include "DCRTMatrix.h"

namespace openfhe
{

DCRTMatrix::DCRTMatrix(Matrix&& matrix) noexcept
    : m_matrix(std::move(matrix))
{ }

std::unique_ptr<DCRTPoly> DCRTMatrix::GetElement(size_t row, size_t col) const
{
    if (row >= m_matrix.GetRows() || col >= m_matrix.GetCols()) {
        return nullptr;
    }
    
return std::make_unique<DCRTPoly>(lbcrypto::DCRTPoly(m_matrix(row, col)));
}
} // namespace openfhe