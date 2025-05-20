#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdlib>

inline float rand_from_to(float from, float to) {
    return from + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (to - from); // NOLINT(*-msc50-cpp)
}

inline float rand_up_to(float to) {
    return rand_from_to(0.0f, to);
}

#endif // UTILITIES_H
