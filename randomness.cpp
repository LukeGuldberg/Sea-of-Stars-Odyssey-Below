#include "randomness.h"
#include <stdexcept>

int randint(int min, int max) {
    if (!(min < max)) {
        std::string msg{"min must be less than max: randint("};
        msg += std::to_string(min) + ", " + std::to_string(max) + ")";
        throw std::runtime_error(msg);
    }
    std::uniform_int_distribution dist{min, max};
    return dist(random_engine);
}
