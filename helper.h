//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_HELPER_H
#define ORAMEVALUATOR_HELPER_H

#include <iostream>
#include <cmath>
#include "assert.h"

struct outType {
    uint64_t gates;
    uint64_t traffic;
    uint64_t rounds;
};

inline uint16_t myLog2(uint64_t x) {
    return (uint16_t) ceil(log(x) / log(2.0));
}

inline bool isPowerOfTwo(uint64_t x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

inline uint16_t getNextPowerOfTwo(uint16_t x) {
    uint16_t log = myLog2(x);
    uint16_t out = (uint16_t) pow(2, log);
    assert(isPowerOfTwo(out));
    return out;
}

#endif //ORAMEVALUATOR_HELPER_H
