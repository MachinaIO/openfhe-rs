#include "PrivateKey.h"

#include "DCRTPoly.h"
#include "openfhe/pke/key/privatekey.h"

namespace openfhe
{

PrivateKeyDCRTPoly::PrivateKeyDCRTPoly(const std::shared_ptr<PrivateKeyImpl>& privateKey) noexcept
    : m_privateKey(privateKey)
{ }
const std::shared_ptr<PrivateKeyImpl>& PrivateKeyDCRTPoly::GetRef() const noexcept
{
    return m_privateKey;
}
std::shared_ptr<PrivateKeyImpl>& PrivateKeyDCRTPoly::GetRef() noexcept
{
    return m_privateKey;
}
std::unique_ptr<DCRTPoly> PrivateKeyDCRTPoly::GetElementAsDCRTPoly() const
{
    if (!m_privateKey)
    {
        return nullptr;
    }

    lbcrypto::DCRTPoly copy(m_privateKey->GetPrivateElement());
    return std::make_unique<DCRTPoly>(std::move(copy));
}

std::unique_ptr<PrivateKeyDCRTPoly> DCRTPolyGenNullPrivateKey()
{
    return std::make_unique<PrivateKeyDCRTPoly>();
}

} // openfhe
