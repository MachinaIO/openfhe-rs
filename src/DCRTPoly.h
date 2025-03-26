#pragma once

#include "openfhe/core/lattice/hal/lat-backend.h"
#include "rust/cxx.h"
#include "openfhe/core/math/matrix.h"
#include "openfhe/core/utils/serial.h"

namespace openfhe
{

using Matrix = lbcrypto::Matrix<lbcrypto::DCRTPoly>;

class DCRTPoly final
{
    lbcrypto::DCRTPoly m_poly;
public:
    DCRTPoly(lbcrypto::DCRTPoly&& poly) noexcept;
    DCRTPoly(const DCRTPoly&) = delete;
    DCRTPoly(DCRTPoly&&) = delete;
    DCRTPoly& operator=(const DCRTPoly&) = delete;
    DCRTPoly& operator=(DCRTPoly&&) = delete;

    [[nodiscard]] const lbcrypto::DCRTPoly& GetPoly() const noexcept;
    [[nodiscard]] rust::String GetString() const;
    [[nodiscard]] bool IsEqual(const DCRTPoly& other) const noexcept;
    [[nodiscard]] rust::Vec<rust::String> GetCoefficients() const;
    [[nodiscard]] rust::Vec<rust::u8> GetCoefficientsBytes() const;
    [[nodiscard]] rust::String GetModulus() const;
    [[nodiscard]] std::unique_ptr<DCRTPoly> Negate() const;
    [[nodiscard]] std::unique_ptr<Matrix> Decompose() const;
};

// Generator functions 
[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyGenFromConst(
    usint n,
    size_t size,
    size_t kRes,
    const rust::String& value
);

[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyGenFromVec(
    usint n,
    size_t size,
    size_t kRes,
    const rust::Vec<rust::String>& values
);

[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyGenFromBug(usint n, size_t size, size_t kRes);
[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyGenFromDug(usint n, size_t size, size_t kRes);
[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyGenFromDgg(usint n, size_t size, size_t kRes, double sigma);

// Arithmetic
[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyAdd(const DCRTPoly& rhs, const DCRTPoly& lhs);
[[nodiscard]] std::unique_ptr<DCRTPoly> DCRTPolyMul(const DCRTPoly& rhs, const DCRTPoly& lhs);

class DCRTPolyParams final
{
    std::shared_ptr<lbcrypto::DCRTPoly::Params> m_params;
public:
    DCRTPolyParams() = default;
    DCRTPolyParams(const std::shared_ptr<lbcrypto::DCRTPoly::Params>& params) noexcept;
    DCRTPolyParams(const DCRTPolyParams&) = delete;
    DCRTPolyParams(DCRTPolyParams&&) = delete;
    DCRTPolyParams& operator=(const DCRTPolyParams&) = delete;
    DCRTPolyParams& operator=(DCRTPolyParams&&) = delete;

    [[nodiscard]] const std::shared_ptr<lbcrypto::DCRTPoly::Params>& GetRef() const noexcept;
};

// Generator functions
[[nodiscard]] std::unique_ptr<DCRTPolyParams> DCRTPolyGenNullParams();

// Matrix functions
[[nodiscard]] std::unique_ptr<Matrix> MatrixGen(
    usint n, 
    size_t size, 
    size_t kRes,
    size_t nrow, 
    size_t ncol);

void SetMatrixElement(
    Matrix& matrix, 
    size_t row, 
    size_t col, 
    const DCRTPoly& element);

[[nodiscard]] std::unique_ptr<DCRTPoly> GetMatrixElement(
    const Matrix& matrix, 
    size_t row, 
    size_t col);
} // openfhe
