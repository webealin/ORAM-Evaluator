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
    uint16_t b2;                // bit width of virtual id
public:
    LinearScanOram(uint64_t m, uint64_t b) : ORAM(m, b, b+myLog2(m), "Linear Scan ORAM"),
                                             useOldFormula(false), b2(myLog2(m)) {}
    LinearScanOram(uint64_t m, uint64_t b, uint16_t b2, bool oldFormula) : ORAM(m, b, b+b2, "Linear Scan ORAM"),
                                                                           useOldFormula(oldFormula), b2(b2) { }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;

    outType& c_RAR(uint64_t b) override;
    outType& c_cRAR(uint64_t b, outType& (*cond)());

    outType& c_add() override;
    outType& c_cAdd(outType& (*cond)());

    outType& c_pop();
    outType& c_cPop(outType& (*cond)());

    outType& c_amortized(uint16_t noAcc, bool values) override;
};

class LinearScan : public ORAM {
protected:
    uint16_t b2;                // bit width of virtual id
public:
    LinearScan(uint64_t m, uint64_t b, uint16_t b2) : ORAM(m, b, b, "Linear Scan"), b2(b2) { }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;

    outType& c_amortized(uint16_t noAcc, bool values) override;
    outType& c_RAR(uint64_t b) override;
    outType& c_add() override;
};

#endif //ORAMEVALUATOR_LINEAR_SCAN_ORAM_H
