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

inline outType& operator+ (const outType& a, const outType& b) {
    outType* out = new outType;
    *out = {a.gates + b.gates, a.traffic + b.traffic, a.rounds + b.rounds};
    delete &a;
    delete &b;

    return *out;
}

inline outType& operator- (const outType& a, const outType& b) {
    outType* out = new outType;
    *out = {a.gates - b.gates, a.traffic - b.traffic, a.rounds - b.rounds};
    delete &a;
    delete &b;

    return *out;
}

inline outType& operator* (const uint64_t m, const outType& b) {
    outType* out = new outType;
    *out = {m*b.gates, m*b.traffic, m*b.rounds};
    delete &b;

    return *out;
}

inline outType& operator/ (const outType& b, const uint64_t d) {
    outType* out = new outType;
    *out = {b.gates/d, b.traffic/d, b.rounds/d};
    delete &b;

    return *out;
}

inline std::ostream& operator<<(std::ostream& ostr, const outType& i) {
    return ostr << "gates: " << i.gates << " traffic: " << i.traffic << " rounds: " << i.rounds;
}

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
