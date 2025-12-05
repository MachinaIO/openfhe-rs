#pragma once

#include "openfhe/pke/schemelet/rlwe-mp.h"

#include "Ciphertext.h"
#include "CryptoContext.h"
#include "DCRTPoly.h"
#include "Params.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "SequenceContainers.h"

#include "rust/cxx.h"

namespace openfhe
{

    [[nodiscard]] std::unique_ptr<ElementParams> SchemeletRLWEMPGetElementParams(
        const PrivateKeyDCRTPoly &privateKey, uint32_t level);

    [[nodiscard]] std::unique_ptr<VectorOfPolys> SchemeletRLWEMPEncryptCoeff(
        const std::vector<int64_t> &input, const std::string &Q, const std::string &p,
        const PrivateKeyDCRTPoly &privateKey, const ElementParams &elementParams,
        bool bitReverse);

    [[nodiscard]] std::unique_ptr<VectorOfPolys> SchemeletRLWEMPEncryptCoeffWithZeroB(
        const std::string &Q, const DCRTPoly &a);

    [[nodiscard]] std::unique_ptr<std::vector<int64_t>> SchemeletRLWEMPDecryptCoeff(
        const VectorOfPolys &input, const std::string &Q, const std::string &p,
        const PrivateKeyDCRTPoly &privateKey, const ElementParams &elementParams,
        uint32_t numSlots, uint32_t length, bool bitReverse);

    [[nodiscard]] std::unique_ptr<std::vector<int64_t>> SchemeletRLWEMPDecryptCoeffWithoutRound(
        const VectorOfPolys &input, const std::string &Q, const PrivateKeyDCRTPoly &privateKey,
        const ElementParams &elementParams, uint32_t numSlots, uint32_t length, bool bitReverse);

    void SchemeletRLWEMPModSwitch(VectorOfPolys &input, const std::string &Q1,
                                  const std::string &Q2);

    [[nodiscard]] std::unique_ptr<CiphertextDCRTPoly> SchemeletRLWEMPConvertRLWEToCKKS(
        const CryptoContextDCRTPoly &cryptoContext, const VectorOfPolys &coeffs,
        const PublicKeyDCRTPoly &publicKey, const std::string &Bigq, uint32_t slots,
        uint32_t level);

    [[nodiscard]] std::unique_ptr<VectorOfPolys> SchemeletRLWEMPConvertCKKSToRLWE(
        const CiphertextDCRTPoly &ciphertext, const std::string &Q);

    [[nodiscard]] rust::String SchemeletRLWEMPGetQPrime(const PublicKeyDCRTPoly &publicKey,
                                                        uint32_t lvls);

} // openfhe
