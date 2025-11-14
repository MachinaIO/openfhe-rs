#include "SchemeletRLWEMP.h"

#include "openfhe/pke/ciphertext.h"

namespace openfhe
{

    std::unique_ptr<ElementParams> SchemeletRLWEMPGetElementParams(
        const PrivateKeyDCRTPoly &privateKey, const uint32_t level)
    {
        return std::make_unique<ElementParams>(lbcrypto::SchemeletRLWEMP::GetElementParams(
            privateKey.GetRef(), level));
    }

    std::unique_ptr<VectorOfPolys> SchemeletRLWEMPEncryptCoeff(
        const std::vector<int64_t> &input, const std::string &Q, const std::string &p,
        const PrivateKeyDCRTPoly &privateKey, const ElementParams &elementParams,
        const bool bitReverse)
    {
        auto copy = input;
        const lbcrypto::BigInteger bigQ(Q);
        const lbcrypto::BigInteger bigP(p);

        return std::make_unique<VectorOfPolys>(lbcrypto::SchemeletRLWEMP::EncryptCoeff(
            std::move(copy), bigQ, bigP, privateKey.GetRef(), elementParams.GetRef(), bitReverse));
    }

    std::unique_ptr<VectorOfPolys> SchemeletRLWEMPEncryptCoeffWithZeroB(
        const std::string &Q, const DCRTPoly &a)
    {
        const lbcrypto::BigInteger bigQ(Q);
        return std::make_unique<VectorOfPolys>(lbcrypto::SchemeletRLWEMP::EncryptCoeffWithZeroB(
            bigQ, a.GetPoly()));
    }

    std::unique_ptr<std::vector<int64_t>> SchemeletRLWEMPDecryptCoeff(
        const VectorOfPolys &input, const std::string &Q, const std::string &p,
        const PrivateKeyDCRTPoly &privateKey, const ElementParams &elementParams,
        const uint32_t numSlots, const uint32_t length, const bool bitReverse)
    {
        const lbcrypto::BigInteger bigQ(Q);
        const lbcrypto::BigInteger bigP(p);

        auto result = lbcrypto::SchemeletRLWEMP::DecryptCoeff(input.GetRef(), bigQ, bigP,
                                                              privateKey.GetRef(), elementParams.GetRef(), numSlots, length, bitReverse);
        return std::make_unique<std::vector<int64_t>>(std::move(result));
    }

    std::unique_ptr<std::vector<int64_t>> SchemeletRLWEMPDecryptCoeffWithoutRound(
        const VectorOfPolys &input, const std::string &Q, const PrivateKeyDCRTPoly &privateKey,
        const ElementParams &elementParams, const uint32_t numSlots, const uint32_t length,
        const bool bitReverse)
    {
        const lbcrypto::BigInteger bigQ(Q);

        auto result = lbcrypto::SchemeletRLWEMP::DecryptCoeffWithoutRound(input.GetRef(), bigQ,
                                                                          privateKey.GetRef(), elementParams.GetRef(), numSlots, length, bitReverse);
        return std::make_unique<std::vector<int64_t>>(std::move(result));
    }

    void SchemeletRLWEMPModSwitch(VectorOfPolys &input, const std::string &Q1,
                                  const std::string &Q2)
    {
        const lbcrypto::BigInteger bigQ1(Q1);
        const lbcrypto::BigInteger bigQ2(Q2);

        lbcrypto::SchemeletRLWEMP::ModSwitch(input.GetRef(), bigQ1, bigQ2);
    }

    std::unique_ptr<CiphertextDCRTPoly> SchemeletRLWEMPConvertRLWEToCKKS(
        const CryptoContextDCRTPoly &cryptoContext, const VectorOfPolys &coeffs,
        const PublicKeyDCRTPoly &publicKey, const std::string &Bigq, const uint32_t slots,
        const uint32_t level)
    {
        const lbcrypto::BigInteger bigQ(Bigq);

        return std::make_unique<CiphertextDCRTPoly>(lbcrypto::SchemeletRLWEMP::ConvertRLWEToCKKS(
            *cryptoContext.GetRef(), coeffs.GetRef(), publicKey.GetRef(), bigQ, slots, level));
    }

    std::unique_ptr<VectorOfPolys> SchemeletRLWEMPConvertCKKSToRLWE(
        const CiphertextDCRTPoly &ciphertext, const std::string &Q)
    {
        const lbcrypto::BigInteger bigQ(Q);
        lbcrypto::ConstCiphertext<lbcrypto::DCRTPoly> constCiphertext(ciphertext.GetRef());

        return std::make_unique<VectorOfPolys>(lbcrypto::SchemeletRLWEMP::ConvertCKKSToRLWE(
            constCiphertext, bigQ));
    }

    rust::String SchemeletRLWEMPGetQPrime(const PublicKeyDCRTPoly &publicKey, const uint32_t lvls)
    {
        return rust::String(lbcrypto::SchemeletRLWEMP::GetQPrime(publicKey.GetRef(), lvls).ToString());
    }

} // openfhe
