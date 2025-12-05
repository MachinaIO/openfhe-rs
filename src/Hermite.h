#pragma once

#include "CryptoContext.h"

#include "openfhe/core/math/hermite.h"

#include <vector>

namespace openfhe
{

std::unique_ptr<std::vector<ComplexPair>> GetHermiteTrigCoefficientsByFunction(
    rust::Fn<int64_t(int64_t)> func, uint32_t p, size_t order, double scale);

}

