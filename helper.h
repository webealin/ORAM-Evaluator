//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_HELPER_H
#define ORAMEVALUATOR_HELPER_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <ctime>
#include <iomanip>
#include <values.h>
#include "settings.h"

inline void measure(void (*func)()) {
    clock_t start = clock();
    func();
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

inline uint16_t myLog2(uint64_t x) {
    if(x < 2)       // TODO: x == 0 should not happen?
        return (uint16_t) x;

    // Bit Twiddeling Hack - source: https://graphics.stanford.edu/~seander/bithacks.html
    const uint64_t b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000, 0xFFFFFFFF00000000};
    const uint64_t S[] = {1, 2, 4, 8, 16, 32};
    x--;
    register uint16_t out = 0;
    for (int8_t i = 5; i >= 0; i--) {
        if (x & b[i]) {
            x >>= S[i];
            out |= S[i];
        }
    }
    return (uint16_t) (out + 1);
}

inline bool isPowerOfTwo(uint64_t x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

#endif //ORAMEVALUATOR_HELPER_H
