#include "Trapdoor.h"

namespace openfhe
{

DCRTTrapdoor::DCRTTrapdoor(DCRTMatrix& publicMatrix, RLWETrapdoorPair&& trapdoorPair) noexcept
    : m_publicMatrix(publicMatrix), m_trapdoorPair(std::move(trapdoorPair))
{ }

std::unique_ptr<RLWETrapdoorPair> DCRTTrapdoor::GetTrapdoorPair() const
{
    return std::make_unique<RLWETrapdoorPair>(m_trapdoorPair);
}

std::unique_ptr<DCRTMatrix> DCRTTrapdoor::GetPublicMatrix() const
{
    return std::make_unique<DCRTMatrix>(m_publicMatrix);
}

} // openfhe