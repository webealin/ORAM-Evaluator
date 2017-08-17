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

    inline outType& operator+= (const outType& b) {
        if(gates + b.gates < gates || gates + b.gates < b.gates)
            gates = UINT64_MAX;
        else gates += b.gates;

        if(traffic + b.traffic < traffic || traffic + b.traffic < b.traffic)
            traffic = UINT64_MAX;
        else traffic += b.traffic;
        rounds = std::max(rounds, b.rounds);

        delete &b;

        return *this;
    }
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

inline std::ostream& operator<<(std::ostream& ostr, const outType* i) {
    return ostr << "gates: " << i->gates << " traffic: " << i->traffic << " rounds: " << i->rounds;
}

inline outType& operator+ (const outType& a, const outType& b) {
    auto* out = new outType;
    *out = {a.gates + b.gates, a.traffic + b.traffic, std::max(a.rounds, b.rounds)};

    if(out->gates < a.gates || out->gates < b.gates)
        out->gates = UINT64_MAX;

    if(out->traffic < a.traffic || out->traffic < b.traffic)
        out->traffic = UINT64_MAX;

    delete &a;
    delete &b;

    return *out;
}

inline outType& addWR(const outType &a, const outType &b) {
    uint64_t rounds = a.rounds + b.rounds;
    //std::cout << "a: " << a.rounds << " b: " << b.rounds << " a+b: " << rounds << std::endl;
    outType& out = a + b;
    out.rounds = rounds;

    return out;
}

inline outType& operator- (const outType& a, const outType& b) {
    auto* out = new outType;
    *out = {a.gates - b.gates, a.traffic - b.traffic, std::max(a.rounds, b.rounds)};

    delete &a;
    delete &b;

    return *out;
}

inline outType& minusWR(const outType &a, const outType &b) {
    uint64_t rounds = a.rounds - b.rounds;
    outType& out = a - b;
    out.rounds = rounds;

    return out;
}

inline outType& operator* (const uint64_t m, const outType& b) {
    auto* out = new outType;
    *out = {m*b.gates, m*b.traffic, b.rounds};

    if(myLog2(m) + myLog2(b.gates) > 64)
        out->gates = UINT64_MAX;

    if(myLog2(m) + myLog2(b.traffic) > 64)
        out->traffic = UINT64_MAX;

    delete &b;

    return *out;
}

inline outType& multiplyWR(const uint64_t m, const outType &b) {
    uint64_t rounds = m * b.rounds;
    outType& out = m * b;
    out.rounds = rounds;

    return out;
}

inline outType& operator/ (const outType& b, const uint16_t d) {
    auto* out = new outType;
    *out = {b.gates/d, b.traffic/d, d};
    delete &b;

    return *out;
}

inline outType& divideWR(const outType& b, const uint16_t d) {
    uint64_t rounds = b.rounds / d;
    outType& out = b / d;
    out.rounds = rounds;

    return out;
}

inline bool operator< (const outType& a, const outType& b) {
    bool out = a.gates < b.gates;       // TODO: Kostenfunktion
    return out;
}

#endif //ORAMEVALUATOR_HELPER_H
