#include "DCRTPoly.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>

namespace openfhe
{
    namespace
    {
        lbcrypto::BigInteger BigIntegerFromFixedLimbsLE(const uint64_t *limbs, size_t limbsPerInt)
        {
            lbcrypto::BigInteger result(0);
            if (limbsPerInt == 0)
            {
                return result;
            }

            lbcrypto::BigInteger factor(1);
            for (size_t i = 0; i < limbsPerInt; ++i)
            {
                const uint64_t limb = limbs[i];
                if (limb != 0)
                {
                    result += lbcrypto::BigInteger(limb) * factor;
                }
                factor <<= 64;
            }
            return result;
        }

        void ValidateStrideOrThrow(const size_t limbsPerInt)
        {
            if (limbsPerInt == 0)
            {
                throw std::runtime_error("limbs_per_int must be > 0");
            }
        }
    } // namespace

    DCRTPoly::DCRTPoly(lbcrypto::DCRTPoly &&poly) noexcept
        : m_poly(std::move(poly))
    {
    }

    const lbcrypto::DCRTPoly &DCRTPoly::GetPoly() const noexcept
    {
        return m_poly;
    }

    rust::String DCRTPoly::GetString() const
    {
        std::stringstream stream;
        stream << m_poly;
        return rust::String(stream.str());
    }

    rust::String DCRTPoly::GetModulus() const
    {
        return m_poly.GetModulus().ToString();
    }

    bool DCRTPoly::IsEqual(const DCRTPoly &other) const noexcept
    {
        return m_poly == other.m_poly;
    }

    rust::Vec<rust::String> DCRTPoly::GetCoefficients() const
    {
        auto tempPoly = m_poly;
        tempPoly.SetFormat(Format::COEFFICIENT);

        lbcrypto::DCRTPoly::PolyLargeType polyLarge = tempPoly.CRTInterpolate();

        const lbcrypto::BigVector &coeffs = polyLarge.GetValues();

        rust::Vec<rust::String> result;
        for (size_t i = 0; i < coeffs.GetLength(); ++i)
        {
            result.push_back(rust::String(coeffs[i].ToString()));
        }

        return result;
    }

    rust::Vec<rust::u8> DCRTPoly::GetCoefficientsBytes() const
    {
        auto tempPoly = m_poly;
        tempPoly.SetFormat(Format::COEFFICIENT);

        lbcrypto::DCRTPoly::PolyLargeType polyLarge = tempPoly.CRTInterpolate();

        const lbcrypto::BigVector &coeffs = polyLarge.GetValues();

        // Serialize the coefficients to a binary format
        std::stringstream ss;
        lbcrypto::Serial::Serialize(coeffs, ss, lbcrypto::SerType::BINARY);

        // Get the binary data as a string
        std::string serializedData = ss.str();

        // Convert to a rust::Vec<rust::u8>
        rust::Vec<rust::u8> result;
        result.reserve(serializedData.size());

        // Copy each byte from the serialized data to the result vector
        for (size_t i = 0; i < serializedData.size(); ++i)
        {
            result.push_back(static_cast<rust::u8>(static_cast<unsigned char>(serializedData[i])));
        }

        return result;
    }

    std::unique_ptr<DCRTPoly> DCRTPoly::Negate() const
    {
        return std::make_unique<DCRTPoly>(-m_poly);
    }

    std::unique_ptr<Matrix> DCRTPoly::Decompose(uint32_t baseBits) const
    {
        std::vector<lbcrypto::DCRTPoly> decomposed = m_poly.CRTDecompose(baseBits);

        auto zero_alloc = lbcrypto::DCRTPoly::Allocator(m_poly.GetParams(), Format::COEFFICIENT);
        lbcrypto::Matrix<lbcrypto::DCRTPoly> decomposedMatrix(zero_alloc, 1, decomposed.size());

        for (size_t i = 0; i < decomposed.size(); i++)
        {
            decomposedMatrix(0, i) = decomposed[i];
        }

        return std::make_unique<Matrix>(std::move(decomposedMatrix));
    }

    // Arithmetic
    std::unique_ptr<DCRTPoly> DCRTPolyAdd(const DCRTPoly &rhs, const DCRTPoly &lhs)
    {
        return std::make_unique<DCRTPoly>(rhs.GetPoly() + lhs.GetPoly());
    }

    std::unique_ptr<DCRTPoly> DCRTPolyMul(const DCRTPoly &rhs, const DCRTPoly &lhs)
    {
        return std::make_unique<DCRTPoly>(rhs.GetPoly() * lhs.GetPoly());
    }

    // Generator functions
    std::unique_ptr<DCRTPoly> DCRTPolyGenFromConst(
        usint n,
        size_t size,
        size_t kRes,
        rust::Slice<const uint64_t> value_limbs)
    {
        const size_t limbsPerInt = value_limbs.size();
        if (limbsPerInt == 0)
        {
            const uint64_t zero = 0;
            return DCRTPolyGenFromVec(n, size, kRes, rust::Slice<const uint64_t>(&zero, 1), 1);
        }
        return DCRTPolyGenFromVec(n, size, kRes, value_limbs, limbsPerInt);
    }

    std::unique_ptr<DCRTPoly> DCRTPolyGenFromVec(
        usint n,
        size_t size,
        size_t kRes,
        rust::Slice<const uint64_t> values_limbs,
        size_t limbs_per_int)
    {
        ValidateStrideOrThrow(limbs_per_int);
        if (values_limbs.size() % limbs_per_int != 0)
        {
            throw std::runtime_error("values_limbs length must be a multiple of limbs_per_int");
        }

        // Create params
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

        // Create a BigVector
        lbcrypto::BigVector bigVec(params->GetRingDimension(), params->GetModulus());
        const size_t count = values_limbs.size() / limbs_per_int;
        const size_t ringDim = params->GetRingDimension();
        const size_t limit = (count < ringDim) ? count : ringDim;
        for (size_t i = 0; i < limit; ++i)
        {
            const uint64_t *limbs = values_limbs.data() + (i * limbs_per_int);
            bigVec[i] = BigIntegerFromFixedLimbsLE(limbs, limbs_per_int);
        }

        // Create a Poly that supports BigInteger coefficients
        lbcrypto::PolyImpl<lbcrypto::BigVector> polyLarge(params, Format::COEFFICIENT);
        polyLarge.SetValues(bigVec, Format::COEFFICIENT);

        // Convert polyLarge to a DCRTPoly
        lbcrypto::DCRTPoly dcrtPoly(polyLarge, params);

        // switch dcrtPoly to EVALUATION format
        dcrtPoly.SetFormat(Format::EVALUATION);

        return std::make_unique<DCRTPoly>(std::move(dcrtPoly));
    }

    std::unique_ptr<DCRTPoly> DCRTPolyGenFromEvalVec(
        usint n,
        size_t size,
        size_t kRes,
        rust::Slice<const uint64_t> values_limbs,
        size_t limbs_per_int)
    {
        ValidateStrideOrThrow(limbs_per_int);
        if (values_limbs.size() % limbs_per_int != 0)
        {
            throw std::runtime_error("values_limbs length must be a multiple of limbs_per_int");
        }

        // Create params
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

        // Create a BigVector for evaluation slots
        lbcrypto::BigVector bigVec(params->GetRingDimension(), params->GetModulus());
        const size_t count = values_limbs.size() / limbs_per_int;
        const size_t ringDim = params->GetRingDimension();
        const size_t limit = (count < ringDim) ? count : ringDim;
        for (size_t i = 0; i < limit; ++i)
        {
            const uint64_t *limbs = values_limbs.data() + (i * limbs_per_int);
            bigVec[i] = BigIntegerFromFixedLimbsLE(limbs, limbs_per_int);
        }

        // Build a Poly with values interpreted in EVALUATION domain
        lbcrypto::PolyImpl<lbcrypto::BigVector> polyLarge(params, Format::EVALUATION);
        polyLarge.SetValues(bigVec, Format::EVALUATION);

        // Convert to a DCRTPoly
        lbcrypto::DCRTPoly dcrtPoly(polyLarge, params);

        return std::make_unique<DCRTPoly>(std::move(dcrtPoly));
    }

    std::unique_ptr<DCRTPoly> DCRTPolyGenFromBug(usint n, size_t size, size_t kRes)
    {
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
        typename lbcrypto::DCRTPoly::BugType bug;
        auto poly = lbcrypto::DCRTPoly(bug, params, Format::EVALUATION);
        return std::make_unique<DCRTPoly>(std::move(poly));
    }

    std::unique_ptr<DCRTPoly> DCRTPolyGenFromDug(usint n, size_t size, size_t kRes)
    {
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
        typename lbcrypto::DCRTPoly::DugType dug;
        auto poly = lbcrypto::DCRTPoly(dug, params, Format::EVALUATION);
        return std::make_unique<DCRTPoly>(std::move(poly));
    }

    std::unique_ptr<DCRTPoly> DCRTPolyGenFromDgg(usint n, size_t size, size_t kRes, double sigma)
    {
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
        typename lbcrypto::DCRTPoly::DggType dgg(sigma);
        auto poly = lbcrypto::DCRTPoly(dgg, params, Format::EVALUATION);
        return std::make_unique<DCRTPoly>(std::move(poly));
    }

    std::unique_ptr<DCRTPoly> DCRTPolyGenFromTug(usint n, size_t size, size_t kRes)
    {
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
        typename lbcrypto::DCRTPoly::TugType tug;
        auto poly = lbcrypto::DCRTPoly(tug, params, Format::EVALUATION);
        return std::make_unique<DCRTPoly>(std::move(poly));
    }

    DCRTPolyParams::DCRTPolyParams(const std::shared_ptr<lbcrypto::DCRTPoly::Params> &params) noexcept
        : m_params(params)
    {
    }
    const std::shared_ptr<lbcrypto::DCRTPoly::Params> &DCRTPolyParams::GetRef() const noexcept
    {
        return m_params;
    }

    // Generator functions
    std::unique_ptr<DCRTPolyParams> DCRTPolyGenNullParams()
    {
        return std::make_unique<DCRTPolyParams>();
    }

    // Matrix functions
    std::unique_ptr<Matrix> MatrixGen(
        usint n,
        size_t size,
        size_t kRes,
        size_t nrow,
        size_t ncol)
    {
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
        auto zero_alloc = lbcrypto::DCRTPoly::Allocator(params, Format::EVALUATION);
        Matrix matrix(zero_alloc, nrow, ncol);
        return std::make_unique<Matrix>(std::move(matrix));
    }

    std::unique_ptr<Matrix> GetMatrixFromFs(
        usint n,
        size_t size,
        size_t kRes,
        const rust::String &path)
    {
        auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

        std::string dataPath = std::string(path);

        lbcrypto::Matrix<lbcrypto::DCRTPoly> deserializedMatrix;
        bool deserializeSuccessResult = lbcrypto::Serial::DeserializeFromFile(dataPath, deserializedMatrix, lbcrypto::SerType::BINARY);
        if (!deserializeSuccessResult)
        {
            throw std::runtime_error("Failed to deserialize result from file");
        }

        auto zero_alloc = lbcrypto::DCRTPoly::Allocator(params, Format::EVALUATION);
        deserializedMatrix.SetAllocator(zero_alloc);

        return std::make_unique<Matrix>(std::move(deserializedMatrix));
    }

    void SetMatrixElement(
        Matrix &matrix,
        size_t row,
        size_t col,
        const DCRTPoly &element)
    {
        matrix(row, col) = element.GetPoly();
    }

    std::unique_ptr<DCRTPoly> GetMatrixElement(
        const Matrix &matrix,
        size_t row,
        size_t col)
    {
        lbcrypto::DCRTPoly copy = matrix(row, col);
        return std::make_unique<DCRTPoly>(std::move(copy));
    }

    size_t GetMatrixRows(const Matrix &matrix)
    {
        return matrix.GetRows();
    }

    size_t GetMatrixCols(const Matrix &matrix)
    {
        return matrix.GetCols();
    }

    std::unique_ptr<Matrix> ExtractMatrixRow(
        const Matrix &matrix,
        size_t row)
    {
        lbcrypto::Matrix<lbcrypto::DCRTPoly> extracted = matrix.ExtractRow(row);
        return std::make_unique<Matrix>(std::move(extracted));
    }

    std::unique_ptr<Matrix> ExtractMatrixCol(
        const Matrix &matrix,
        size_t col)
    {
        lbcrypto::Matrix<lbcrypto::DCRTPoly> extracted = matrix.ExtractCol(col);
        return std::make_unique<Matrix>(std::move(extracted));
    }

    std::unique_ptr<Matrix> ExtractMatrixRows(
        const Matrix &matrix,
        size_t startRow,
        size_t endRow)
    {
        lbcrypto::Matrix<lbcrypto::DCRTPoly> extracted = matrix.ExtractRows(startRow, endRow);
        return std::make_unique<Matrix>(std::move(extracted));
    }

    std::unique_ptr<Matrix> ExtractMatrixCols(
        const Matrix &matrix,
        size_t startCol,
        size_t endCol)
    {
        lbcrypto::Matrix<lbcrypto::DCRTPoly> result = matrix.ExtractCol(startCol);
        for (size_t i = startCol + 1; i < endCol; i++)
        {
            result.HStack(matrix.ExtractCol(i));
        }
        return std::make_unique<Matrix>(std::move(result));
    }

    void FormatMatrixCoefficient(
        Matrix &matrix)
    {
        matrix.SetFormat(Format::COEFFICIENT);
    }

} // openfhe
