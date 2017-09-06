//
// Created by weber on 20.08.2017.
//

#ifndef ORAMEVALUATOR_OPTIMIZED_SQUARE_ROOT_H
#define ORAMEVALUATOR_OPTIMIZED_SQUARE_ROOT_H

#include "oram.h"
#include "linear_scan_oram.h"
#include "map_based_oram.h"

class OSquareRoot : public MapBasedORAM {
protected:
    uint16_t T;                     // period
    uint16_t t;                     // number accesses this period
public:
    OSquareRoot(uint64_t m, uint64_t b, uint16_t c) : ORAM(m, b, b + myLog2(m), "Optimized Square-Root"), MapBasedORAM(c),
                                                      T((uint16_t)(floor(sqrt(m * myLog2(m))))), t(0) { }

    OSquareRoot(uint64_t m, uint64_t b, uint16_t c, uint16_t T) : ORAM(m, b, b + myLog2(m), "Optimized Square-Root"),
                                                                  MapBasedORAM(c), T(T), t(0) { }

    inline TrivialLinearScan* createLSMap(uint64_t newM) {
        return new SQR_TLS(newM, (uint64_t) (c * myLog2(m) + 1));
    }

    inline OSquareRoot* createMap(uint64_t newM) {
        return new OSquareRoot(newM, c*myLog2(m), c, T);
    }

    inline bool recursionCond(uint16_t counter) {
        return counter > 0 && m >= c*T;
    }

    outType& c_LUMU();

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_acc(uint32_t noAcc, uint64_t b);
    outType& c_amortized(uint32_t noAcc, bool values) override;

    outType& c_epochWO();
    outType& c_update();
    outType& c_permute();
};

#endif //ORAMEVALUATOR_OPTIMIZED_SQUARE_ROOT_H