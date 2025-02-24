#pragma once

#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/lattice/hal/default/dcrtpoly.h"
#include "openfhe/core/math/math-hal.h"
#include "Params.h"

namespace openfhe
{

class DCRTPoly final
{
    lbcrypto::DCRTPoly m_poly;
public:
    DCRTPoly(lbcrypto::DCRTPoly&& poly) noexcept;
    DCRTPoly(const DCRTPoly&) = delete;
    DCRTPoly(DCRTPoly&&) = delete;
    DCRTPoly& operator=(const DCRTPoly&) = delete;
    DCRTPoly& operator=(DCRTPoly&&) = delete;
};

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

using DCRTPolyImpl = lbcrypto::DCRTPolyImpl<lbcrypto::BigVector>;

/**
 * @brief Generates a DCRTPoly object using discrete uniform distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPolyImpl object
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParams& params);

/**
 * @brief Generates a DCRTPoly object using discrete Gaussian distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @param sigma The standard deviation for the Gaussian distribution
 * @return A unique pointer to the newly generated DCRTPolyImpl object
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDgg(const ILDCRTParams& params, double sigma);

/**
 * @brief Generates a DCRTPoly object using discrete binary distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPolyImpl object
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromBug(const ILDCRTParams& params);

/**
 * @brief Adds two DCRTPoly objects
 *
 * @param rhs The right-hand side DCRTPoly object
 * @param lhs The left-hand side DCRTPoly object    
 * @return A new DCRTPoly object representing the sum of the two operands
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyAdd(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs);

/**
 * @brief Multiplies two DCRTPoly objects
 *
 * @param rhs The right-hand side DCRTPoly object
 * @param lhs The left-hand side DCRTPoly object    
 * @return A new DCRTPoly object representing the product of the two operands
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyMul(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs);
} // openfhe
