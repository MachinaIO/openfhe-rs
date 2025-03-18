#include "Trapdoor.h"
#include "Params.h"

namespace openfhe
{

DCRTTrapdoor::DCRTTrapdoor(Matrix&& publicMatrix, Matrix&& trapdoorFirst, Matrix&& trapdoorSecond) noexcept
    : m_publicMatrix(std::move(publicMatrix)), m_trapdoorFirst(std::move(trapdoorFirst)), m_trapdoorSecond(std::move(trapdoorSecond))
{ }

std::unique_ptr<Matrix> DCRTTrapdoor::GetTrapdoorFirst() const
{
    return std::make_unique<Matrix>(m_trapdoorFirst);
}

std::unique_ptr<Matrix> DCRTTrapdoor::GetTrapdoorSecond() const
{
    return std::make_unique<Matrix>(m_trapdoorSecond);
}

std::unique_ptr<Matrix> DCRTTrapdoor::GetPublicMatrix() const
{
    return std::make_unique<Matrix>(m_publicMatrix);
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
        std::move(trapdoor.second.m_e),
        std::move(trapdoor.second.m_r)
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
        std::move(trapdoor.second.m_e),
        std::move(trapdoor.second.m_r)
    );
}

// Gauss sample functions
std::unique_ptr<Matrix> DCRTTrapdoorGaussSamp(usint n, usint k, const Matrix& publicMatrix, const Matrix& trapdoorFirst, const Matrix& trapdoorSecond, const DCRTPoly& u, int64_t base, double sigma)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    double c = (base + 1) * sigma;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    lbcrypto::RLWETrapdoorPair trapdoor(trapdoorFirst, trapdoorSecond);

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSamp(
        n,
        k,
        publicMatrix,
        trapdoor,
        u.GetPoly(),
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}

std::unique_ptr<Matrix> DCRTSquareMatTrapdoorGaussSamp(usint n, usint k, const Matrix& publicMatrix, const Matrix& trapdoorFirst, const Matrix& trapdoorSecond, const Matrix& U, int64_t base, double sigma)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    double c = (base + 1) * sigma;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    lbcrypto::RLWETrapdoorPair trapdoor(trapdoorFirst, trapdoorSecond);

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSampSquareMat(
        n,
        k,
        publicMatrix,
        trapdoor,
        U,
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}
} // openfhe