#include "Trapdoor.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "openfhe/core/lattice/dgsampling.h"

namespace openfhe
{

std::unique_ptr<TrapdoorOutput> DCRTPolyTrapdoorGen(
    const ILDCRTParams& params,
    int64_t base,
    bool balanced)
{

    std::shared_ptr<ILDCRTParams> params_ptr = std::make_shared<ILDCRTParams>(params);

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGen(
        params_ptr,
        lbcrypto::SIGMA,
        base,
        balanced
    );

    return std::make_unique<TrapdoorOutput>(TrapdoorOutput{
        std::move(result.first),
        std::move(result.second)
    });
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
        trapdoor.m,
        trapdoor.tp,
        u.GetPoly(),
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}
} // openfhe 