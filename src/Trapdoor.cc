#include "Trapdoor.h"
#include "Params.h"

namespace openfhe
{

DCRTTrapdoor::DCRTTrapdoor(Matrix&& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_publicMatrix(std::move(publicMatrix)), m_trapdoorPair(std::move(trapdoorPair))
{ }

std::unique_ptr<RLWETrapdoorPair> DCRTTrapdoor::GetTrapdoorPair() const
{
    return std::make_unique<RLWETrapdoorPair>(m_trapdoorPair);
}

std::unique_ptr<DCRTPoly> DCRTTrapdoor::GetPublicMatrixElement(size_t row, size_t col) const
{
    if (row >= m_publicMatrix.GetRows() || col >= m_publicMatrix.GetCols()) {
        return nullptr;
    }
    
    lbcrypto::DCRTPoly copy = m_publicMatrix(row, col);
    return std::make_unique<DCRTPoly>(std::move(copy));
}

// Generator functions
std::unique_ptr<DCRTTrapdoor> DCRTTrapdoorGen(
    usint n, 
    size_t size, 
    size_t kRes,
    double sigma,
    int64_t base,
    bool balanced)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    
    auto trapdoor = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGen(
        params,
        sigma,
        base,
        balanced
    );

    return std::make_unique<DCRTTrapdoor>(
        std::move(trapdoor.first),
        std::move(trapdoor.second)
    );
}

std::unique_ptr<DCRTTrapdoor> DCRTSquareMatTrapdoorGen(
    usint n, 
    size_t size, 
    size_t kRes,
    size_t d,
    double sigma,
    int64_t base,
    bool balanced)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

    auto trapdoor = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGenSquareMat(
        params,
        sigma,
        d,
        base,
        balanced
    );
    
    return std::make_unique<DCRTTrapdoor>(
        std::move(trapdoor.first),
        std::move(trapdoor.second)
    );
}

// Matrix functions
std::unique_ptr<Matrix> MatrixGen(
    usint n, 
    size_t size, 
    size_t kRes,
    size_t nrow, 
    size_t ncol)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    auto zero_alloc = lbcrypto::DCRTPoly::Allocator(params, Format::EVALUATION);
    Matrix matrix(zero_alloc, nrow, ncol);
    return std::make_unique<Matrix>(std::move(matrix));
}

void SetMatrixElement(
    Matrix& matrix, 
    size_t row, 
    size_t col, 
    const DCRTPoly& element)
{
    matrix(row, col) = element.GetPoly();
}

std::unique_ptr<DCRTPoly> GetMatrixElement(
    const Matrix& matrix, 
    size_t row, 
    size_t col)
{   
    lbcrypto::DCRTPoly copy = matrix(row, col);
    return std::make_unique<DCRTPoly>(std::move(copy));
}
} // openfhe