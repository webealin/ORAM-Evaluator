//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_LINEAR_SCAN_ORAM_H
#define ORAMEVALUATOR_LINEAR_SCAN_ORAM_H

#include "oram.h"

class LinearScanOram : public ORAM {
protected:
    bool useOldFormula;         // use old Linear Scan ORAM without isDummy field
                                // no isDummy bit -> additional equality comparator over b2 bit
    uint16_t b2;            // bit width of virtual id
public:
    LinearScanOram(uint64_t m, uint64_t b) : ORAM(m, b, b+myLog2(m), "Linear Scan ORAM"),
                                             useOldFormula(false), b2(myLog2(m)) {}
    LinearScanOram(uint64_t m, uint64_t b, uint16_t b2, bool oldFormula) : ORAM(m, b, b+b2, "Linear Scan ORAM"),
                                                                           useOldFormula(oldFormula), b2(b2) { }

    uint64_t c_init(bool values);
    uint64_t c_acc(uint64_t b);

    uint64_t c_RAR(uint64_t b);
    uint64_t c_cRAR(uint64_t b, uint64_t (*cond)());

    uint64_t c_add();
    uint64_t c_cAdd(uint64_t (*cond)());

    uint64_t c_pop();
    uint64_t c_cPop(uint64_t (*cond)());

    uint64_t c_amortized(uint16_t noAcc, bool values);
};

#endif //ORAMEVALUATOR_LINEAR_SCAN_ORAM_H
