#include "Trapdoor.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "openfhe/core/lattice/dgsampling.h"

namespace openfhe
{

DCRTTrapdoorImpl::DCRTTrapdoorImpl(std::vector<std::unique_ptr<DCRTPolyImpl>>&& publicVector, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_publicVector(std::move(publicVector)), m_trapdoorPair(std::move(trapdoorPair))
{ }

// std::unique_ptr<std::vector<DCRTPolyImpl>> DCRTTrapdoorImpl::GetPublicVectorPtr() const
// {
//     return std::make_unique<std::vector<DCRTPolyImpl>>(m_publicVector);
// }

// const Matrix& DCRTTrapdoorImpl::GetMatrix() const noexcept
// {
//     return m_matrix;
// }

// const RLWETrapdoorPair& DCRTTrapdoorImpl::GetTrapdoor() const noexcept
// {
//     return m_trapdoorPair;
// }

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

    std::vector<std::unique_ptr<DCRTPolyImpl>> publicVector;
    publicVector.reserve(trapPair.first.GetCols());
    for (size_t i = 0; i < trapPair.first.GetCols(); i++) {
        publicVector.push_back(std::make_unique<DCRTPolyImpl>(std::move(trapPair.first(0, i))));
    }

    return std::make_unique<DCRTTrapdoorImpl>(
        std::move(publicVector),
        std::move(trapPair.second)
    );
}

// std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const DCRTTrapdoorImpl& trapdoor, const DCRTPolyImpl& u, int64_t base)
// {
//     lbcrypto::DCRTPoly::DggType dgg(lbcrypto::SIGMA);

//     double c = (base + 1) * lbcrypto::SIGMA;
//     double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
//     lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

//     auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSamp(
//         n,
//         k,
//         trapdoor.GetMatrix(),
//         trapdoor.GetTrapdoor(),
//         u.GetPoly(),
//         dgg,
//         dggLargeSigma,
//         base
//     );

//     return std::make_unique<Matrix>(std::move(result));
// }
} // openfhe 