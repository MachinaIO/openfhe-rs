#include "LWEPrivateKey.h"

#include "openfhe/binfhe/lwe-privatekey.h"

#include "DCRTPoly.h"
#include "openfhe/pke/constants.h"

#include <vector>

namespace openfhe
{

LWEPrivateKey::LWEPrivateKey(std::shared_ptr<LWEPrivateKeyImpl>&& lwePrivateKey) noexcept
    : m_lwePrivateKey(std::move(lwePrivateKey))
{ }
const std::shared_ptr<LWEPrivateKeyImpl>& LWEPrivateKey::GetRef() const noexcept
{
    return m_lwePrivateKey;
}

std::unique_ptr<DCRTPoly> LWEPrivateKey::GetElementAsDCRTPoly() const
{
    if (!m_lwePrivateKey)
    {
        return nullptr;
    }

    const auto& nativeVec = m_lwePrivateKey->GetElement();
    using PolyType = lbcrypto::DCRTPoly::PolyType;
    auto ilParams = std::make_shared<lbcrypto::ILNativeParams>(nativeVec.GetLength(),
        nativeVec.GetModulus());
    PolyType poly(ilParams, Format::COEFFICIENT, true);
    poly.SetValues(nativeVec, Format::COEFFICIENT);

    std::vector<PolyType> elements;
    elements.emplace_back(std::move(poly));

    lbcrypto::DCRTPoly dcrtPoly(elements);
    return std::make_unique<DCRTPoly>(std::move(dcrtPoly));
}

} // openfhe
