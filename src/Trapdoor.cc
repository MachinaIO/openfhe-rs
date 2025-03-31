#include "Trapdoor.h"
#include "Params.h"
#include <filesystem>

namespace openfhe
{

DCRTTrapdoor::DCRTTrapdoor(Matrix&& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_publicMatrix(std::move(publicMatrix)), m_trapdoorPair(std::move(trapdoorPair))
{ }

std::unique_ptr<RLWETrapdoorPair> DCRTTrapdoor::GetTrapdoorPair() const
{
    return std::make_unique<RLWETrapdoorPair>(m_trapdoorPair);
}

std::unique_ptr<Matrix> DCRTTrapdoor::GetPublicMatrix() const
{
    return std::make_unique<Matrix>(m_publicMatrix);
}

std::unique_ptr<DCRTPoly> DCRTTrapdoor::GetPublicMatrixElement(size_t row, size_t col) const
{
    if (row >= m_publicMatrix.GetRows() || col >= m_publicMatrix.GetCols()) {
        return nullptr;
    }
    
    lbcrypto::DCRTPoly copy = m_publicMatrix(row, col);
    return std::make_unique<DCRTPoly>(std::move(copy));
}

// Generator functions
std::unique_ptr<DCRTTrapdoor> DCRTTrapdoorGen(
    usint n, 
    size_t size, 
    size_t kRes,
    double sigma,
    int64_t base,
    bool balanced)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    
    auto trapdoor = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGen(
        params,
        sigma,
        base,
        balanced
    );

    return std::make_unique<DCRTTrapdoor>(
        std::move(trapdoor.first),
        std::move(trapdoor.second)
    );
}

std::unique_ptr<DCRTTrapdoor> DCRTSquareMatTrapdoorGen(
    usint n, 
    size_t size, 
    size_t kRes,
    size_t d,
    double sigma,
    int64_t base,
    bool balanced)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);

    auto trapdoor = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGenSquareMat(
        params,
        sigma,
        d,
        base,
        balanced
    );
    
    return std::make_unique<DCRTTrapdoor>(
        std::move(trapdoor.first),
        std::move(trapdoor.second)
    );
}

// Gauss sample functions
std::unique_ptr<Matrix> DCRTTrapdoorGaussSamp(usint n, usint k, const Matrix& publicMatrix, const RLWETrapdoorPair& trapdoor, const DCRTPoly& u, int64_t base, double sigma)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    double c = (base + 1) * sigma;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSamp(
        n,
        k,
        publicMatrix,
        trapdoor,
        u.GetPoly(),
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}

void DCRTTrapdoorGaussSampToFs(usint n, usint k, const Matrix& publicMatrix, const RLWETrapdoorPair& trapdoor, const DCRTPoly& u, int64_t base, double sigma, const rust::String& path)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    double c = (base + 1) * sigma;
    double s = lbcrypto::SPECTRAL_BOUND(n, k, base);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSamp(
        n,
        k,
        publicMatrix,
        trapdoor,
        u.GetPoly(),
        dgg,
        dggLargeSigma,
        base
    );

    std::string dataPath = std::string(path);

    std::filesystem::path fsPath(dataPath);
    std::filesystem::create_directories(fsPath.parent_path());

    bool serializeSuccessResult = lbcrypto::Serial::SerializeToFile(dataPath, result, lbcrypto::SerType::BINARY);
    if (!serializeSuccessResult) {
        throw std::runtime_error("Failed to serialize result to file");
    }
}

std::unique_ptr<Matrix> DCRTSquareMatTrapdoorGaussSamp(usint n, usint k, const Matrix& publicMatrix, const RLWETrapdoorPair& trapdoor, const Matrix& U, int64_t base, double sigma)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    size_t d = U.GetCols(); // U is a square matrix

    double c = (base + 1) * sigma;
    double s = lbcrypto::SPECTRAL_BOUND_D(n, k, base, d);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSampSquareMat(
        n,
        k,
        publicMatrix,
        trapdoor,
        U,
        dgg,
        dggLargeSigma,
        base
    );

    return std::make_unique<Matrix>(std::move(result));
}

void DCRTSquareMatTrapdoorGaussSampToFs(usint n, usint k, const Matrix& publicMatrix, const RLWETrapdoorPair& trapdoor, const Matrix& U, int64_t base, double sigma, const rust::String& path)
{
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    size_t d = U.GetCols(); // U is a square matrix

    double c = (base + 1) * sigma;
    double s = lbcrypto::SPECTRAL_BOUND_D(n, k, base, d);
    lbcrypto::DCRTPoly::DggType dggLargeSigma(sqrt(s * s - c * c));

    auto result = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::GaussSampSquareMat(
        n,
        k,
        publicMatrix,
        trapdoor,
        U,
        dgg,
        dggLargeSigma,
        base
    );

    std::string dataPath = std::string(path);

    std::filesystem::path fsPath(dataPath);
    std::filesystem::create_directories(fsPath.parent_path());

    bool serializeSuccessResult = lbcrypto::Serial::SerializeToFile(dataPath, result, lbcrypto::SerType::BINARY);
    if (!serializeSuccessResult) {
        throw std::runtime_error("Failed to serialize result to file");
    }
}

int64_t GenerateIntegerKarney(double mean, double stddev)
{
    // dgg is not used in the Karney method
    lbcrypto::DCRTPoly::DggType dgg(0.0);
    return dgg.GenerateIntegerKarney(mean, stddev);
}

rust::Vec<int64_t> DCRTGaussSampGqArbBase(
    const DCRTPoly& syndrome,
    double c,
    usint n,
    size_t size,
    size_t kRes,
    int64_t base,
    double sigma,
    size_t tower_idx)
{
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    
    lbcrypto::NativeInteger qu = params->GetParams()[tower_idx]->GetModulus();
    
    lbcrypto::DCRTPoly::DggType dgg(sigma);
    
    lbcrypto::DCRTPoly syndromePoly = syndrome.GetPoly();
    
    lbcrypto::Matrix<int64_t> digits([]() { return 0; }, kRes, n);
    
    lbcrypto::LatticeGaussSampUtility<lbcrypto::NativePoly>::GaussSampGqArbBase(
        syndromePoly.GetElementAtIndex(tower_idx), c, kRes, qu, base, dgg, &digits);
    
    // Convert the matrix to a flattened vector
    rust::Vec<int64_t> result;
    result.reserve(kRes * n);
    
    // Flatten the matrix into a vector
    for (size_t i = 0; i < kRes; i++) {
        for (size_t j = 0; j < n; j++) {
            result.push_back(digits(i, j));
        }
    }
    return result;
}

std::unique_ptr<Matrix> SampleP1ForPertSquareMat(
    Matrix& A,
    Matrix& B,
    Matrix& D,
    Matrix& tp2,
    usint n,
    size_t size,
    size_t kRes,
    double sigma,
    double s)
{
    size_t d = A.GetRows();
    
    auto params = std::make_shared<lbcrypto::ILDCRTParams<lbcrypto::BigInteger>>(2 * n, size, kRes);
    
    lbcrypto::DCRTPoly::DggType dgg(sigma);

    auto zero_alloc = lbcrypto::DCRTPoly::Allocator(params, Format::EVALUATION);

    // Switch the ring elements (Polynomials) to coefficient representation
    A.SetFormat(Format::COEFFICIENT);
    B.SetFormat(Format::COEFFICIENT);
    D.SetFormat(Format::COEFFICIENT);
    tp2.SetFormat(Format::COEFFICIENT);

    lbcrypto::Matrix<lbcrypto::Field2n> AF([&]() { return lbcrypto::Field2n(n, Format::COEFFICIENT); }, d, d);
    lbcrypto::Matrix<lbcrypto::Field2n> BF([&]() { return lbcrypto::Field2n(n, Format::COEFFICIENT); }, d, d);
    lbcrypto::Matrix<lbcrypto::Field2n> DF([&]() { return lbcrypto::Field2n(n, Format::COEFFICIENT); }, d, d);
    
    
    double scalarFactor = -sigma * sigma;

    for (size_t i = 0; i < d; i++) {
        for (size_t j = 0; j < d; j++) {
            AF(i, j) = lbcrypto::Field2n(A(i, j));
            AF(i, j) = AF(i, j).ScalarMult(scalarFactor);
            BF(i, j) = lbcrypto::Field2n(B(i, j));
            BF(i, j) = BF(i, j).ScalarMult(scalarFactor);
            DF(i, j) = lbcrypto::Field2n(D(i, j));
            DF(i, j) = DF(i, j).ScalarMult(scalarFactor);
            if (i == j) {
                AF(i, j) = AF(i, j) + s * s;
                DF(i, j) = DF(i, j) + s * s;
            }
        }
    }

    // converts the field elements to DFT representation
    AF.SetFormat(Format::EVALUATION);
    BF.SetFormat(Format::EVALUATION);
    DF.SetFormat(Format::EVALUATION);

    Matrix p1(zero_alloc, 1, 1);

    for (size_t j = 0; j < d; j++) {
        lbcrypto::Matrix<lbcrypto::Field2n> c([&]() { return lbcrypto::Field2n(n, Format::COEFFICIENT); }, 2 * d, 1);

        for (size_t i = 0; i < d; i++) {
            c(i, 0)     = lbcrypto::Field2n(tp2(i, j)).ScalarMult(-sigma * sigma / (s * s - sigma * sigma));
            c(i + d, 0) = lbcrypto::Field2n(tp2(i + d, j)).ScalarMult(-sigma * sigma / (s * s - sigma * sigma));
        }

        auto p1ZVector = std::make_shared<lbcrypto::Matrix<int64_t>>([]() { return 0; }, n * 2 * d, 1);

        lbcrypto::LatticeGaussSampUtility<lbcrypto::DCRTPoly>::SampleMat(AF, BF, DF, c, dgg, p1ZVector);

        if (j == 0)
            p1 = lbcrypto::SplitInt64IntoElements<lbcrypto::DCRTPoly>(*p1ZVector, n, params);
        else
            p1.HStack(lbcrypto::SplitInt64IntoElements<lbcrypto::DCRTPoly>(*p1ZVector, n, params));
    }

    p1.SetFormat(Format::EVALUATION);

    return std::make_unique<Matrix>(std::move(p1));
}
} // openfhe