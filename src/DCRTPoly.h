#pragma once

#include "openfhe/core/lattice/hal/lat-backend.h"
#include "openfhe/core/lattice/hal/default/dcrtpoly.h"
#include "openfhe/core/math/math-hal.h"
#include "Params.h"
#include "rust/cxx.h"

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
    [[nodiscard]] std::unique_ptr<DCRTPolyImpl> Negate() const;
    [[nodiscard]] rust::String GetString() const;
    [[nodiscard]] bool IsEqual(const DCRTPolyImpl& other) const noexcept;
    void SwitchFormat();
};

// Generator functions
/**
 * @brief Generates a DCRTPolyImpl object sampled uniformly from the discrete binary distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPolyImpl object (by default, the format is EVALUATION)
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromBug(const ILDCRTParamsImpl& params);

/**
 * @brief Generates a DCRTPolyImpl object sampled uniformly from the discrete uniform distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPolyImpl object (by default, the format is EVALUATION)
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDug(const ILDCRTParamsImpl& params);

/**
 * @brief Generates a DCRTPolyImpl object sampled uniformly from the discrete Gaussian distribution
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @return A unique pointer to the newly generated DCRTPolyImpl object (by default, the format is EVALUATION)
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromDgg(const ILDCRTParamsImpl& params, double sigma);

/**
 * @brief Generates a DCRTPolyImpl object from a constant integer value
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @param value The integer value to be converted to a DCRTPolyImpl object
 * @return A unique pointer to the newly generated DCRTPolyImpl object (constant polynomial) (by default, the format is EVALUATION)
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromConst(const ILDCRTParamsImpl& params, const rust::String& value);

/**
 * @brief Generates a DCRTPolyImpl object from a vector of integers
 *
 * @param params The parameters defining the ring structure for the polynomial
 * @param values The vector of integers to be converted to a DCRTPolyImpl object
 * @return A unique pointer to the newly generated DCRTPolyImpl object (constant polynomial) (by default, the format is EVALUATION)
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyGenFromVec(const ILDCRTParamsImpl& params, const rust::Vec<rust::String>& values);

/**
 * @brief Adds two DCRTPolyImpl objects
 *
 * @param rhs The right-hand side DCRTPolyImpl object
 * @param lhs The left-hand side DCRTPolyImpl object    
 * @return A new DCRTPolyImpl object representing the sum of the two operands
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyAdd(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs);

/**
 * @brief Multiplies two DCRTPolyImpl objects
 *
 * Assumes both operands are in EVALUATION format
 * @param rhs The right-hand side DCRTPolyImpl object
 * @param lhs The left-hand side DCRTPolyImpl object    
 * @return A new DCRTPolyImpl object representing the product of the two operands
 */
[[nodiscard]] std::unique_ptr<DCRTPolyImpl> DCRTPolyMul(const DCRTPolyImpl& rhs, const DCRTPolyImpl& lhs);

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

} // openfhe
