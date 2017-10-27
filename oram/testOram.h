//
// Created by weber on 26.10.2017.
//

#ifndef ORAMEVALUATOR_TESTORAM_H
#define ORAMEVALUATOR_TESTORAM_H


#include <cstdint>
#include "binary_tree_oram.h"

class testOram : public TreeInterface {
protected:
    uint16_t s;
    LinearScan* stash{};
public:
    testOram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : ORAM(m, b, b+2*myLog2(m)+1, "Path ORAM"),
                                                                       TreeInterface(myLog2(m), B, c),
                                                                       s(s) { }

    /*~testORAM() override {
        delete stash;
    }*/
    void build(uint16_t counter) override;

    ORAM* createMap(uint64_t newM) override;

    inline bool recursionCond(uint16_t counter) override {
        return counter > 0 && m > 4*c;          // d = log(m) and log(d) is used
    }
    LinearScanOram* createBuckets() override;

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_RAR(uint64_t b) override;

    virtual outType& c_addAndEvict();

    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_PDStash();
    outType& c_PDStage();
    outType& c_PD();
    outType& c_PT();
    outType& c_evictOnceW();
    outType& c_evictOnce();
};


#endif //ORAMEVALUATOR_TESTORAM_H
