#include "SequenceContainers.h"

#include "openfhe/pke/constants.h"

#include <cstddef>

namespace openfhe
{

VectorOfCiphertexts::VectorOfCiphertexts(
    std::vector<std::shared_ptr<CiphertextImpl>>&& ciphertexts) noexcept
    : m_ciphertexts(std::move(ciphertexts))
{ }
const std::vector<std::shared_ptr<CiphertextImpl>>& VectorOfCiphertexts::GetRef() const noexcept
{
    return m_ciphertexts;
}
std::vector<std::shared_ptr<CiphertextImpl>>& VectorOfCiphertexts::GetRef() noexcept
{
    return m_ciphertexts;
}

VectorOfDCRTPolys::VectorOfDCRTPolys(
    std::shared_ptr<std::vector<lbcrypto::DCRTPoly>>&& elements) noexcept
    : m_elements(std::move(elements))
{ }
const std::shared_ptr<std::vector<lbcrypto::DCRTPoly>>& VectorOfDCRTPolys::GetRef() const noexcept
{
    return m_elements;
}

VectorOfPolys::VectorOfPolys(std::vector<lbcrypto::Poly>&& polys) noexcept
    : m_polys(std::move(polys))
{ }
const std::vector<lbcrypto::Poly>& VectorOfPolys::GetRef() const noexcept
{
    return m_polys;
}
std::vector<lbcrypto::Poly>& VectorOfPolys::GetRef() noexcept
{
    return m_polys;
}
rust::Vec<rust::String> VectorOfPolys::GetElementCoefficients(const size_t index) const
{
    rust::Vec<rust::String> result;
    if (index >= m_polys.size())
    {
        return result;
    }

    auto poly = m_polys[index];
    poly.SetFormat(Format::COEFFICIENT);

    const auto& coeffs = poly.GetValues();
    result.reserve(coeffs.GetLength());
    for (usint i = 0; i < coeffs.GetLength(); ++i)
    {
        result.push_back(rust::String(coeffs[i].ToString()));
    }

    return result;
}

VectorOfEvalKeys::VectorOfEvalKeys(std::vector<std::shared_ptr<EvalKeyImpl>> evalKeys)
    : m_evalKeys(std::move(evalKeys))
{ }
const std::vector<std::shared_ptr<EvalKeyImpl>>& VectorOfEvalKeys::GetRef() const noexcept
{
    return m_evalKeys;
}

VectorOfLWECiphertexts::VectorOfLWECiphertexts(
    std::vector<std::shared_ptr<LWECiphertextImpl>>&& lweCiphertexts) noexcept
    : m_lweCiphertexts(std::move(lweCiphertexts))
{ }
std::vector<std::shared_ptr<LWECiphertextImpl>>& VectorOfLWECiphertexts::GetRef() noexcept
{
    return m_lweCiphertexts;
}

VectorOfPrivateKeys::VectorOfPrivateKeys(
    std::vector<std::shared_ptr<PrivateKeyImpl>>&& privateKeys) noexcept
    : m_privateKeys(std::move(privateKeys))
{ }
const std::vector<std::shared_ptr<PrivateKeyImpl>>& VectorOfPrivateKeys::GetRef() const noexcept
{
    return m_privateKeys;
}

VectorOfVectorOfCiphertexts::VectorOfVectorOfCiphertexts(
    std::vector<std::vector<std::shared_ptr<CiphertextImpl>>>&& ciphertexts) noexcept
    : m_ciphertexts(std::move(ciphertexts))
{ }
std::vector<std::vector<std::shared_ptr<CiphertextImpl>>>&
    VectorOfVectorOfCiphertexts::GetRef() noexcept
{
    return m_ciphertexts;
}

} // openfhe
