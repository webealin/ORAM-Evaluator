//
// Created by weber on 30.06.2017.
//

#ifndef ORAMEVALUATOR_PATH_ORAM_H
#define ORAMEVALUATOR_PATH_ORAM_H

#include "binary_tree_oram.h"
#include "../helper.h"
#include <cassert>
#include <utility>

class Path : public TreeInterface {
protected:
    uint16_t s;
    LinearScan* stash{};
public:
    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s, const std::string& type) :
            TreeInterface(m, b, (uint16_t) (myLog2(m)-1), b+2* myLog2(m), B, c, type), s(s) {}

    Path(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) :
            TreeInterface(m, b, (uint16_t) (myLog2(m)-1), b+2* myLog2(m), B, c, "Path ORAM"), s(s) {}

    ~Path() override {
        delete stash;
    }
    void build(uint16_t counter) override;
    void build() override;

    Path* createMap(uint64_t newM) override;
    inline bool recursionCond(uint16_t counter) override {
        return counter > 0 && m > 8*c;          // d = log(m)-1 and log(d) is used
    }
    LinearScanOram* createBuckets() override;

    outType& c_init(bool values) override;
    outType& c_acc(uint64_t b) override;
    outType& c_RAR(uint64_t b) override;

    outType& c_LCA(uint64_t b);
    virtual outType& c_addAndEvict();
};

class PathSC : public Path {
public:
    PathSC(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "PathSC ORAM") {
        // B has to be power of two otherwise c_offset would need to include multiplier
        assert(isPowerOfTwo(B));
    }
    PathSC* createMap(uint64_t newM) override;
    outType& c_addAndEvict() override;

    outType& c_dud(uint64_t m, uint64_t b);
    outType& c_offset(uint64_t m, uint64_t b);
    outType& sort1();
    outType& sort23();
};

class Scoram : public Path {
public:
    Scoram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "SCORAM") {}
    Scoram* createMap(uint64_t newM) override;
    outType& c_addAndEvict() override;

    outType& c_cPut();
    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_RDP();
    outType& c_GPP();
};

class Coram : public Path {
public:
    Coram(uint64_t m, uint64_t b, uint16_t B, uint16_t c, uint16_t s) : Path(m, b, B, c, s, "Circuit ORAM") {}
    void build() override;

    Coram* createMap(uint64_t newM) override;
    outType& c_addAndEvict() override;

    outType& c_minLCA(uint64_t m, uint64_t b);
    outType& c_PDStash();
    outType& c_PDStage();
    outType& c_PD();
    outType& c_PT();
    outType& c_evictOnceW();
    outType& c_evictOnce();
};

#endif //ORAMEVALUATOR_PATH_ORAM_H
