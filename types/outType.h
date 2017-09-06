//
// Created by alina on 31.08.17.
//

#ifndef ORAMEVALUATOR_OUTTYPE_H
#define ORAMEVALUATOR_OUTTYPE_H

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <values.h>
#include "../settings.h"
#include "../helper.h"

struct outType {
    uint64_t gates{UINT64_MAX};
    uint64_t traffic{UINT64_MAX};
    uint64_t rounds{UINT64_MAX};

    outType() = default;
    outType(uint64_t g, uint64_t t, uint64_t r) : gates(g), traffic(t), rounds(r) { }

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

inline double_t needsTime(const outType& outType) {
    double_t rounds = Settings::latency * outType.rounds;                           // calculation is in ms
    double_t traffic = outType.traffic / (1024*1024*Settings::bandwidth / 1000);
    double_t gates = outType.gates / Settings::calc;

    double_t out1 = rounds + traffic;
    double_t out = out1 + gates;
    if(out1 < rounds || out1 < traffic || out < out1 || out < gates)
        out = -1;
    return out;
}

inline std::ostream& operator<<(std::ostream& ostr, const outType& i) {
    return ostr << "gates: " << i.gates << " traffic: " << i.traffic << " rounds: " << i.rounds << " time: " << needsTime(i) << " ms";
}

inline std::ostream& operator<<(std::ostream& ostr, const outType* i) {
    return ostr << "gates: " << i->gates << " traffic: " << i->traffic << " rounds: " << i->rounds << " time: " << needsTime(*i) << " ms";
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

inline outType& operator/ (const outType& b, const uint32_t d) {
    auto* out = new outType;
    *out = {(uint64_t)ceil(b.gates/d), (uint64_t)ceil(b.traffic/d), d};
    delete &b;

    return *out;
}

inline outType& divideWR(const outType& b, const uint32_t d) {
    auto rounds = (uint64_t) ceil(b.rounds / d);
    outType& out = b / d;
    out.rounds = rounds;

    return out;
}

inline bool operator< (const outType& a, const outType& b) {
    return needsTime(a) < needsTime(b);
}


#endif //ORAMEVALUATOR_OUTTYPE_H
