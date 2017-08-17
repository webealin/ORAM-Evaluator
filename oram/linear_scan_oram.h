//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_LINEAR_SCAN_ORAM_H
#define ORAMEVALUATOR_LINEAR_SCAN_ORAM_H

#include "oram.h"

class TrivialLinearScan : public ORAM {
protected:
public:
    TrivialLinearScan(uint64_t m, uint64_t b, const std::string &type) : ORAM(m, b, b, type) {}
    TrivialLinearScan(uint64_t m, uint64_t b) : ORAM(m, b, b, "TrivialLinearScan") {}

/**
     * costs of read access on standard Linear Scan
     * @param m: number of elements of array
     * @param b: number of bits per array element
     * @return costs for read access
     */
    static inline outType& c_read(uint64_t m, uint64_t b) {
        // tMux(m, b)
        return c_mux(m, b);
    }

    /**
     * costs of write access on standard Linear Scan
     * @param m: number of elements of array
     * @param b: number of bits per array element
     * @return costs for write access
     */
    static inline outType& c_write(uint64_t m, uint64_t b) {
        // m*mux(b)+decode(m)
        return (m*c_mux(b))+c_decode(myLog2(m));
    }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_amortized(uint16_t noAcc, bool values) override;
};


class LinearScan : public extendedORAM {
protected:
    bool useOldFormula;         // use old Linear Scan ORAM without isDummy field
                                // no isDummy bit -> additional equality comparator over b2 bit
    uint16_t b2;                // bit width of virtual id
public:
    LinearScan(uint64_t m, uint64_t b) : extendedORAM(m, b, b+myLog2(m), "Linear Scan"), b2(myLog2(m)), useOldFormula(false) { }
    LinearScan(uint64_t m, uint64_t b, const std::string &type) : extendedORAM(m, b, b+myLog2(m), type), b2(myLog2(m)), useOldFormula(false) { }
    LinearScan(uint64_t m, uint64_t b, uint16_t b2, bool oldFormula) : extendedORAM(m, b, b+b2, "Linear Scan"), b2(b2), useOldFormula(oldFormula) { }
    LinearScan(uint64_t m, uint64_t b, uint16_t b2, bool oldFormula, const std::string &type) : extendedORAM(m, b, b+b2, type), b2(b2), useOldFormula(oldFormula) { }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_amortized(uint16_t noAcc, bool values) override;

    outType& c_RAR(uint64_t b) override;
    virtual outType& c_cRAR(uint64_t b, outType& (*cond)());

    outType& c_add() override;
    virtual outType& c_cAdd(outType& (*cond)());

    virtual outType& c_pop();
    virtual outType& c_cPop(outType& (*cond)());
};

class LinearScanOram : public LinearScan {
protected:
public:
    LinearScanOram(uint64_t m, uint64_t b) : LinearScan(m, b, "Linear Scan ORAM") {}
    LinearScanOram(uint64_t m, uint64_t b, uint16_t b2, bool oldFormula) : LinearScan(m, b, b2, oldFormula, "Linear Scan ORAM"){ }

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;

    outType& c_RAR(uint64_t b) override;
    outType& c_add() override;
    outType& c_pop() override;
};

#endif //ORAMEVALUATOR_LINEAR_SCAN_ORAM_H