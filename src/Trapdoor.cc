#include "Trapdoor.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "openfhe/core/lattice/dgsampling.h"
#include "openfhe/src/lib.rs.h"

namespace openfhe
{

DCRTSquareMatTrapdoorImpl::DCRTSquareMatTrapdoorImpl(DCRTMatrixImpl&& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_publicMatrix(std::move(publicMatrix)), m_trapdoorPair(std::move(trapdoorPair))
{ }

std::unique_ptr<RLWETrapdoorPair> DCRTSquareMatTrapdoorImpl::GetTrapdoorPtr() const
{
    return std::make_unique<RLWETrapdoorPair>(m_trapdoorPair);
}

std::unique_ptr<DCRTMatrixImpl> DCRTSquareMatTrapdoorImpl::GetPublicMatrixPtr() const
{
    return std::make_unique<DCRTMatrixImpl>(Matrix(m_publicMatrix.GetMatrix()));
}

std::unique_ptr<DCRTSquareMatTrapdoorImpl> DCRTPolySquareMatTrapdoorGen(
    const ILDCRTParamsImpl& params,
    double sigma,
    size_t d,
    int64_t base,
    bool balanced)
{
    auto trapPair = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGenSquareMat(
        params.GetRef(),
        sigma,
        d,
        base,
        balanced
    );

    DCRTMatrixImpl publicMatrix(std::move(trapPair.first));

    return std::make_unique<DCRTSquareMatTrapdoorImpl>(
        std::move(publicMatrix),
        std::move(trapPair.second)
    );
}

std::unique_ptr<DCRTMatrixImpl> DCRTPolySquareMatGaussSamp(size_t n, size_t k, const DCRTMatrixImpl& publicMatrix, const RLWETrapdoorPair& trapdoor, const DCRTMatrixImpl& U, int64_t base, double sigma)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    double c = (base + 1) * lbcrypto::SIGMA;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSampSquareMat(
        n,
        k,
        publicMatrix.GetMatrix(),
        trapdoor,
        U.GetMatrix(),
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<DCRTMatrixImpl>(Matrix(std::move(result)));
}
} // openfhe 