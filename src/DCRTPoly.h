#pragma once

#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/lattice/hal/default/dcrtpoly.h"
#include "openfhe/core/math/math-hal.h"
#include "Params.h"

namespace openfhe
{

class DCRTPolyImpl final
{
    lbcrypto::DCRTPoly m_poly;
public:
    DCRTPolyImpl(lbcrypto::DCRTPoly&& poly) noexcept;
    DCRTPolyImpl(const DCRTPolyImpl&) = delete;
    DCRTPolyImpl(DCRTPolyImpl&&) = delete;
    DCRTPolyImpl& operator=(const DCRTPolyImpl&) = delete;
    DCRTPolyImpl& operator=(DCRTPolyImpl&&) = delete;

    [[nodiscard]] const lbcrypto::DCRTPoly& GetPoly() const noexcept;

};

// Generator functions
/**
 * @brief Generates a DCRTPoly object sampled uniformly from the discrete binary distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPoly object
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromBug(const ILDCRTParams& params);

/**
 * @brief Generates a DCRTPoly object sampled uniformly from the discrete uniform distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPoly object
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParams& params);

/**
 * @brief Generates a DCRTPoly object sampled uniformly from the discrete Gaussian distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPoly object
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDgg(const ILDCRTParams& params, double sigma);

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

// /**
//  * @brief Adds two DCRTPoly objects
//  *
//  * @param rhs The right-hand side DCRTPoly object
//  * @param lhs The left-hand side DCRTPoly object    
//  * @return A new DCRTPoly object representing the sum of the two operands
//  */
// [[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyAdd(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs);

// /**
//  * @brief Multiplies two DCRTPoly objects
//  *
//  * @param rhs The right-hand side DCRTPoly object
//  * @param lhs The left-hand side DCRTPoly object    
//  * @return A new DCRTPoly object representing the product of the two operands
//  */
// [[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyMul(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs);

} // openfhe
