#include "Trapdoor.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "openfhe/core/lattice/dgsampling.h"
#include "openfhe/src/lib.rs.h"

namespace openfhe
{

DCRTTrapdoorImpl::DCRTTrapdoorImpl(std::vector<DCRTPolyImpl>&& publicVector, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_publicVector(std::move(publicVector)), m_trapdoorPair(std::move(trapdoorPair))
{ }

std::unique_ptr<DCRTPolyImpl> DCRTTrapdoorImpl::GetPolyAtIndex(size_t index) const
{
    if (index >= m_publicVector.size()) {
        return nullptr;
    }
    
    return std::make_unique<DCRTPolyImpl>(m_publicVector[index]);
}

const RLWETrapdoorPair& DCRTTrapdoorImpl::GetTrapdoor() const noexcept
{
    return m_trapdoorPair;
}

size_t DCRTTrapdoorImpl::GetPublicVectorSize() const noexcept
{
    return m_publicVector.size();
}

std::unique_ptr<DCRTPolyImpl> GetMatrixElement(const Matrix& matrix, size_t row, size_t col)
{
    if (row >= matrix.GetRows() || col >= matrix.GetCols()) {
        return nullptr;
    }
    
    lbcrypto::DCRTPoly copy = matrix(row, col);
    return std::make_unique<DCRTPolyImpl>(std::move(copy));
}

std::unique_ptr<RLWETrapdoorPair> DCRTTrapdoorImpl::GetTrapdoorPtr() const
{
    return std::make_unique<RLWETrapdoorPair>(m_trapdoorPair);
}

std::unique_ptr<DCRTTrapdoorImpl> DCRTPolyTrapdoorGen(
    const ILDCRTParamsImpl& params,
    int64_t base,
    bool balanced)
{
    auto trapPair = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGen(
        params.GetRef(),
        lbcrypto::SIGMA,
        base,
        balanced
    );

    std::vector<DCRTPolyImpl> publicVector;
    publicVector.reserve(trapPair.first.GetCols());
    for (size_t i = 0; i < trapPair.first.GetCols(); i++) {
        publicVector.emplace_back(std::move(trapPair.first(0, i)));
    }

    return std::make_unique<DCRTTrapdoorImpl>(
        std::move(publicVector),
        std::move(trapPair.second)
    );
}

std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const DCRTTrapdoorImpl& trapdoor, const DCRTPolyImpl& u, int64_t base)
{
    lbcrypto::DCRTPoly::DggType dgg(lbcrypto::SIGMA);

    double c = (base + 1) * lbcrypto::SIGMA;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto zero_alloc                        = lbcrypto::DCRTPoly::Allocator(u.GetPoly().GetParams(), Format::EVALUATION);

    lbcrypto::Matrix<lbcrypto::DCRTPoly> A(zero_alloc, 1, k + 2);

    // Get the size of the vector 
    size_t size = trapdoor.GetPublicVectorSize();

    for (size_t i = 0; i < size; i++) {
        A(0, i) = trapdoor.GetPolyAtIndex(i)->GetPoly();
    }

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSamp(
        n,
        k,
        A,
        trapdoor.GetTrapdoor(),
        u.GetPoly(),
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}
} // openfhe 