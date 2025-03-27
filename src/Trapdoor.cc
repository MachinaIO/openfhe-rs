#include "Trapdoor.h"
#include "Params.h"
#include <filesystem>
#include <mach/mach.h>

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
    
    lbcrypto::DCRTPoly& polyRef = const_cast<lbcrypto::DCRTPoly&>(m_publicMatrix(row, col));
    // Get a reference to the element in the matrix
    // Move that element out of the matrix into the new DCRTPoly object
    // Leave the original matrix element in a moved-from state (which is fine since you don't care about preserving the matrix)

    return std::make_unique<DCRTPoly>(std::move(polyRef));
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

    size_t memoryUsageBefore = getMemoryUsageBytes();
    std::cout << "Memory usage before trapdoor generation: " << memoryUsageBefore << " bytes" << std::endl;

    auto trapdoor = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGen(
        params,
        sigma,
        base,
        balanced
    );

    size_t memoryUsageAfter = getMemoryUsageBytes();
    std::cout << "Memory usage after trapdoor generation: " << memoryUsageAfter << " bytes" << std::endl;

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

    size_t memoryUsageBefore = getMemoryUsageBytes();
    std::cout << "Memory usage before trapdoor generation: " << memoryUsageBefore << " bytes" << std::endl;

    auto trapdoor = lbcrypto::RLWETrapdoorUtility<lbcrypto::DCRTPoly>::TrapdoorGenSquareMat(
        params,
        sigma,
        d,
        base,
        balanced
    );

    size_t memoryUsageAfter = getMemoryUsageBytes();
    std::cout << "Memory usage after trapdoor generation: " << memoryUsageAfter << " bytes" << std::endl;
    
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

// Function to get current memory usage in bytes
size_t getMemoryUsageBytes() {
    task_basic_info info;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS)
        return 0;

    return info.resident_size;
}


} // openfhe