#pragma once
#include "Params.h"
#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/math/matrix.h"
#include "openfhe/core/lattice/trapdoor.h"
#include "DCRTPoly.h"

namespace openfhe
{

using Matrix = lbcrypto::Matrix<lbcrypto::DCRTPoly>;   
using RLWETrapdoorPair = lbcrypto::RLWETrapdoorPair<lbcrypto::DCRTPoly>;

// struct TrapdoorOutput
// {
//     Matrix m;
//     RLWETrapdoorPair tp;
// };

class TrapdoorOutput final
{
    Matrix m_matrix;
    RLWETrapdoorPair m_trapdoorPair;
public:
    TrapdoorOutput(Matrix&& matrix, RLWETrapdoorPair&& trapdoorPair) noexcept;
    TrapdoorOutput(const TrapdoorOutput&) = delete;
    TrapdoorOutput(TrapdoorOutput&&) = delete;
    TrapdoorOutput& operator=(const TrapdoorOutput&) = delete;
    TrapdoorOutput& operator=(TrapdoorOutput&&) = delete;

    [[nodiscard]] const Matrix& GetMatrix() const noexcept;
    [[nodiscard]] const RLWETrapdoorPair& GetTrapdoorPair() const noexcept;
};

// Generator functions
[[nodiscard]] std::unique_ptr<TrapdoorOutput> DCRTPolyTrapdoorGen(
    const ILDCRTParamsImpl& params,
    int64_t base,
    bool balanced);

// Sample function
[[nodiscard]] std::unique_ptr<Matrix> DCRTPolyGaussSamp(size_t n, size_t k, const TrapdoorOutput& trapdoor, const DCRTPolyImpl& u, int64_t base);
} // openfhe