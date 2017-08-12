//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_HELPER_H
#define ORAMEVALUATOR_HELPER_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <ctime>

struct outType {
    uint64_t gates;
    uint64_t traffic;
    uint64_t rounds;
};

inline void measure(void (*func)()) {
    clock_t start = clock();
    func();
    float elapsed = (float)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "time needed: " << elapsed << "\n" << std::endl;
}

inline uint16_t myLog2(uint64_t x) {
    return (uint16_t) ceil(log(x) / log(2.0));
}

inline bool isPowerOfTwo(uint64_t x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

inline std::ostream& operator<<(std::ostream& ostr, const outType& i) {
    return ostr << "gates: " << i.gates << " traffic: " << i.traffic << " rounds: " << i.rounds;
}

inline outType& operator+ (const outType& a, const outType& b) {
    auto* out = new outType;
    *out = {a.gates + b.gates, a.traffic + b.traffic, std::max(a.rounds, b.rounds)};

    if(out->gates < a.gates || out->gates < b.gates) {
        //std::cout << "WARNING: Overflow of gates in addition: " << a.gates << " + " << b.gates<< std::endl;
        out->gates = UINT64_MAX;
    }
    if(out->traffic < a.traffic || out->traffic < b.traffic)
        out->traffic = UINT64_MAX;

    delete &a;
    delete &b;

    return *out;
}

inline outType& operator- (const outType& a, const outType& b) {
    auto* out = new outType;
    *out = {a.gates - b.gates, a.traffic - b.traffic, std::max(a.rounds, b.rounds)};

    delete &a;
    delete &b;

    return *out;
}

inline outType& operator* (const uint64_t m, const outType& b) {
    auto* out = new outType;
    *out = {m*b.gates, m*b.traffic, b.rounds};
    delete &b;


    if(myLog2(m) + myLog2(b.gates) > 64) {
        //std::cout << "WARNING: Overflow of gates in multiplication: " << b.gates << " * " << m << std::endl;
        out->gates = UINT64_MAX;
    }
    if(myLog2(m) + myLog2(b.traffic) > 64) {
        out->traffic = UINT64_MAX;
    }

    return *out;
}

inline outType& operator/ (const outType& b, const uint64_t d) {
    auto* out = new outType;
    *out = {b.gates/d, b.traffic/d, d};
    delete &b;

    return *out;
}

inline bool operator< (const outType& a, const outType& b) {
    bool out = a.gates < b.gates;       // TODO: Kostenfunktion
    return out;
}

inline outType& addRounds (outType& outT, int rounds) {
    outT.rounds += rounds;
    return outT;
}

inline outType& multiplyRounds (outType& outT, int mul) {
    outT.rounds *= mul;
    return outT;
}

inline outType& divideRounds (outType& outT, int divisor) {
    outT.rounds /= divisor;
    return outT;
}

#endif //ORAMEVALUATOR_HELPER_H
