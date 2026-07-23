#include "Utils.h"
#include <cstdlib>

bool chance_of(std::int32_t percent) noexcept {
    return std::rand() % 100 < percent;
}
