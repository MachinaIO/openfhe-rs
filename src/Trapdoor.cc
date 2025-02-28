#include "Trapdoor.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "openfhe/core/lattice/dgsampling.h"

namespace openfhe
{

TrapdoorOutput::TrapdoorOutput(Matrix&& matrix, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_matrix(std::move(matrix)), m_trapdoorPair(std::move(trapdoorPair))
{ }

const Matrix& TrapdoorOutput::GetMatrix() const noexcept
{
    return m_matrix;
}

const RLWETrapdoorPair& TrapdoorOutput::GetTrapdoor() const noexcept
{
    return m_trapdoorPair;
}

std::unique_ptr<RLWETrapdoorPair> TrapdoorOutput::GetTrapdoorPtr() const
{
    return std::make_unique<RLWETrapdoorPair>(m_trapdoorPair);
}

std::unique_ptr<TrapdoorOutput> DCRTPolyTrapdoorGen(
    const ILDCRTParamsImpl& params,
    int64_t base,
    bool balanced)
{
    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGen(
        params.GetRef(),
        lbcrypto::SIGMA,
        base,
        balanced
    );

    return std::make_unique<TrapdoorOutput>(
        std::move(result.first),
        std::move(result.second)
    );
}

std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const TrapdoorOutput& trapdoor, const DCRTPolyImpl& u, int64_t base)
{
    lbcrypto::DCRTPoly::DggType dgg(lbcrypto::SIGMA);

    double c = (base + 1) * lbcrypto::SIGMA;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSamp(
        n,
        k,
        trapdoor.GetMatrix(),
        trapdoor.GetTrapdoor(),
        u.GetPoly(),
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}
} // openfhe 